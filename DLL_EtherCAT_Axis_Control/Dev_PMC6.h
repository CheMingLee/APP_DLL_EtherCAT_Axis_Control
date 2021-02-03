#pragma once

#include "Windows.h"
#include "ctrlcard.h"        // This defines the IOCTL constants.

//Bill_20200901 �䴩���γn��ק�(SC)
#define MAX_ID 16
#define MAX_DEVICE			MAX_ID
#define MAX_AXIS_CNT		40
#define MAX_BUFFER_PACKET	128

#define USB_HANDLE_OUT		0
#define USB_HANDLE_IN		1

typedef struct   
{
    ULONG   LongData;
    USHORT  ShortData;
    UCHAR   CharData;
}DataBuffer;			     // Buffer received from driver (Data).
							 // KM: �� structure �� driver �ݭn�@�վ�

class CDev_PMC6
{
public:
	CDev_PMC6(void);
	virtual ~CDev_PMC6(void);
	HANDLE	m_hHandle[MAX_DEVICE];		// �w�]�̦h���d4�T
	HANDLE	m_hUSBHandle[MAX_DEVICE][2];		// �w�]�̦h���d4�T
#ifndef PMC6_WIN64
	DWORD	m_dwMemBases[MAX_DEVICE];	// �w�]�̦h���d4�T
#else
	UINT64	m_dwMemBases[MAX_DEVICE];	// �w�]�̦h���d4�T
#endif
	DWORD	m_dwCardNum;		// �����w�������O�d�ƶq
	DWORD	m_dwSelectCard;		// ��ܨϥΪ��O�d�s��(sequential programming)

	char	m_WriteBuffer[MAX_DEVICE][256];
	char	m_ReadBuffer[MAX_DEVICE][256];
	int		m_iWritePtr[MAX_DEVICE];
	int		m_iReadSize[MAX_DEVICE];

	HANDLE Dev_Open ();						// �}�ҳ]��

											// �^�� Device Handle
	HANDLE Dev_ReOpen ();						// �}�ҳ]��


	void Dev_Close ();						// �����]��
											// Device Handle
	
	BOOL
	Dev_DeviceIo(							// �]�Ʊ���
		DWORD			i_dwIoControlCode,	// operation
		LPVOID			i_lpInBuffer,		// input data buffer
		DWORD			i_nInBufferSize,	// size of input data buffer
		LPVOID			o_lpOutBuffer,		// output data buffer
		DWORD			i_nOutBufferSize,	// size of output data buffer
		LPDWORD			o_lpBytesReturned,	// byte count
		LPOVERLAPPED	i_lpOverlapped		// overlapped information
	);

	typedef void (*FIntrCallBack)(void*);
	BOOL
	Dev_InterruptEnable (					// �Ұʥ~�����_
		HANDLE			i_hDevice,			// handle to device
		FIntrCallBack	i_pfnCBFunc,		// Interrupt �� Call Back Function
		void*			i_pCBParam			// Interrupt �� Call Back �Ѽ�
	);										// ���\�h�^�� TRUE

	void Dev_InterruptDisable (HANDLE i_hDevice);		// �����~�����_�\��
														// handle to device

	BOOL	Dev_Write(char *WBuf,DWORD iSize,LPDWORD iWSize);
	BOOL	Dev_Read(char *RBuf,DWORD iSize,LPDWORD iRSize);

private:
	BOOL GetMemAddr ();
};

extern CDev_PMC6 g_DevPMC6;