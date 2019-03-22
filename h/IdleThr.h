/*
 * IdleThr.h
 *
 *  Created on: Aug 22, 2018
 *      Author: OS1
 */

#ifndef OS1_IDLETHR_H_
#define OS1_IDLETHR_H_

#include "syncPrnt.h"

#include "Thread.h"
#include <STDIO.H>

class IdleThread : public Thread{
public:
	IdleThread() : Thread(defaultStackSize, 1) {};
	void run() {
		//syncPrintf("Idle");
		while(1) {
			dispatch();
		}
	};

	static IdleThread * idle;

};



#endif /* OS1_IDLETHR_H_ */
