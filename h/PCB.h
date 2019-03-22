/*
 * PCB.h
 *
 *  Created on: Jun 15, 2018
 *      Author: OS1
 */

#ifndef MOJPROJ_OS1_PCB_H_
#define MOJPROJ_OS1_PCB_H_

#include "PCBList.h"
#include "SleepQ.h"
#include "Thread.h"


enum State {CREATED = 0, READY = 1, BLOCKED = 2, DONE = 3, SLEEP = 4, RUNNING};


class PCBList;
class Thread;

class PCB{
public:

	//Constructor and destructor
	PCB(Thread *thread, StackSize stackSize, Time timeSlice);
	~PCB();

	Thread *myThread;
	Time timeSlice;

	State state;

	//Stack
	unsigned *stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;

	void waitToComplete();
	void start();

	static void wrapper();

	static void sleep(Time timeToSleep);
	Time timeToSleep;

	PCBList* waitToCompleteList;

	static int counter;
	static PCB* runningPCB;
	static PCBList* allPcbsList;
	static SleepQueue* sleepingList;

	int getId() const{
		return id;
	}

	static int ID;
	int id;
};



#endif /* MOJPROJ_OS1_PCB_H_ */
