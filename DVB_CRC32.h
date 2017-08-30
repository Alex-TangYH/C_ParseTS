/*
 * DVB_CRC32.h
 *
 *  Created on: 2017年8月30日
 *      Author: Administrator
 */

#ifndef _DVB_CRC32_H_
#define _DVB_CRC32_H_

unsigned long OsgCalSectionCRC(unsigned char *pDataBuffer, unsigned char startIndex, unsigned short calLength);
int Verify_CRC_32(unsigned char *pucSectionBuffer);

#endif /* _DVB_CRC32_H_ */
