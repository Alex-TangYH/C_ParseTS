/*
 * DVB_CRC32.c
 *
 *  Created on: 2017年8月30日
 *      Author: Administrator
 */

#include "DVB_CRC32.h"
#include "TsParser.h"

static unsigned long osgCalCrc(unsigned long byte, unsigned long crc);

static unsigned long osgCalCrc(unsigned long byte, unsigned long crc)
{
	static unsigned long mask_CRC[16] = { 0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005, 0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd };
	crc = (crc << 4) ^ mask_CRC[((crc >> 28) ^ (byte >> 4)) & 0x0f];
	crc = (crc << 4) ^ mask_CRC[((crc >> 28) ^ (byte)) & 0x0f];
	return crc;
}

unsigned long OsgCalSectionCRC(unsigned char *pDataBuffer, unsigned char startIndex, unsigned short calLength)
{
	unsigned long CRC = 0xffffffff;
	unsigned char *pDataBlock = (pDataBuffer + startIndex);
	int loop;
	for (loop = 0; loop < calLength; loop++)
	{
		CRC = osgCalCrc(*pDataBlock++, CRC);
	}
	return CRC;
}

/******************************************
 *
 * CRC32校验
 * 1:CRC32一致
 * 0:CRC32不一致
 ******************************************/
int Verify_CRC_32(unsigned char *pucSectionBuffer)
{
	int iLength = 0;
	int iSectionLength = 0;
	unsigned long Cal_CRC_32 = 0xffffffff;
	unsigned long Section_CRC_32 = 0x00000000;
	iLength = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	iSectionLength = iLength + 3;
	Cal_CRC_32 = OsgCalSectionCRC(pucSectionBuffer, 0, (unsigned short) (iSectionLength - 4));
	Section_CRC_32 = (pucSectionBuffer[iSectionLength - 4] << 24) | (pucSectionBuffer[iSectionLength - 3] << 16) | (pucSectionBuffer[iSectionLength - 2] << 8) | (pucSectionBuffer[iSectionLength - 1]);
	if (Cal_CRC_32 == Section_CRC_32)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
