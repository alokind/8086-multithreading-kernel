/*
 * KernSem.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#include "PCBList.h"
#include "Utility.h"
#include "KernSem.h"



KernelSem::KernelSem (int init){

	this->value = init;
	blockedList = new PCBList();
}

KernelSem::~KernelSem (){


	//If list isn't empty
	if (blockedList->howMany()!=0){
	    while (blockedList->head) {
	    	PCBList::Elem* old = blockedList->head;
	    	old->pcb->state=READY;
	    	Scheduler::put(old->pcb);
	    	blockedList->head = blockedList->head->next;
	    	delete old;
	    }
	    blockedList->head = blockedList->tail = 0;
	    blockedList->count = 0;
	}

	delete blockedList;
}

int KernelSem::wait (int toBlock){

#ifndef BCC_BLOCK_IGNORE
	lock()
#endif

int ret = 0;

if (!toBlock && value<=0) ret = -1;
else
	if (--value<0) {
		ret = 1;
		blockSem();
	}
#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
	return ret;
}


void KernelSem::signal (){

	#ifndef BCC_BLOCK_IGNORE
	lock()
#endif

			if(value++<0)
		deblockSem();

#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}

int KernelSem:: val () const {

	return this->value;
}

void KernelSem:: blockSem() {

	PCB::runningPCB->state=BLOCKED;
	blockedList->addToStart(PCB::runningPCB);
	dispatch();
}

void KernelSem:: deblockSem(){

	PCB* temp = blockedList->takeFirst();
	temp->state=READY;
	Scheduler::put(temp);
}


