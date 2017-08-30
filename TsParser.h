/*
 * TsParser.h
 *
 *  Created on: 2017年8月30日
 *      Author: Administrator
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



#endif /* _TSPARSER_H_ */
