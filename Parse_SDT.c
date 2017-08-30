#include <string.h>
#include <stdio.h>

#include "Parse_SDT.h"
#include "Parse_Descriptor.h"
#include "Parse_DesciptorStream.h"
#include "Get_Section.h"
#include "TsParser.h"

#define SECTION_MAX_LENGTH_4096 1024*4
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256

#define SDT_PID 0x011
#define SDT_TABLE_ID 0x42
#define OUTPUT_PREFIX_SIZE 256

/******************************************
 *
 * 解析SDT段头部数据
 *
 ******************************************/

void ParseSDT_SectionHead(TS_SDT_T *pstTS_SDT, unsigned char *pucSectionBuffer)
{
	int iSDT_SectionLength;
	pstTS_SDT->uiTable_id = pucSectionBuffer[0];
	pstTS_SDT->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_SDT->uiReserved_future_use_first = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_SDT->uiReserved_first = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_SDT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	pstTS_SDT->uiTransport_stream_id = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_SDT->uiReserved_second = pucSectionBuffer[5] >> 6;
	pstTS_SDT->uiVersion_number = (pucSectionBuffer[5] >> 1) & 0x1f;
	pstTS_SDT->uiCurrent_next_indicator = pucSectionBuffer[5] & 0x01;
	pstTS_SDT->uiSection_number = pucSectionBuffer[6];
	pstTS_SDT->uiLast_section_number = pucSectionBuffer[7];
	pstTS_SDT->uiOriginal_network_id = (pucSectionBuffer[8] << 8) | pucSectionBuffer[9];
	pstTS_SDT->uiReserved_future_use_second = pucSectionBuffer[10];

	iSDT_SectionLength = 3 + pstTS_SDT->uiSection_length;
	pstTS_SDT->uiCRC_32 = (pucSectionBuffer[iSDT_SectionLength - 4] << 24) | (pucSectionBuffer[iSDT_SectionLength - 3] << 16) | (pucSectionBuffer[iSDT_SectionLength - 2] << 8) | (pucSectionBuffer[iSDT_SectionLength - 1]);
}

/******************************************
 *
 * 解析SDT段数据
 *
 ******************************************/
int ParseSDT_Section(TS_SDT_T *pstTS_SDT, unsigned char *pucSectionBuffer)
{
	int iSDT_SectionLength = 0;
	int iSectionPostion = 0;
	int iServiceCount = 0;
	
	memset(pstTS_SDT, 0, sizeof(TS_SDT_T));

	ParseSDT_SectionHead(pstTS_SDT, pucSectionBuffer);
	iSDT_SectionLength = 3 + pstTS_SDT->uiSection_length;

	for (iSectionPostion = 11; iSectionPostion < iSDT_SectionLength - 4; iSectionPostion += 5)
	{
		pstTS_SDT->stSDT_info[iServiceCount].uiService_id = (pucSectionBuffer[iSectionPostion] << 8) | pucSectionBuffer[1 + iSectionPostion];
		pstTS_SDT->stSDT_info[iServiceCount].uiReserved_future_use_third = pucSectionBuffer[2 + iSectionPostion] >> 2;
		pstTS_SDT->stSDT_info[iServiceCount].uiEIT_schedule_flag = (pucSectionBuffer[2 + iSectionPostion] >> 1) & 0x01;
		pstTS_SDT->stSDT_info[iServiceCount].uiEIT_present_following_flag = pucSectionBuffer[2 + iSectionPostion] & 0x01;
		pstTS_SDT->stSDT_info[iServiceCount].uiRunning_status = pucSectionBuffer[3 + iSectionPostion] >> 5;
		pstTS_SDT->stSDT_info[iServiceCount].uiFree_CA_mode = (pucSectionBuffer[3 + iSectionPostion] >> 4) & 0x01;
		pstTS_SDT->stSDT_info[iServiceCount].uiDescriptor_loop_length = ((pucSectionBuffer[3 + iSectionPostion] & 0x0f) << 8) | pucSectionBuffer[4 + iSectionPostion];
		if (pstTS_SDT->stSDT_info[iServiceCount].uiDescriptor_loop_length > 0)
		{
			memcpy(pstTS_SDT->stSDT_info[iServiceCount].aucDescriptor, pucSectionBuffer + iSectionPostion + 5, pstTS_SDT->stSDT_info[iServiceCount].uiDescriptor_loop_length);
			iSectionPostion += pstTS_SDT->stSDT_info[iServiceCount].uiDescriptor_loop_length;
		}
		
		iServiceCount++;
	}

	return iServiceCount;
}

/******************************************
 *
 * 输出SDT表
 *
 ******************************************/
void PrintSDT(TS_SDT_T *pstTS_SDT, int iServiceCount)
{
	int iServiceLoopTime = 0;
	char acOutputPrefix[OUTPUT_PREFIX_SIZE] = { 0 };
	
	DUBUGPRINTF("SDT.Table_id: 0x%02x\n", pstTS_SDT->uiTable_id);
	DUBUGPRINTF("SDT.Section_syntax_indicator: 0x%02x\n", pstTS_SDT->uiSection_syntax_indicator);
	DUBUGPRINTF("SDT.Reserved_future_use_first: 0x%02x\n", pstTS_SDT->uiReserved_future_use_first);
	DUBUGPRINTF("SDT.Reserved_first: 0x%02x\n", pstTS_SDT->uiReserved_first);
	DUBUGPRINTF("SDT.Section_length: 0x%02x\n", pstTS_SDT->uiSection_length);
	DUBUGPRINTF("SDT.Transport_stream_id: 0x%02x\n", pstTS_SDT->uiTransport_stream_id);
	DUBUGPRINTF("SDT.Reserved_second: 0x%02x\n", pstTS_SDT->uiReserved_second);
	DUBUGPRINTF("SDT.Version_number: 0x%02x\n", pstTS_SDT->uiVersion_number);
	DUBUGPRINTF("SDT.Current_next_indicator: 0x%02x\n", pstTS_SDT->uiCurrent_next_indicator);
	DUBUGPRINTF("SDT.Section_number: 0x%02x\n", pstTS_SDT->uiSection_number);
	DUBUGPRINTF("SDT.Last_section_number: 0x%02x\n", pstTS_SDT->uiLast_section_number);
	DUBUGPRINTF("SDT.Original_network_id: 0x%02x\n", pstTS_SDT->uiOriginal_network_id);
	DUBUGPRINTF("SDT.Reserved_future_use_second: 0x%02x\n", pstTS_SDT->uiReserved_future_use_second);
	DUBUGPRINTF("SDT.CRC_32: 0x%08lx\n", pstTS_SDT->uiCRC_32);
	
	for (iServiceLoopTime = 0; iServiceLoopTime < iServiceCount; iServiceLoopTime++)
	{
		DUBUGPRINTF("SDT.SDT_info[%d].Service_id: 0x%02x\n", iServiceLoopTime, pstTS_SDT->stSDT_info[iServiceLoopTime].uiService_id);
		DUBUGPRINTF("SDT.SDT_info[%d].Reserved_future_use_third: 0x%02x\n", iServiceLoopTime, pstTS_SDT->stSDT_info[iServiceLoopTime].uiReserved_future_use_third);
		DUBUGPRINTF("SDT.SDT_info[%d].EIT_schedule_flag: 0x%02x\n", iServiceLoopTime, pstTS_SDT->stSDT_info[iServiceLoopTime].uiEIT_schedule_flag);
		DUBUGPRINTF("SDT.SDT_info[%d].EIT_present_following_flag: 0x%02x\n", iServiceLoopTime, pstTS_SDT->stSDT_info[iServiceLoopTime].uiEIT_present_following_flag);
		DUBUGPRINTF("SDT.SDT_info[%d].Running_status: 0x%02x\n", iServiceLoopTime, pstTS_SDT->stSDT_info[iServiceLoopTime].uiRunning_status);
		DUBUGPRINTF("SDT.SDT_info[%d].Free_CA_mode: 0x%02x\n", iServiceLoopTime, pstTS_SDT->stSDT_info[iServiceLoopTime].uiFree_CA_mode);
		DUBUGPRINTF("SDT.SDT_info[%d].Descriptor_loop_length: 0x%02x\n", iServiceLoopTime, pstTS_SDT->stSDT_info[iServiceLoopTime].uiDescriptor_loop_length);

		if (pstTS_SDT->stSDT_info[iServiceLoopTime].uiDescriptor_loop_length > 0)
		{
			memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
			sprintf(acOutputPrefix, "SDT.SDT_info[%d].", iServiceLoopTime);
			ParseDescriptor(pstTS_SDT->stSDT_info[iServiceLoopTime].aucDescriptor, pstTS_SDT->stSDT_info[iServiceLoopTime].uiDescriptor_loop_length, acOutputPrefix);
		}
	}
}

/******************************************
 *
 * SDT去重
 *
 ******************************************/
int IsSDTSectionGetBefore(unsigned char *pucSectionBuffer, SDT_IDENTIFICATION_INFO_T *past_SDT_identification, int *piSDTCount, TS_SDT_T *pstTS_SDT)

{
	memset(pstTS_SDT, 0, sizeof(TS_SDT_T));

	ParseSDT_SectionHead(pstTS_SDT, pucSectionBuffer);
	
	int iLoopTime = 0;
	
	for (iLoopTime = 0; iLoopTime < *piSDTCount; iLoopTime++)
	{
		if ((past_SDT_identification[iLoopTime].uiTable_id == pstTS_SDT->uiTable_id) && (past_SDT_identification[iLoopTime].uiTransport_stream_id == pstTS_SDT->uiTransport_stream_id)
				&& (past_SDT_identification[iLoopTime].uiSection_number == pstTS_SDT->uiSection_number) && (past_SDT_identification[iLoopTime].uiVersion_number == pstTS_SDT->uiVersion_number)
				&& (past_SDT_identification[iLoopTime].uiOriginal_network_id == pstTS_SDT->uiOriginal_network_id))
		{
			return 1;
		}
	}
	
	past_SDT_identification[*piSDTCount].uiTable_id = pstTS_SDT->uiTable_id;
	past_SDT_identification[*piSDTCount].uiTransport_stream_id = pstTS_SDT->uiTransport_stream_id;
	past_SDT_identification[*piSDTCount].uiSection_number = pstTS_SDT->uiSection_number;
	past_SDT_identification[*piSDTCount].uiVersion_number = pstTS_SDT->uiVersion_number;
	past_SDT_identification[*piSDTCount].uiOriginal_network_id = pstTS_SDT->uiOriginal_network_id;
	(*piSDTCount)++;
	
	return 0;
}

/******************************************
 *
 * 从TS流中解析SDT表
 *
 ******************************************/
int ParseSDT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	DUBUGPRINTF("\n\n=================================ParseSDT_Table Start================================= \n");
	TS_SDT_T stTS_SDT = { 0 };
	int iTemp = 0;
	int iServiceCount = 0;
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4096] = { 0 };
	SDT_IDENTIFICATION_INFO_T ast_SDT_identification[SECTION_COUNT_256] = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	int iSDTCount = 0;

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("ParseSDT_Table error when fseek(pfTsFile, iTsPosition, SEEK_SET)\n");
		return -1;
	}

	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, SDT_PID, SDT_TABLE_ID, &uiVersion);
		switch (iTemp)
		{
			case 0:
				uiVersion = INITIAL_VERSION;
				fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
				break;
			case 1:
				if (0 == IsSDTSectionGetBefore(ucSectionBuffer, ast_SDT_identification, &iSDTCount, &stTS_SDT))
				{
					iServiceCount = ParseSDT_Section(&stTS_SDT, ucSectionBuffer);
					PrintSDT(&stTS_SDT, iServiceCount);
				}
				break;
			case 2:
				break;
			case -1:
				return 1;
				break;
			default:
				LOG("ParseSDT_Table switch (iTemp) default\n");
				break;
		}
	}
	DUBUGPRINTF("\n\n=================================ParseSDT_Table end================================= \n");
	return 0;
}

