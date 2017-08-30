#include <stdio.h>
#include <string.h>

#include "Parse_BAT.h"
#include "Parse_Descriptor.h"
#include "Parse_DesciptorStream.h"
#include "Get_Section.h"
#include "TsParser.h"

#define BAT_PID 0x0011
#define BAT_TABLE_ID 0x4A
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4092 1024 * 4
#define OUTPUT_PREFIX_SIZE 256

/******************************************
 *
 * 解析BAT段头部数据
 *
 ******************************************/
void ParseBAT_SectionHead(TS_BAT_T *pstTS_BAT, unsigned char *pucSectionBuffer)
{
	int iBAT_SectionLength = 0;
	pstTS_BAT->uitable_id = pucSectionBuffer[0];
	pstTS_BAT->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_BAT->uiReserved_future_use_first = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_BAT->uiReserved_first = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_BAT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	pstTS_BAT->uiBoquet_id = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_BAT->uiReserved_second = pucSectionBuffer[5] >> 6;
	pstTS_BAT->uiVersion_number = (pucSectionBuffer[5] >> 1) & 0x1f;
	pstTS_BAT->uiCurrent_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_BAT->uiSection_number = pucSectionBuffer[6];
	pstTS_BAT->uiLast_section_number = pucSectionBuffer[7];
	pstTS_BAT->uiReserved_future_use_second = pucSectionBuffer[8] >> 4;
	pstTS_BAT->uiBoquet_descriptor_length = ((pucSectionBuffer[8] & 0x0f) << 8) | pucSectionBuffer[9];

	if (pstTS_BAT->uiBoquet_descriptor_length > 0)
	{
		memcpy(pstTS_BAT->aucDescriptor, pucSectionBuffer + 10, pstTS_BAT->uiBoquet_descriptor_length);
	}
	pstTS_BAT->uiReserved_third = pucSectionBuffer[10 + pstTS_BAT->uiBoquet_descriptor_length] >> 4;
	pstTS_BAT->uiTransport_stream_loop_lenth = ((pucSectionBuffer[10 + pstTS_BAT->uiBoquet_descriptor_length] & 0x0f) << 8) | pucSectionBuffer[11 + pstTS_BAT->uiBoquet_descriptor_length];

	iBAT_SectionLength = 3 + pstTS_BAT->uiSection_length;
	pstTS_BAT->uiCRC_32 = (pucSectionBuffer[iBAT_SectionLength - 4] << 24) | (pucSectionBuffer[iBAT_SectionLength - 3] << 16) | (pucSectionBuffer[iBAT_SectionLength - 2] << 8) | (pucSectionBuffer[iBAT_SectionLength - 1]);

}

/******************************************
 *
 * 解析BAT段数据
 *
 ******************************************/
int ParseBAT_Section(TS_BAT_T *pstTS_BAT, unsigned char *pucSectionBuffer)
{
	int iBAT_SectionLength = 0;
	int iLoopPosition = 0;
	int iLoopCount = 0;
	ParseBAT_SectionHead(pstTS_BAT, pucSectionBuffer);
	iBAT_SectionLength = 3 + pstTS_BAT->uiSection_length;
	for (iLoopPosition = 10 + pstTS_BAT->uiBoquet_descriptor_length + 2; iLoopPosition < iBAT_SectionLength - 4; iLoopPosition += 6)
	{
		pstTS_BAT->stBAT_info[iLoopCount].uiTransport_stream_id = (pucSectionBuffer[iLoopPosition + 0] << 8) | pucSectionBuffer[iLoopPosition + 1];
		pstTS_BAT->stBAT_info[iLoopCount].uiOriginal_network_id = (pucSectionBuffer[iLoopPosition + 2] << 8) | pucSectionBuffer[iLoopPosition + 3];
		pstTS_BAT->stBAT_info[iLoopCount].uiReserved_future_use_third = pucSectionBuffer[iLoopPosition + 4] >> 4;
		pstTS_BAT->stBAT_info[iLoopCount].uiTransport_descriptor_length = ((pucSectionBuffer[iLoopPosition + 4] & 0x0f) << 8) | pucSectionBuffer[iLoopPosition + 5];
		if (pstTS_BAT->stBAT_info[iLoopCount].uiTransport_descriptor_length > 0)
		{
			memcpy(pstTS_BAT->stBAT_info[iLoopCount].aucDescriptor, pucSectionBuffer + iLoopPosition + 6, pstTS_BAT->stBAT_info[iLoopCount].uiTransport_descriptor_length);
			iLoopPosition += pstTS_BAT->stBAT_info[iLoopCount].uiTransport_descriptor_length;
		}
		iLoopCount++;
	}
	return iLoopCount;
}

/******************************************
 *
 * 输出BAT表
 *
 ******************************************/
void PrintBAT(TS_BAT_T *pstTS_BAT, int iBAT_InfoCount)
{
	int iLoopTime = 0;
	char acOutputPrefix[OUTPUT_PREFIX_SIZE] = { 0 };
	
	DUBUGPRINTF("\n-------------BAT info start-------------\n");
	
	DUBUGPRINTF("BAT->table_id: 0x%02x\n", pstTS_BAT->uitable_id);
	DUBUGPRINTF("BAT->Section_syntax_indicator: 0x%02x\n", pstTS_BAT->uiSection_syntax_indicator);
	DUBUGPRINTF("BAT->Reserved_future_use_first: 0x%02x\n", pstTS_BAT->uiReserved_future_use_first);
	DUBUGPRINTF("BAT->Reserved_first: 0x%02x\n", pstTS_BAT->uiReserved_first);
	DUBUGPRINTF("BAT->Section_length: 0x%02x\n", pstTS_BAT->uiSection_length);
	DUBUGPRINTF("BAT->Boquet_id: 0x%02x\n", pstTS_BAT->uiBoquet_id);
	DUBUGPRINTF("BAT->Reserved_second: 0x%02x\n", pstTS_BAT->uiReserved_second);
	DUBUGPRINTF("BAT->Version_number: 0x%02x\n", pstTS_BAT->uiVersion_number);
	DUBUGPRINTF("BAT->Current_next_indicator: 0x%02x\n", pstTS_BAT->uiCurrent_next_indicator);
	DUBUGPRINTF("BAT->Section_number: 0x%02x\n", pstTS_BAT->uiSection_number);
	DUBUGPRINTF("BAT->Last_section_number: 0x%02x\n", pstTS_BAT->uiLast_section_number);
	DUBUGPRINTF("BAT->Reserved_future_use_second: 0x%02x\n", pstTS_BAT->uiReserved_future_use_second);
	DUBUGPRINTF("BAT->Boquet_descriptor_length: 0x%02x\n", pstTS_BAT->uiBoquet_descriptor_length);
	
	if (pstTS_BAT->uiBoquet_descriptor_length > 0)
	{
		memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
		sprintf(acOutputPrefix, "BAT->");
		ParseDescriptor(pstTS_BAT->aucDescriptor, pstTS_BAT->uiBoquet_descriptor_length, acOutputPrefix);
	}
	
	DUBUGPRINTF("BAT->Reserved_future_use_second: 0x%02x\n", pstTS_BAT->uiReserved_future_use_second);
	DUBUGPRINTF("BAT->Transport_stream_loop_lenth: 0x%02x\n", pstTS_BAT->uiTransport_stream_loop_lenth);
	DUBUGPRINTF("BAT->CRC_32: 0x%08lx\n", pstTS_BAT->uiCRC_32);

	for (iLoopTime = 0; iLoopTime < iBAT_InfoCount; iLoopTime++)
	{
		DUBUGPRINTF("BAT->BAT_info[%d].Transport_stream_id: 0x%02x\n", iLoopTime, pstTS_BAT->stBAT_info[iLoopTime].uiTransport_stream_id);
		DUBUGPRINTF("BAT->BAT_info[%d].Original_network_id: 0x%02x\n", iLoopTime, pstTS_BAT->stBAT_info[iLoopTime].uiOriginal_network_id);
		DUBUGPRINTF("BAT->BAT_info[%d].Reserved_future_use_third: 0x%02x\n", iLoopTime, pstTS_BAT->stBAT_info[iLoopTime].uiReserved_future_use_third);
		DUBUGPRINTF("BAT->BAT_info[%d].Transport_descriptor_length: 0x%02x\n", iLoopTime, pstTS_BAT->stBAT_info[iLoopTime].uiTransport_descriptor_length);
		
		if (pstTS_BAT->uiBoquet_descriptor_length > 0)
		{
			memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
			sprintf(acOutputPrefix, "BAT->BAT_info[%d].", iLoopTime);
			ParseDescriptor(pstTS_BAT->stBAT_info[iLoopTime].aucDescriptor, pstTS_BAT->stBAT_info[iLoopTime].uiTransport_descriptor_length, acOutputPrefix);
		}
	}
	DUBUGPRINTF("\n-------------BAT info end-------------\n");
}

/******************************************
 *
 * 从缓存中解析BAT表
 *
 ******************************************/
int ParseBAT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	DUBUGPRINTF("\n\n=================================ParseBAT_Table Start================================= \n");
	int iTemp = 0;
	int iBAT_InfoCount = 0;
	TS_BAT_T stTS_BAT = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4092] = { 0 };
	unsigned int uiRecordGetSection[SECTION_COUNT_256] = { 0 };
	
	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("Parse BAT error\n");
		return -1;
	}
	
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, BAT_PID, BAT_TABLE_ID, &uiVersion);
		switch (iTemp)
		{
			case 0:
				uiVersion = INITIAL_VERSION;
				memset(uiRecordGetSection, 0, sizeof(char) * SECTION_COUNT_256);
				fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
				break;
			case 1:
				if (0 == IsSectionGetBefore(ucSectionBuffer, uiRecordGetSection))
				{
					iBAT_InfoCount = ParseBAT_Section(&stTS_BAT, ucSectionBuffer);
					PrintBAT(&stTS_BAT, iBAT_InfoCount);
				}
				if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordGetSection))
				{
					DUBUGPRINTF("\n=================================ParseBAT_Table END=================================== \n\n");
					return 1;
				}
				break;
			case 2:
				break;
			case -1:
				DUBUGPRINTF("\n=================================ParseBAT_Table END=================================== \n\n");
				return 1;
				break;
			default:
				LOG("ParseBAT_Table switch (iTemp) default\n");
				break;
		}
	}

	DUBUGPRINTF("\n\n=================================ParseBAT_Table End================================= \n");
	return -1;
}
