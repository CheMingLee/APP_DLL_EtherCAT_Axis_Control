#pragma once

#include "Windows.h"
#include "ctrlcard.h"        // This defines the IOCTL constants.

//Bill_20200901 支援切割軟體修改(SC)
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
							 // KM: 此 structure 據 driver 需要作調整

class CDev_PMC6
{
public:
	CDev_PMC6(void);
	virtual ~CDev_PMC6(void);
	HANDLE	m_hHandle[MAX_DEVICE];		// 預設最多版卡4枚
	HANDLE	m_hUSBHandle[MAX_DEVICE][2];		// 預設最多版卡4枚
#ifndef PMC6_WIN64
	DWORD	m_dwMemBases[MAX_DEVICE];	// 預設最多版卡4枚
#else
	UINT64	m_dwMemBases[MAX_DEVICE];	// 預設最多版卡4枚
#endif
	DWORD	m_dwCardNum;		// 紀錄已偵測的板卡數量
	DWORD	m_dwSelectCard;		// 選擇使用的板卡編號(sequential programming)

	char	m_WriteBuffer[MAX_DEVICE][256];
	char	m_ReadBuffer[MAX_DEVICE][256];
	int		m_iWritePtr[MAX_DEVICE];
	int		m_iReadSize[MAX_DEVICE];

	HANDLE Dev_Open ();						// 開啟設備

											// 回傳 Device Handle
	HANDLE Dev_ReOpen ();						// 開啟設備


	void Dev_Close ();						// 關閉設備
											// Device Handle
	
	BOOL
	Dev_DeviceIo(							// 設備控制
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
	Dev_InterruptEnable (					// 啟動外部中斷
		HANDLE			i_hDevice,			// handle to device
		FIntrCallBack	i_pfnCBFunc,		// Interrupt 的 Call Back Function
		void*			i_pCBParam			// Interrupt 的 Call Back 參數
	);										// 成功則回傳 TRUE

	void Dev_InterruptDisable (HANDLE i_hDevice);		// 關閉外部中斷功能
														// handle to device

	BOOL	Dev_Write(char *WBuf,DWORD iSize,LPDWORD iWSize);
	BOOL	Dev_Read(char *RBuf,DWORD iSize,LPDWORD iRSize);

private:
	BOOL GetMemAddr ();
};

extern CDev_PMC6 g_DevPMC6;