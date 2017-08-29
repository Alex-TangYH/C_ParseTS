#ifndef _PARSE_PAT_H_
#define _PARSE_PAT_H_

#define PAT_PROGARM_MAX 512

typedef struct TS_PAT_PROGRAM_T
{
	unsigned int uiProgram_number :16;
	unsigned int uiReserved_third :3;
	unsigned int uiProgram_map_PID :13;
} TS_PAT_PROGRAM_T;

typedef struct TS_PAT_T
{
	unsigned int uiTable_id :8;
	unsigned int uiSection_syntax_indicator :1;
	unsigned int uiZero :1;
	unsigned int uiReserved_first :2;
	unsigned int uiSection_length :12;
	unsigned int uiTransport_stream_id :16;
	unsigned int uiReserved_second :2;
	unsigned int uiVersion_number :5;
	unsigned int uiCurrent_next_indicator :1;
	unsigned int uiSection_number :8;
	unsigned int uiLast_section_number :8;

	TS_PAT_PROGRAM_T stPAT_Program[PAT_PROGARM_MAX];

	unsigned int uiNetwork_PID :13;
	unsigned int uiCRC_32 :32;
} TS_PAT_T;

typedef struct PAT_INFO_T
{
	unsigned int uiProgramNumber :16;
	unsigned int uiPMT_PID :13;
} PAT_INFO_T;

int ParsePAT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, PAT_INFO_T *pstPAT_Info);
#endif
