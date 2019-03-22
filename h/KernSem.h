/*
 * KernelSem.h
 *
 *  Created on: Aug 18, 2018
 *      Author: OS1
 */

#ifndef MOJPROJ_OS1_KERNSEM_H_
#define MOJPROJ_OS1_KERNSEM_H_

#include "PCBList.h"

class PCBList;

class KernelSem {

public:

 KernelSem (int init=1);
 virtual ~KernelSem ();

 virtual int wait (int toBlock);
 virtual void signal();
 int val () const; // Returns the current value of the semaphore

private:

 PCBList *blockedList;
 int value;
 void blockSem();
 void deblockSem();

};


#endif /* MOJPROJ_OS1_KERNSEM_H_ */
