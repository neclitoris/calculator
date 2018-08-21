#ifndef CALCULATOR_ELEMENTS_H
#define CALCULATOR_ELEMENTS_H

#include "Math.h"

class Elem {
protected:
	std::string name;

public:
	virtual bool is_number() const { return 0; }

	virtual bool is_variable() const { return 0; }

	virtual bool is_function() const { return 0; }

	virtual bool is_binary() const { return 0; }

	virtual bool is_unary() const { return 0; }

	virtual bool is_open_brace() const { return 0; }

	virtual bool is_close_brace() const { return 0; }

	virtual bool is_delim() const { return 0; }

	virtual std::string to_string() { return this->name; }

	virtual bool operator==(Elem &&rhs) {
		return
		this->is_number() == rhs.is_number() &&
		this->is_function() == rhs.is_function() &&
		this->is_unary() == rhs.is_unary() &&
		this->is_binary() == rhs.is_binary() &&
		this->is_open_brace() == rhs.is_open_brace() &&
		this->is_close_brace() == rhs.is_close_brace() &&
		this->is_variable() == rhs.is_variable() &&
		this->is_delim() == rhs.is_delim() && this->name == rhs.name;
	}

	virtual bool operator==(const Elem &rhs) {
		return
			this->is_number() == rhs.is_number() &&
			this->is_function() == rhs.is_function() &&
			this->is_unary() == rhs.is_unary() &&
			this->is_binary() == rhs.is_binary() &&
			this->is_open_brace() == rhs.is_open_brace() &&
			this->is_close_brace() == rhs.is_close_brace() &&
			this->is_variable() == rhs.is_variable() &&
			this->is_delim() == rhs.is_delim() && this->name == rhs.name;
	}

	virtual bool operator<(Elem rhs) {
		return this->name < rhs.name;
	}

	virtual bool operator>(Elem rhs) {
		return rhs.name < this->name;
	}

	virtual bool operator>=(Elem rhs) {
		return !(this->name < rhs.name);
	}

	virtual bool operator<=(Elem rhs) {
		return !(rhs.name < this->name);
	}
};

class Number : public Elem {
private:
	Bignum number;

public:
	Number(Bignum t) : number(std::move(t)) {
		this->name = this->number.to_string();
	}

	bool is_number() const { return 1; }

	Bignum operator*() { return this->number; }
};

class Variable : public Elem {
public:
	Variable(std::string t) {
		this->name = std::move(t);
	}

	bool is_variable() const { return 1; }

	std::string operator*() { return this->name; }
};

class Function : public Elem {
public:
	bool is_function() const { return 1; }
};

class Binary : public Function {
private:
	struct Call {
		Bignum (*fun)(Bignum, Bignum);

		Bignum (Bignum::*mem_fun)(Bignum) const;

		Call() : fun(nullptr), mem_fun(nullptr) {}

		Call(Bignum (*f)(Bignum, Bignum)) : Call() {
			fun = f;
		}

		Call(Bignum (Bignum::*f)(Bignum) const) : Call() {
			mem_fun = f;
		}

		Bignum operator()(Bignum a, Bignum b) const {
			if (fun)
				return fun(a, b);
			return (a.*mem_fun)(b);
		}
	} call;

public:
	Binary(Bignum (*f)(Bignum, Bignum)) : call(f) {
		if (f == Math::pow)
			name = "^";
		if (f == Math::root)
			name = "root";
		if (f == Math::log)
			name = "log";
	}

	Binary(Bignum (Bignum::*f)(Bignum) const) : call(f) {
		if (f == Bignum::operator+)
			name = "+";
		if (f == (Bignum (Bignum::*)(Bignum) const) Bignum::operator-)
			name = "-";
		if (f == (Bignum (Bignum::*)(Bignum) const) Bignum::operator*)
			name = "*";
		if (f == Bignum::operator/)
			name = "/";
	}

	bool is_binary() const { return 1; }

	Call operator*() const { return this->call; }

	Bignum operator()(Bignum arg1, Bignum arg2) { return this->call(arg1, arg2); }

	bool operator==(Binary rhs) const {
		return this->call.fun == rhs.call.fun || this->call.mem_fun == rhs.call.mem_fun;
	}
};

class Unary : public Function {
private:
	struct Call {
		Bignum (*fun)(Bignum);

		Bignum (Bignum::*mem_fun)() const;

		Call() : fun(nullptr), mem_fun(nullptr) {}

		Call(Bignum (*f)(Bignum)) : Call() {
			fun = f;
		}

		Call(Bignum (Bignum::*f)() const) : Call() {
			mem_fun = f;
		}

		Bignum operator()(Bignum a) const {
			if (fun)
				return fun(a);
			return (a.*mem_fun)();
		}
	} call;

public:
	Unary(Bignum (*f)(Bignum)) : call(f) {
		if (f == Math::sin)
			name = "sin";
		if (f == Math::cos)
			name = "cos";
		if (f == Math::exp)
			name = "exp";
		if (f == Math::ln)
			name = "ln";
		if (f == Math::sqrt)
			name = "sqrt";
	}

	Unary(Bignum (Bignum::*f)() const) : call(f) {
		if (f == (Bignum (Bignum::*)() const) Bignum::operator-)
			name = "-";
	}

	bool is_unary() const { return 1; }

	Call operator*() const { return this->call; }

	Bignum operator()(Bignum arg) { return this->call(arg); }

	bool operator==(Unary rhs) const {
		return this->call.fun == rhs.call.fun || this->call.mem_fun == rhs.call.mem_fun;
	}
};

class Open_brace : public Elem {
public:
	Open_brace() {
		this->name = "(";
	}

	bool is_open_brace() const { return 1; }
};

class Close_brace : public Elem {
public:
	Close_brace() {
		this->name = ")";
	}

	bool is_close_brace() const { return 1; }
};

class Delim : public Elem {
public:
	Delim() {
		this->name = ",";
	}

	virtual bool is_delim() const { return 1; }
};

#endif //CALCULATOR_ELEMENTS_H
