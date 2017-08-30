#include <stdio.h>
#include <string.h>

#include "Parse_NIT.h"
#include "Parse_Descriptor.h"
#include "Get_Section.h"
#include "Parse_DesciptorStream.h"
#include "TsParser.h"


#define NIT_PID 0x0010
#define NIT_TABLE_ID 0x40

#define SECTION_COUNT_256 256
#define SECTION_MAX_LENGTH_4096 1024*4
#define INITIAL_VERSION 0xff
#define OUTPUT_PREFIX_SIZE 256

/******************************************
 *
 *解析NIT段头部信息
 *
 ******************************************/
void ParseNIT_SectionHead(TS_NIT_T * pstTS_NIT, unsigned char *pucSectionBuffer)
{
	int iNIT_SectionLength = 0;
	
	pstTS_NIT->uiTable_id = pucSectionBuffer[0];
	pstTS_NIT->uiSection_sytax_indicator = pucSectionBuffer[1] >> 7;
	pstTS_NIT->uiReserved_future_use_first = (pucSectionBuffer[1] >> 6) & 0x01;
	pstTS_NIT->uiReserved_first = (pucSectionBuffer[1] >> 4) & 0x03;
	pstTS_NIT->uiSection_length = ((pucSectionBuffer[1] & 0x0f) << 8) | pucSectionBuffer[2];
	pstTS_NIT->uiNetwork_id = (pucSectionBuffer[3] << 8) | pucSectionBuffer[4];
	pstTS_NIT->uiReserved_second = pucSectionBuffer[5] >> 6;
	pstTS_NIT->uiVersion_number = (pucSectionBuffer[5] >> 1) & 0x1f;
	pstTS_NIT->uiCurrent_next_indicator = pucSectionBuffer[5] >> 7;
	pstTS_NIT->uiSection_number = pucSectionBuffer[6];
	pstTS_NIT->uiLast_section_number = pucSectionBuffer[7];
	pstTS_NIT->uiReserved_future_use_second = pucSectionBuffer[8] >> 4;
	pstTS_NIT->uiNetwork_descriptor_length = ((pucSectionBuffer[8] & 0x0f) << 8) | pucSectionBuffer[9];
	memcpy(pstTS_NIT->ucDescriptor, pucSectionBuffer + 10, pstTS_NIT->uiNetwork_descriptor_length);
	pstTS_NIT->uiReserved_future_use_third = pucSectionBuffer[10 + pstTS_NIT->uiNetwork_descriptor_length] >> 4;
	pstTS_NIT->uiTransport_stream_loop_Length = ((pucSectionBuffer[10 + pstTS_NIT->uiNetwork_descriptor_length] & 0x0f) << 8) | pucSectionBuffer[11 + pstTS_NIT->uiNetwork_descriptor_length];
	iNIT_SectionLength = 3 + pstTS_NIT->uiSection_length;
	pstTS_NIT->uiCRC_32 = (pucSectionBuffer[iNIT_SectionLength - 4] << 24) | (pucSectionBuffer[iNIT_SectionLength - 3] << 16) | (pucSectionBuffer[iNIT_SectionLength - 2] << 8) | (pucSectionBuffer[iNIT_SectionLength - 1]);
}

/******************************************
 *
 *解析NIT段信息
 *
 ******************************************/

int ParseNIT_Section(TS_NIT_T * pstTS_NIT, unsigned char *pucSectionBuffer)
{
	int iNetworkDescriptorLength = 0;
	int iNIT_Length = 0;
	int iTransportStreamPostion = 0;
	int iTransportStreamCount = 0;
	memset(pstTS_NIT, 0, sizeof(TS_NIT_T));

	ParseNIT_SectionHead(pstTS_NIT, pucSectionBuffer);
	iNetworkDescriptorLength = pstTS_NIT->uiNetwork_descriptor_length;
	iNIT_Length = 3 + pstTS_NIT->uiSection_length;

	for (iTransportStreamPostion = 10 + iNetworkDescriptorLength + 2; iTransportStreamPostion < iNIT_Length - 4; iTransportStreamPostion += 6)
	{
		pstTS_NIT->stNIT_stream[iTransportStreamCount].uiTransport_stream_id = (pucSectionBuffer[iTransportStreamPostion] << 8) | pucSectionBuffer[1 + iTransportStreamPostion];
		pstTS_NIT->stNIT_stream[iTransportStreamCount].uiOriginal_network_id = (pucSectionBuffer[2 + iTransportStreamPostion] << 8) | pucSectionBuffer[3 + iTransportStreamPostion];
		pstTS_NIT->stNIT_stream[iTransportStreamCount].uiReserved_future_use_fourth = pucSectionBuffer[4 + iTransportStreamPostion] >> 4;
		pstTS_NIT->stNIT_stream[iTransportStreamCount].uiTransport_descriport_length = ((pucSectionBuffer[4 + iTransportStreamPostion] & 0x0f) << 8) | pucSectionBuffer[5 + iTransportStreamPostion];
		if (0 != pstTS_NIT->stNIT_stream[iTransportStreamCount].uiTransport_descriport_length)
		{
			memcpy(pstTS_NIT->stNIT_stream[iTransportStreamCount].aucDescriptor, pucSectionBuffer + iTransportStreamPostion + 6, pstTS_NIT->stNIT_stream[iTransportStreamCount].uiTransport_descriport_length);
			iTransportStreamPostion += pstTS_NIT->stNIT_stream[iTransportStreamCount].uiTransport_descriport_length;
		}
		iTransportStreamCount++;
	}

	return iTransportStreamCount;
}

void PrintNIT(TS_NIT_T * pstTS_NIT, int iNIT_TransportStreamCount)
{
	int iLoopTime = 0;
	char acOutputPrefix[OUTPUT_PREFIX_SIZE] = { 0 };
	DUBUGPRINTF("\n");
	DUBUGPRINTF("-------------NIT info start-------------\n");
	DUBUGPRINTF("NIT->Table_id : 0x%02x \n", pstTS_NIT->uiTable_id);
	DUBUGPRINTF("NIT->Section_syntax_indicator : 0x%02x \n", pstTS_NIT->uiSection_sytax_indicator);
	DUBUGPRINTF("NIT->Reserved_future_use_first : 0x%02x \n", pstTS_NIT->uiReserved_future_use_first);
	DUBUGPRINTF("NIT->Reserved_first : 0x%02x \n", pstTS_NIT->uiReserved_first);
	DUBUGPRINTF("NIT->Section_length : 0x%02x \n", pstTS_NIT->uiSection_length);
	DUBUGPRINTF("NIT->Network_id : 0x%02x \n", pstTS_NIT->uiNetwork_id);
	DUBUGPRINTF("NIT->Reserved_second : 0x%02x \n", pstTS_NIT->uiReserved_second);
	DUBUGPRINTF("NIT->Version_number : 0x%02x \n", pstTS_NIT->uiVersion_number);
	DUBUGPRINTF("NIT->Current_next_indicator : 0x%02x \n", pstTS_NIT->uiCurrent_next_indicator);
	DUBUGPRINTF("NIT->Section_number : 0x%02x \n", pstTS_NIT->uiSection_number);
	DUBUGPRINTF("NIT->Last_section_number : 0x%02x \n", pstTS_NIT->uiLast_section_number);
	DUBUGPRINTF("NIT->Network_descriptor_length : 0x%02x \n", pstTS_NIT->uiNetwork_descriptor_length);

	if (pstTS_NIT->uiNetwork_descriptor_length > 0)
	{
		memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
		sprintf(acOutputPrefix, "NIT->");
		ParseDescriptor(pstTS_NIT->ucDescriptor, pstTS_NIT->uiNetwork_descriptor_length, acOutputPrefix);
	}
	DUBUGPRINTF("NIT->Reserved_future_use_second : 0x%02x \n", pstTS_NIT->uiReserved_future_use_second);
	DUBUGPRINTF("NIT->Transport_stream_loop_Length : 0x%02x \n", pstTS_NIT->uiTransport_stream_loop_Length);
	DUBUGPRINTF("NIT->CRC_32 : 0x%02x \n", pstTS_NIT->uiCRC_32);

	if (0 != pstTS_NIT->uiTransport_stream_loop_Length)
	{
		for (iLoopTime = 0; iLoopTime < iNIT_TransportStreamCount; iLoopTime++)
		{
			DUBUGPRINTF("NIT->NIT_stream[%d].Transport_stream_id : 0x%02x \n", iLoopTime, pstTS_NIT->stNIT_stream[iLoopTime].uiTransport_stream_id);
			DUBUGPRINTF("NIT->NIT_stream[%d].Original_network_id : 0x%02x \n", iLoopTime, pstTS_NIT->stNIT_stream[iLoopTime].uiOriginal_network_id);
			DUBUGPRINTF("NIT->NIT_stream[%d].Reserved_future_use_fourth : 0x%02x \n", iLoopTime, pstTS_NIT->stNIT_stream[iLoopTime].uiReserved_future_use_fourth);
			DUBUGPRINTF("NIT->NIT_stream[%d].uiTransport_descriport_length : 0x%02x \n", iLoopTime, pstTS_NIT->stNIT_stream[iLoopTime].uiTransport_descriport_length);
			if (pstTS_NIT->stNIT_stream[iLoopTime].uiTransport_descriport_length > 0)
			{
				memset(acOutputPrefix, 0, OUTPUT_PREFIX_SIZE);
				sprintf(acOutputPrefix, "NIT->NIT_stream[%d].", iLoopTime);
				ParseDescriptor(pstTS_NIT->stNIT_stream[iLoopTime].aucDescriptor, pstTS_NIT->stNIT_stream[iLoopTime].uiTransport_descriport_length, acOutputPrefix);
			}
		}
	}

	DUBUGPRINTF("-------------NIT info end-------------\n\n");
}

/******************************************
 *
 *解析NIT信息
 *
 ******************************************/
int ParseNIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength)
{
	DUBUGPRINTF("\n\n=================================ParseNIT_Table Start================================= \n");
	TS_NIT_T stTS_NIT = { 0 };
	unsigned char ucSectionBuffer[SECTION_MAX_LENGTH_4096] = { 0 };
	unsigned int uiRecordGetSection[SECTION_COUNT_256] = { 0 };
	unsigned int uiVersion = INITIAL_VERSION;
	int iTemp = 0;
	int iTransportStreamCount = 0;

	if (-1 == fseek(pfTsFile, iTsPosition, SEEK_SET))
	{
		DUBUGPRINTF("ParseNIT_Table error when fseek(pfTsFile, iTsPosition, SEEK_SET)\n");
		return -1;
	}

	while (!feof(pfTsFile))
	{
		iTemp = GetOneSection(pfTsFile, iTsLength, ucSectionBuffer, NIT_PID, NIT_TABLE_ID, &uiVersion);

		if (0 == iTemp)
		{
			uiVersion = INITIAL_VERSION;
			memset(uiRecordGetSection, 0, sizeof(char) * SECTION_COUNT_256);
			fseek(pfTsFile, 0 - iTsLength, SEEK_CUR);
		}
		if (1 == iTemp)
		{
			if (0 == IsSectionGetBefore(ucSectionBuffer, uiRecordGetSection))
			{
				iTransportStreamCount = ParseNIT_Section(&stTS_NIT, ucSectionBuffer);
				PrintNIT(&stTS_NIT, iTransportStreamCount);
			}
			if (1 == IsAllSectionOver(ucSectionBuffer, uiRecordGetSection))
			{
				DUBUGPRINTF("\n=================================ParseNIT_Table END=================================== \n\n");
				return 1;
			}
		}
		if (-1 == iTemp)
		{
			DUBUGPRINTF("There is not NIT table in the transport stream \n ");
			DUBUGPRINTF("\n=================================ParseNIT_Table END=================================== \n\n");
			return 0;
		}
	}
	
	DUBUGPRINTF("\n=================================ParseNIT_Table END=================================== \n\n");
	return 1;
}
