#include <stdio.h>
#include <string.h>

#include "Parse_EIT.h"
#include "Parse_Descriptor.h"

#define EIT_PID 0x0012
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4092 1024 * 4

#define OUTPUT_PREFIX_SIZE 256

#define PRINTFEID_INFO 1

/******************************************
 *
 * 解析EIT段头部数据
 *
 ******************************************/
void ParseEIT_SectionHead(TS_EIT_T *pstTS_EIT, unsigned char *pucSectionBuffer)
{
	int iEIT_SectionLength = 0;
	
	pstTS_EIT->uiTable_id = pucSectionBuffer[0];
	pstTS_EIT->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_EIT->uiReserved_future_use_first = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_EIT->uiReserved_first = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_EIT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	pstTS_EIT->uiService_id = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_EIT->uiReserved_second = pucSectionBuffer[5] >> 6;
	pstTS_EIT->uiVersion_number = (pucSectionBuffer[5] >> 1) & 0x1f;
	pstTS_EIT->uiCurrent_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_EIT->uiSection_number = pucSectionBuffer[6];
	pstTS_EIT->uiLast_section_number = pucSectionBuffer[7];
	pstTS_EIT->uiTransport_stream_id = (pucSectionBuffer[8] << 8) | pucSectionBuffer[9];
	pstTS_EIT->uiOriginal_network_id = (pucSectionBuffer[10] << 8) | pucSectionBuffer[11];
	pstTS_EIT->uiSegment_last_section_number = pucSectionBuffer[12];
	pstTS_EIT->uiLast_table_id = pucSectionBuffer[13];
	iEIT_SectionLength = 3 + pstTS_EIT->uiSection_length;
	pstTS_EIT->uiCRC_32 = (pucSectionBuffer[iEIT_SectionLength - 4] << 24) | (pucSectionBuffer[iEIT_SectionLength - 3] << 16) | (pucSectionBuffer[iEIT_SectionLength - 2] << 8) | (pucSectionBuffer[iEIT_SectionLength - 1]);
}

/******************************************
 *
 * 解析EIT段数据
 *
 ******************************************/
int ParseEIT_Section(TS_EIT_T *pstTS_EIT, unsigned char *pucSectionBuffer)
{
	int iEIT_SectionLength = 0;
	int iUTC_timePosition = 0;
	int iLoopPosition = 0;
	int iLoopCount = 0;
	
	ParseEIT_SectionHead(pstTS_EIT, pucSectionBuffer);
	iEIT_SectionLength = 3 + pstTS_EIT->uiSection_length;

	for (iLoopPosition = 14; iLoopPosition < iEIT_SectionLength - 4; iLoopPosition += 12)
	{
		pstTS_EIT->astEIT_info[iLoopCount].uiEvent_id = (pucSectionBuffer[iLoopPosition + 0] << 8) | pucSectionBuffer[iLoopPosition + 1];
		for (iUTC_timePosition = 0; iUTC_timePosition < 5; iUTC_timePosition++)
		{
			pstTS_EIT->astEIT_info[iLoopCount].auiStart_time[iUTC_timePosition] = pucSectionBuffer[iLoopPosition + iUTC_timePosition + 2];
		}
		pstTS_EIT->astEIT_info[iLoopCount].uiDuration[0] = pucSectionBuffer[iLoopPosition + 7];
		pstTS_EIT->astEIT_info[iLoopCount].uiDuration[1] = pucSectionBuffer[iLoopPosition + 8];
		pstTS_EIT->astEIT_info[iLoopCount].uiDuration[2] = pucSectionBuffer[iLoopPosition + 9];

		pstTS_EIT->astEIT_info[iLoopCount].uiRunning_status = pucSectionBuffer[iLoopPosition + 10] >> 5;
		pstTS_EIT->astEIT_info[iLoopCount].uiFree_CA_mode = (pucSectionBuffer[iLoopPosition + 10] >> 4) & 0x01;
		pstTS_EIT->astEIT_info[iLoopCount].uiDescriptors_loop_length = ((pucSectionBuffer[iLoopPosition + 10] & 0x0f) << 8) | pucSectionBuffer[iLoopPosition + 11];
		if (pstTS_EIT->astEIT_info[iLoopCount].uiDescriptors_loop_length > 0)
		{
			memcpy(pstTS_EIT->astEIT_info[iLoopCount].aucDescriptor, pucSectionBuffer + iLoopPosition + 12, pstTS_EIT->astEIT_info[iLoopCount].uiDescriptors_loop_length);
			iLoopPosition += pstTS_EIT->astEIT_info[iLoopCount].uiDescriptors_loop_length;
		}
		iLoopCount++;
	}
	return iLoopCount;
}

/******************************************
 *
 * 输出EIT表
 *
 ******************************************/
void PrintEIT(TS_EIT_T *pstTS_EIT, int iEIT_InfoCount)
{
	int iLoopTime = 0;
	SHORT_EVENT_DESCRIPTOR_T stShortEventDescriptor = { 0 };
	EXTENDED_EVENT_DESCRIPTOR_T stExtended_event_descriptor = { 0 };
	char acOutputPrefix[OUTPUT_PREFIX_SIZE] = { 0 };
	
	printf("\n-------------EIT info start-------------\n");
	
	printf("EIT->table_id: 0x%02x\n", pstTS_EIT->uiTable_id);
	printf("EIT->Section_syntax_indicator: 0x%02x\n", pstTS_EIT->uiSection_syntax_indicator);
	printf("EIT->Reserved_future_use_first: 0x%02x\n", pstTS_EIT->uiReserved_future_use_first);
	printf("EIT->Reserved_first: 0x%02x\n", pstTS_EIT->uiReserved_first);
	printf("EIT->Section_length: 0x%02x\n", pstTS_EIT->uiSection_length);
	printf("EIT->Service_id: 0x%02x\n", pstTS_EIT->uiService_id);
	printf("EIT->Reserved_second: 0x%02x\n", pstTS_EIT->uiReserved_second);
	printf("EIT->Version_number: 0x%02x\n", pstTS_EIT->uiVersion_number);
	printf("EIT->Current_next_indicator: 0x%02x\n", pstTS_EIT->uiCurrent_next_indicator);
	printf("EIT->Section_number: 0x%02x\n", pstTS_EIT->uiSection_number);
	printf("EIT->Last_section_number: 0x%02x\n", pstTS_EIT->uiLast_section_number);
	printf("EIT->Transport_stream_id: 0x%02x\n", pstTS_EIT->uiTransport_stream_id);
	printf("EIT->Original_network_id: 0x%02x\n", pstTS_EIT->uiOriginal_network_id);
	printf("EIT->Segment_last_section_number: 0x%02x\n", pstTS_EIT->uiSegment_last_section_number);
	printf("EIT->Last_table_id: 0x%02x\n", pstTS_EIT->uiLast_table_id);
	printf("EIT->CRC_32: 0x%02x\n", pstTS_EIT->uiCRC_32);
	for (iLoopTime = 0; iLoopTime < iEIT_InfoCount; iLoopTime++)
	{
		char acUTC_time[50] = { 0 };
		printf("EIT->EIT_info[%d].Event_id: 0x%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiEvent_id);
		FormatUTC_TimeFormMJD(acUTC_time, pstTS_EIT->astEIT_info[iLoopTime].auiStart_time);
		printf("EIT->EIT_info[%d].Start_time: %s\n", iLoopTime, acUTC_time);
		printf("EIT->EIT_info[%d].Duration: %02x:%02x:%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiDuration[0], pstTS_EIT->astEIT_info[iLoopTime].uiDuration[1], pstTS_EIT->astEIT_info[iLoopTime].uiDuration[2]);
		printf("EIT->EIT_info[%d].Running_status: 0x%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiRunning_status);
		printf("EIT->EIT_info[%d].Free_CA_mode: 0x%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiFree_CA_mode);
		printf("EIT->EIT_info[%d].Descriptors_loop_length: 0x%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiDescriptors_loop_length);
		if (pstTS_EIT->astEIT_info[iLoopTime].uiDescriptors_loop_length > 0)
		{
			memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
			sprintf(acOutputPrefix, "EIT->EIT_info[%d].", iLoopTime);
			if (-1 != GetShortEventDescriptor(&stShortEventDescriptor, pstTS_EIT->astEIT_info[iLoopTime].aucDescriptor, pstTS_EIT->astEIT_info[iLoopTime].uiDescriptors_loop_length))
			{
				Print_ShortEventDescriptor(&stShortEventDescriptor, acOutputPrefix);
			}
			if (-1 != GetExtendedEventDescriptor(&stExtended_event_descriptor, pstTS_EIT->astEIT_info[iLoopTime].aucDescriptor, pstTS_EIT->astEIT_info[iLoopTime].uiDescriptors_loop_length))
			{
				Print_ExtendedEventDescriptor(&stExtended_event_descriptor, acOutputPrefix);
			}
		}
	}
	printf("\n-------------EIT info end-------------\n");
}

/******************************************
 *
 * EIT去重
 *
 ******************************************/

int IsEITSectionGetBefore(unsigned char *pucSectionBuffer, EIT_IDENTIFICATION_INFO_T *past_EIT_identification, int *piEITCount, TS_EIT_T *pstTS_EIT)
{
	int iLoopTime = 0;
	memset(pstTS_EIT, 0, sizeof(TS_EIT_T));
	ParseEIT_SectionHead(pstTS_EIT, pucSectionBuffer);
	for (iLoopTime = 0; iLoopTime < *piEITCount; iLoopTime++)
	{
		if ((past_EIT_identification[iLoopTime].uiTable_id == pstTS_EIT->uiTable_id) && (past_EIT_identification[iLoopTime].uiTransport_stream_id == pstTS_EIT->uiTransport_stream_id)
				&& (past_EIT_identification[iLoopTime].uiSection_number == pstTS_EIT->uiSection_number) && (past_EIT_identification[iLoopTime].uiVersion_number == pstTS_EIT->uiVersion_number)
				&& (past_EIT_identification[iLoopTime].uiService_id == pstTS_EIT->uiService_id))
		{
			return 1;
		}
	}
	past_EIT_identification[*piEITCount].uiTable_id = pstTS_EIT->uiTable_id;
	past_EIT_identification[*piEITCount].uiTransport_stream_id = pstTS_EIT->uiTransport_stream_id;
	past_EIT_identification[*piEITCount].uiSection_number = pstTS_EIT->uiSection_number;
	past_EIT_identification[*piEITCount].uiVersion_number = pstTS_EIT->uiVersion_number;
	past_EIT_identification[*piEITCount].uiService_id = pstTS_EIT->uiService_id;
	(*piEITCount)++;
	
	return 0;
}

/******************************************
 *
 * 从缓存中解析EIT表
 *
 ******************************************/
int ParseEIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, int iEIT_table_id)
{
	printf("\n\n=================================ParseEIT_Table Start================================= \n");
	int iTemp = 0;
	int iEIT_InfoCount = 0;
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4092] = { 0 };
	unsigned int uiRecordGetSection[SECTION_COUNT_256] = { 0 };
	TS_EIT_T stTS_EIT = { 0 };
	EIT_IDENTIFICATION_INFO_T ast_EIT_identification[SECTION_COUNT_256] = { 0 };
	int iEITCount = 0;
	
	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		printf("Parse EIT error\n");
		return -1;
	}
	
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, EIT_PID, iEIT_table_id, &uiVersion);

		if (0 == iTemp)
		{
			uiVersion = INITIAL_VERSION;
			memset(uiRecordGetSection, 0, sizeof(char) * SECTION_COUNT_256);
			fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
		}

		if (1 == iTemp)
		{
			if (0 == IsEITSectionGetBefore(ucSectionBuffer, ast_EIT_identification, &iEITCount, &stTS_EIT))
			{
				iEIT_InfoCount = ParseEIT_Section(&stTS_EIT, ucSectionBuffer);
				if (1 == PRINTFEID_INFO)
				{
					PrintEIT(&stTS_EIT, iEIT_InfoCount);
				}
			}
			/*if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordGetSection))
			 {
			 printf("\n=================================ParseEIT_Table END=================================== \n\n");
			 return 1;
			 }*/
		}
		if (-1 == iTemp)
		{
			printf("iEITCount : %d\n", iEITCount);
			printf("\n=================================ParseEIT_Table END=================================== \n\n");
			return 1;
		}
	}

	printf("\n\n=================================ParseEIT_Table End================================= \n");
	return -1;
}

