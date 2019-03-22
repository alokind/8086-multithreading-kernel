/*
 * SleepQ.h
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#ifndef MOJPROJ_OS1_SLEEPQ_H_
#define MOJPROJ_OS1_SLEEPQ_H_

#include "pcb.h"
#include "SCHEDULE.H"

class SleepQueue {
 private:
		struct Elem {
		Elem (PCB* p) : pcb(p), next(0) {}
		PCB* pcb;
		Elem* next;
		};

		int count;

 public:
	Elem *head, *tail;


	SleepQueue();
	~SleepQueue();

	//Standard linked list methods
	void addToEnd (PCB * p);
	void addToStart (PCB * p);
	PCB* takeFirst();
	PCB* takeLast();
	int howMany();
	void removeById(int iid);

	//For Sleep Queue, sorted by PCB::timeToSleep
	void addInSorted(PCB * p);
	void decrementAllTimeToSleep();

};




#endif /* MOJPROJ_OS1_SLEEPQ_H_ */
