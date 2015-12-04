/*
 * Semaphore.cpp
 *
 *  Created on: Sep 13, 2011
 *      Author: pajace
 */

#include "Semaphore.h"
#include <iostream>
#include <cstdlib>
#include <sys/sem.h>
#include "semun.h"
#include "IptLog.h"
using namespace std;

Semaphore* Semaphore::mInstance = NULL;

Semaphore::~Semaphore() {
	// TODO Auto-generated destructor stub
}

Semaphore* Semaphore::getInstance() {
	if( mInstance==NULL ) {
		mInstance = new Semaphore();
	}
	return mInstance;
}


int Semaphore::set_semvalue(void) {
	IptLog::getInstance()->logDebug("Semaphore::set_semvalue");
	union semun sem_union;
	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
		return (0);
	IptLog::getInstance()->logDebug("Set Semaphore p");
	return (1);
}

void Semaphore::del_semvalue(void) {
	IptLog::getInstance()->logDebug("Semaphore::Semaphore::del_semvalue");
	union semun sem_union;
	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		IptLog::getInstance()->logError("Failed to delete semaphore");
}

int Semaphore::semaphore_p(void) {
	IptLog::getInstance()->logDebug("Semaphore::Try semaphore_p");
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; /* P() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		IptLog::getInstance()->logError("semaphore_p failed");
		return (0);
	}
	return (1);
}

int Semaphore::semaphore_v(void) {
	IptLog::getInstance()->logDebug("Semaphore::Try semaphore_v");
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		IptLog::getInstance()->logError("semaphore_v failed");
		return (0);
	}
	return (1);
}

