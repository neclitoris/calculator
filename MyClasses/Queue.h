#ifndef CALCULATOR_QUEUE_H
#define CALCULATOR_QUEUE_H

#include "Deque.h"

template <typename T>
class Queue : public Deque<T> {
private:
	T pop_front() = delete;

	T pop_back() = delete;

	void push_front(T value) = delete;

	void push_back(T value) = delete;

public:
	T pop() {
		return this->Deque<T>::pop_front();
	}

	Queue<T> &push(T value) {
		return this->Deque<T>::push_back(value), *this;
	}
};

#endif //CALCULATOR_QUEUE_H
