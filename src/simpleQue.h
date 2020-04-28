/*
 * simpleQue.h
 *
 *  Created on: Apr 7, 2019
 *      Author: filip
 */

#ifndef SIMPLEQUE_H_
#define SIMPLEQUE_H_

#include "simpleQue.h"
#include <deque>
#include <algorithm>

//simple additional queue functions

using namespace std;

template <typename T>
class simque
{
	deque<T> dque;
public:
	simque() {};
	~simque() {};

	T& front()
	{
		return dque.front();
	}

	void put(const T& val)
	{
		dque.push_back(val);
	}

	T get()
	{
		T Ret=dque.front();
		dque.pop_front();
		return Ret;
	}

	int countEven();
	int countOdd();

};

	template<> int simque<int>::countEven()
{
	int S=0;
	for_each (dque.begin(), dque.end(), [&](const int& i) { if(i%2==0) ++S; });
	return S;
}

	template<> int simque<int>::countOdd()
{
	int S=0;
	for_each (dque.begin(), dque.end(), [&](const int& i) { if(i%2==1) ++S; });
	return S;
}

#endif /* SIMPLEQUE_H_ */
