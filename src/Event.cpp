/*
 * Event.cpp
 *
 *  Created on: Aug 20, 2018
 *      Author: OS1
 */

#include "Utility.h"

#include "Event.h"
#include "KernelEv.h"


Event::Event(IVTNo ivtNo) {

#ifndef BCC_BLOCK_IGNORE
	lock()
#endif
		myImpl = new KernelEv(ivtNo);
#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}


Event::~Event() {
	delete myImpl;
}


void Event::wait() {

#ifndef BCC_BLOCK_IGNORE
	lock()
#endif
		myImpl->wait();
#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}

void Event::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock()
#endif
		myImpl->signal();
#ifndef BCC_BLOCK_IGNORE
	unlock()
#endif
}



