/*
 * Semaphor.h
 *
 *  Created on: Aug 18, 2018
 *      Author: OS1
 */

#ifndef MOJPROJ_OS1_SEMAPHOR_H_
#define MOJPROJ_OS1_SEMAPHOR_H_

#include "KernSem.h"

class KernelSem;

class Semaphore {

public:

 Semaphore (int init=1);
 virtual ~Semaphore ();

 virtual int wait (int toBlock);
 virtual void signal();
 int val () const; // Returns the current value of the semaphore

private:

 KernelSem* myImpl;

};



#endif /* MOJPROJ_OS1_SEMAPHOR_H_ */
