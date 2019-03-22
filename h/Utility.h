/*
 * Utility.h
 *
 *  Created on: Aug 17, 2018
 *      Author: OS1
 */

#ifndef MOJPROJ_OS1_UTILITY_H_
#define MOJPROJ_OS1_UTILITY_H_

#define lock() { asm pushf; asm cli; }
#define unlock() { asm popf; }

typedef unsigned char IVTNo;
typedef void interrupt(*pInterrupt)(...);



#endif /* MOJPROJ_OS1_UTILITY_H_ */
