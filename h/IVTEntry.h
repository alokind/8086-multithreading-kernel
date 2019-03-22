/*
 * IVTEntry.h
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#ifndef MOJPROJ_OS1_IVTENTRY_H_
#define MOJPROJ_OS1_IVTENTRY_H_

#include "Utility.h"
#include "KernelEv.h"
#include "Event.h"

class KernelEv;

class IVTEntry{
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newRout);
	~IVTEntry();

	void signal();
	void callOldRout();
	void initKernelEvent(KernelEv* ev);
	void resetKernelEvent();


	static IVTEntry* InterruptVectorTable[256];
	static IVTEntry* getEntry(IVTNo ivtNo);

	pInterrupt oldRout;
	IVTNo ivtNo;
	KernelEv* myEvent;
};

#define PREPAREENTRY(noEntry, callOld)\
	void interrupt inter##noEntry(...); \
	IVTEntry newEntry##noEntry(noEntry, inter##noEntry); \
	void interrupt inter##noEntry(...) {\
		newEntry##noEntry.signal();\
		if (callOld == 1)\
		newEntry##noEntry.callOldRout();\
	}

#endif /* MOJPROJ_OS1_IVTENTRY_H_ */
