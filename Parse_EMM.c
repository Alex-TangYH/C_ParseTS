#include <string.h>
#include <stdio.h>
#include "Parse_EMM.h"
#include "Parse_CAT.h"
#include "TsParser.h"
#include "Get_Section.h"

#define INITIAL_VERSION 0xff
#define DEFAULT_TABLE_ID 0x80
#define SECTION_MAX_LENGTH_4096 4096
#define SECTION_COUNT_256 256

/******************************************
 *
 *½âÎöEMMÊý¾Ý
 *
 ******************************************/
int ParseEMM_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, CAT_INFO_T *pstCAT_Info)
{
	DUBUGPRINTF("\n\n=================================ParseEMM_Table Start================================= \n");
	int iTemp = 0;
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4096] = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned int uiRecordSectionNumber[SECTION_COUNT_256] = { 0 };
	unsigned int uiCA_Pid = pstCAT_Info->uiCA_PID;
	
	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("Parse ParseEMM_Table occurs error\n");
		return -1;
	}
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSectionByPID(pfTsFile, iTsLength, ucSectionBuffer, uiCA_Pid, &uiVersion);
		switch (iTemp)
		{
			case 0:
				DUBUGPRINTF("Enter if (0 == iTemp) in ParseEMM_Table\n");
				break;
			case 1:
				if (0 == IsSectionGetBefore(ucSectionBuffer, uiRecordSectionNumber))
				{
					DUBUGPRINTF("Enter if (0 == IsSectionGetBefore) in ParseEMM_Table\n");
					//ParseEMM_Section(&stTS_EMM, ucSectionBuffer);
				}
				if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordSectionNumber))
				{
					DUBUGPRINTF("Enter if (1 == IsAllSectionOver) in ParseEMM_Table\n");
					DUBUGPRINTF("\n=================================ParseEMM_Table END=================================== \n\n");
					return 0;
				}
				break;
			case -1:
				DUBUGPRINTF("Enter if (-1 == iTemp) in ParseEMM_Table\n");
				DUBUGPRINTF("return 0\n");
				DUBUGPRINTF("\n=================================ParseEMM_Table END=================================== \n\n");
				return 0;
				break;
			default:
				LOG("ParseEMM_Table switch (iTemp) default\n");
				break;
		}
	}
	DUBUGPRINTF("return 0\n");
	DUBUGPRINTF("\n=================================ParseEMM_Table END=================================== \n\n");
	return 0;
}

