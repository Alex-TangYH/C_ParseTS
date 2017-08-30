/*
 * FormatUtils.c
 *
 *  Created on: 2017年8月29日
 *      Author: AlexTang
 */
#include <stdio.h>
#include <string.h>
#include "FormatUtils.h"

/******************************************
 *
 *将MJD时间iMJD转换为UTC时间
 *
 ******************************************/
void MJDtoUTC(char *pcUTC_time, int iMJD)
{
	int k = -1;
	int iYear = (int) (((float) iMJD - 15078.2) / 365.25);
	int iMomth = (int) (((float) iMJD - 14956.1 - (int) (iYear * 365.25)) / 30.6001);
	int iDay = (int) (iMJD - 14956 - (int) (iYear * 365.25) - (int) (iMomth * 30.6001));
	if (iMomth == 14 || iMomth == 15)
	{
		k = 1;
	}
	else
	{
		k = 0;
	}

	iYear += k;
	iMomth = iMomth - 1 - k * 12;

	sprintf(pcUTC_time, "%04d/%02d/%02d", iYear + 1900, iMomth, iDay);
}



/***************************************
 *
 * 将UTC_TIME[5] MJD转换为UTC并格式化
 *
 ***************************************/
void FormatUTC_TimeFormMJD(char *pacUTC_time, unsigned int *puiUTC_time)
{

	char acTemp[20] = { 0 };
	int iMJD = puiUTC_time[0] * 16 * 16 + puiUTC_time[1];
	MJDtoUTC(acTemp, iMJD);
	sprintf(pacUTC_time, "%s %02x:%02x:%02x", acTemp, puiUTC_time[2], puiUTC_time[3], puiUTC_time[4]);
	int i = 0;
	for (i = 0; i < 5; i++)
	{
		if (i == 0)
		{
			sprintf(acTemp, "[原始数据：%02x, ", puiUTC_time[i]);
		}
		else if (i < 5 - 1)
		{
			sprintf(acTemp, "%02x, ", puiUTC_time[i]);
		}
		else
		{
			sprintf(acTemp, "%02x]", puiUTC_time[i]);
		}
		strcat(pacUTC_time, acTemp);
	}
}
