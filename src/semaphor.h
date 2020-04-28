/*
 * semaphor.h
 *
 *  Created on: Apr 7, 2019
 *      Author: filip
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

//All semaphores are binary!!!

#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

union semun {
            int val;
            struct semid_ds *buf;
            unsigned short  *array;
};


int semAllocate (key_t key) {
	return semget(key, 1, 0666 | IPC_CREAT);
}

int semDeallocate (int semid) {
	union semun arg;
	return semctl(semid, 1, IPC_RMID, arg);
}

int semInit(int semid, int value) {
	if (value!=0 && value!=1) return -2;
	union semun arg;
	unsigned short vals[1];
	vals[0] = value;
	arg.array = vals;
	return semctl(semid, 0, SETALL, arg);
}

int semP (int semid) {
	struct sembuf sem = { 0, -1, SEM_UNDO };
	return semop(semid, &sem, 1);
}

int semV (int semid) {
	if (semctl(semid, 0, GETVAL, 0)==1) return 0;
	struct sembuf sem = { 0, 1, SEM_UNDO };
	return semop(semid, &sem, 1);
}

int semGetVal(int semid)
{
	return semctl(semid, 0, GETVAL, 0);
}

#endif /* SEMAPHOR_H_ */
