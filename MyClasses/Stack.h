#ifndef CALCULATOR_STACK_H
#define CALCULATOR_STACK_H

#include "Deque.h"

template <typename T>
class Stack : public Deque<T> {
private:
	T &front() const = delete;

	T &back() const = delete;

	T pop_front() = delete;

	T pop_back() = delete;

	void push_front(T value) = delete;

	void push_back(T value) = delete;

public:
	T &top() const {
		return this->Deque<T>::front();
	}

	T pop() {
		return this->Deque<T>::pop_front();
	}

	Stack<T> &push(T value) {
		return this->Deque<T>::push_front(value), *this;
	}
};

#endif //CALCULATOR_STACK_H
