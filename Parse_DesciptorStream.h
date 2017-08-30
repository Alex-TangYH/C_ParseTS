/*
 * Parse_DesciptorStream.h
 *
 *  Created on: 2017��8��29��
 *      Author: AlexTang
 */

#ifndef _PARSE_DESCIPTORSTREAM_H_
#define _PARSE_DESCIPTORSTREAM_H_


int ParseDescriptor(unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, char *pacOutputPrefix);
int GetDescriptorTag(int *piTag, int iDescriptorPosition, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength);
int GetDescriptorTagPosition(int *piDescriptorPosition, unsigned char *pucDescriptorBuffer, int iDescriptorBufferLength, int iTagValue);

#endif /* PARSE_DESCIPTORSTREAM_H_ */
