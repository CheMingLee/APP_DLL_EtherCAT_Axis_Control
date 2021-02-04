// pci.cpp : 定義 PCI通訊功能
#include "stdafx.h"
#include <stdio.h>
#include "pci.h"
#include "Dev_PMC6.h"
#include <setupapi.h>
#include <winioctl.h>
#include "ctrlcard.h"
//#define _MEGSHOW
#define KER_RW

//Bill_20100202 reply status
#define PCI_NO_DATA		0
#define PCI_READ_END	1
#define PCI_RETRY		2

#define _NOCARD

#define PCI_READ_OFFSET 	128
#define PCI_WRITE_OFFSET 	0


extern CDev_PMC6 g_DevPMC6;

unsigned long g_lErr;
HDC hDC;

extern long g_PMC6_Auth[MAX_ID];

void PCI_ShowErrCode(BYTE byErrCode,BYTE bReturnCode)
{
	char			szT[256];

	#ifndef _DEBUG
		return;
	#endif

	g_lErr++;

	hDC = ::GetDC(NULL);
	if (byErrCode == 0)
	{
		sprintf_s(szT, "<<%ld >>" , g_lErr);
	}
	else
	{
		sprintf_s(szT, "<<Retry = %ld, %ld>>" , byErrCode,bReturnCode);
	}
	::TextOut(hDC, 0,0, szT, (int)strlen(szT));
	::ReleaseDC(NULL, hDC);

	//MessageBox(NULL,szT,0,0);
}

// PCI control base function
//============================================================
//Bill_20100709
BOOL PCI_TransmitBlock(BYTE *WBuf,ULONG WSize,BYTE *RBuf,ULONG RBSize,ULONG *RSize,ULONG BackData)
{
	BYTE  TBuf[256];
	DWORD IoctlCode = IOCTL_GPD_TRANSMITBLOCK;
	#ifndef PMC6_WIN64
		DWORD addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard];
	#else
		UINT64 addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard];
	#endif

	#ifdef _NOCARD
		if(g_DevPMC6.m_hHandle[g_DevPMC6.m_dwSelectCard] == NULL)
		{
			return FALSE;
		}
	#endif

		if(WSize > (248))
			return FALSE;

	#ifndef PMC6_WIN64
		*(PULONG)TBuf = addr;
		*(PULONG)(TBuf + 4) = BackData;
		*(PULONG)(TBuf + 8) = WSize;
		
		memcpy(TBuf + 12,WBuf,WSize);


		return g_DevPMC6.Dev_DeviceIo(                            
			IoctlCode,          // IO Control code for Read
			TBuf,				// Buffer to driver.
			WSize + 12,		// Length of buffer in bytes.
			RBuf,        // Buffer from driver.
			RBSize,         // Length of buffer in bytes.
			RSize,    // Bytes placed in DataBuffer.
			NULL                // NULL means wait till op. completes.
			);
	#else
		*(PUINT64)TBuf = addr;
		*(PULONG)(TBuf + 8) = BackData;
		*(PULONG)(TBuf + 12) = WSize;
		
		memcpy(TBuf + 16,WBuf,WSize);


		return g_DevPMC6.Dev_DeviceIo(                            
			IoctlCode,          // IO Control code for Read
			TBuf,				// Buffer to driver.
			WSize + 16,		// Length of buffer in bytes.
			RBuf,        // Buffer from driver.
			RBSize,         // Length of buffer in bytes.
			RSize,    // Bytes placed in DataBuffer.
			NULL                // NULL means wait till op. completes.
			);
	#endif
}

DWORD PCI_GetStReg()
{
	BYTE buf[4];
	DWORD Databuf;
	DWORD DataLength;
	DWORD ReturnedLength;
	DWORD IoctlCode = IOCTL_GPD_READ_MEMORY_ULONG;
	#ifndef PMC6_WIN64
		DWORD addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard];
	#else
		UINT64 addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard];
	#endif

	DataLength = 4;

	g_DevPMC6.Dev_DeviceIo(                            
          IoctlCode,          // IO Control code for Read
          &addr,				// Buffer to driver.
          sizeof(addr),		// Length of buffer in bytes.
          buf,        // Buffer from driver.
          DataLength,         // Length of buffer in bytes.
          &ReturnedLength,    // Bytes placed in DataBuffer.
          NULL                // NULL means wait till op. completes.
          );
	//emission_20100109_改善效能問題
	BYTE* pbyData = (BYTE*)(&Databuf);
	pbyData[0] = buf[0];
	pbyData[1] = buf[1];
	pbyData[2] = buf[2];
	pbyData[3] = buf[3];
	/*
	memcpy(((BYTE *)&sDatabuf)    ,&buf[1],1);
	memcpy(((BYTE *)&sDatabuf + 1),&buf[0],1);
	*/
	//emission_20100109_改善效能問題

	return Databuf;
}

BOOL PCI_SetStReg(WORD bit,WORD On)
{
	BYTE *buf;
	BYTE data_buf[12];
	DWORD StReg;
	DWORD mask = 1 << bit;
	DWORD DataLength;
	DWORD ReturnedLength;
	DWORD IoctlCode = IOCTL_GPD_WRITE_MEMORY_ULONG;
	#ifndef PMC6_WIN64
		DWORD addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard];

		//emission_20100109_改善效能問題
		BYTE* pbyData = (BYTE*)(&addr);
		data_buf[0] = pbyData[0];
		data_buf[1] = pbyData[1];
		data_buf[2] = pbyData[2];
		data_buf[3] = pbyData[3];
	#else
		UINT64 addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard];

		//emission_20100109_改善效能問題
		BYTE* pbyData = (BYTE*)(&addr);
		data_buf[0] = pbyData[0];
		data_buf[1] = pbyData[1];
		data_buf[2] = pbyData[2];
		data_buf[3] = pbyData[3];
		data_buf[4] = pbyData[4];
		data_buf[5] = pbyData[5];
		data_buf[6] = pbyData[6];
		data_buf[7] = pbyData[7];
	#endif
		
		StReg = PCI_GetStReg();
		if(On)
			StReg = StReg | mask;
		else
		{
			mask = ~mask;
			StReg = StReg & mask;
		}

		buf = (BYTE *)&StReg;
	#ifndef PMC6_WIN64
		data_buf[4] = buf[0];
		data_buf[5] = buf[1];
		data_buf[6] = buf[2];
		data_buf[7] = buf[3];

		DataLength = 8;
	#else
		data_buf[8] = buf[0];
		data_buf[9] = buf[1];
		data_buf[10] = buf[2];
		data_buf[11] = buf[3];

		DataLength = 12;
	#endif
		/*
		memcpy(data_buf,&addr,4);
		sStReg = PCI_GetStReg();
		if(On)
			sStReg = sStReg | mask;
		else
		{
			mask = !mask;
			sStReg = sStReg & mask;
		}
		memcpy(&data_buf[4],&buf[1],1);
		memcpy(&data_buf[5],&buf[0],1);
		*/
		//emission_20100109_改善效能問題



	return g_DevPMC6.Dev_DeviceIo(                            
          IoctlCode,          // IO Control code for Read
          data_buf,				// Buffer to driver.
          DataLength,		// Length of buffer in bytes.
          NULL,        // Buffer from driver.
          0,         // Length of buffer in bytes.
          &ReturnedLength,    // Bytes placed in DataBuffer.
          NULL                // NULL means wait till op. completes.
          );
}

BOOL PCI_SetDataSize(WORD sDSize)
{
	BYTE *buf;
	BYTE data_buf[6];
	DWORD DataLength;
	DWORD ReturnedLength;
	DWORD IoctlCode = IOCTL_GPD_WRITE_MEMORY_USHORT;
	#ifndef PMC6_WIN64
		DWORD addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard] + PCI_WRITE_OFFSET + 2;

		memcpy(data_buf,&addr,4);
		buf = (BYTE *)&sDSize;

		//emission_20100109_改善效能問題
		data_buf[4] = buf[1];
		data_buf[5] = buf[0];
	//	memcpy(&data_buf[4],&buf[1],1);
	//	memcpy(&data_buf[5],&buf[0],1);
		//emission_20100109_改善效能問題

		DataLength = 6;

	#else
		UINT64 addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard] + PCI_WRITE_OFFSET + 2;

		memcpy(data_buf,&addr,8);
		buf = (BYTE *)&sDSize;
		data_buf[8] = buf[1];
		data_buf[9] = buf[0];
		DataLength = 10;
	#endif

	return g_DevPMC6.Dev_DeviceIo(                            
          IoctlCode,          // IO Control code for Read
          data_buf,				// Buffer to driver.
          DataLength,		// Length of buffer in bytes.
          NULL,        // Buffer from driver.
          0,         // Length of buffer in bytes.
          &ReturnedLength,    // Bytes placed in DataBuffer.
          NULL                // NULL means wait till op. completes.
          );
}

WORD PCI_GetDataSize()
{
	BYTE buf[2];
	WORD sDatabuf;
	DWORD DataLength;
	DWORD ReturnedLength;
	DWORD IoctlCode = IOCTL_GPD_READ_MEMORY_USHORT;
	#ifndef PMC6_WIN64
		DWORD addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard] + PCI_READ_OFFSET + 2;
	#else
		UINT64 addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard] + PCI_READ_OFFSET + 2;
	#endif
	DataLength = 2;

	g_DevPMC6.Dev_DeviceIo(                            
			IoctlCode,          // IO Control code for Read
			&addr,				// Buffer to driver.
			sizeof(addr),		// Length of buffer in bytes.
			buf,        // Buffer from driver.
			DataLength,         // Length of buffer in bytes.
			&ReturnedLength,    // Bytes placed in DataBuffer.
			NULL                // NULL means wait till op. completes.
			);
	//emission_20100109_改善效能問題
	BYTE* pbyData = (BYTE*)(&sDatabuf);
	*pbyData = buf[1];

	pbyData = (BYTE*)(&sDatabuf)+1;
	*pbyData = buf[0];
	//memcpy(((BYTE *)&sDatabuf)    ,&buf[1],1);
	//memcpy(((BYTE *)&sDatabuf + 1),&buf[0],1);
	//emission_20100109_改善效能問題

	return sDatabuf;
}

BOOL PCI_Write(BYTE* data_buf,WORD data_count)
{
	BYTE copy_buf[8];
	DWORD DataLength;
	DWORD ReturnedLength;
	BOOL IoctlResult;
	DWORD IoctlCode;
	#ifndef PMC6_WIN64
		DWORD addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard] + PCI_WRITE_OFFSET + 4;	
	#else
		UINT64 addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard] + PCI_WRITE_OFFSET + 4;	
	#endif
		WORD PCI_StReg;
		WORD data_count4; //data 4 倍數
		WORD data_remain; // 非4倍數的剩餘資料
		//wait write tag low


	#ifdef _NOCARD
		if(g_DevPMC6.m_hHandle[g_DevPMC6.m_dwSelectCard] == NULL)
		{
			return 0;
		}
	#endif

	while((PCI_GetStReg() & 0x0001) == 0x0001)
	{

	}

	if(data_count > 4096)
		return 0;

	PCI_SetDataSize(data_count);
	
	data_count4 = data_count / 4;
	data_remain = data_count % 4;

	IoctlCode = IOCTL_GPD_WRITE_MEMORY_ULONG;
	//4
	for(int i = 0 ; i< data_count4 ; i++)
	{
		#ifndef PMC6_WIN64
				memcpy(copy_buf,&addr,4);
				memcpy(copy_buf + 4,data_buf + (i * 4),4);
				DataLength =8;
				addr+= 4;

				IoctlResult = g_DevPMC6.Dev_DeviceIo(                            
										IoctlCode,          // IO Control code for Read
										copy_buf,				// Buffer to driver.
										DataLength,		// Length of buffer in bytes.
										NULL,        // Buffer from driver.
										0,         // Length of buffer in bytes.
										&ReturnedLength,    // Bytes placed in DataBuffer.
										NULL                // NULL means wait till op. completes.
										);
		#else
				memcpy(copy_buf,&addr,8);
				memcpy(copy_buf + 8,data_buf + (i * 4),4);
				DataLength =12;
				addr+= 4;

				IoctlResult = g_DevPMC6.Dev_DeviceIo(                            
										IoctlCode,          // IO Control code for Read
										copy_buf,				// Buffer to driver.
										DataLength,		// Length of buffer in bytes.
										NULL,        // Buffer from driver.
										0,         // Length of buffer in bytes.
										&ReturnedLength,    // Bytes placed in DataBuffer.
										NULL                // NULL means wait till op. completes.
										);
		#endif
				if(!IoctlResult)
					return IoctlResult;
	}

	if(data_remain > 0)
	{
		IoctlCode = IOCTL_GPD_WRITE_MEMORY_UCHAR;
		//1
		for(int i = 0 ; i< data_remain ; i++)
		{
			#ifndef PMC6_WIN64
					memcpy(copy_buf,&addr,4);
					memcpy(copy_buf + 4,data_buf + (i + data_count4 * 4),1);
					DataLength =5;
					addr++;
					
					IoctlResult = g_DevPMC6.Dev_DeviceIo(IoctlCode,copy_buf,DataLength,NULL,0,&ReturnedLength,NULL);
			#else
					memcpy(copy_buf,&addr,8);
					memcpy(copy_buf + 8,data_buf + (i + data_count4 * 4),1);
					DataLength =9;
					addr++;
					
					IoctlResult = g_DevPMC6.Dev_DeviceIo(IoctlCode,copy_buf,DataLength,NULL,0,&ReturnedLength,NULL);

			#endif
					if(!IoctlResult)
						return IoctlResult;
		}
	}

	PCI_StReg = 0x0001;

	PCI_SetStReg(0,1);

	return 1;
}

WORD PCI_Read(BYTE* data_buf,DWORD data_count)
{
	//Bill_20201216 多卡上限調整修改
	static int iVarInit = 0;
	static WORD index[MAX_ID] = {0,0,0,0};
	WORD dsize,i;
	BYTE bybuf[4];
	DWORD ReturnedLength;
	DWORD IoctlCode;

	//Bill_20201216 多卡上限調整修改
	if(!iVarInit)
	{
		iVarInit = 1;
		for(i = 0;i < MAX_ID;i++)
			index[i] = 0;
	}	
	#ifndef PMC6_WIN64
		DWORD addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard] + PCI_READ_OFFSET + 4 + index[g_DevPMC6.m_dwSelectCard];	
	#else
		UINT64 addr = g_DevPMC6.m_dwMemBases[g_DevPMC6.m_dwSelectCard] + PCI_READ_OFFSET + 4 + index[g_DevPMC6.m_dwSelectCard];	
	#endif
		WORD data_count4;
		WORD data_remain;

	#ifdef _NOCARD
		if(g_DevPMC6.m_hHandle[g_DevPMC6.m_dwSelectCard] == NULL)
		{
			return 1;
		}
	#endif

	//wait write tag low
	if((PCI_GetStReg() & 0x0002) == 0x0002)
	{
		dsize = PCI_GetDataSize();

		if((index[g_DevPMC6.m_dwSelectCard] + data_count) > dsize)
			data_count = dsize - index[g_DevPMC6.m_dwSelectCard];

		index[g_DevPMC6.m_dwSelectCard] += (WORD)data_count;

		data_count4 = (WORD)data_count / 4;
		data_remain = (WORD)data_count % 4;

		i = 0;
		while(i < data_count)
		{
			if(addr % 4 != 0)
			{
				IoctlCode = IOCTL_GPD_READ_MEMORY_UCHAR;
				g_DevPMC6.Dev_DeviceIo(IoctlCode,&addr,sizeof(addr),bybuf,1,&ReturnedLength,NULL);
				*(data_buf + i) = bybuf[0];
				addr ++;
				i++;
			}
			else
			{
				if((DWORD)(i + 4) <= data_count)
				{
					IoctlCode = IOCTL_GPD_READ_MEMORY_ULONG;
					g_DevPMC6.Dev_DeviceIo(IoctlCode,&addr,sizeof(addr),bybuf,4,&ReturnedLength,NULL);
					memcpy(data_buf + i,bybuf,4);
					addr += 4;
					i += 4;
				}
				else
				{
					IoctlCode = IOCTL_GPD_READ_MEMORY_UCHAR;
					g_DevPMC6.Dev_DeviceIo(IoctlCode,&addr,sizeof(addr),bybuf,1,&ReturnedLength,NULL);
					*(data_buf + i) = bybuf[0];
					addr ++;
					i++;
				}
			}
		}

		if(index[g_DevPMC6.m_dwSelectCard] >= dsize)
		{
			index[g_DevPMC6.m_dwSelectCard] = 0;
			PCI_SetStReg(1,0);
		}
	}
	else
		return 0;

	

	return 1;
}
//============================================================
//判斷是否需要回應
BOOL PCI_CheckReplay(unsigned short usCmd)
{
	BOOL bReBack;
	bReBack = FALSE;

	switch (usCmd)
	{
		case 1:
		{
			bReBack = true;
			break;
		}
		
		default:
		{
			break;
		}
	}
	
	return bReBack;
}

BOOL PCI_Write_Datas(unsigned short usCmd, char *pData, unsigned short usSize)
{
	BOOL bRet = TRUE;

	BYTE szExt[256];
	BYTE ReadBuf[256];
	ULONG ReturnLen;
	ULONG ReBack = PCI_CheckReplay(usCmd);
	BOOL Ret = PCI_READ_END;

	memcpy(szExt, &usCmd, 2);
	memcpy(szExt + 2, &usSize, 2);
	if(usSize > 0 && usSize <= 252)
	{
		memcpy(szExt + 4, pData, usSize);
	}

	do
	{
		if(!PCI_TransmitBlock(szExt, 4 + usSize, ReadBuf, sizeof(ReadBuf), &ReturnLen, ReBack))
		{
			bRet = FALSE;
			break;
		}

		if(ReBack)
		{
			if(ReturnLen < 4)
				Ret = PCI_RETRY;
			else
			{
				unsigned short FbCmd;

				memcpy(&FbCmd, ReadBuf, 2);

				if(FbCmd != usCmd)
					Ret = PCI_RETRY;
				else
				{
					unsigned short FbSize;

					memcpy(&FbSize, ReadBuf + 2, 2);
					memset(&g_DevPMC6.m_ReadBuffer[g_DevPMC6.m_dwSelectCard][0], 0, 256);
					memcpy(&g_DevPMC6.m_ReadBuffer[g_DevPMC6.m_dwSelectCard][0], ReadBuf + 4, FbSize);
					Ret = PCI_READ_END;
				}
			}
		}

	}while(Ret == PCI_RETRY);

	return bRet;
}
