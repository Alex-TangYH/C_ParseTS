/****************************************************************
 *
 * FLIENAME: ParseTS_Length.c
 *
 * DESCRIPTION: Parse TS to get length
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

#define PACKET_LENGTH_188 188
#define PACKET_LENGTH_204 204

/**************************************************
 * ParseTsLength(FILE *pfTsFile, int *piTsPosition)
 *
 **************************************************/

int ParseTsLength(FILE *pfTsFile, int *piTsPosition)
{
	int iFirstPackageByte = 0;
	while (!feof(pfTsFile))
	{
		iFirstPackageByte = fgetc(pfTsFile);
		if (0x47 == iFirstPackageByte)
		{
			if (PACKET_LENGTH_188 == JudgmentPackageTenTimes(pfTsFile, *piTsPosition, PACKET_LENGTH_188))
			{
				return PACKET_LENGTH_188;
			}
			if (PACKET_LENGTH_204 == JudgmentPackageTenTimes(pfTsFile, *piTsPosition, PACKET_LENGTH_204))
			{
				return PACKET_LENGTH_204;
			}
		}
		(*piTsPosition)++;

		if (-1 == fseek(pfTsFile, *piTsPosition, SEEK_SET))
		{
			printf("The file error\n");
			return -1;
		}
	}
	printf("The file is not the transport stream\n");
	return -1;
}

/**************************************************
 * JudgmentPackageTenTimes(FILE *pfTsFile, int iTsPosition, int iTsLength)
 **************************************************/

JudgmentPackageTenTimes(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	int iLoopTime = 0;
	int iFirstPackageByte = 0;
	
	if (-1 == fseek(pfTsFile, iTsPosition + 1, SEEK_SET))
	{
		return -1;
	}
	
	for (iLoopTime = 0; iLoopTime < 10; iLoopTime++)
	{
		if (-1 == fseek(pfTsFile, iTsLength - 1, SEEK_CUR))
		{
			return -1;
		}

		if (feof(pfTsFile))
		{
			return -1;
		}
		iFirstPackageByte = fgetc(pfTsFile);
		if (0x47 != iFirstPackageByte)
		{
			return -1;
		}
		
	}
	return iTsLength;
}

