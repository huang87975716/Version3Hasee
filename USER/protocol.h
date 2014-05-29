/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : protocol.h
* Abstract    : 通讯协议的相关定义
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
*
*****************************************************************************
*/

#include "stdint.h"
#include "stm32f10x.h"

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

	typedef struct tagProtocol
	{
			uint8_t header;
			uint8_t command;
			uint8_t data[4];
			uint8_t checksum;
			uint8_t dataIndex;
			uint8_t protocol_ok;
			
	}PROTOCOL_t;   
#endif
