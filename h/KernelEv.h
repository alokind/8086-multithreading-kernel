/*
 * KernelEv.h
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#ifndef MOJPROJ_OS1_KERNELEV_H_
#define MOJPROJ_OS1_KERNELEV_H_

#include "Utility.h"

#include "IVTEntry.h"
#include "PCB.h"

class KernelEv {
public:
	KernelEv (IVTNo ivtNo);
	~KernelEv ();

	void wait ();
 	void signal(); // can call KernelEv

 	PCB* creator;
 	IVTNo ivtNo;
 	int val;
};



#endif /* MOJPROJ_OS1_KERNELEV_H_ */
