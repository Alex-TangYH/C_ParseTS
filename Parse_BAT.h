#ifndef _PARSE_BAT_H_
#define _PARSE_BAT_H_

#define BAT_INFO_MAX 252
#define BAT_DESCRIPTOR_MAX 1024 * 4

typedef struct BAT_INFO_T
{
	unsigned int uiTransport_stream_id :16;
	unsigned int uiOriginal_network_id :16;
	unsigned int uiReserved_future_use_third :4;
	unsigned int uiTransport_descriptor_length :12;
	unsigned char aucDescriptor[BAT_DESCRIPTOR_MAX];
} BAT_INFO_T;

typedef struct TS_BAT_T
{
	unsigned int uitable_id :8;
	unsigned int uiSection_syntax_indicator :1;
	unsigned int uiReserved_future_use_first :1;
	unsigned int uiReserved_first :2;
	unsigned int uiSection_length :12;
	unsigned int uiBoquet_id :12;
	unsigned int uiReserved_second :2;
	unsigned int uiVersion_number :5;
	unsigned int uiCurrent_next_indicator :1;
	unsigned int uiSection_number :8;
	unsigned int uiLast_section_number :8;
	unsigned int uiReserved_future_use_second :4;
	unsigned int uiBoquet_descriptor_length :12;
	unsigned char aucDescriptor[BAT_DESCRIPTOR_MAX];
	unsigned int uiReserved_third :4;
	unsigned int uiTransport_stream_loop_lenth :12;
	BAT_INFO_T stBAT_info[BAT_INFO_MAX];
	
	unsigned int uiCRC_32 :32;

} TS_BAT_T;

int ParseBAT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength);
#endif
