/*
 * Thread.cpp
 *
 *  Created on: Aug 18, 2018
 *      Author: OS1
 */

#include "Thread.h";
#include "PCB.h";
#include "Utility.h";


int Thread::ID = 0;

Thread::Thread (StackSize stackSize, Time timeSlice){
#ifndef BCC_BLOCK_IGNORE
	lock()
#endif

	this->myPCB = new PCB(this, stackSize, timeSlice);
	this->id=++ID;

#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}

Thread::~Thread() {
#ifndef BCC_BLOCK_IGNORE
	lock()
#endif

	waitToComplete();
	delete myPCB;

#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}

void Thread::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
	lock()
#endif

		myPCB->waitToComplete();

#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}

void Thread::start(){
#ifndef BCC_BLOCK_IGNORE
	lock()
#endif

		myPCB->start();

#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}

void Thread::sleep(Time timeToSleep){

	PCB::sleep(timeToSleep);

}

extern void interrupt timer(...);
extern volatile int context_switch_requested;

void dispatch() {
#ifndef BCC_BLOCK_IGNORE
		lock()
#endif

		context_switch_requested = 1;
		timer();

#ifndef BCC_BLOCK_IGNORE
		unlock()
#endif
}



