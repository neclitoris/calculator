#ifndef LIST_H
#define LIST_H

#include <iterator>

template<typename T>
class List {
protected:

	class Node {
	public:
		T *val;
		Node *prev, *next;

		Node(T v, Node *p = nullptr, Node *n = nullptr) {
			this->val = new T(v);
			this->prev = p;
			this->next = n;
		}

		Node(Node *p = nullptr, Node *n = nullptr) {
			this->val = nullptr;
			this->prev = p;
			this->next = n;
		}

		Node &operator=(const Node &rhs) = delete;

		Node &operator=(Node &&rhs) = delete;

		~Node() {
			delete this->val;
		}
	};

public:

	class iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
		friend class List<T>;

	protected:
		Node *pos;

	protected:
		iterator(Node *p) {
			this->pos = p;
		}

		operator Node *() {
			return this->pos;
		}

	public:
		iterator() {
			this->pos = nullptr;
		}

		iterator operator=(Node *b) {
			this->pos = b;
			return *this;
		}

		bool operator==(iterator b) const {
			return this->pos == b.pos;
		}

		bool operator!=(iterator b) const {
			return this->pos != b.pos;
		}

		bool operator==(std::nullptr_t b) const {
			return !this->pos || !this->pos->val;
		}

		bool operator!=(std::nullptr_t b) const {
			return this->pos && this->pos->val;
		}

		T &operator*() const {
			if (!this->pos || !this->pos->val)
				throw std::out_of_range("");
			return *this->pos->val;
		}

		T *operator->() const {
			if (!this->pos || !this->pos->val)
				throw std::out_of_range("");
			return this->pos->val;
		}

		iterator &operator++() {
			if (this->pos->next != nullptr)
				this->pos = this->pos->next;
			return *this;
		}

		iterator operator++(int) {
			iterator old = *this;
			if (this->pos->next != nullptr)
				this->pos = this->pos->next;
			return old;
		}

		iterator &operator--() {
			if (this->pos->prev != nullptr)
				this->pos = this->pos->prev;
			return *this;
		}

		iterator operator--(int) {
			iterator old = *this;
			if (this->pos->prev != nullptr)
				this->pos = this->pos->prev;
			return old;
		}
	};

	class r_iterator : public iterator {
		friend class List<T>;

	protected:
		using iterator::pos;

	protected:
		r_iterator(Node *p) : iterator(p) {}

	public:
		r_iterator() : iterator() {}

		r_iterator(const iterator &it) : iterator(it) {}

		r_iterator(const iterator &&it) : iterator(it) {}

		r_iterator &operator++() {
			if (this->pos->prev != nullptr)
				this->pos = this->pos->prev;
			return *this;
		}

		r_iterator operator++(int) {
			r_iterator old = *this;
			if (this->pos->prev != nullptr)
				this->pos = this->pos->prev;
			return old;
		}

		r_iterator &operator--() {
			if (this->pos->next != nullptr)
				this->pos = this->pos->next;
			return *this;
		}

		r_iterator operator--(int) {
			r_iterator old = *this;
			if (this->pos->next != nullptr)
				this->pos = this->pos->next;
			return old;
		}
	};

protected:
	List<T>::iterator _begin, _end;
	size_t _size;

	List<T>(iterator &a, iterator &b, int size_tip = 0) {
		a.pos->prev = this->_begin = new Node(nullptr, a);
		b.pos->next = this->_end = new Node(b);
		if (this->_size = size_tip)
			return;
		while (a.pos != b.pos) {
			++this->_size;
			++a;
		}
		++this->_size;
	}

public:
	List<T>() {
		this->_size = 0;
		this->_begin = new Node(), this->_end = new Node();
		iter_conn(this->_begin, this->_end);
	}

	List<T>(unsigned int n, T m) : List<T>() {
		this->_size = n;
		while (n--)
			this->_end.pos->prev =
			this->_end.pos->prev->next =
				new Node(m, this->_end.pos->prev, this->_end);
	}

	List<T>(std::initializer_list<T> input) : List<T>() {
		this->_size = input.size();
		iterator p = this->_begin;
		for (auto it: input)
			p = p.pos->next = new Node(it, p, _end);
		this->_end.pos->prev = p;
	}

	List<T>(const List<T> &source) : List<T>() {
		if (source._size == 0)
			return;
		this->_size = source._size;
		iterator p = this->_begin, q = source._begin;
		while (q.pos->next != source._end.pos) {
			p = p.pos->next = new Node(*q.pos->next->val, p, _end);
			q++;
		}
		this->_end.pos->prev = p;
	}

	List<T>(List<T> &&source) : List<T>() {
		this->swap(source);
	}

	List<T>(const T &a) : List<T>() {
		this->_end.pos->prev = this->_begin.pos->next = new Node(a, _begin, _end);
		++this->_size;
	}

	List<T>(T &&a) : List<T>() {
		this->_end.pos->prev = this->_begin.pos->next = new Node(std::move(a), _begin, _end);
		++this->_size;
	}

	~List() {
		iterator t = this->_begin;
		while (t != this->_end)
			delete t++;
		delete this->_end;
	}

	List<T> &operator=(const List<T> &source) {
		List<T> tmp(source);
		this->swap(tmp);
		return *this;
	}

	List<T> &operator=(List<T> &&source) {
		this->swap(source);
		return *this;
	}

	iterator begin() const { return this->_begin.pos->next; }

	iterator end() const { return this->_end; }

	r_iterator rbegin() const { return this->_end.pos->prev; }

	r_iterator rend() const { return this->_begin; }

	long long size() const { return this->_size; }

	bool empty() const { return !this->_size; }

	T &back() const { return *this->rbegin(); }

	T &front() const { return *this->begin(); }

	List<T> &insert(List<T> range, iterator place = nullptr) {
		if (place == nullptr)
			place = this->_end;
		this->_size += range._size;
		iterator lb = range._begin.pos->next, rb = range._end.pos->prev;
		place.pos->prev->next = lb;
		lb.pos->prev = place.pos->prev;
		rb.pos->next = place;
		place.pos->prev = rb;
		iter_conn(range._begin, range._end);
		return *this;
	}

	List<T> &push_back(T value) {
		return this->insert(std::move(value));
	}

	List<T> &push_front(T value) {
		return this->insert(std::move(value), this->begin());
	}

	List<T> &erase(iterator &place) {
		--this->_size;
		iterator q = place.pos->next;
		iter_conn(place.pos->prev, q);
		delete place;
		place = q;
		return *this;
	}

	List<T> &erase(iterator &&place = nullptr) {
		if (place == nullptr)
			place = this->rbegin();
		return this->erase(place);
	}

	T pop_front() {
		T ans = *this->begin();
		this->erase(this->begin());
		return ans;
	}

	T pop_back() {
		T ans = *this->rbegin();
		this->erase(this->rbegin());
		return ans;
	}

	List<T> &erase(iterator &__begin, iterator &__end) {
		iter_conn(__begin.pos->prev, __end);
		while (__begin != __end) {
			--this->_size;
			delete __begin++;
		}
		--__begin;
		return *this;
	}

	List<T> &clear() {
		for (iterator it = this->begin(); it != this->end();)
			delete it++;
		iter_conn(_begin, _end);
		this->_size = 0;
		return *this;
	}

	List<T> take(iterator __begin, iterator __end = nullptr, int tip = 0) {
		if (__end.pos == nullptr)
			__end = __begin.pos->next;
		if (__begin == __end)
			return List<T>();
		if (__begin == this->_begin || __begin == this->_end)
			return List<T>();
		iterator p = __begin, q = __end;
		--p;
		--__end;
		iter_conn(p, q);
		List<T> ans(__begin, __end, tip);
		this->_size -= ans._size;
		return ans;
	}

	List<T> operator+(List<T> b) {
		List<T> tmp(*this);
		iter_conn(tmp._end.pos->prev, b._begin.pos->next);
		iter_conn(b._begin, tmp._end);
		std::swap(tmp._end, b._end);
		tmp._size += b.size();
		return std::move(tmp);
	}

	std::pair<List<T>, List<T>> split() {
		iterator p = begin();
		for (int i = 0; i < ((_size + 1 & ~(_size ^ _size + 1)) >> 1); i++, p++);
		iterator q = p++;
		q.pos->next = new Node(q);
		p.pos->prev = new Node(nullptr, p);
		++q, --p;
		iterator b1 = rend(), e1 = q, b2 = p, e2 = end();
		long long s1 = this->_size >> 1, s2 = this->_size - s1;
		List<T> t1(b1, e1, s1), t2(b2, e2, s2);
		*this = List<T>();
		return {t1, t2};
	}

	List<T> &merge(List<T> rhs) {
		iterator l = begin(), r = rhs.begin();
		while (l != nullptr && r != nullptr) {
			if (*l <= *r)
				++l;
			else {
				iter_conn(l.pos->prev, r);
				++r;
				iter_conn(l.pos->prev->next, l);
			}
		}
		iter_conn(rbegin(), r);
		std::swap(this->_end, rhs._end);
		iter_conn(rhs._begin, rhs._end);
		this->_size += rhs.size();
		return *this;
	}

	List<T> &merge_unique(List<T> rhs) {
		this->merge(std::move(rhs));
		for (auto next = this->begin(), it = next++; next != this->end(); ++it, ++next)
			while (next != this->end() && *next == *it)
				erase(next);
		return *this;
	}

	List<T> &append(unsigned int n, T val) {
		return this->insert(List<T>(n, std::move(val)));
	}

	inline void swap(List<T> &b) {
		std::swap(_begin, b._begin);
		std::swap(_end, b._end);
		std::swap(_size, b._size);
	}

protected:
	static void iter_conn(iterator a, iterator b) {
		a.pos->next = b;
		b.pos->prev = a;
	}
};

template<typename T>
inline void swap(List<T> &a, List<T> &b) {
	a.swap(b);
}

#endif //LIST_H