#ifndef _PARSE_DIT_H_
#define _PARSE_DIT_H_

typedef struct TS_DIT_T
{
	unsigned int uitable_id :8;
	unsigned int uiSection_syntax_indicator :1;
	unsigned int uiReserved_future_use_first :1;
	unsigned int uiReserved :2;
	unsigned int uiSection_length :12;
	unsigned int uiTransition_flag :1;
	unsigned int uiReserved_future_use_second :7;
} TS_DIT_T;

int ParseDIT_Table(FILE *pfTsFile, int iTsPosition, int iTsLength);
#endif
