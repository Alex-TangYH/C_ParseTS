#ifndef _PARSE_PMT_H_
#define _PARSE_PMT_H_

#define PMT_VUDIO_COUNT 32
#define PMT_DESCRIPTOR_MAX 1024
#define PMT_PROGRAM_DESCRIPTOR_MAX 4096

typedef struct PMT_CAT_INFO_T
{
	unsigned int uiPMT_CA_system_id :16;
	unsigned int uiPMT_CA_PID :13;
} PMT_CAT_INFO_T;

typedef struct PMT_INFO_T
{
	unsigned int uiProgramNumber :16;
	unsigned int uiVideoPID :13;
	unsigned int uiAudioPID[PMT_VUDIO_COUNT];
} PMT_INFO_T;

typedef struct TS_PMT_STREAM_T
{
	unsigned int uiStream_type :8;
	unsigned int uiReserved_fifth :3;
	unsigned int uiElementary_PID :13;
	unsigned int uiReserved_sixth :4;
	unsigned int uiES_info_length :12;
	unsigned char aucDescriptor[PMT_DESCRIPTOR_MAX];
} TS_PMT_STREAM_T;

typedef struct TS_PMT_T
{
	unsigned int uiTable_id :8;
	unsigned int uiSection_syntax_indicator :1;
	unsigned int uiZero :1;
	unsigned int uiReserved_first :2;
	unsigned int uiSection_length :12;
	unsigned int uiProgram_number :16;
	unsigned int uiReserved_second :2;
	unsigned int uiVersion_number :5;
	unsigned int uiCurrent_next_indicator :1;
	unsigned int uiSection_number :8;
	unsigned int uiLast_section_number :8;
	unsigned int uiReserved_third :3;
	unsigned int uiPCR_PID :13;
	unsigned int uiReserved_fourth :4;
	unsigned int uiProgram_info_length :12;
	unsigned char aucProgramDescriptor[PMT_PROGRAM_DESCRIPTOR_MAX];
	TS_PMT_STREAM_T stPMT_Stream[PMT_DESCRIPTOR_MAX];
	unsigned long uiCRC_32 :32;
} TS_PMT_T;

int ParsePMT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength, unsigned int uiPMTPid, PMT_INFO_T *pstPMT_Info);
#endif
