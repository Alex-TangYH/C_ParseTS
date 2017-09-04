/*
 * TsParser.h
 *
 *  Created on: 2017Äê8ÔÂ30ÈÕ
 *      Author: AlexTang
 */

#ifndef _TSPARSER_H_
#define _TSPARSER_H_

#define DEBUG
#define LOGPRINTF

#ifdef DEBUG
    #define DUBUGPRINTF printf
#else
    #define DUBUGPRINTF
#endif

#ifdef LOGPRINTF
    #define LOG printf
#else
    #define LOG
#endif


int parseStream();
#endif /* _TSPARSER_H_ */
