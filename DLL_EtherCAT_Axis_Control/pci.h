// CMD define
#define CMD_SET_DATASIZE 0
#define CMD_SET_TXDATA 1
#define CMD_SET_SEND 2
#define CMD_GET_BUSY 3
#define CMD_GET_RXDATA 4
#define CMD_SET_PARAMS 5

#ifndef _PCI_
#define _PCI_
BOOL	PCI_Write_Datas(unsigned short usCmd, char *pData, unsigned short usSize);
DWORD	PCI_GetStReg();
BOOL	PCI_SetStReg(WORD bit,WORD On);
#endif
