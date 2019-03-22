/*
 * PCBList.cpp
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#include "pcb.h"
#include "SCHEDULE.H"


	PCBList::PCBList(){
		head=tail=0;
		count = 0;
	};


	PCBList::~PCBList(){
	    while (head) { Elem* old = head; head = head->next; delete old; }
	    head = tail = 0;
	    count = 0;
	};


	void PCBList::addToEnd (PCB * p){
		tail = (!head ? head : tail->next) = new Elem(p);
		count++;
		return;
	}


	void PCBList::addToStart (PCB * p){
		Elem * t = new Elem(p);
		if (head == 0){
			head = tail = t;
			count++;
			return;
		}
		else{
			t->next= head;
			head = t;
			count++;
			return;
		}
	}


	PCB* PCBList::takeFirst(){
		PCB* tmpPCB=0;

		if (head!=0) {
			tmpPCB=head->pcb;
			Elem* tmpElem=head;
			head=head->next;
			delete tmpElem;
			count--;
		}
		if (head==0) tail=0;
		return tmpPCB;
	}


	PCB* PCBList::takeLast(){
		PCB* tmpPCB=0;
		Elem *tmpCurr, *tmpPrev;

		tmpCurr=head;

		while (tmpCurr->next!=0){
			tmpPrev=tmpCurr;
			tmpCurr=tmpCurr->next;
		}

		if (tmpCurr == tail && tmpCurr!=0 && tmpPrev!=0){
			tail = tmpPrev;
			tail->next = 0;
			tmpPCB = tmpCurr->pcb;
			count --;
			delete tmpCurr;
			return tmpPCB;
		}
		if (tmpCurr == tail && tmpCurr==head && tmpCurr!=0){
			tmpPCB = tmpCurr->pcb;
			delete tmpCurr;
			count = 0;
			tail = head = 0;
			return tmpPCB;
		}
		return 0;
	}

	int PCBList::howMany(){
		return count;
	}

	void PCBList::removeById(int iid){
			Elem *tmpCurr, *tmpPrev=0;
			tmpCurr=head;
			while(tmpCurr!=0){
				if (tmpCurr->pcb->id==iid){
					if(tmpCurr==head && tmpCurr==tail){
						delete tmpCurr;
						count--;
						head=tail=0;
						return;
					}
					if(tmpCurr==head){
						head = tmpCurr->next;
						delete tmpCurr;
						count--;
						return;
					}
					if(tmpCurr==tail){
						tail = tmpPrev;
						tail->next = 0;
						delete tmpCurr;
						count--;
						return;
					}
					//In the middle
					if(tmpCurr!=0 && tmpPrev!=0){
						tmpPrev->next=tmpCurr->next;
						delete tmpCurr;
						count--;
						return;
					}

				}
				tmpPrev=tmpCurr;
				tmpCurr=tmpCurr->next;
			}
			return;
		}

	void PCBList::putEveryoneInScheduler(){
	    while (head) {
	    	Elem* old = head;
	    	if (old->pcb->state == BLOCKED){ //POTENCIJALNA GRESKA
	    	old->pcb->state = READY;
	    	Scheduler::put(old->pcb);
	    	}
	    	head = head->next;
	    	delete old;
	    }
	    head = tail = 0;
	    count = 0;
	}
