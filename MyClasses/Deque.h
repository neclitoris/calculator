#ifndef DEQUE_H
#define DEQUE_H

#include "List.h"

template<typename T>
class Deque {
private:
	List<T> container;

public:
	Deque<T>() : container() {}

	Deque<T>(std::initializer_list<T> input) : container(std::move(input)) {}

	Deque<T>(const Deque<T> &source) : container(source.container) {}

	Deque<T>(Deque<T> &&source) : container(std::move(source.container)) {}

	Deque<T>(const T &a) : container(a) {}

	Deque<T>(T &&a) : container(std::move(a)) {}

	Deque<T> &operator=(const Deque<T> &source) {
		this->container = source.container;
		return *this;
	}

	Deque<T> &operator=(Deque<T> &&source) {
		this->container = std::move(source.container);
		return *this;
	}

	T &front() const { return this->container.front(); }

	T &back() const { return this->container.back(); }

	T pop_front() { return this->container.pop_front(); }

	T pop_back() { return this->container.pop_back(); }

	Deque<T> &push_front(T value) { this->container.push_front(value); return *this; }

	Deque<T> &push_back(T value) { this->container.push_back(value); return *this; }

	bool empty() { return this->container.empty(); }

	bool size() { return this->container.size(); }

	void clear() { this->container.clear(); return; }
};

#endif //DEQUE_H
