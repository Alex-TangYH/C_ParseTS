#include <stdio.h>
#include <string.h>

#include "Get_Section.h"
#include "TsParser.h"
#include "DVB_CRC32.h"

#define PACKAGE_MAX_LENGTH_204 204
#define PRINTFTS_HEAD 0
#define SECTION_MAX_LENGTH_4096 1024*4
#define INITIAL_VERSION 0xff
#define DEFAULT_TABLE_ID 0x80

#define BAT_TABLE_ID 0x4A
#define PAT_TABLE_ID 0x00
#define PMT_TABLE_ID 0x02
#define NIT_TABLE_ID 0x40
#define SDT_TABLE_ID 0x42
#define TOT_TABLE_ID 0x73
#define SIT_TABLE_ID 0x7E
#define CAT_TABLE_ID 0x01
#define EIT_PF_TABLE_ID 0x4e
#define EIT_SCHEDULE50_TABLE_ID 0x50
#define EIT_SCHEDULE51_TABLE_ID 0x51
/******************************************
 *
 * �ж��Ƿ��ѻ�ȡ
 *
 ******************************************/
int IsSectionGetBefore(unsigned char *pucSectionBuffer, unsigned int *puiRecordSectionNumber)
{
	int iLoopTime = 1;
	int iLength = (int) puiRecordSectionNumber[0];
	unsigned int uiSectionNumber = pucSectionBuffer[6];

	for (iLoopTime = 1; iLoopTime < iLength + 1; iLoopTime++)
	{
		if (puiRecordSectionNumber[iLoopTime] == uiSectionNumber)
		{
			return 1;
		}
	}
	puiRecordSectionNumber[iLoopTime] = uiSectionNumber;
	puiRecordSectionNumber[0]++;
	return 0;
}

/******************************************
 *
 * �ж��Ƿ��ѻ�ȡ
 *
 ******************************************/
int IsAllSectionOver(unsigned char *pucSectionBuffer, unsigned int *puiRecordSectionNumber)
{
	unsigned int uiSectionCount = puiRecordSectionNumber[0];
	unsigned int uiLastSectionNumber = pucSectionBuffer[7];

	if (uiSectionCount == (uiLastSectionNumber + 1))
	{
		return 1;
	}
	return 0;
}

/******************************************
 *
 *ͨ��PID����section
 *
 ******************************************/

int GetOneSectionByPID(FILE *pfTsFile, int iTsLength, unsigned char *pucSectionBuffer, unsigned int uiPID, unsigned int *puiVersion)
{
	unsigned int uiVersion = INITIAL_VERSION;
	return GetOneSection(pfTsFile, iTsLength, pucSectionBuffer, uiPID, DEFAULT_TABLE_ID, &uiVersion);
}

/*********************************************
 * ����TS�İ�ͷ
 *********************************************/

static void ParseTS_PackageHead(TS_PACKAGE_HEAD_T *pstTS_PackageHead, unsigned char *pucPackageBuffer)
{
	pstTS_PackageHead->uiSync_byte = pucPackageBuffer[0];
	pstTS_PackageHead->uiTransport_error_indicator = pucPackageBuffer[1] >> 7;
	pstTS_PackageHead->uiPayload_unit_start_indicator = (pucPackageBuffer[1] >> 6) & 0x01;
	pstTS_PackageHead->uiTransport_priority = (pucPackageBuffer[1] >> 5) & 0x01;
	pstTS_PackageHead->uiPID = ((pucPackageBuffer[1] & 0x1F) << 8) | pucPackageBuffer[2];
	pstTS_PackageHead->uiTransport_scrambling_control = pucPackageBuffer[3] >> 6;
	pstTS_PackageHead->uiAdapptation_field_control = (pucPackageBuffer[3] >> 4) & 0x03;
	pstTS_PackageHead->uiContinuity_counter = pucPackageBuffer[3] & 0x0f;
}

/*********************************************
 * ����TS����Ч���صĿ�ʼλ��
 * ��ȡ������������е��������ֶε�ֵ
 * ���Ϊ0x00��0x10����û����Ч���ط���-1��
 * ���Ϊ0x01��0x11��������Ч���صĿ�ʼλ��
 *********************************************/
int GetTheLoadBeginPostion(unsigned char *pucPackageBuffer)
{
	int iLoadBeginPostion = -1;
	TS_PACKAGE_HEAD_T stTS_PackageHead = { 0 };

	ParseTS_PackageHead(&stTS_PackageHead, pucPackageBuffer);
	switch (stTS_PackageHead.uiAdapptation_field_control)
	{
		case 0:
			return -1;
			break;
		case 1:
			iLoadBeginPostion = 4;
			break;
		case 2:
			return -1;
			break;
		case 3:
			iLoadBeginPostion = 4 + 1 + pucPackageBuffer[4];
			break;
	}
	if (stTS_PackageHead.uiPayload_unit_start_indicator)
	{
		iLoadBeginPostion = iLoadBeginPostion + pucPackageBuffer[iLoadBeginPostion] + 1;
	}
	return iLoadBeginPostion;
}

/*********************************************
 * �ѻ������ݸ��Ƶ�Section��
 *********************************************/
static void PutTheBufferToSection(unsigned char *pucSectionBuffer, unsigned char *pucPackageBuffer, int *piAlreadyAddSection, SECTION_HEAD_T *pstPAT_SectionHead, int iLoadBeginPosition)
{
	int iCopyLength = 0;

	iCopyLength = 188 - iLoadBeginPosition;
	if ((int) (pstPAT_SectionHead->uiSection_Length + 3) < (188 - iLoadBeginPosition + (*piAlreadyAddSection)))
	{
		iCopyLength = pstPAT_SectionHead->uiSection_Length + 3 - (*piAlreadyAddSection);
	}
	memcpy(pucSectionBuffer + (*piAlreadyAddSection), pucPackageBuffer + iLoadBeginPosition, iCopyLength);
	(*piAlreadyAddSection) = (*piAlreadyAddSection) + iCopyLength;
}

/*********************************************
 * �ж�һ��Section�Ƿ����
 *********************************************/
static int IsOneSectionOver(SECTION_HEAD_T stSectionHead, int iAddLength)
{
	int iSectionLength = (int) (stSectionHead.uiSection_Length + 3);
	if (iAddLength == iSectionLength)
	{
		return 1;
	}
	return 0;
}

/*********************************************
 * ����Section�汾����
 *********************************************/
static int IsVersionChange(SECTION_HEAD_T *pstSectionHead, unsigned int *puiRecordVersion)
{
	if (INITIAL_VERSION == (*puiRecordVersion))
	{
		(*puiRecordVersion) = pstSectionHead->uiVersion_number;
	}
	if ((*puiRecordVersion) != pstSectionHead->uiVersion_number)
	{
		return 1;
	}
	return 0;
}

/*********************************************
 * ����TSͷ����Ϣ�ı�־λ
 * ����1��ʾû������
 * ����-1��ʾ��Ҫ����
 *********************************************/
static int CheckIndicateOfPackageHead(TS_PACKAGE_HEAD_T stTS_PackageHead)
{
	if (stTS_PackageHead.uiTransport_error_indicator == 1)
	{
		return -1;
	}

	return 1;
}

/*********************************************
 * ���TS_HEAD��Ϣ
 *********************************************/
static void PrintTS_PES_HEAD(TS_PACKAGE_HEAD_T stTS_PackageHead)
{
	DUBUGPRINTF("TS_PackageHead.Sync_byte: 0x%02x\n", stTS_PackageHead.uiSync_byte);
	DUBUGPRINTF("TS_PackageHead.Transport_error_indicator: 0x%02x\n", stTS_PackageHead.uiTransport_error_indicator);
	DUBUGPRINTF("TS_PackageHead.Payload_unit_start_indicator: 0x%02x\n", stTS_PackageHead.uiPayload_unit_start_indicator);
	DUBUGPRINTF("TS_PackageHead.Transport_priority: 0x%02x\n", stTS_PackageHead.uiTransport_priority);
	DUBUGPRINTF("TS_PackageHead.PID: 0x%02x\n", stTS_PackageHead.uiPID);
	DUBUGPRINTF("TS_PackageHead.Transport_scrambling_control: 0x%02x\n", stTS_PackageHead.uiTransport_scrambling_control);
	DUBUGPRINTF("TS_PackageHead.Adapptation_field_control: 0x%02x\n", stTS_PackageHead.uiAdapptation_field_control);
	DUBUGPRINTF("TS_PackageHead.Continuity_counter: 0x%02x\n", stTS_PackageHead.uiContinuity_counter);
}

/*********************************************
 * ����PAT Sectionͷ����������
 *********************************************/

void ParsePATSectionHeader(SECTION_HEAD_T *pstSectionHead, unsigned char *pucPackageBuffer)
{
	int iPayloadPosition = -1;

	iPayloadPosition = GetTheLoadBeginPostion(pucPackageBuffer);
	pstSectionHead->uiTable_id = pucPackageBuffer[iPayloadPosition];
	pstSectionHead->uiSection_syntax_indicator = pucPackageBuffer[1 + iPayloadPosition] >> 7;
	pstSectionHead->uiZero = (pucPackageBuffer[1 + iPayloadPosition] >> 6) & 0x1;
	pstSectionHead->uiReservedFirst = (pucPackageBuffer[1 + iPayloadPosition] >> 4) & 0x3;
	pstSectionHead->uiSection_Length = (pucPackageBuffer[1 + iPayloadPosition] & 0xf) << 8 | (pucPackageBuffer[2 + iPayloadPosition]);
	pstSectionHead->uiTransport_stream_id = pucPackageBuffer[3 + iPayloadPosition] << 8 | pucPackageBuffer[4 + iPayloadPosition];
	pstSectionHead->uiReservedSecond = pucPackageBuffer[5 + iPayloadPosition] >> 6;
	pstSectionHead->uiVersion_number = (pucPackageBuffer[5 + iPayloadPosition] >> 1) & 0x1f;
	pstSectionHead->uiCurrent_next_indicator = (pucPackageBuffer[5 + iPayloadPosition] << 7) >> 7;
	pstSectionHead->uiSection_number = pucPackageBuffer[6 + iPayloadPosition];
	pstSectionHead->uiLast_section_number = pucPackageBuffer[7 + iPayloadPosition];
}

/*********************************************
 *
 * �ж�TableId�Ƿ���������CRC32У����ı�
 *
 *********************************************/
int isHasCRC32_TableId(unsigned int uiTableId)
{
	int iIndex = 0;
	unsigned int auiHasCRC32_TableId[11] = { BAT_TABLE_ID, PAT_TABLE_ID, PMT_TABLE_ID, SDT_TABLE_ID, SIT_TABLE_ID, CAT_TABLE_ID, NIT_TABLE_ID, TOT_TABLE_ID, EIT_PF_TABLE_ID, EIT_SCHEDULE50_TABLE_ID, EIT_SCHEDULE51_TABLE_ID };
	for (iIndex = 0; iIndex < 11; iIndex++)
	{
		if (uiTableId == auiHasCRC32_TableId[iIndex])
		{
			return 1;
		}
		else
		{
		}
	}
	return 0;
}

/*********************************************
 *
 * ����TS��һ��Section
 * -1����������û��Section
 * 0���汾�仯��
 * 1����ȡ����ȷ��Section
 * 2:�����Ǵ����Section
 *
 *********************************************/
int GetOneSection(FILE *pfTsFile, int iTsLength, unsigned char *pucSectionBuffer, unsigned int uiPID, unsigned int uiTableId, unsigned int *puiVersion)
{
	int iPayloadPosition = -1;
	int iFlagSectionStart = 0;
	int iAlreadyAddSection = 0;
	int iLoadBeginPosition = 0;
	unsigned char ucPackageBuffer[PACKAGE_MAX_LENGTH_204] = { 0 };
	TS_PACKAGE_HEAD_T stTS_PackageHead = { 0 };
	SECTION_HEAD_T stSectionHead = { 0 };
	memset(pucSectionBuffer, 0, sizeof(char) * SECTION_MAX_LENGTH_4096);
	while (!feof(pfTsFile))
	{
		if ((sizeof(char) * iTsLength) != (fread((unsigned char *) ucPackageBuffer, sizeof(char), iTsLength, pfTsFile)))
		{
			break;
		}
		ParseTS_PackageHead(&stTS_PackageHead, ucPackageBuffer);

		//���TS_HEAD��Ϣ
		if (1 == PRINTFTS_HEAD)
		{
			PrintTS_PES_HEAD(stTS_PackageHead);
		}

		//���head��־
		if (-1 == CheckIndicateOfPackageHead(stTS_PackageHead))
		{
			continue;
		}

		if ((stTS_PackageHead.uiPID == uiPID) && (0x47 == stTS_PackageHead.uiSync_byte))
		{

			iPayloadPosition = GetTheLoadBeginPostion(ucPackageBuffer);
			if (-1 == iPayloadPosition) /* û����Ч���� */
			{
				continue;
			}

			if ((1 == stTS_PackageHead.uiPayload_unit_start_indicator) && ((ucPackageBuffer[iPayloadPosition] == uiTableId) || (DEFAULT_TABLE_ID == uiTableId)) && (1 != iFlagSectionStart))
			{
				ParsePATSectionHeader(&stSectionHead, ucPackageBuffer);

				if (1 == IsVersionChange(&stSectionHead, puiVersion))
				{
					return 0; /* version number change */
				}
				iFlagSectionStart = 1;
				iLoadBeginPosition = GetTheLoadBeginPostion(ucPackageBuffer);
				PutTheBufferToSection(pucSectionBuffer, ucPackageBuffer, &iAlreadyAddSection, &stSectionHead, iLoadBeginPosition);
			}
			else	// ������ݴ���
			{
				if (1 == iFlagSectionStart)
				{
					if (1 == stTS_PackageHead.uiPayload_unit_start_indicator)
					{
						PutTheBufferToSection(pucSectionBuffer, ucPackageBuffer, &iAlreadyAddSection, &stSectionHead, 4 + 1);
					}
					else
					{
						PutTheBufferToSection(pucSectionBuffer, ucPackageBuffer, &iAlreadyAddSection, &stSectionHead, 4);
					}
				}
			}
		}
		if (1 == iFlagSectionStart)
		{
			if (1 == IsOneSectionOver(stSectionHead, iAlreadyAddSection))
			{
				return 1;

				if (1 == isHasCRC32_TableId(uiTableId))	// ���������ҪCRC32У��ı�� �Ƚ���CRC32У�� ͨ��TABLEID�ж�
				{
					if (1 == Verify_CRC_32(pucSectionBuffer))
					{
						return 1;
					}
					else
					{
						LOG("Verify_CRC_32 a CRC error \n");
						return 2;
					}
				}
				else
				{
					return 1;
				}
			}
		}
	}
	return -1;
}

