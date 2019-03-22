/*
 * PCB.cpp
 *
 *  Created on: Aug 13, 2018
 *      Author: OS1
 */
#include <dos.h>
//#include "Utility.h"

#include "PCB.h"
#include "PCBList.h"
#include "SleepQ.h"
#include "IdleThr.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX_STACK_SIZE 65535

//Static variables initialization
PCB* PCB::runningPCB = 0;
int PCB::ID = 0;
PCBList* PCB::allPcbsList= new PCBList();
SleepQueue* PCB::sleepingList= new SleepQueue();
//Used in timer, for keeping timeSlice of running pcb
int PCB::counter=0;


PCB::PCB(Thread *thread, StackSize stackSize, Time timeSlice){

	this->myThread = thread;
	this->timeSlice = timeSlice;

	stackSize = MIN(stackSize, MAX_STACK_SIZE);

	stackSize/=sizeof(unsigned);
	stack = new unsigned[stackSize];

	ss=sp=bp=0; //WORKAROUND for bug detection, only temporary!

#ifndef BCC_BLOCK_IGNORE

	//Enable I flag in PSW
	stack[stackSize - 1] = 0x200;

	//Setting PC
	stack[stackSize - 2] = FP_SEG(PCB::wrapper);
	stack[stackSize - 3] = FP_OFF(PCB::wrapper);

	//Saving stack related registers SS, SP, BP
	this->sp = FP_OFF(&stack[stackSize - 12]);
	this->ss = FP_SEG(&stack[stackSize - 12]);
	bp=sp;

#endif

	this->waitToCompleteList = new PCBList();
	this->state = CREATED;
	this->timeToSleep=0;

	this->id=++ID;

}

PCB::~PCB() {

	//Removing this PCB from list of all PCBs
	allPcbsList->removeById(id);

	delete[] stack;
}

void PCB::sleep(Time timeToSleep){
	if (timeToSleep <= 0) dispatch();

	PCB::runningPCB->state = SLEEP;
	PCB::runningPCB->timeToSleep = timeToSleep;
	sleepingList->addInSorted(PCB::runningPCB);

	dispatch();
}

void PCB::start(){
#ifndef BCC_BLOCK_IGNORE
	lock()
#endif

	if (this->state == CREATED){
		this->state=READY;
		Scheduler::put(this);
	}

#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}

void PCB::waitToComplete(){
#ifndef BCC_BLOCK_IGNORE
		lock()
#endif
			if(PCB::runningPCB!= this && this->state != DONE && this->myThread!= IdleThread::idle){
				PCB::runningPCB->state=BLOCKED;
				waitToCompleteList->addToEnd(PCB::runningPCB);

				dispatch();
			}
#ifndef BCC_BLOCK_IGNORE
		unlock()
#endif
}

void PCB::wrapper(){

	PCB::runningPCB->myThread->run();
	PCB::runningPCB->state = DONE;

	//Iterating through list
    while (PCB::runningPCB->waitToCompleteList->head) {
    	PCBList::Elem* old = PCB::runningPCB->waitToCompleteList->head;
    	old->pcb->state=READY;
    	Scheduler::put(old->pcb);
    	PCB::runningPCB->waitToCompleteList->head = PCB::runningPCB->waitToCompleteList->head->next;
    	delete old;
    }
    PCB::runningPCB->waitToCompleteList->head = PCB::runningPCB->waitToCompleteList->tail = 0;
    PCB::runningPCB->waitToCompleteList->count = 0;

    dispatch();

}



