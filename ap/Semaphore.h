/*
 * Semaphore.h
 *
 *  Created on: Sep 13, 2011
 *      Author: pajace
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

class Semaphore {
public:
	static Semaphore* getInstance();
	virtual ~Semaphore();

	int set_semvalue(void);
	void del_semvalue(void);
	int semaphore_p(void);
	int semaphore_v(void);
	int sem_id;

private:
	Semaphore(){};
	Semaphore(Semaphore const&){};
	static Semaphore* mInstance;
};

#endif /* SEMAPHORE_H_ */
