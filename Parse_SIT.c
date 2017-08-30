#include <stdio.h>
#include <string.h>

#include "Parse_SIT.h"
#include "Parse_DesciptorStream.h"
#include "Get_Section.h"
#include "TsParser.h"

#define SIT_PID 0x001E
#define SIT_TABLE_ID 0x7E
#define INITIAL_VERSION 0xff
#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4 4
#define OUTPUT_PREFIX_SIZE 256

/******************************************
 *
 * 解析SIT段信息
 *
 ******************************************/

void ParseSIT_Section(TS_SIT_T *pstTS_SIT, unsigned char *pucSectionBuffer)
{
	int iSIT_sectionLength = 0;
	pstTS_SIT->uitable_id = pucSectionBuffer[0];
	pstTS_SIT->uiSection_syntax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_SIT->uiDVB_Reserved_future_use_first = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_SIT->uiISO_Reserved_first = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_SIT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	pstTS_SIT->uiDVB_Reserved_future_use_second = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_SIT->uiISO_Reserved_second = pucSectionBuffer[5] >> 6;
	pstTS_SIT->uiVersion_number = (pucSectionBuffer[5] >> 1) & 0x1f;
	pstTS_SIT->uiCurrent_next_indicator = (pucSectionBuffer[5] << 7) >> 7;
	pstTS_SIT->uiSection_number = pucSectionBuffer[6];
	pstTS_SIT->uiLast_section_number = pucSectionBuffer[7];
	pstTS_SIT->uiDVB_Reserved_future_use_third = pucSectionBuffer[8] >> 4;
	pstTS_SIT->uiTransmission_info_loop_length = ((pucSectionBuffer[8] & 0x0f) << 8) | pucSectionBuffer[9];
	if (pstTS_SIT->uiTransmission_info_loop_length > 0)
	{
		memcpy(pstTS_SIT->aucSIT_info_descriptor, pucSectionBuffer + 10, pstTS_SIT->uiTransmission_info_loop_length);
	}
	iSIT_sectionLength = 3 + pstTS_SIT->uiSection_length;
	pstTS_SIT->uiCRC32 = (pucSectionBuffer[iSIT_sectionLength - 4] << 24) | (pucSectionBuffer[iSIT_sectionLength - 3] << 16) | (pucSectionBuffer[iSIT_sectionLength - 2] << 8) | (pucSectionBuffer[iSIT_sectionLength - 1]);
}

/******************************************
 *
 *输出SIT信息
 *
 ******************************************/
void PrintSIT(TS_SIT_T *pstTS_SIT)
{
	char acOutputPrefix[OUTPUT_PREFIX_SIZE] = { 0 };
	DUBUGPRINTF("\n-------------SIT info SITart-------------\n");

	DUBUGPRINTF("SIT->table_id: %02x\n", pstTS_SIT->uitable_id);
	DUBUGPRINTF("SIT->Section_syntax_indicator: %02x\n", pstTS_SIT->uiSection_syntax_indicator);
	DUBUGPRINTF("SIT->DVB_Reserved_future_use_first: %02x\n", pstTS_SIT->uiDVB_Reserved_future_use_first);
	DUBUGPRINTF("SIT->ISO_Reserved_first: %02x\n", pstTS_SIT->uiISO_Reserved_first);
	DUBUGPRINTF("SIT->Section_length: %02x\n", pstTS_SIT->uiSection_length);
	DUBUGPRINTF("SIT->DVB_Reserved_future_use_second: %02x\n", pstTS_SIT->uiDVB_Reserved_future_use_second);
	DUBUGPRINTF("SIT->ISO_Reserved_second: %02x\n", pstTS_SIT->uiISO_Reserved_second);
	DUBUGPRINTF("SIT->Version_number: %02x\n", pstTS_SIT->uiVersion_number);
	DUBUGPRINTF("SIT->Current_next_indicator: %02x\n", pstTS_SIT->uiCurrent_next_indicator);
	DUBUGPRINTF("SIT->Section_number: %02x\n", pstTS_SIT->uiSection_number);
	DUBUGPRINTF("SIT->Last_section_number: %02x\n", pstTS_SIT->uiLast_section_number);
	DUBUGPRINTF("SIT->DVB_Reserved_future_use_third: %02x\n", pstTS_SIT->uiDVB_Reserved_future_use_third);
	DUBUGPRINTF("SIT->Transmission_info_loop_length: %02x\n", pstTS_SIT->uiTransmission_info_loop_length);
	if (pstTS_SIT->uiTransmission_info_loop_length > 0)
	{
		memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
		sprintf(acOutputPrefix, "SIT->.");
		ParseDescriptor(pstTS_SIT->aucSIT_info_descriptor, pstTS_SIT->uiTransmission_info_loop_length, acOutputPrefix);
	}
	DUBUGPRINTF("SIT->CRC32: 0x%08lx\n", pstTS_SIT->uiCRC32);
	DUBUGPRINTF("\n-------------SIT info end-------------\n");

}

/******************************************
 *
 *从流中解析SIT信息
 *
 ******************************************/
int ParseSIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	DUBUGPRINTF("\n\n=================================ParseSIT_Table SITart================================= \n");
	int iTemp = 0;
	TS_SIT_T stTS_SIT = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4] = { 0 };
	unsigned int uiRecordGetSection[SECTION_COUNT_256] = { 0 };

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("Parse SIT error\n");
		return -1;
	}
	
	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, SIT_PID, SIT_TABLE_ID, &uiVersion);
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
					ParseSIT_Section(&stTS_SIT, ucSectionBuffer);
					PrintSIT(&stTS_SIT);
				}
				if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordGetSection))
				{
					DUBUGPRINTF("\n=================================ParseSIT_Table END=================================== \n\n");
					return 1;
				}
				break;
			case 2:
				break;
			case -1:
				DUBUGPRINTF("\n\n=================================ParseSIT_Table End================================= \n");
				return 1;
				break;
			default:
				LOG("ParseSIT_Table switch (iTemp) default\n");
				break;
		}
	}
	DUBUGPRINTF("\n\n=================================ParseSIT_Table End================================= \n");
	return -1;
}

