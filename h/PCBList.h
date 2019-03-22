/*
 * PCBList.h
 *
 *  Created on: Aug 13, 2018
 *      Author: OS1
 */

#ifndef MOJPROJ_OS1_PCBLIST_H_
#define MOJPROJ_OS1_PCBLIST_H_

#include "pcb.h"
#include "SCHEDULE.H"

class PCBList {
 public:
		struct Elem {
		Elem (PCB* p) : pcb(p), next(0) {}
		PCB* pcb;
		Elem* next;
		};

		int count;

	Elem *head, *tail;

	PCBList();
	~PCBList();

	//Standard linked list methods
	void addToEnd (PCB * p);
	void addToStart (PCB * p);
	PCB* takeFirst();
	PCB* takeLast();
	int howMany();
	void removeById(int iid);
	void putEveryoneInScheduler();


};

#endif /* MOJPROJ_OS1_PCBLIST_H_ */
