/******************************************************************************
 *	File	:	PdoDefine.h
 *	Version :	0.1
 *	Date	:	2020/04/24
 *	Author	:	XFORCE
 *
 *
 *	Define slave device PDO data structure
 *
 *	You can configure the PDO structure you wanted,
 *	or you can just use the default PDO structure.
 *	No matter what, the example program need to know.
 *
 * @copyright (C) 2020 NEXTW TECHNOLOGY CO., LTD.. All rights reserved.
 *
 ******************************************************************************/
#ifndef _ECM_USR_PDO_DEFINE_H_
#define _ECM_USR_PDO_DEFINE_H_

#define RxPDO_ASSIGN_IDX		0x1C12
#define TxPDO_ASSIGN_IDX		0x1C13

#define RxPDO_MAP_IDX			0x1600
#define TxPDO_MAP_IDX			0x1A00

#define RXPDO_SIZE_BYTES		6
#define TXPDO_SIZE_BYTES		6

#ifdef _MSC_VER  
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
#else 
#include <stdint.h>  // for integer typedefs
#endif 

#pragma pack(push,1)
typedef struct _rxpdo_st_def_t
{
	uint16_t u16CtlWord;
	int32_t n32TarPos;
}RXPDO_ST_DEF_T;

typedef struct _txpdo_st_def_t
{
	uint16_t u16StaWord;
	int32_t n32AcuPos;
	uint32_t u32DigInputs;
}TXPDO_ST_DEF_T;
#pragma pack(pop)

#endif
