#include <stdio.h>
#include <string.h>

#include "Parse_PMT.h"
#include "Parse_Descriptor.h"
#include "Get_Section.h"
#include "Parse_DesciptorStream.h"
#include "TsParser.h"

#define PMT_TABLE_ID 0x02
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4096 1024*4
#define OUTPUT_PREFIX_SIZE 256

#define PRINTFPMT_INFO 1

/******************************************
 *
 *重置PMT的section信息
 *
 ******************************************/
void CleanPMT_Info(PMT_INFO_T *pstPMT_Info, int *piAudioCount)
{
	(*piAudioCount) = 0;
	memset(pstPMT_Info, 0, sizeof(TS_PMT_STREAM_T));
}

/******************************************
 *
 *解析PMT的section头部数据
 *
 ******************************************/
void ParsePMT_SectionHead(TS_PMT_T *pstTS_PMT, unsigned char *pucSectionBuffer, int *piCA_DescriptorCount)
{
	int iPMT_Length = 0;
	
	pstTS_PMT->uiTable_id = pucSectionBuffer[0];
	pstTS_PMT->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_PMT->uiZero = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_PMT->uiReserved_first = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_PMT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	pstTS_PMT->uiProgram_number = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_PMT->uiReserved_second = pucSectionBuffer[5] >> 6;
	pstTS_PMT->uiVersion_number = (pucSectionBuffer[5] >> 1) & 0x1f;
	pstTS_PMT->uiCurrent_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_PMT->uiSection_number = pucSectionBuffer[6];
	pstTS_PMT->uiLast_section_number = pucSectionBuffer[7];
	pstTS_PMT->uiReserved_third = pucSectionBuffer[8] >> 5;
	pstTS_PMT->uiPCR_PID = ((pucSectionBuffer[8] & 0x1f) << 8) | pucSectionBuffer[9];
	pstTS_PMT->uiReserved_fourth = pucSectionBuffer[10] >> 4;
	pstTS_PMT->uiProgram_info_length = ((pucSectionBuffer[10] & 0x0f) << 8) | pucSectionBuffer[11];
	if (pstTS_PMT->uiProgram_info_length > 0)
	{
		memcpy(pstTS_PMT->aucProgramDescriptor, pucSectionBuffer + 12, pstTS_PMT->uiProgram_info_length);
	}
	iPMT_Length = pstTS_PMT->uiSection_length + 3;
	pstTS_PMT->uiCRC_32 = (pucSectionBuffer[iPMT_Length - 4] << 24) | (pucSectionBuffer[iPMT_Length - 3] << 16) | (pucSectionBuffer[iPMT_Length - 2] << 8) | (pucSectionBuffer[iPMT_Length - 1]);
}

/******************************************
 *
 * 读取所需要的CAT数据
 *
 ******************************************/
void GetPMT_CAT_Info(CA_DESCRIPTOR_T *pstCA_Descriptor, int iCA_DescriptorCount, PMT_CAT_INFO_T *pstCAT_Info)
{
	pstCAT_Info[iCA_DescriptorCount].uiPMT_CA_PID = pstCA_Descriptor->uiCA_PID;
	pstCAT_Info[iCA_DescriptorCount].uiPMT_CA_system_id = pstCA_Descriptor->uiCA_system_ID;
}

/******************************************
 *
 * 解析PMT的section数据
 *
 ******************************************/
int ParsePMT_Section(TS_PMT_T *pstTS_PMT, unsigned char *pucSectionBuffer, PMT_CAT_INFO_T *pstPMT_CAT_Info)
{
	int iStreamPosition = 12;
	int iPMT_Length = 0;
	int iStreamCount = 0;
	int iCA_DescriptorCount = 0;
	
	ParsePMT_SectionHead(pstTS_PMT, pucSectionBuffer, &iCA_DescriptorCount);

	iPMT_Length = 3 + pstTS_PMT->uiSection_length;
	for (iStreamPosition += pstTS_PMT->uiProgram_info_length; iStreamPosition < iPMT_Length - 4; iStreamPosition += 5)
	{
		pstTS_PMT->stPMT_Stream[iStreamCount].uiStream_type = pucSectionBuffer[iStreamPosition];
		pstTS_PMT->stPMT_Stream[iStreamCount].uiReserved_fifth = pucSectionBuffer[1 + iStreamPosition] >> 5;
		pstTS_PMT->stPMT_Stream[iStreamCount].uiElementary_PID = ((pucSectionBuffer[1 + iStreamPosition] & 0x1f) << 8) | pucSectionBuffer[2 + iStreamPosition];
		pstTS_PMT->stPMT_Stream[iStreamCount].uiReserved_sixth = pucSectionBuffer[3 + iStreamPosition] >> 4;
		pstTS_PMT->stPMT_Stream[iStreamCount].uiES_info_length = ((pucSectionBuffer[3 + iStreamPosition] & 0x0f) << 8) | pucSectionBuffer[4 + iStreamPosition];
		if (0 != pstTS_PMT->stPMT_Stream[iStreamCount].uiES_info_length)
		{
			memcpy(pstTS_PMT->stPMT_Stream[iStreamCount].aucDescriptor, pucSectionBuffer + 5 + iStreamPosition, pstTS_PMT->stPMT_Stream[iStreamCount].uiES_info_length);
			iStreamPosition += pstTS_PMT->stPMT_Stream[iStreamCount].uiES_info_length;
		}
		iStreamCount++;
	}
	
	return iStreamCount;
}

/******************************************
 *
 *提取需要的PMT信息，存到目标参数中
 *
 ******************************************/
void GetPMT_Info(TS_PMT_T *pstTS_PMT, int iStreamCount, PMT_INFO_T *pstPMT_Info, int *iVideoCount)
{
	int iLoopTime = 0;
	unsigned int uiStreamType = 0;
	unsigned int uiElementrayPID = 0;

	pstPMT_Info->uiProgramNumber = pstTS_PMT->uiProgram_number;
	for (iLoopTime = 0; iLoopTime < iStreamCount; ++iLoopTime)
	{
		uiStreamType = pstTS_PMT->stPMT_Stream[iLoopTime].uiStream_type;
		uiElementrayPID = pstTS_PMT->stPMT_Stream[iLoopTime].uiElementary_PID;
		/* get audio */
		if ((0x04 == uiStreamType) || (0x03 == uiStreamType) || (0x0f == uiStreamType) || (0x11 == uiStreamType))
		{
			DUBUGPRINTF("Enter get audio\n");
			pstPMT_Info->uiAudioPID[*iVideoCount] = uiElementrayPID;
			(*iVideoCount)++;
		}

		/* get video */
		if ((0x01 == uiStreamType) || (0x02 == uiStreamType) || (0x1b == uiStreamType) || (0x11 == uiStreamType))
		{
			pstPMT_Info->uiVideoPID = uiElementrayPID;
		}
	}
}

/******************************************
 *
 *打印一个PMT信息
 *
 ******************************************/
void PrintPMT(TS_PMT_T *pstTS_PMT, int iStreamCount)
{
	char acOutputPrefix[OUTPUT_PREFIX_SIZE] = { 0 };
	
	DUBUGPRINTF("\n-------------PMT info start-------------\n");
	DUBUGPRINTF("PMT->Table_id : 0x%02x \n", pstTS_PMT->uiTable_id);
	DUBUGPRINTF("PMT->Section_syntax_indicator : 0x%02x \n", pstTS_PMT->uiSection_syntax_indicator);
	DUBUGPRINTF("PMT->Zero : 0x%02x \n", pstTS_PMT->uiZero);
	DUBUGPRINTF("PMT->Reserved_first : 0x%02x \n", pstTS_PMT->uiReserved_first);
	DUBUGPRINTF("PMT->Section_length : 0x%02x \n", pstTS_PMT->uiSection_length);
	DUBUGPRINTF("PMT->Program_number : 0x%02x \n", pstTS_PMT->uiProgram_number);
	DUBUGPRINTF("PMT->Reserved_second : 0x%02x \n", pstTS_PMT->uiReserved_second);
	DUBUGPRINTF("PMT->Version_number : 0x%02x \n", pstTS_PMT->uiVersion_number);
	DUBUGPRINTF("PMT->Current_next_indicator : 0x%02x \n", pstTS_PMT->uiCurrent_next_indicator);
	DUBUGPRINTF("PMT->Section_number : 0x%02x \n", pstTS_PMT->uiSection_number);
	DUBUGPRINTF("PMT->Last_section_number : 0x%02x \n", pstTS_PMT->uiLast_section_number);
	DUBUGPRINTF("PMT->Reserved_third : 0x%02x \n", pstTS_PMT->uiReserved_third);
	DUBUGPRINTF("PMT->PCR_PID : 0x%02x \n", pstTS_PMT->uiPCR_PID);
	DUBUGPRINTF("PMT->Reserved_fourth : 0x%02x \n", pstTS_PMT->uiReserved_fourth);
	DUBUGPRINTF("PMT->Program_info_length : 0x%02x \n", pstTS_PMT->uiProgram_info_length);
	DUBUGPRINTF("PMT->CRC_32 : 0x%08lx \n", pstTS_PMT->uiCRC_32);

	if (pstTS_PMT->uiProgram_info_length > 0)
	{
		memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
		sprintf(acOutputPrefix, "PMT->ProgramDescriptor.");
		ParseDescriptor(pstTS_PMT->aucProgramDescriptor, pstTS_PMT->uiProgram_info_length, acOutputPrefix);
	}
	
	int iLoopTime = 0;
	
	for (iLoopTime = 0; iLoopTime < iStreamCount; ++iLoopTime)
	{
		DUBUGPRINTF("PMT->PMT_Stream[%d].Stream_type : 0x%02x \n", iLoopTime, pstTS_PMT->stPMT_Stream[iLoopTime].uiStream_type);
		DUBUGPRINTF("PMT->PMT_Stream[%d].Reserved_fifth : 0x%02x \n", iLoopTime, pstTS_PMT->stPMT_Stream[iLoopTime].uiReserved_fifth);
		DUBUGPRINTF("PMT->PMT_Stream[%d].Elementary_PID : 0x%02x \n", iLoopTime, pstTS_PMT->stPMT_Stream[iLoopTime].uiElementary_PID);
		DUBUGPRINTF("PMT->PMT_Stream[%d].Reserved_sixth : 0x%02x \n", iLoopTime, pstTS_PMT->stPMT_Stream[iLoopTime].uiReserved_sixth);
		DUBUGPRINTF("PMT->PMT_Stream[%d].ES_info_length : 0x%02x \n", iLoopTime, pstTS_PMT->stPMT_Stream[iLoopTime].uiES_info_length);
		if (0 != pstTS_PMT->stPMT_Stream[iLoopTime].uiES_info_length)
		{
			memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
			sprintf(acOutputPrefix, "PMT->PMT_Stream[%d].", iLoopTime);
			ParseDescriptor(pstTS_PMT->stPMT_Stream[iLoopTime].aucDescriptor, pstTS_PMT->stPMT_Stream[iLoopTime].uiES_info_length, acOutputPrefix);
		}
	}
	DUBUGPRINTF("-------------PMT info end-------------\n\n");
}

/******************************************
 *
 *解析ECM信息
 *
 ******************************************/
int ParseECM(FILE *pfTsFile, int iTsLength, unsigned char *pucSectionBuffer, unsigned int uiPID, unsigned int *puiVersion)
{
	int iTemp = 0;
	
	iTemp = GetOneSectionByPID(pfTsFile, iTsLength, pucSectionBuffer, uiPID, puiVersion);
	DUBUGPRINTF("========================ParseECM iTemp = %d========================\n", iTemp);
	return 1;
}

/******************************************
 *
 *解析单个PMT信息
 *
 ******************************************/
int ParsePMT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int uiPMTPid, PMT_INFO_T *pstPMT_Info)
{
	DUBUGPRINTF("\n\n=================================ParsePMT_Table Start================================= \n");
	int iTemp = 0;
	int iAudioCount = 0;
	int iStreamCount = 0;
	TS_PMT_T stTS_PMT = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned int uiRecordSectionNumber[SECTION_COUNT_256] = { 0 };
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4096] = { 0 };
	
	PMT_CAT_INFO_T stPMT_CAT_Info[2] = { 0 };
	
	memset(pstPMT_Info, 0, sizeof(PMT_INFO_T));
	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("Parse one PMT error, error PID is %d\n", uiPMTPid);
		return -1;
	}

	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, uiPMTPid, PMT_TABLE_ID, &uiVersion);
		switch (iTemp)
		{
			case 0:
				DUBUGPRINTF("Enter if (0 == iTemp) in PARSE_PMT\n");
				uiVersion = INITIAL_VERSION;
				memset(uiRecordSectionNumber, 0, sizeof(char) * SECTION_COUNT_256);
				fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
				CleanPMT_Info(pstPMT_Info, &iAudioCount);
				break;
			case 1:
				if (0 == IsSectionGetBefore(ucSectionBuffer, uiRecordSectionNumber))
				{
					DUBUGPRINTF("Enter if (0 == IsSectionGetBefore) in PARSE_PMT\n");
					iStreamCount = ParsePMT_Section(&stTS_PMT, ucSectionBuffer, stPMT_CAT_Info);
					GetPMT_Info(&stTS_PMT, iStreamCount, pstPMT_Info, &iAudioCount);
					//GetPMT_CAT_Info(pstTS_PMT, iCA_DescriptorCount, pstPMT_CAT_Info);
					if (1 == PRINTFPMT_INFO)
					{
						PrintPMT(&stTS_PMT, iStreamCount);
					}

					//解析ECM
					ParseECM(pfTsFile, iTsLength, ucSectionBuffer, stPMT_CAT_Info[0].uiPMT_CA_PID, &uiVersion);
				}
				if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordSectionNumber))
				{
					DUBUGPRINTF("Enter if (1 == IsAllSectionOver) in PARSE_PMT\n");
					DUBUGPRINTF("return iAudioCount, iAudioCount is: %d\n", iAudioCount);
					DUBUGPRINTF("\n=================================ParsePMT_Table END=================================== \n\n");
					return iAudioCount;
				}
				break;
			case 2:
				break;
			case -1:
				DUBUGPRINTF("Enter if (-1 == iTemp) in PARSE_PMT\n");
				DUBUGPRINTF("PMTPid %x is not a PMT table \n", uiPMTPid);
				DUBUGPRINTF("return 0\n");
				DUBUGPRINTF("\n=================================ParsePMT_Table END=================================== \n\n");
				return 0;
				break;
			default:
				LOG("ParsePMT_Table switch (iTemp) default\n");
				break;
		}
	}
	DUBUGPRINTF("return 0\n");
	DUBUGPRINTF("\n=================================ParsePMT_Table END=================================== \n\n");
	return 0;
}

