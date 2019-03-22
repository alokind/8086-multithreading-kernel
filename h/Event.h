/*
 * Event.h
 *
 *  Created on: Aug 19, 2018
 *      Author: OS1
 */

#ifndef MOJPROJ_OS1_EVENT_H_
#define MOJPROJ_OS1_EVENT_H_

#include "Utility.h"
#include "KernelEv.h"


class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:
	friend class KernelEv;
 	 void signal(); // can call KernelEv

private:
 	 KernelEv* myImpl;

};

#endif /* MOJPROJ_OS1_EVENT_H_ */
