//============================================================================
// Name        : Semaphors.cpp
// Author      : Filip Przybysz
// Version     :
// Copyright   : 
// Description : Simple synchronization mechanisms
//============================================================================

#include <iostream>
#include <thread>
#include "semaphor.h"
#include "simpleQue.h"
#include "processes.h"

using namespace std;

int mutex, mutexCout, mutexProdEven, mutexProdOdd, mutexConsEven, mutexConsOdd;
int PROD_EVEN_NUM=5;
int PROD_ODD_NUM=5;
int CONS_EVEN_NUM=5;
int CONS_ODD_NUM=5;

void prepMutex()
{
	mutex=semAllocate(6584);
	mutexCout=semAllocate(6579);
	mutexProdEven=semAllocate(6580);
	mutexProdOdd=semAllocate(6581);
	mutexConsEven=semAllocate(6582);
	mutexConsOdd=semAllocate(6583);

	semInit(mutex, 1);
	semInit(mutexCout, 1);
	semInit(mutexProdEven, 0);
	semInit(mutexProdOdd, 0);
	semInit(mutexConsEven, 0);
	semInit(mutexConsOdd, 0);
}

void delMutex()
{
	semDeallocate(mutex);
	semDeallocate(mutexCout);
	semDeallocate(mutexProdEven);
	semDeallocate(mutexProdOdd);
	semDeallocate(mutexConsEven);
	semDeallocate(mutexConsOdd);
}

void thProdEven (simque<int>* que)
{
	while(1) {prodEvenRun(que);};
}

void thProdOdd (simque<int>* que)
{
	while(1) {prodOddRun(que);};
}

void thConsEven (simque<int>* que)
{
	while(1) {consEvenRun(que);};
}

void thConsOdd (simque<int>* que)
{
	while(1) {consOddRun(que);};
}

int main() {

	prepMutex();

	simque<int> Buff;

	thread PE[PROD_EVEN_NUM], PO[PROD_ODD_NUM], CE[CONS_EVEN_NUM], CO[CONS_ODD_NUM];

	for (int i=0; i<PROD_ODD_NUM; ++i)
	{
		PO[i]=thread(thProdOdd, &Buff);
	}
	for (int i=0; i<PROD_EVEN_NUM; ++i)
	{
		PE[i]=thread(thProdEven, &Buff);
	}
	for (int i=0; i<CONS_EVEN_NUM; ++i)
	{
		CE[i]=thread(thConsEven, &Buff);
	}
	for (int i=0; i<CONS_ODD_NUM; ++i)
	{
		CO[i]=thread(thConsOdd, &Buff);
	}

	for (int i=0; i<PROD_EVEN_NUM; ++i)
	{
		PE[i].join();
	}
	for (int i=0; i<PROD_ODD_NUM; ++i)
	{
		PO[i].join();
	}
	for (int i=0; i<CONS_EVEN_NUM; ++i)
	{
		CE[i].join();
	}
	for (int i=0; i<CONS_ODD_NUM; ++i)
	{
		CO[i].join();
	}

	/*
	cout<<mutex<<endl;
	cout<<mutexCout<<endl;
	cout<<mutexProdEven<<endl;
	cout<<mutexProdOdd<<endl;
	cout<<mutexConsEven<<endl;
	cout<<mutexConsOdd<<endl;
	*/
	printVar();

	delMutex();

	return 0;
}
