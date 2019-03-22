/*
 * KernelEv.cpp
 *
 *  Created on: Aug 20, 2018
 *      Author: OS1
 */

#include "Utility.h"

#include "KernelEv.h"
#include "IVTEntry.h"
#include "PCB.h"
#include "Event.h"
#include "Schedule.h"



KernelEv::KernelEv(IVTNo ivtNo){

	this->val = 1;
	this->ivtNo = ivtNo;
	creator = PCB::runningPCB;
	IVTEntry::InterruptVectorTable[ivtNo]->initKernelEvent(this);
}

KernelEv::~KernelEv(){

	this->ivtNo = 0;
	this->creator = 0;
	IVTEntry::InterruptVectorTable[ivtNo]->resetKernelEvent();
}

void KernelEv::wait(){
	if (PCB::runningPCB != creator) return;
	else{
		if (--val<0) {
			if (val<-1) return; //ERROR
			creator->state=BLOCKED;
			dispatch();
		}
	}
}

void KernelEv::signal(){
	if(val++<0){
		creator->state=READY;
		Scheduler::put(creator);
	}
}
