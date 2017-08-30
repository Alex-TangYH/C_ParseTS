#include <stdio.h>
#include <string.h>

#include "Parse_DIT.h"
#include "TsParser.h"
#include "Get_Section.h"

#define DIT_PID 0x001E
#define DIT_TABLE_ID 0x7E
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4096 4096

/******************************************
 *
 *解析DIT段信息
 *
 ******************************************/

void ParseDIT_Section(TS_DIT_T *pstTS_DIT, unsigned char *pucSectionBuffer)
{
	pstTS_DIT->uitable_id = pucSectionBuffer[0];
	pstTS_DIT->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_DIT->uiReserved_future_use_first = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_DIT->uiReserved = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_DIT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	pstTS_DIT->uiTransition_flag = pucSectionBuffer[3] >> 7;
	pstTS_DIT->uiReserved_future_use_second = (pucSectionBuffer[3] << 1) >> 1;
}

/******************************************
 *
 *输出DIT信息
 *
 ******************************************/
void PrintDIT(TS_DIT_T *pstTS_DIT)
{
	DUBUGPRINTF("\n-------------DIT info DITart-------------\n");
	DUBUGPRINTF("DIT->table_id: %02x\n", pstTS_DIT->uitable_id);
	DUBUGPRINTF("DIT->Section_syntax_indicator: %02x\n", pstTS_DIT->uiSection_syntax_indicator);
	DUBUGPRINTF("DIT->Reserved_future_use_first: %02x\n", pstTS_DIT->uiReserved_future_use_first);
	DUBUGPRINTF("DIT->Reserved: %02x\n", pstTS_DIT->uiReserved);
	DUBUGPRINTF("DIT->Section_length: %02x\n", pstTS_DIT->uiSection_length);
	DUBUGPRINTF("DIT->Transition_flag: %02x\n", pstTS_DIT->uiTransition_flag);
	DUBUGPRINTF("DIT->Reserved_future_use_second: %02x\n", pstTS_DIT->uiReserved_future_use_second);
	DUBUGPRINTF("\n-------------DIT info end-------------\n");
}

/******************************************
 *
 *从流中解析DIT信息
 *
 ******************************************/
int ParseDIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	DUBUGPRINTF("\n\n=================================ParseDIT_Table DITart================================= \n");
	int iTemp = 0;
	TS_DIT_T stTS_DIT = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4096] = { 0 };
	unsigned int uiRecordGetSection[SECTION_COUNT_256] = { 0 };

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("Parse DIT error\n");
		return -1;
	}
	
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, DIT_PID, DIT_TABLE_ID, &uiVersion);
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
					ParseDIT_Section(&stTS_DIT, ucSectionBuffer);
					PrintDIT(&stTS_DIT);
				}
				if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordGetSection))
				{
					DUBUGPRINTF("\n=================================ParseDIT_Table END=================================== \n\n");
					return 1;
				}
				break;
			case -1:
				DUBUGPRINTF("\n\n=================================ParseDIT_Table End================================= \n");
				return 1;
				break;
			default:
				LOG("ParseDIT_Table switch (iTemp) default\n");
				break;
		}
	}

	DUBUGPRINTF("\n\n=================================ParseDIT_Table End================================= \n");
	return -1;
}

