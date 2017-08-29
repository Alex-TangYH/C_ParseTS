#ifndef _PARSE_RST_H_
#define _PARSE_RST_H_

#define TS_RST_INFO_MAX 1024 * 4

typedef struct RST_INFO_T
{
	unsigned int uiTransport_stream_id :16;
	unsigned int uiOriginal_network_id :16;
	unsigned int uiService_id :16;
	unsigned int uiEvent_id :16;
	unsigned int uiReserved_future_use_second :5;
	unsigned int uiRunning_status :3;
} RST_INFO_T;

typedef struct TS_RST_T
{
	unsigned int uitable_id :8;
	unsigned int uiSection_syntax_indicator :1;
	unsigned int uiReserved_future_use_first :1;
	unsigned int uiReserved :2;
	unsigned int uiSection_length :12;
	RST_INFO_T astRST_info[TS_RST_INFO_MAX];
} TS_RST_T;

int ParseRST_Table(FILE *pfTsFile, int iTsPosition, int iTsLength);
#endif
