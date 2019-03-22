/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#include <dos.h>

#include "Utility.h";
#include "IVTEntry.h";

IVTEntry* IVTEntry::InterruptVectorTable[256] = { 0 };

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRout){

 if (ivtNo>=0 && ivtNo<256 && IVTEntry::InterruptVectorTable[ivtNo] == 0){
	this->ivtNo = ivtNo;
	this->oldRout = 0; //ONLY TEMPORARY, WORKARAOUND FOR BUG DETECTION
	this->myEvent = 0; //ONLY TEMPORARY, WORKARAOUND FOR BUG DETECTION

	IVTEntry::InterruptVectorTable[ivtNo] = this;

#ifndef BCC_BLOCK_IGNORE
	oldRout = getvect(ivtNo);
	setvect(ivtNo, newRout);
#endif
 }
}

IVTEntry::~IVTEntry(){

	this->oldRout=0;
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRout);
#endif
	IVTEntry::InterruptVectorTable[ivtNo] = 0;
}

void IVTEntry::signal(){

	if (myEvent==0) return;
	myEvent->signal();
}

void IVTEntry::callOldRout(){
	if (oldRout == 0) return;
	oldRout();
}

IVTEntry* IVTEntry::getEntry(IVTNo ivtNo){

	return IVTEntry::InterruptVectorTable[ivtNo];
}

void IVTEntry::initKernelEvent(KernelEv* ev){
	myEvent = ev;
}

void IVTEntry::resetKernelEvent(){
	myEvent = 0;
}

