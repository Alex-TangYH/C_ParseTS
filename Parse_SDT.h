#ifndef _PARSE_SDT_H_
#define _PARSE_SDT_H_

#define SDT_INFO_COUNT_MAX 202
#define SDT_INFO_DESCRIPTOR_MAX 205 //1002 * 8

typedef struct SDT_INFO_T
{
	unsigned int uiService_id :16;
	unsigned int uiReserved_future_use_third :6;
	unsigned int uiEIT_schedule_flag :1;
	unsigned int uiEIT_present_following_flag :1;
	unsigned int uiRunning_status :3;
	unsigned int uiFree_CA_mode :1;
	unsigned int uiDescriptor_loop_length :12;
	unsigned char aucDescriptor[SDT_INFO_DESCRIPTOR_MAX];
} SDT_INFO_T;

typedef struct SDT_IDENTIFICATION_INFO_T
{
	unsigned int uiTable_id :8;
	unsigned int uiOriginal_network_id :16;
	unsigned int uiTransport_stream_id :16;
	unsigned int uiVersion_number :5;
	unsigned int uiSection_number :8;
} SDT_IDENTIFICATION_INFO_T;

typedef struct TS_SDT_T
{
	unsigned int uiTable_id :8;
	unsigned int uiSection_syntax_indicator :1;
	unsigned int uiReserved_future_use_first :1;
	unsigned int uiReserved_first :2;
	unsigned int uiSection_length :12;
	unsigned int uiTransport_stream_id :16;
	unsigned int uiReserved_second :2;
	unsigned int uiVersion_number :5;
	unsigned int uiCurrent_next_indicator :1;
	unsigned int uiSection_number :8;
	unsigned int uiLast_section_number :8;
	unsigned int uiOriginal_network_id :16;
	unsigned int uiReserved_future_use_second :8;
	SDT_INFO_T stSDT_info[SDT_INFO_COUNT_MAX];
	unsigned long uiCRC_32 :32;
} TS_SDT_T;

int ParseSDT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength);
#endif
