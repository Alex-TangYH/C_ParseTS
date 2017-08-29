/****************************************************************
*
* FLIENAME: TsParser.c
*
* DESCRIPTION: Parse TS
*
* AUTHOR: tangyh
*
* DATE : 04/08/2002
*
* HISTORY //not need if controlled by ClearCase
*
*    Date	 Rev.	    Who	       Details
*  ==========	=======   =========   =========
*  03/08/2017     0.01     Tangyh    First Release
*****************************************************************/


#include <stdio.h>

#include "ParseTS_Length.h"

#include "Parse_EMM.h"
#include "Parse_PAT.h"
#include "Parse_PMT.h"
#include "Parse_CAT.h"
#include "Parse_SDT.h"
#include "Parse_BAT.h"
#include "Parse_EIT.h"


#define PROGRAM_MAX 128
#define CA_SYSTEM_MAX 64

#define EIT_PF_ACTUAL_TABLE_ID 0x4e
#define EIT_EIT_SCHEDULE_TABLE_ID_ONE 0x50
#define EIT_EIT_SCHEDULE_TABLE_ID_TWO 0x51



/******************************************
*
*获取所有PMT信息
*
******************************************/

int ParseAllProgramPMT(FILE *pfTsFile, int iTsPosition, int iTsLength, PAT_INFO_T *pstPAT_Info,
		int iProgramCount, PMT_INFO_T *pstPMT_Info)
{
	int iProgramIndex = 0;
	unsigned int uiPMT_PID = 0;
	PMT_INFO_T stOnePMT_Info = {0};
	printf("ParseAllProgramPMT\nProgramCount：%d\n", iProgramCount);

	for (iProgramIndex = 0; iProgramIndex < iProgramCount; iProgramIndex++)
	{
		uiPMT_PID = pstPAT_Info[iProgramIndex].uiPMT_PID;
		if (-1 == ParsePMT_Table(pfTsFile, iTsPosition, iTsLength, uiPMT_PID, &stOnePMT_Info))
		{
			printf("Parse PMT error, the ProgramIndex is %d, PMT_PID is 0x%0x", iProgramIndex, uiPMT_PID);
			return -1;
		}
		pstPMT_Info[iProgramIndex] = stOnePMT_Info;
	}
	return 0;
}



/******************************************
*
* 获取所有EIT信息
*
******************************************/

int ParseALLEIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	ParseEIT_Table(pfTsFile, iTsPosition, iTsLength, EIT_PF_ACTUAL_TABLE_ID);
//	ParseEIT_Table(pfTsFile, iTsPosition, iTsLength, EIT_EIT_SCHEDULE_TABLE_ID_ONE);
//	ParseEIT_Table(pfTsFile, iTsPosition, iTsLength, EIT_EIT_SCHEDULE_TABLE_ID_TWO);
//	 TODO 对返回值进行判断
	return 0;
}





/****************************************************************
* funtion: ParseTransportStream
* param: 
*	 FILE *pfTsFile		pointer of transport stream
* feature: Parse transport stream
*
****************************************************************/
int ParseTransportStream(FILE *pfTsFile)
{
	int iTsPosition = 0;
	int iTsLength = 0;
	int iProgramCount = 0;
	int iEmmPosition = 0;
	int iEmmCount = 0;

	PAT_INFO_T stPAT_Info[PROGRAM_MAX] = {0};
	PMT_INFO_T stPMT_Info[PROGRAM_MAX] = {0};
	CAT_INFO_T stCAT_Info[CA_SYSTEM_MAX] = {0};

	iTsLength = ParseTsLength(pfTsFile, &iTsPosition);
	if (-1 == iTsLength)
	{
		printf("The file is not the transport stream file\n");
		return -1;
	}
	printf("The position is %d\n", iTsPosition);
	printf("The package length is %d\n",iTsLength);


	iEmmCount = ParseCAT_Table(pfTsFile, iTsPosition, iTsLength, stCAT_Info);
	if (-1 == iEmmCount)
	{
		printf("ParseCAT_Table error \n");
		return -1;
	}
	if (0 == iEmmCount)
	{
		printf("This file had not be encrypted\n");
	}
	else
	{
		for (iEmmPosition = 0; iEmmPosition < iEmmCount; iEmmPosition++)
		{
			if (-1 == ParseEMM_Table(pfTsFile, iTsPosition, iTsLength, &stCAT_Info[iEmmPosition]))
			{
				return -1;
			}else
			{
				/*TODO: ParseECM  */
			}
		}
	}


//	iProgramCount = ParsePAT_Table(pfTsFile, iTsPosition, iTsLength, stPAT_Info);
//	if (iProgramCount <= 0)
//	{
//		return -1;
//	}
//
//	if (-1 == ParseSDT_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		return -1;
//	}
//
//	if (-1 == ParseAllProgramPMT(pfTsFile, iTsPosition, iTsLength, stPAT_Info, iProgramCount, stPMT_Info))
//	{
//		return -1;
//	}

//	if (-1 == ParseNIT_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		return -1;
//	}
//
//	if (-1 == ParseTDT_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		return -1;
//	}
//
//	if (-1 == ParseTOT_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		return -1;
//	}
//
//
//	if (-1 == ParseBAT_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		return -1;
//	}
//
//	if (-1 == ParseALLEIT_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		//return -1;
//	}
//
//	if (-1 == ParseRST_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		//return -1;
//	}
//
//	if (-1 == ParseST_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		//return -1;
//	}
//
//	if (-1 == ParseDIT_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		//return -1;
//	}
//
//	if (-1 == ParseSIT_Table(pfTsFile, iTsPosition, iTsLength))
//	{
//		//return -1;
//	}

	return 0;
}

/****************************************************************
* funtion: main()
* param:
* feature: Parse transport stream
*
****************************************************************/
int main()
{
	FILE *pfTsFile = NULL;
	
	pfTsFile = fopen("test.ts","rb");
	if (NULL == pfTsFile)
	{
		pfTsFile = fopen("test.TS","rb");
		if (NULL == pfTsFile)
		{
			printf("file does not exist \n");
			return 0;
		}
	}

	ParseTransportStream(pfTsFile);
	printf("完成\n");
	fclose(pfTsFile);
	return 1;
}