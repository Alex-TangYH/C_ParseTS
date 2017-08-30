#include <stdio.h>
#include <string.h>

#include "Parse_EIT.h"
#include "Parse_Descriptor.h"
#include "FormatUtils.h"
#include "Parse_DesciptorStream.h"
#include "Get_Section.h"
#include "TsParser.h"

#define EIT_PID 0x0012
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4092 1024 * 4
#define OUTPUT_PREFIX_SIZE 256
#define PRINTFEID_INFO 1

/******************************************
 *
 * ����EIT��ͷ������
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
 * ����EIT������
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
 * ���EIT��
 *
 ******************************************/
void PrintEIT(TS_EIT_T *pstTS_EIT, int iEIT_InfoCount)
{
	int iLoopTime = 0;
	char acOutputPrefix[OUTPUT_PREFIX_SIZE] = { 0 };
	
	DUBUGPRINTF("\n-------------EIT info start-------------\n");
	
	DUBUGPRINTF("EIT->table_id: 0x%02x\n", pstTS_EIT->uiTable_id);
	DUBUGPRINTF("EIT->Section_syntax_indicator: 0x%02x\n", pstTS_EIT->uiSection_syntax_indicator);
	DUBUGPRINTF("EIT->Reserved_future_use_first: 0x%02x\n", pstTS_EIT->uiReserved_future_use_first);
	DUBUGPRINTF("EIT->Reserved_first: 0x%02x\n", pstTS_EIT->uiReserved_first);
	DUBUGPRINTF("EIT->Section_length: 0x%02x\n", pstTS_EIT->uiSection_length);
	DUBUGPRINTF("EIT->Service_id: 0x%02x\n", pstTS_EIT->uiService_id);
	DUBUGPRINTF("EIT->Reserved_second: 0x%02x\n", pstTS_EIT->uiReserved_second);
	DUBUGPRINTF("EIT->Version_number: 0x%02x\n", pstTS_EIT->uiVersion_number);
	DUBUGPRINTF("EIT->Current_next_indicator: 0x%02x\n", pstTS_EIT->uiCurrent_next_indicator);
	DUBUGPRINTF("EIT->Section_number: 0x%02x\n", pstTS_EIT->uiSection_number);
	DUBUGPRINTF("EIT->Last_section_number: 0x%02x\n", pstTS_EIT->uiLast_section_number);
	DUBUGPRINTF("EIT->Transport_stream_id: 0x%02x\n", pstTS_EIT->uiTransport_stream_id);
	DUBUGPRINTF("EIT->Original_network_id: 0x%02x\n", pstTS_EIT->uiOriginal_network_id);
	DUBUGPRINTF("EIT->Segment_last_section_number: 0x%02x\n", pstTS_EIT->uiSegment_last_section_number);
	DUBUGPRINTF("EIT->Last_table_id: 0x%02x\n", pstTS_EIT->uiLast_table_id);
	DUBUGPRINTF("EIT->CRC_32: 0x%08lx\n", pstTS_EIT->uiCRC_32);
	for (iLoopTime = 0; iLoopTime < iEIT_InfoCount; iLoopTime++)
	{
		char acUTC_time[50] = { 0 };
		DUBUGPRINTF("EIT->EIT_info[%d].Event_id: 0x%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiEvent_id);
		FormatUTC_TimeFormMJD(acUTC_time, pstTS_EIT->astEIT_info[iLoopTime].auiStart_time);
		DUBUGPRINTF("EIT->EIT_info[%d].Start_time: %s\n", iLoopTime, acUTC_time);
		DUBUGPRINTF("EIT->EIT_info[%d].Duration: %02x:%02x:%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiDuration[0], pstTS_EIT->astEIT_info[iLoopTime].uiDuration[1], pstTS_EIT->astEIT_info[iLoopTime].uiDuration[2]);
		DUBUGPRINTF("EIT->EIT_info[%d].Running_status: 0x%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiRunning_status);
		DUBUGPRINTF("EIT->EIT_info[%d].Free_CA_mode: 0x%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiFree_CA_mode);
		DUBUGPRINTF("EIT->EIT_info[%d].Descriptors_loop_length: 0x%02x\n", iLoopTime, pstTS_EIT->astEIT_info[iLoopTime].uiDescriptors_loop_length);
		if (pstTS_EIT->astEIT_info[iLoopTime].uiDescriptors_loop_length > 0)
		{
			memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
			sprintf(acOutputPrefix, "EIT->EIT_info[%d].", iLoopTime);
			ParseDescriptor(pstTS_EIT->astEIT_info[iLoopTime].aucDescriptor, pstTS_EIT->astEIT_info[iLoopTime].uiDescriptors_loop_length, acOutputPrefix);
		}
	}
	DUBUGPRINTF("\n-------------EIT info end-------------\n");
}

/******************************************
 *
 * EITȥ��
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
 * �ӻ����н���EIT��
 *
 ******************************************/
int ParseEIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, int iEIT_table_id)
{
	DUBUGPRINTF("\n\n=================================ParseEIT_Table Start================================= \n");
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
		DUBUGPRINTF("Parse EIT error\n");
		return -1;
	}
	
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, EIT_PID, iEIT_table_id, &uiVersion);
		switch (iTemp)
		{
			case 0:
				uiVersion = INITIAL_VERSION;
				memset(uiRecordGetSection, 0, sizeof(char) * SECTION_COUNT_256);
				fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
				break;
			case 1:
				if (0 == IsEITSectionGetBefore(ucSectionBuffer, ast_EIT_identification, &iEITCount, &stTS_EIT))
				{
					iEIT_InfoCount = ParseEIT_Section(&stTS_EIT, ucSectionBuffer);
					if (1 == PRINTFEID_INFO)
					{
						PrintEIT(&stTS_EIT, iEIT_InfoCount);
					}
				}
				//TODO ������ǰֹͣ��������
				break;
			case 2:
				break;
			case -1:
				DUBUGPRINTF("iEITCount : %d\n", iEITCount);
				DUBUGPRINTF("\n=================================ParseEIT_Table END=================================== \n\n");
				return 1;
				break;
			default:
				LOG("ParseEIT_Table switch (iTemp) default\n");
				break;
		}
	}
	DUBUGPRINTF("\n\n=================================ParseEIT_Table End================================= \n");
	return -1;
}

