#ifndef _GET_SECTION_
#define _GET_SECTION_

typedef struct SECTION_HEAD_T
{
	unsigned int uiTable_id :8;
	unsigned int uiSection_syntax_indicator :1;
	unsigned int uiZero :1;
	unsigned int uiReservedFirst :2;
	unsigned int uiSection_Length :12;
	unsigned int uiTransport_stream_id :16;
	unsigned int uiReservedSecond :2;
	unsigned int uiVersion_number :5;
	unsigned int uiCurrent_next_indicator :1;
	unsigned int uiSection_number :8;
	unsigned int uiLast_section_number :8;
} SECTION_HEAD_T;

typedef struct TS_PACKAGE_HEAD_T
{
	unsigned int uiSync_byte :8;
	unsigned int uiTransport_error_indicator :1;
	unsigned int uiPayload_unit_start_indicator :1;
	unsigned int uiTransport_priority :1;
	unsigned int uiPID :13;
	unsigned int uiTransport_scrambling_control :2;
	unsigned int uiAdapptation_field_control :2;
	unsigned int uiContinuity_counter :4;
} TS_PACKAGE_HEAD_T;

int IsSectionGetBefore(unsigned char *pucSectionBuffer, unsigned int *puiRecordSectionNumber);
int IsAllSectionOver(unsigned char *pucSectionBuffer, unsigned int *puiRecordSectionNumber);
int GetOneSection(FILE *pfTsFile, int iTsLength, unsigned char *pucSectionBuffer, unsigned int uiPID, unsigned int uiTableId, unsigned int *puiVersion);
int GetOneSectionByPID(FILE *pfTsFile, int iTsLength, unsigned char *pucSectionBuffer, unsigned int uiPID, unsigned int *puiVersion);
#endif
