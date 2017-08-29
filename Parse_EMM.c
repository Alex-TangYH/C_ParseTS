#include <string.h>
#include <stdio.h>
#include "Parse_EMM.h"
#include "Parse_CAT.h"

#define INITIAL_VERSION 0xff
#define DEFAULT_TABLE_ID 0x80
#define SECTION_MAX_LENGTH_4096 4096
#define SECTION_COUNT_256 256

/******************************************
 *
 *解析EMM数据
 *
 ******************************************/
int ParseEMM_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, CAT_INFO_T *pstCAT_Info)
{
	printf("\n\n=================================ParseEMM_Table Start================================= \n");
	int iTemp = 0;
	
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4096] = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned int uiRecordSectionNumber[SECTION_COUNT_256] = { 0 };
	unsigned int uiCA_Pid = pstCAT_Info->uiCA_PID;
	
	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		printf("Parse ParseEMM_Table occurs error\n");
		return -1;
	}

	while (!feof(pfTsFile))
	{
		iTemp = GetOneSectionByPID(pfTsFile, iTsLength, ucSectionBuffer, uiCA_Pid, &uiVersion);

		if (0 == iTemp)
		{
			printf("Enter if (0 == iTemp) in ParseEMM_Table\n");
		}
		if (1 == iTemp)
		{
			if (0 == IsSectionGetBefore(ucSectionBuffer, uiRecordSectionNumber))
			{
				printf("Enter if (0 == IsSectionGetBefore) in ParseEMM_Table\n");
				//ParseEMM_Section(&stTS_EMM, ucSectionBuffer);
			}
			if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordSectionNumber))
			{
				printf("Enter if (1 == IsAllSectionOver) in ParseEMM_Table\n");
				printf("\n=================================ParseEMM_Table END=================================== \n\n");
				return 0;
			}
		}
		if (-1 == iTemp)
		{
			printf("Enter if (-1 == iTemp) in ParseEMM_Table\n");
			printf("return 0\n");
			printf("\n=================================ParseEMM_Table END=================================== \n\n");
			return 0;
		}
	}
	printf("return 0\n");
	printf("\n=================================ParseEMM_Table END=================================== \n\n");
	return 0;
}

