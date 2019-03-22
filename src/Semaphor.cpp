/*
 * Semaphor.cpp
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#include "Utility.h"

#include "KernSem.h"
#include "Semaphor.h"


Semaphore::Semaphore (int init){

#ifndef BCC_BLOCK_IGNORE
	lock()
#endif

	myImpl = new KernelSem(init);

#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}

Semaphore::~Semaphore (){
#ifndef BCC_BLOCK_IGNORE
	lock()
#endif

	delete myImpl;

#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}

int Semaphore::wait (int toBlock){

	return myImpl->wait(toBlock);
}


void Semaphore::signal (){

	myImpl->signal();
}

int Semaphore:: val () const {

	return myImpl->val();
}


