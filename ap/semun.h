/*
 * semun.h
 *
 *  Created on: Sep 13, 2011
 *      Author: pajace
 */

#ifndef SEMUN_H_

union semun {
	int val; /* Value for SETVAL */
	struct semid_ds *buf; /* Buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* Array for GETALL, SETALL */
	struct seminfo *__buf; /* Buffer for IPC_INFO (Linux-specific) */
};

#define SEMUN_H_

#endif /* SEMUN_H_ */
