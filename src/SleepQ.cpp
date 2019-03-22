/*
 * SleepQ.cpp
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#include "pcb.h"
#include "SCHEDULE.H"


	SleepQueue::SleepQueue(){
		head=tail=0;
		count = 0;
	};


	SleepQueue::~SleepQueue(){
	    while (head) { Elem* old = head; head = head->next; delete old; }
	    head = tail = 0;
	    count = 0;
	};


	void SleepQueue::addToEnd (PCB * p){
		tail = (!head ? head : tail->next) = new Elem(p);
		count++;
		return;
	}


	void SleepQueue::addToStart (PCB * p){
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


	PCB* SleepQueue::takeFirst(){
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


	PCB* SleepQueue::takeLast(){
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

	int SleepQueue::howMany(){
		return count;
	}

	void SleepQueue::removeById(int iid){
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

	//FOR SLEEP LIST
	//This is ordinary linked list sorted by timeToSleep.
	//It can be done by saving just the difference of timeToSleep
	//and then decrementing only the first one in the list
	//but ain't no body got time for that
	void SleepQueue::addInSorted(PCB * p){
		Elem * t = new Elem(p);

		//If the list is empty
		if(head==0 && tail==0){
			head=tail=t;
			count++;
			return;
		}
		//If it's on the beginning
		if (t->pcb->timeToSleep <= head->pcb->timeToSleep){
			t->next= head;
			head = t;
			count++;
			return;
		}
		//If it's on the end
		if (t->pcb->timeToSleep >= tail->pcb->timeToSleep){
					tail->next= t;
					tail = t;
					count++;
					return;
		}
		//If it's in the middle
		if (t->pcb->timeToSleep > head->pcb->timeToSleep &&
			t->pcb->timeToSleep < tail->pcb->timeToSleep	){
			Elem *curr=0, *prev=0;
			curr = head;

			while(curr->pcb->timeToSleep < t->pcb->timeToSleep){
				prev = curr;
				curr = curr->next;
			}
			prev->next=t;
			t->next=curr;
			count++;
			return;
		}
		//ERROR
		return;
	}

	void SleepQueue::decrementAllTimeToSleep(){
		if (count==0) return;

		//Decrementing all of the PCBs in the list timeToSleep after 55ms
		//when CPU calls Interrupt Routine at 0x08
		//if timeToSleep, marking it as ready and putting it back in Scheduler
		Elem* iterator=0;
		for (iterator = head; iterator!=0; iterator = iterator->next){
			iterator->pcb->timeToSleep--;
			if (iterator->pcb->timeToSleep==0){
				iterator->pcb->state=READY;
				Scheduler::put(iterator->pcb);
				removeById(iterator->pcb->id);
			}
		}


	}

