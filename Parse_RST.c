#include <stdio.h>
#include <string.h>

#include "Parse_RST.h"
#include "TsParser.h"
#include "Get_Section.h"

#define RST_PID 0x0013
#define RST_TABLE_ID 0x71
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4092 1024 * 4

/******************************************
 *
 *解析RST段信息
 *
 ******************************************/

int ParseRST_Section(TS_RST_T *pstTS_RST, unsigned char *pucSectionBuffer)
{
	int iLoopPosition = 0;
	int iLoopCount = 0;
	pstTS_RST->uitable_id = pucSectionBuffer[0];
	pstTS_RST->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_RST->uiReserved_future_use_first = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_RST->uiReserved = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_RST->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];

	if (pstTS_RST->uiSection_length > 0)
	{
		for (iLoopPosition = 3; iLoopPosition < pstTS_RST->uiSection_length + 3; iLoopPosition += 3)
		{
			pstTS_RST->astRST_info[iLoopCount].uiTransport_stream_id = (pucSectionBuffer[0] << 8) | pucSectionBuffer[1];
			pstTS_RST->astRST_info[iLoopCount].uiOriginal_network_id = (pucSectionBuffer[2] << 8) | pucSectionBuffer[3];
			pstTS_RST->astRST_info[iLoopCount].uiService_id = (pucSectionBuffer[4] << 8) | pucSectionBuffer[5];
			pstTS_RST->astRST_info[iLoopCount].uiEvent_id = (pucSectionBuffer[6] << 8) | pucSectionBuffer[7];
			pstTS_RST->astRST_info[iLoopCount].uiReserved_future_use_second = pucSectionBuffer[8] >> 3;
			pstTS_RST->astRST_info[iLoopCount].uiRunning_status = pucSectionBuffer[8] & 0x07;
			iLoopCount++;
		}
	}
	
	return iLoopCount;
}

/******************************************
 *
 *输出RST信息
 *
 ******************************************/
void PrintRST(TS_RST_T *pstTS_RST, int iRST_LoopCount)
{
	int iLoopTime = 0;
	DUBUGPRINTF("\n-------------RST info start-------------\n");

	DUBUGPRINTF("RST->table_id: %02x\n", pstTS_RST->uitable_id);
	DUBUGPRINTF("RST->uiSection_syntax_indicator: %02x\n", pstTS_RST->uiSection_syntax_indicator);
	DUBUGPRINTF("RST->uiReserved_future_use_first: %02x\n", pstTS_RST->uiReserved_future_use_first);
	DUBUGPRINTF("RST->uiReserved: %02x\n", pstTS_RST->uiReserved);
	DUBUGPRINTF("RST->uiSection_length: %02x\n", pstTS_RST->uiSection_length);
	if (pstTS_RST->uiSection_length > 0)
	{
		for (iLoopTime = 0; iLoopTime < iRST_LoopCount; iLoopTime++)
		{
			DUBUGPRINTF("RST->RST_info[%d].uiTransport_stream_id: %02x\n", iLoopTime, pstTS_RST->astRST_info[iLoopTime].uiTransport_stream_id);
			DUBUGPRINTF("RST->RST_info[%d].uiOriginal_network_id: %02x\n", iLoopTime, pstTS_RST->astRST_info[iLoopTime].uiOriginal_network_id);
			DUBUGPRINTF("RST->RST_info[%d].uiService_id: %02x\n", iLoopTime, pstTS_RST->astRST_info[iLoopTime].uiService_id);
			DUBUGPRINTF("RST->RST_info[%d].uiEvent_id: %02x\n", iLoopTime, pstTS_RST->astRST_info[iLoopTime].uiEvent_id);
			DUBUGPRINTF("RST->RST_info[%d].uiReserved_future_use_second: %02x\n", iLoopTime, pstTS_RST->astRST_info[iLoopTime].uiReserved_future_use_second);
			DUBUGPRINTF("RST->RST_info[%d].uiRunning_status: %02x\n", iLoopTime, pstTS_RST->astRST_info[iLoopTime].uiRunning_status);
		}
	}
	
	DUBUGPRINTF("\n-------------RST info end-------------\n");
}

/******************************************
 *
 *从流中解析RST信息
 *
 ******************************************/
int ParseRST_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	DUBUGPRINTF("\n\n=================================ParseRST_Table Start================================= \n");
	int iTemp = 0;
	int iRST_LoopCount = 0;
	TS_RST_T stTS_RST = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4092] = { 0 };
	unsigned int uiRecordGetSection[SECTION_COUNT_256] = { 0 };

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("Parse RST error\n");
		return -1;
	}
	
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, RST_PID, RST_TABLE_ID, &uiVersion);
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
					iRST_LoopCount = ParseRST_Section(&stTS_RST, ucSectionBuffer);
					PrintRST(&stTS_RST, iRST_LoopCount);
				}
				if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordGetSection))
				{
					DUBUGPRINTF("\n=================================ParseRST_Table END=================================== \n\n");
					return 1;
				}
				break;
			case -1:
				DUBUGPRINTF("\n\n=================================ParseRST_Table End================================= \n");
				return 1;
				break;
			default:
				LOG("ParseRST_Table switch (iTemp) default\n");
				break;
		}
	}

	DUBUGPRINTF("\n\n=================================ParseRST_Table End================================= \n");
	return -1;
}

