#include <stdio.h>
#include <string.h>

#include "Parse_TDT.h"
#include "TsParser.h"
#include "FormatUtils.h"
#include "Get_Section.h"

#define TDT_PID 0x0014
#define TDT_TABLE_ID 0x70
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4092 1024 * 4

void ParseTDT_Section(TS_TDT_T *pstTS_TDT, unsigned char *pucSectionBuffer)
{
	int iUTC_timePosition = 0;
	pstTS_TDT->uitable_id = pucSectionBuffer[0];
	pstTS_TDT->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_TDT->uiReserved_future_use = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_TDT->uiReserved = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_TDT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	for (iUTC_timePosition = 0; iUTC_timePosition < 5; iUTC_timePosition++)
	{
		pstTS_TDT->uiUTC_time[iUTC_timePosition] = pucSectionBuffer[iUTC_timePosition + 3];
	}
}

/******************************************
 *
 * 输出TDT信息
 *
 ******************************************/
void PrintTDT(TS_TDT_T *pstTS_TDT)
{
	DUBUGPRINTF("\n-------------TDT info start-------------\n");
	DUBUGPRINTF("TDT->table_id: 0x%02x\n", pstTS_TDT->uitable_id);
	DUBUGPRINTF("TDT->Section_syntax_indicator: 0x%02x\n", pstTS_TDT->uiSection_syntax_indicator);
	DUBUGPRINTF("TDT->Reserved_future_use: 0x%02x\n", pstTS_TDT->uiReserved_future_use);
	DUBUGPRINTF("TDT->Reserved: 0x%02x\n", pstTS_TDT->uiReserved);
	DUBUGPRINTF("TDT->Section_length: 0x%02x\n", pstTS_TDT->uiSection_length);
	
	char acUTC_time[20] = { 0 };
	int iMJD = (pstTS_TDT->uiUTC_time[0] * 16 * 16 + pstTS_TDT->uiUTC_time[1]);
	MJDtoUTC(acUTC_time, iMJD);
	DUBUGPRINTF("TDT->ucUTC_time: %s %02x:%02x:%02x", acUTC_time, pstTS_TDT->uiUTC_time[2], pstTS_TDT->uiUTC_time[3], pstTS_TDT->uiUTC_time[4]);
	int i = 0;
	for (i = 0; i < 5; ++i)
	{
		if (i == 0)
		{
			DUBUGPRINTF("[原始数据：%02x, ", pstTS_TDT->uiUTC_time[i]);
		}
		else if (i < 5 - 1)
		{
			DUBUGPRINTF("%02x, ", pstTS_TDT->uiUTC_time[i]);
		}
		else
		{
			DUBUGPRINTF("%02x] \n", pstTS_TDT->uiUTC_time[i]);
		}
		
	}
	DUBUGPRINTF("\n-------------TDT info end-------------\n");
}

/******************************************
 *
 *从流中解析TDT信息
 *
 ******************************************/
int ParseTDT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	DUBUGPRINTF("\n\n=================================ParseTDT_Table Start================================= \n");
	int iTemp = 0;
	TS_TDT_T stTS_TDT = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4092] = { 0 };
	unsigned int uiRecordGetSection[SECTION_COUNT_256] = { 0 };

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("Parse TDT error\n");
		return -1;
	}
	
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, TDT_PID, TDT_TABLE_ID, &uiVersion);
		switch (iTemp)
		{
			case 0:
				uiVersion = INITIAL_VERSION;
				memset(uiRecordGetSection, 0, sizeof(char) * SECTION_COUNT_256);
				fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
				break;
			case 1:
				ParseTDT_Section(&stTS_TDT, ucSectionBuffer);
				PrintTDT(&stTS_TDT);
				DUBUGPRINTF("\n=================================ParseTDT_Table END=================================== \n\n");
				return 1;
				break;
			case -1:
				return 1;
				break;
			default:
				LOG("ParseTDT_Table switch (iTemp) default\n");
				break;
		}
	}

	DUBUGPRINTF("\n\n=================================ParseTDT_Table End================================= \n");
	return -1;
}

