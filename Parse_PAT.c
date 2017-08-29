#include <string.h>
#include <stdio.h>
#include "Parse_PAT.h"

#define INITIAL_VERSION 0xff

#define PAT_PID 0x0000
#define PAT_TABLE_ID 0x00
#define DEFAULT_TABLE_ID 0x80

#define SECTION_COUNT_256 256
#define PACKAGE_MAX_LENGTH_204 204
#define SECTION_MAX_LENGTH_4096 1024*4

#define PRINTFPAT_INFO 1

/******************************************************************
 * 将SectionBuffer的PAT头部信息存入TS_PAT中
 ******************************************************************/

void ParsePAT_SectionHead(TS_PAT_T *pstTS_PAT, unsigned char *pucSectionBuffer)
{
	int iPAT_Length = 0;

	pstTS_PAT->uiTable_id = pucSectionBuffer[0];
	pstTS_PAT->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_PAT->uiZero = (pucSectionBuffer[1] >> 6) & 0x1;
	pstTS_PAT->uiReserved_first = (pucSectionBuffer[1] >> 4) & 0x3;
	pstTS_PAT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	pstTS_PAT->uiTransport_stream_id = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_PAT->uiReserved_second = pucSectionBuffer[5] >> 6;
	pstTS_PAT->uiVersion_number = (pucSectionBuffer[5] >> 1) & 0x1f;
	pstTS_PAT->uiCurrent_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_PAT->uiSection_number = pucSectionBuffer[6];
	pstTS_PAT->uiLast_section_number = pucSectionBuffer[7];
	iPAT_Length = 3 + pstTS_PAT->uiSection_length;
	pstTS_PAT->uiCRC_32 = (pucSectionBuffer[iPAT_Length - 4] << 24) | (pucSectionBuffer[iPAT_Length - 3] << 16) | (pucSectionBuffer[iPAT_Length - 2] << 8) | (pucSectionBuffer[iPAT_Length - 1]);
}

/******************************************************************
 * 解析PAT的关键信息，并将解析到的PAT头部信息与其拼接成完整的PAT表
 ******************************************************************/
int ParsePAT_Section(TS_PAT_T *pstTS_PAT, unsigned char *pucSectionBuffer)
{
	int iPAT_Length = 0;
	int iPATProgramPosition = 8;
	int iPAT_ProgramCount = 0;

	memset(pstTS_PAT, 0, sizeof(TS_PAT_T));
	ParsePAT_SectionHead(pstTS_PAT, pucSectionBuffer);
	iPAT_Length = 3 + pstTS_PAT->uiSection_length;
	for (iPATProgramPosition = 8; iPATProgramPosition < iPAT_Length - 4; iPATProgramPosition += 4)
	{
		if (0x00 == ((pucSectionBuffer[iPATProgramPosition] << 8) | pucSectionBuffer[1 + iPATProgramPosition]))
		{
			pstTS_PAT->uiNetwork_PID = ((pucSectionBuffer[2 + iPATProgramPosition] & 0x1f) << 8) | pucSectionBuffer[3 + iPATProgramPosition];
			printf("*********************The network_PID is 0x%02x*******************\n", pstTS_PAT->uiNetwork_PID);
		}
		else
		{
			pstTS_PAT->stPAT_Program[iPAT_ProgramCount].uiProgram_number = (pucSectionBuffer[iPATProgramPosition] << 8) | pucSectionBuffer[1 + iPATProgramPosition];
			pstTS_PAT->stPAT_Program[iPAT_ProgramCount].uiReserved_third = pucSectionBuffer[2 + iPATProgramPosition] >> 5;
			pstTS_PAT->stPAT_Program[iPAT_ProgramCount].uiProgram_map_PID = ((pucSectionBuffer[2 + iPATProgramPosition] & 0x1f) << 8) | pucSectionBuffer[3 + iPATProgramPosition];
			iPAT_ProgramCount++;
		}
	}
	return iPAT_ProgramCount;
}

/******************************************
 *
 * 从PAT中获取PMT的前提信息
 *
 ******************************************/

void GetPAT_Info(TS_PAT_T *pstTS_PAT, int iPAT_ProgramCount, PAT_INFO_T *pstPAT_Info, int *iInfoCount)
{
	int iLoopTime = 0;

	for (iLoopTime = 0; iLoopTime < iPAT_ProgramCount; iLoopTime++)
	{
		pstPAT_Info[*iInfoCount].uiPMT_PID = pstTS_PAT->stPAT_Program[iLoopTime].uiProgram_map_PID;
		pstPAT_Info[*iInfoCount].uiProgramNumber = pstTS_PAT->stPAT_Program[iLoopTime].uiProgram_number;
		(*iInfoCount)++;
	}
}

/******************************************
 *
 * 重置PAT数据
 *
 ******************************************/
void CleanPAT_Info(PAT_INFO_T *pstPAT_Info, int *piInfoCount)
{
	*piInfoCount = 0;
	memset(pstPAT_Info, 0, sizeof(TS_PAT_PROGRAM_T) * PAT_PROGARM_MAX);
}

/******************************************
 *
 * 输出PAT表信息
 *
 ******************************************/
void PrintPAT(TS_PAT_T *pstTS_PAT_T, int iPAT_ProgramCount)
{
	printf("\n\n\n");
	printf("-------------PAT info start-------------\n");
	printf("PAT->Table_id: 0x%02x\n", pstTS_PAT_T->uiTable_id);
	printf("PAT->Section_syntax_indicator: 0x%02x\n", pstTS_PAT_T->uiSection_syntax_indicator);
	printf("PAT->Zero: 0x%02x\n", pstTS_PAT_T->uiZero);
	printf("PAT->Reserved_first: 0x%02x\n", pstTS_PAT_T->uiReserved_first);
	printf("PAT->Section_length: 0x%02x\n", pstTS_PAT_T->uiSection_length);
	printf("PAT->Transport_stream_id: 0x%02x\n", pstTS_PAT_T->uiTransport_stream_id);
	printf("PAT->Reserved_second: 0x%02x\n", pstTS_PAT_T->uiReserved_second);
	printf("PAT->Version_number: 0x%02x\n", pstTS_PAT_T->uiVersion_number);
	printf("PAT->Current_next_indicator: 0x%02x\n", pstTS_PAT_T->uiCurrent_next_indicator);
	printf("PAT->Section_number: 0x%02x\n", pstTS_PAT_T->uiSection_number);
	printf("PAT->Last_section_number: 0x%02x\n", pstTS_PAT_T->uiLast_section_number);
	printf("PAT->CRC_32: 0x%02x\n", pstTS_PAT_T->uiCRC_32);
	
	int iLoopTime = 0;
	for (iLoopTime = 0; iLoopTime < iPAT_ProgramCount; iLoopTime++)
	{
		printf("PAT->PAT_Program[%d].Program_number: 0x%02x\n", iLoopTime, pstTS_PAT_T->stPAT_Program[iLoopTime].uiProgram_number);
		if (0 == pstTS_PAT_T->stPAT_Program[iLoopTime].uiProgram_number)
		{
			printf("PAT->uiNetwork_PID: 0x%02x\n", pstTS_PAT_T->uiNetwork_PID);
		}
		else
		{
			printf("PAT->PAT_Program[%d].Reserved_third: 0x%02x\n", iLoopTime, pstTS_PAT_T->stPAT_Program[iLoopTime].uiReserved_third);
			printf("PAT->PAT_Program[%d].Program_map_PID: 0x%02x\n", iLoopTime, pstTS_PAT_T->stPAT_Program[iLoopTime].uiProgram_map_PID);
		}
	}
	printf("-------------PAT info end-------------\n\n");
}

/******************************************
 *
 * 从TS流中解析PAT表
 *
 ******************************************/
int ParsePAT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, PAT_INFO_T *pstPAT_Info_T)
{
	printf("\n=================================ParsePAT_Table Start================================= \n");
	int iTemp = 0;
	int iInfoCount = 0;
	int iPATProgramCount = 0;
	TS_PAT_T stTS_PAT_T = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned int uiRecordSectionNumber[SECTION_COUNT_256] = { 0 };
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4096] = { 0 };

	unsigned long ulCRC32 = 0;
	
	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		printf("Parse table error\n");
		return -1;
	}

	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, PAT_PID, PAT_TABLE_ID, &uiVersion);
		if (0 == iTemp)
		{
			printf("Enter if (0 == iTemp) in PARSE_PAT\n");
			uiVersion = INITIAL_VERSION;
			memset(uiRecordSectionNumber, 0, sizeof(char) * SECTION_COUNT_256);
			fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
			CleanPAT_Info(pstPAT_Info_T, &iInfoCount);
		}
		if (1 == iTemp)
		{
			printf("Enter if (1 == iTemp) in PARSE_PAT\n");
			if (0 == IsSectionGetBefore(ucSectionBuffer, uiRecordSectionNumber))
			{
				printf("Enter if (0 == IsSectionGetBefore) in PARSE_PAT\n");
				iPATProgramCount = ParsePAT_Section(&stTS_PAT_T, ucSectionBuffer);
				
				GetPAT_Info(&stTS_PAT_T, iPATProgramCount, pstPAT_Info_T, &iInfoCount);
				if (1 == PRINTFPAT_INFO)
				{
					PrintPAT(&stTS_PAT_T, iPATProgramCount);
				}
			}
			if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordSectionNumber))
			{
				printf("Enter if (1 == IsAllSectionOver) in PARSE_PAT\n");
				printf("return iInfoCount, iInfoCount is: %d\n", iInfoCount);
				printf("\n\n=================================ParsePAT_Table End=================================== \n\n");
				return iInfoCount;
			}
		}
		if (-1 == iTemp)
		{
			printf("Enter if (-1 == iTemp) in PARSE_PAT\n");
			printf("return iInfoCount, iInfoCount is: %d\n", iInfoCount);
			printf("\n\n=================================ParsePAT_Table End=================================== \n\n");
			return iInfoCount;
		}
	}
	printf("return 0\n");
	printf("\n=================================ParsePAT_Table End===================================\n\n");
	return 0;
}
