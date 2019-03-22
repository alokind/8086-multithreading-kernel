/*
 * Kernel.cpp
 *
 *  Created on: Aug 21, 2018
 *      Author: OS1
 */
#include <dos.h>
#include <STDIO.H>

#include "Utility.h"
#include "Thread.h"
#include "PCB.h"
#include "PCBList.h"
#include "SleepQ.h"
#include "Semaphor.h"
#include "Event.h"
#include "KernSem.h"
#include "KernelEv.h"
#include "IdleThr.h"
#include "syncPrnt.h"




pInterrupt oldInterruptRoutine;

unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int context_switch_requested;
extern void tick();


void interrupt timer(...);

void inic() {
#ifndef BCC_BLOCK_IGNORE
	lock()

	oldInterruptRoutine = getvect(8);
	setvect(8, timer);

	unlock()
#endif
}

void restore() {
#ifndef BCC_BLOCK_IGNORE
	lock()

	setvect(8, oldInterruptRoutine);

	unlock()

#endif
}
/*
void interrupt timer(...){

		if (context_switch_requested == 1 || PCB::counter == 0) {

			//Saving PCB registers
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif

			PCB::runningPCB->sp = tsp;
			PCB::runningPCB->ss = tss;
			PCB::runningPCB->bp = tbp;

			//Taking different thread from Scheduler
			//PAZI OVDE NE PROVERAVAS DA LI JE RUNNING

			//DO NOT PUT IDLE THREAD IN SCHEDULER
			if (PCB::runningPCB != IdleThread::idle->getPCB()) {
				PCB::runningPCB->state=READY;
				Scheduler::put(PCB::runningPCB);
			}

			PCB::runningPCB = Scheduler::get();

			//If there are no threads in Scheduler, run Idle
			if (PCB::runningPCB == 0) PCB::runningPCB = IdleThread::idle->getPCB();

			tsp = PCB::runningPCB->sp;
			tss = PCB::runningPCB->ss;
			tbp = PCB::runningPCB->bp;

			PCB::counter = PCB::runningPCB->timeSlice;

			//Restoring PCB registers
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
#endif
		}

		//Calling oldInterruptRoutine that was in 0x08
		//System calls this routine for maintenance every 55ms
		//It is called only if there was no context switch requested
		if(context_switch_requested == 0) {
			PCB::counter--;
			oldInterruptRoutine();
			tick();

			if (PCB::sleepingList->howMany()!=0){
				PCB::sleepingList->decrementAllTimeToSleep();
			}
		}

		context_switch_requested = 0;

}
*/

void interrupt timer(...) {

	if (PCB::counter == 0 || context_switch_requested) {
#ifndef BCC_BLOCK_IGNORE
		asm {
			mov tss, ss;
			mov tsp, sp;
			mov tbp, bp;
		};
#endif

		PCB::runningPCB->ss = tss;
		PCB::runningPCB->sp = tsp;
		PCB::runningPCB->bp = tbp;


		if (PCB::runningPCB != 0 && PCB::runningPCB->state == RUNNING && PCB::runningPCB->myThread != IdleThread::idle) {
			PCB::runningPCB->state = READY;
			Scheduler::put(PCB::runningPCB);
		}

		PCB::runningPCB = Scheduler::get();

		if (PCB::runningPCB == 0) {
			PCB::runningPCB = IdleThread::idle->getPCB();
		}

		PCB::runningPCB->state = RUNNING;

		tsp = PCB::runningPCB->sp;
		tss = PCB::runningPCB->ss;
		tbp = PCB::runningPCB->bp;

		PCB::counter = PCB::runningPCB->timeSlice;

#ifndef BCC_BLOCK_IGNORE
		asm {
			mov bp, tbp
			mov ss, tss
			mov sp, tsp
		}
#endif

	}


	if (!context_switch_requested) {

		PCB::counter--;
		tick();
		oldInterruptRoutine();
		if (PCB::sleepingList->howMany() != 0) {

			PCB::sleepingList->decrementAllTimeToSleep();

		}
	}

	context_switch_requested = 0;

}

extern int userMain(int argc, char* argv[]);

class MainThread : public Thread {
	int argc;
	char **argv;
public:

	MainThread(int argc, char **argv) : Thread(){
		this->argc = argc;
		this->argv = argv;
	}
	~MainThread() { waitToComplete(); }

	static int val;
	void run() { val = userMain(argc, argv); }

};

int MainThread::val = -1 ;


int main(int argc, char *argv[]) {

	PCB *mainPcb = new PCB(0, 0, 1);
	mainPcb->state = RUNNING;
	PCB::runningPCB = mainPcb;
	MainThread* mainThr= new MainThread(argc, argv);

	mainThr->start();
	inic();

	delete mainThr;

	restore();

	int ret =  MainThread::val;

	delete mainPcb;
	delete IdleThread::idle;

	return ret;

}
