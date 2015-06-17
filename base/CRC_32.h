//================================================================
//	CRC-32 params:
//		Name : "CRC-32"
//		Width : 32
//		Poly : 04C11DB7
//		Init : FFFFFFFF
//		RefIn : True
//		RefOut : True
//		XorOut : FFFFFFFF
//		Check : CBF43926
//	Author: baobao
//	Time: 2015/6/14
//==================================================================
#ifndef CRC_32_H
#define CRC_32_H

#include <vector>

#include "../WirelessType.h"
#include "help_functions.h"

#define INIT  0xFFFFFFFF
#define XOROT 0xFFFFFFFF
#define POLY  0x04C11DB7

class crc32er {
public:
	crc32er();
	~crc32er();

	/* CRC-32 查询表（与表生成发二选一使用） */
	static const uint32_t CRC32_Table[256];

	/* 计算 CRC-32 的值 */
	uint32_t calculate_CRC32 (uint32_t *pStart, size_t uSize);
	uint32_t calculate_CRC32 (bvec message);

private:
	/* 比特反转（配合 CRC-32 表生成函数使用） */
	static uint32_t Reflect(uint32_t ref, char len);

	/* 生成 CRC-32 的查询表 */
	static uint32_t* genTable32();
};
#endif

