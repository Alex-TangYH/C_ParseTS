#ifndef _PARSE_ST_H_
#define _PARSE_ST_H_

#define TS_ST_DATA_MAX 4193

typedef struct TS_ST_T
{
	unsigned int uitable_id :8;
	unsigned int uiSection_syntax_indicator :1;
	unsigned int uiReserved_future_use :1;
	unsigned int uiReserved :2;
	unsigned int uiSection_length :12;
	unsigned char aucST_data[TS_ST_DATA_MAX];
} TS_ST_T;

int ParseST_Table(FILE *pfTsFile, int iTsPosition, int iTsLength);
#endif
