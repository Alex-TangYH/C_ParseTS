#ifndef _PARSE_NIT_H_
#define _PARSE_NIT_H_

#define NIT_DESCRIPTOR_MAX 1024
#define NIT_STREAM_MAX 128

typedef struct TS_NIT_STREAM_T
{
	unsigned int uiTransport_stream_id :16;
	unsigned int uiOriginal_network_id :16;
	unsigned int uiReserved_future_use_fourth :4;
	unsigned int uiTransport_descriport_length :12;
	unsigned char aucDescriptor[NIT_DESCRIPTOR_MAX];
} TS_NIT_STREAM_T;

typedef struct TS_NIT_T
{
	unsigned int uiTable_id :8;
	unsigned int uiSection_sytax_indicator :1;
	unsigned int uiReserved_future_use_first :1;
	unsigned int uiReserved_first :2;
	unsigned int uiSection_length :12;
	unsigned int uiNetwork_id :16;
	unsigned int uiReserved_second :2;
	unsigned int uiVersion_number :5;
	unsigned int uiCurrent_next_indicator :1;
	unsigned int uiSection_number :8;
	unsigned int uiLast_section_number :8;
	unsigned int uiReserved_future_use_second :4;
	unsigned int uiNetwork_descriptor_length :12;
	unsigned char ucDescriptor[NIT_DESCRIPTOR_MAX];
	unsigned int uiReserved_future_use_third :4;
	unsigned int uiTransport_stream_loop_Length :12;
	TS_NIT_STREAM_T stNIT_stream[NIT_STREAM_MAX];
	unsigned int uiCRC_32 :32;
} TS_NIT_T;

int ParseNIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength);
#endif
