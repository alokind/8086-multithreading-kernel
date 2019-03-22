/*
 * syncPrnt.cpp
 *
 *  Created on: Aug 22, 2018
 *      Author: OS1
 */

#include "syncPrnt.h"

#ifndef BCC_BLOCK_IGNORE
int syncPrintf(const char *format, ...){
	int res;
	va_list args;
	lock();
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock();
		return res;
}

#endif

