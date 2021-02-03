#include "stdafx.h"
#include "Dev_PMC6.h"
#include "guid_def.h"
#include <setupapi.h>
#include <winioctl.h>
#include <TCHAR.h>
#include <stdio.h>
#include <stdlib.h>
#include "ctrlcard.h"

#pragma warning(disable: 4996)
#define _CRT_SECURE_NO_WARNINGS


CDev_PMC6 g_DevPMC6;
//void ReadByteFromDevice ();	//KM: Test Read Data

CDev_PMC6::CDev_PMC6(void)
{
	int i;

	for(i=0;i < MAX_DEVICE;i++)
	{
		m_hHandle[i] = NULL;
		m_hUSBHandle[i][USB_HANDLE_OUT] = NULL;
		m_hUSBHandle[i][USB_HANDLE_IN] = NULL;
		m_dwMemBases[i] = 0;
		memset(&m_WriteBuffer[i][0],0,256);
		memset(&m_ReadBuffer[i][0],0,256);
		m_iWritePtr[i] = 0;
	}
	m_dwCardNum = 0;
	m_dwSelectCard = 0;


}

CDev_PMC6::~CDev_PMC6(void)
{
	Dev_Close ();
}

HANDLE CDev_PMC6::Dev_Open ()
{
	HANDLE	hDevice = NULL;

	// 取所有適合的 Card Device Name, 給之後 CreateFile 使用
	TCHAR astDevicePathList[MAX_DEVICE][255];	// Class Name

    HDEVINFO							hardwareDeviceInfo;
    SP_DEVICE_INTERFACE_DATA			deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA	deviceInterfaceDetailData = NULL;
    ULONG                               predictedLength = 0;
    ULONG                               requiredLength = 0, i;
    DWORD                               error;
	GUID								InterfaceGuid = DRIVER_GUID;

	memset(astDevicePathList,0,1020);

    hardwareDeviceInfo = SetupDiGetClassDevs (
                       (LPGUID)&InterfaceGuid,
                       NULL, // Define no enumerator (global)
                       NULL, // Define no
                       (DIGCF_PRESENT | // Only Devices present
                       DIGCF_DEVICEINTERFACE)); // Function class devices.
    if(INVALID_HANDLE_VALUE == hardwareDeviceInfo)
    {
		return NULL;
    }

    //
    // Enumerate devices of a specific interface class
    //
    deviceInterfaceData.cbSize = sizeof(deviceInterfaceData);

    for (i=0; SetupDiEnumDeviceInterfaces (hardwareDeviceInfo,
                                 0, // No care about specific PDOs
                                 (LPGUID)&InterfaceGuid,
                                 i, //
                                 &deviceInterfaceData); i++ )
	{
        //
        // Allocate a function class device data structure to
        // receive the information about this particular device.
        //

        //
        // First find out required length of the buffer
        //
        if (deviceInterfaceDetailData)
		{
            HeapFree (GetProcessHeap(), 0, deviceInterfaceDetailData);
        }

        if(!SetupDiGetDeviceInterfaceDetail (
                hardwareDeviceInfo,
                &deviceInterfaceData,
                NULL, // probing so no output buffer yet
                0, // probing so output buffer length of zero
                &requiredLength,
                NULL) && 
				((error = GetLastError()) != ERROR_INSUFFICIENT_BUFFER))
        {
			SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
			return NULL;
        }

        predictedLength = requiredLength;

        deviceInterfaceDetailData = 
			(PSP_DEVICE_INTERFACE_DETAIL_DATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                                        predictedLength);
        if (deviceInterfaceDetailData == NULL)
		{
			SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
			return NULL;
        }

        deviceInterfaceDetailData->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);

        if (! SetupDiGetDeviceInterfaceDetail (
                   hardwareDeviceInfo,
                   &deviceInterfaceData,
                   deviceInterfaceDetailData,
                   predictedLength,
                   &requiredLength,
                   NULL))
		{
			if (deviceInterfaceDetailData)
			{
				HeapFree (GetProcessHeap(), 0, deviceInterfaceDetailData);
			}

			SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);
			return NULL;
        }

		// Copy the device path so that we can open the device using CreateFile.
		if (i < MAX_DEVICE)
			_tcscpy (&astDevicePathList[i][0], deviceInterfaceDetailData->DevicePath);
	}
 
	if (deviceInterfaceDetailData)
	{
		HeapFree (GetProcessHeap(), 0, deviceInterfaceDetailData);
	}

    SetupDiDestroyDeviceInfoList (hardwareDeviceInfo);

	// 取得 Handle
	for(i = 0 ; i < MAX_DEVICE ; i++)
	{
		if(astDevicePathList[i][0] != 0)
		{			

			hDevice = CreateFile(						
						(LPCTSTR)&astDevicePathList[i][0],
					    GENERIC_READ | GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
		                NULL,
			            OPEN_EXISTING,
						0,
					    NULL
						);

			if (hDevice != INVALID_HANDLE_VALUE)
			{	

				m_hHandle[i] = hDevice;
				m_dwCardNum++;
			}
			//Open In EP
		}
	}

    if (m_dwCardNum == 0)
		return NULL;

	if (!GetMemAddr ())
	{
		Dev_Close ();
		return NULL;
	}
	return hDevice;
}

void CDev_PMC6::Dev_Close ()
{
	int i;

	for(i=0;i<MAX_DEVICE;i++)
	{
		if (m_hHandle[i])
		{
			CloseHandle(m_hHandle[i]);                  // Close the Device "file".
			m_hHandle[i] = NULL;
		}
		if (m_hUSBHandle[i][USB_HANDLE_OUT])
		{
			CloseHandle(m_hUSBHandle[i][USB_HANDLE_OUT]);
			m_hUSBHandle[i][USB_HANDLE_OUT] = NULL;
		}
		if (m_hUSBHandle[i][USB_HANDLE_IN])
		{
			CloseHandle(m_hUSBHandle[i][USB_HANDLE_IN]);
			m_hUSBHandle[i][USB_HANDLE_IN] = NULL;
		}
	}
}

HANDLE CDev_PMC6::Dev_ReOpen()
{
	Dev_Close();
	Sleep(2000);
	return Dev_Open();
}

BOOL CDev_PMC6::Dev_DeviceIo(
	DWORD			i_dwIoControlCode,	// operation
	LPVOID			i_lpInBuffer,		// input data buffer
	DWORD			i_nInBufferSize,	// size of input data buffer
	LPVOID			o_lpOutBuffer,		// output data buffer
	DWORD			i_nOutBufferSize,	// size of output data buffer
	LPDWORD			o_lpBytesReturned,	// byte count
	LPOVERLAPPED	i_lpOverlapped		// overlapped information
)
{
	HANDLE hHD = m_hHandle[m_dwSelectCard];

	if (m_hHandle[m_dwSelectCard] == NULL)
		return FALSE;					//  傳入參數有誤

	int iStatus = DeviceIoControl (
		hHD,
		i_dwIoControlCode,
		i_lpInBuffer,
		i_nInBufferSize,
		o_lpOutBuffer,
		i_nOutBufferSize,
		o_lpBytesReturned,
		i_lpOverlapped
		);

	if(!iStatus)
	{
		int err = GetLastError();
		char err_buf[100];
		char err_num[10];
		strcpy(err_buf,"err code :");
		itoa(err,err_num,10);
		strcat(err_buf,err_num);
#ifdef _DEBUG
		//MessageBox(NULL,err_buf,0,0);
#endif
	}

	return iStatus;
}

BOOL CDev_PMC6::GetMemAddr()
{
#ifndef PMC6_WIN64
	DWORD adwMemBases[6];	// Bar(0-5)
#else
	UINT64 adwMemBases[6];	// Bar(0-5)
#endif
	DWORD ReturnedLength;
	BOOL bRet = FALSE;
	int i;

	for(i=0;i<MAX_DEVICE;i++)
	{
		if (m_hHandle[i] == NULL)
			continue;
		
		memset (adwMemBases, 0, sizeof (adwMemBases));

		m_dwSelectCard = i;
		bRet = Dev_DeviceIo (
			IOCTL_GPD_GET_MAPPED_MEMORY_BASES,
			NULL,
			0,
			adwMemBases,
			sizeof (adwMemBases),
			&ReturnedLength,
			NULL
			);

		if (bRet == FALSE)
			return FALSE;

		m_dwMemBases[i] = adwMemBases[0];
	}

	return TRUE;
}

BOOL CDev_PMC6::Dev_InterruptEnable(HANDLE i_hDevice, FIntrCallBack i_pfnCBFunc, void *i_pCBParam)
{
	return FALSE;
}

void CDev_PMC6::Dev_InterruptDisable(HANDLE i_hDevice)
{
}

BOOL CDev_PMC6::Dev_Write(char *WBuf,DWORD iSize,LPDWORD iWSize)
{
	if(m_hUSBHandle[m_dwSelectCard][USB_HANDLE_OUT] == NULL)
		return TRUE;

	return WriteFile(m_hUSBHandle[m_dwSelectCard][USB_HANDLE_OUT],WBuf,iSize,iWSize,NULL);
}

BOOL CDev_PMC6::Dev_Read(char *RBuf,DWORD iSize,LPDWORD iRSize)
{
	if(m_hUSBHandle[m_dwSelectCard][USB_HANDLE_IN] == NULL)
		return TRUE;

	return ReadFile(m_hUSBHandle[m_dwSelectCard][USB_HANDLE_IN],RBuf,iSize,iRSize,NULL);
}
