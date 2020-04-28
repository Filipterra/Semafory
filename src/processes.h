/*
 * processes.h
 *
 *  Created on: Apr 7, 2019
 *      Author: filip
 */

#ifndef PROCESSES_H_
#define PROCESSES_H_

#include <queue>
#include <iostream>
#include "semaphor.h"
#include "simpleQue.h"

using namespace std;

int prodEvenWaiting=0;
int prodOddWaiting=0;
int consEvenWaiting=0;
int consOddWaiting=0;

int nextEven=0;
int nextOdd=1;

extern int mutex, mutexCout, mutexProdEven, mutexProdOdd, mutexConsEven, mutexConsOdd;


void printVar()
{
	cout<<"prodEvenWaiting="<<prodEvenWaiting<<endl;
	cout<<"prodOddWaiting="<<prodOddWaiting<<endl;
	cout<<"consEvenWaiting="<<consEvenWaiting<<endl;
	cout<<"consOddWaiting="<<consOddWaiting<<endl;

	cout<<"nextEven="<<nextEven<<endl;
	cout<<"nextOdd="<<nextOdd<<endl;

	cout<<mutex<<"="<<semGetVal(mutex)<<endl
	<<mutexCout<<"="<<semGetVal(mutexCout)<<endl
	<<mutexProdEven<<"="<<semGetVal(mutexProdEven)
	<<endl<<mutexProdOdd<<"="<<semGetVal(mutexProdOdd)
	<<endl<<mutexConsEven<<"="<<semGetVal(mutexConsEven)
	<<endl<<mutexConsOdd<<"="<<semGetVal(mutexConsOdd)<<endl;
}


void printQue(simque<int>* que)
{
queue<int> T;
	while (que->countEven()+que->countOdd()>0)
		{
		cout<<que->front()<<" ";
		T.push(que->get());
		}
	cout<<endl;

	while(!T.empty()){
		que->put(T.front());
		T.pop();
	}
}

bool prodEvenCanProduce (simque<int>* que)
{
	if (que->countEven()<10) return true;
	else return false;
}

bool prodOddCanProduce (simque<int>* que)
{
	if (que->countEven()>que->countOdd()) return true;
	else return false;
}

bool consEvenCanConsume (simque<int>* que)
{
	if (que->countEven()>0 && (que->countEven())+(que->countOdd())>=3) return true;
	else return false;
}

bool consOddCanConsume (simque<int>* que)
{
	if (que->countOdd()>0 && (que->countEven())+(que->countOdd())>=7) return true;
	else return false;
}

void prodEvenProduce (simque<int>* que)
{
	que->put(nextEven);
	nextEven=(nextEven+2)%100;
}

void prodOddProduce (simque<int>* que)
{
	que->put(nextOdd);
	nextOdd=(nextOdd+2)%100;
}

int consEvenConsume (simque<int>* que)
{
	queue<int> T;
	while (que->front()%2==1) T.push(que->get());
	int Ret=que->get();
	while (que->countEven()+que->countOdd()>0) T.push(que->get());
	while(!T.empty()){
		que->put(T.front());
		T.pop();
	}
	return Ret;
}

int consOddConsume (simque<int>* que)
{
	queue<int> T;
	while (que->front()%2==0) T.push(que->get());
	int Ret=que->get();
	while (que->countEven()+que->countOdd()>0) T.push(que->get());
	while(!T.empty()){
		que->put(T.front());
		T.pop();
	}
	return Ret;
}

void prodEvenRun (simque<int>* que)
{
	semP(mutex);

	if (!prodEvenCanProduce(que))
	{
		++prodEvenWaiting;
		semV(mutex);
		semP(mutexProdEven);
	}

	prodEvenProduce(que);

	semP(mutexCout);
	cout<<"prodEven"<<endl;
	printQue(que);
	//cout<<"Odd="<<que->countOdd()<<endl;
	//cout<<"Even="<<que->countEven()<<endl;
	semV(mutexCout);

	if (prodOddCanProduce(que) && prodOddWaiting>0)
	{
		--prodOddWaiting;
		semV(mutexProdOdd);
	}
	else if (consOddCanConsume(que) && consOddWaiting>0)
	{
		--consOddWaiting;
		semV(mutexConsOdd);
	}
	else if (consEvenCanConsume(que) && consEvenWaiting>0)
	{
		--consEvenWaiting;
		semV(mutexConsEven);
	}
	else semV(mutex);
}

void prodOddRun (simque<int>* que)
{
	semP(mutex);

	if (!prodOddCanProduce(que))
	{
		++prodOddWaiting;
		semV(mutex);
		semP(mutexProdOdd);
	}

	prodOddProduce(que);

	semP(mutexCout);
	cout<<"prodOdd"<<endl;
	printQue(que);
	//cout<<"Odd="<<que->countOdd()<<endl;
	//cout<<"Even="<<que->countEven()<<endl;
	semV(mutexCout);

	if (prodEvenCanProduce(que) && prodEvenWaiting>0)
	{
		--prodEvenWaiting;
		semV(mutexProdEven);
	}
	else if (consOddCanConsume(que) && consOddWaiting>0)
	{
		--consOddWaiting;
		semV(mutexConsOdd);
	}
	else if (consEvenCanConsume(que) && consEvenWaiting>0)
	{
		--consEvenWaiting;
		semV(mutexConsEven);
	}
	else semV(mutex);
}

void consEvenRun (simque<int>* que)
{
	semP(mutex);

	if (!consEvenCanConsume(que))
	{
		++consEvenWaiting;
		semV(mutex);
		semP(mutexConsEven);
	}

	consEvenConsume(que);

	semP(mutexCout);
	cout<<"consEven"<<endl;
	printQue(que);
	//cout<<"Odd="<<que->countOdd()<<endl;
	//cout<<"Even="<<que->countEven()<<endl;
	//cout<<que->countOdd()+que->countEven()<<endl;
	semV(mutexCout);

	if (consOddCanConsume(que) && consOddWaiting>0)
	{
		--consOddWaiting;
		semV(mutexConsOdd);
	}
	else if (prodEvenCanProduce(que) && prodEvenWaiting>0)
	{
		--prodEvenWaiting;
		semV(mutexProdEven);
	}
	else if (prodOddCanProduce(que) && prodOddWaiting>0)
	{
		--prodOddWaiting;
		semV(mutexProdOdd);
	}
	else semV(mutex);
}

void consOddRun (simque<int>* que)
{
	semP(mutex);

	if (!consOddCanConsume(que))
	{
		++consOddWaiting;
		semV(mutex);
		semP(mutexConsOdd);
	}

	consOddConsume(que);

	semP(mutexCout);
	cout<<"consOdd"<<endl;
	printQue(que);
	//cout<<"Odd="<<que->countOdd()<<endl;
	//cout<<"Even="<<que->countEven()<<endl;
	//cout<<que->countOdd()+que->countEven()<<endl;
	semV(mutexCout);

	if (consEvenCanConsume(que) && consEvenWaiting>0)
	{
		--consEvenWaiting;
		semV(mutexConsEven);
	}
	else if (prodOddCanProduce(que) && prodOddWaiting>0)
	{
		--prodOddWaiting;
		semV(mutexProdOdd);
	}
	else if (prodEvenCanProduce(que) && prodEvenWaiting>0)
	{
		--prodEvenWaiting;
		semV(mutexProdEven);
	}
	else semV(mutex);
}

#endif /* PROCESSES_H_ */
