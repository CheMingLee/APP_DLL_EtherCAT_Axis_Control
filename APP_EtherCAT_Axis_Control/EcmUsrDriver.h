/******************************************************************************
 *	File	:	EcmUsrDriver.h
 *	Version :	0.1
 *	Date	:	2020/04/24
 *	Author	:	XFORCE
 *
 *	ECM-XF basic driver example - Header file
 *
 *	Demonstrate how to implement API type user driver
 *
 * @copyright (C) 2020 NEXTW TECHNOLOGY CO., LTD.. All rights reserved.
 *
 ******************************************************************************/

#ifndef _ECM_USR_DRV_H_
#define _ECM_USR_DRV_H_

#include <stdio.h>
#include <string.h>
#include "EcmDriver.h"
#include "PdoDefine.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TEST_CYCTIME			250000
#define TEST_SERVO_CNT			2
#define TEST_SPI_DATA_SIZE		PKG_DATA_DEFAULT_SIZE
#define TEST_FIFO_CNT			1

#define TEST_RXPDO_SIZE			6
#define TEST_RXPDO_BUF_SIZE		(TEST_RXPDO_SIZE*TEST_SERVO_CNT)

#pragma pack(push,1)
typedef struct spi_cmd_package_t{
	SPI_CMD_HEADER	Head;
	uint8_t			Data[TEST_SPI_DATA_SIZE];
	uint32_t		Crc;
	uint32_t		StopWord;
} SPI_CMD_PACKAGE_T;
typedef struct spi_ret_package_t{
	SPI_RET_HEADER	Head;
	uint8_t			Data[TEST_SPI_DATA_SIZE];
	uint32_t		Crc;
	uint32_t		StopWord;
} SPI_RET_PACKAGE_T;
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
