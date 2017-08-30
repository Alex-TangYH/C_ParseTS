#ifndef _PARSE_SIT_H_
#define _PARSE_SIT_H_

#define SIT_DESCRIPTOR_INFO_MAX 1024 * 4

typedef struct SIT__T
{

} SIT__T;

typedef struct TS_SIT_T
{
	unsigned int uitable_id :8;
	unsigned int uiSection_syntax_indicator :1;
	unsigned int uiDVB_Reserved_future_use_first :1;
	unsigned int uiISO_Reserved_first :2;
	unsigned int uiSection_length :12;
	unsigned int uiDVB_Reserved_future_use_second :16;
	unsigned int uiISO_Reserved_second :2;
	unsigned int uiVersion_number :5;
	unsigned int uiCurrent_next_indicator :1;
	unsigned int uiSection_number :8;
	unsigned int uiLast_section_number :8;
	unsigned int uiDVB_Reserved_future_use_third :4;
	unsigned int uiTransmission_info_loop_length :12;
	unsigned char aucSIT_info_descriptor[SIT_DESCRIPTOR_INFO_MAX];
	unsigned long uiCRC32 :32;
} TS_SIT_T;

int ParseSIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength);
#endif
