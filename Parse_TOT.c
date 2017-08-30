#include <stdio.h>
#include <string.h>

#include "Parse_TOT.h"
#include "Get_Section.h"
#include "FormatUtils.h"
#include "TsParser.h"
#include "Parse_DesciptorStream.h"

#define TOT_PID 0x0014
#define TOT_TABLE_ID 0x73
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4092 1024 * 4
#define OUTPUT_PREFIX_SIZE 256

void ParseTOT_Section(TS_TOT_T *pstTS_TOT, unsigned char *pucSectionBuffer)
{
	int iTOT_length = 0;
	int iUTC_timePosition = 0;
	pstTS_TOT->uitable_id = pucSectionBuffer[0];
	pstTS_TOT->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_TOT->uiReserved_future_use = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_TOT->uiReserved_first = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_TOT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	
	for (iUTC_timePosition = 0; iUTC_timePosition < 5; iUTC_timePosition++)
	{
		pstTS_TOT->uiUTC_time[iUTC_timePosition] = pucSectionBuffer[iUTC_timePosition + 3];
	}
	pstTS_TOT->uiReserved_second = pucSectionBuffer[8] >> 4;
	pstTS_TOT->uiDescriptors_loop_length = ((pucSectionBuffer[8] & 0x0f) << 8) | pucSectionBuffer[9];
	memcpy(pstTS_TOT->aucDescriptor, pucSectionBuffer + 10, pstTS_TOT->uiDescriptors_loop_length);
	iTOT_length = 3 + pstTS_TOT->uiSection_length;
	pstTS_TOT->uiCRC_32 = (pucSectionBuffer[iTOT_length - 4] << 24) | (pucSectionBuffer[iTOT_length - 3] << 16) | (pucSectionBuffer[iTOT_length - 2] << 8) | (pucSectionBuffer[iTOT_length - 1]);
	
}

void PrintTOT(TS_TOT_T *pstTS_TOT)
{
	char acOutputPrefix[OUTPUT_PREFIX_SIZE] = { 0 };
	DUBUGPRINTF("\n-------------TOT info start-------------\n");
	DUBUGPRINTF("TOT->table_id: 0x%02x\n", pstTS_TOT->uitable_id);
	DUBUGPRINTF("TOT->Section_syntax_indicator: 0x%02x\n", pstTS_TOT->uiSection_syntax_indicator);
	DUBUGPRINTF("TOT->Reserved_future_use: 0x%02x\n", pstTS_TOT->uiReserved_future_use);
	DUBUGPRINTF("TOT->Reserved_first: 0x%02x\n", pstTS_TOT->uiReserved_first);
	DUBUGPRINTF("TOT->Section_length: 0x%02x\n", pstTS_TOT->uiSection_length);
	
	char acUTC_time[50] = { 0 };
	FormatUTC_TimeFormMJD(acUTC_time, pstTS_TOT->uiUTC_time);
	DUBUGPRINTF("TOT->UTC_time: %s\n", acUTC_time);
	DUBUGPRINTF("TOT->Reserved_second: 0x%02x\n", pstTS_TOT->uiReserved_second);
	DUBUGPRINTF("TOT->Descriptors_loop_length: 0x%02x\n", pstTS_TOT->uiDescriptors_loop_length);
	if (pstTS_TOT->uiDescriptors_loop_length > 0)
	{
		memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
		sprintf(acOutputPrefix, "TOT->");
		ParseDescriptor(pstTS_TOT->aucDescriptor, pstTS_TOT->uiDescriptors_loop_length, acOutputPrefix);
	}
	else
	{
	}
	DUBUGPRINTF("TOT->CRC_32: 0x%08lx\n", pstTS_TOT->uiCRC_32);
	DUBUGPRINTF("\n-------------TOT info end-------------\n");
}

int ParseTOT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	DUBUGPRINTF("\n\n=================================ParseTOT_Table Start================================= \n");
	int iTemp = 0;
	TS_TOT_T stTS_TOT = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4092] = { 0 };
	unsigned int uiRecordGetSection[SECTION_COUNT_256] = { 0 };
	
	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("Parse TOT error\n");
		return -1;
	}
	
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, TOT_PID, TOT_TABLE_ID, &uiVersion);
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
					ParseTOT_Section(&stTS_TOT, ucSectionBuffer);
					PrintTOT(&stTS_TOT);
					DUBUGPRINTF("\n=================================ParseTOT_Table END=================================== \n\n");
					return 1;
				}
				break;
			case 2:
				break;
			case -1:
				DUBUGPRINTF("\n=================================ParseTOT_Table END=================================== \n\n");
				return 1;
				break;
			default:
				LOG("ParseTOT_Table switch (iTemp) default\n");
				break;
		}
	}
	DUBUGPRINTF("\n\n=================================ParseTOT_Table End================================= \n");
	return -1;
}

