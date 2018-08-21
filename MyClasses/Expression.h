#ifndef CALCULATOR_EXPRESSION_H
#define CALCULATOR_EXPRESSION_H

#include <functional>
#include <iostream>
#include "Elements.h"
#include "Exception.h"
#include "Parcer.h"
#include "Stack.h"

int priority(Elem *op) {
	if (*op == Binary(Bignum::operator+) || *op == Binary(Bignum::operator-))
		return 6;
	if (*op == Binary(Bignum::operator*) || *op == Binary(Bignum::operator/))
		return 5;
	if (*op == Binary(Math::pow))
		return 4;
	if (*op == Unary(Bignum::operator-))
		return 3;
	return 0;
}

class Expression {
private:
	Queue<Elem *> m;
	List<std::string> vars;

public:
	Expression() {}

	Expression(Queue<Elem *> q) {
		Stack<Elem *> stack;
		while (!q.empty()) {
			auto temp = q.pop();
			if (temp->is_delim()) {
				delete temp;
				while (!stack.empty() && !stack.top()->is_open_brace())
					this->m.push(stack.pop());
			}
			else if (temp->is_variable()) {
				try {
					this->vars.merge_unique(temp->to_string());
					this->m.push(temp);
				} catch (std::bad_cast &e) {}
			} else if (temp->is_number())
				this->m.push(temp);
			else if (temp->is_open_brace()) {
				stack.push(temp);
			} else if (temp->is_close_brace()) {
				delete temp;
				while (!stack.empty() && !(temp = stack.pop())->is_open_brace())
					this->m.push(temp);
				delete temp;
				if (!stack.empty() && stack.top()->is_function())
					this->m.push(stack.pop());
			} else {
				if (*temp == Binary(Bignum::operator-) || *temp == Binary(Math::pow)) {
					while (!stack.empty() && stack.top()->is_function() && priority(stack.top()) < priority(temp))
						this->m.push(stack.pop());
					stack.push(temp);
				} else {
					while (!stack.empty() && stack.top()->is_function() && priority(stack.top()) <= priority(temp))
						this->m.push(stack.pop());
					stack.push(temp);
				}
			}
		}
	}

	void print() {
		auto expr = this->m;
		while (!expr.empty())
			std::cout << expr.pop()->to_string() << " ";
		std::cout << std::endl;
	}

	void calculate() {
		Queue<Elem *> queue(this->m);
		Stack<Elem *> stack;
		while (!queue.empty()) {
			if (queue.front()->is_function()) {
				if (queue.front()->is_unary()) {
					Unary *fun = dynamic_cast<Unary *>(queue.pop());
					Elem *arg = stack.pop();
					try {
						stack.push(new Number((*fun)(*dynamic_cast<Number &>(*arg))));
						delete arg;
						delete fun;
					} catch (std::bad_cast &) {
						stack.push(arg).push(fun);
					}
				} else {
					Binary *fun = dynamic_cast<Binary *>(queue.pop());
					Elem *arg2 = stack.pop();
					Elem *arg1 = stack.pop();
					try {
						stack.push(new Number((*fun)(*dynamic_cast<Number &>(*arg1), *dynamic_cast<Number &>(*arg2))));
						delete arg1;
						delete arg2;
						delete fun;
					} catch (std::bad_cast &) {
						stack.push(arg1).push(arg2).push(fun);
					}
				}
			} else stack.push(queue.pop());
		}
		this->m.clear();
		Stack<Elem *> temp_stack;
		while (!stack.empty()) temp_stack.push(stack.pop());
		while (!temp_stack.empty()) this->m.push(temp_stack.pop());
	}

	void replace_var(std::string name, Bignum val) {
		Queue<Elem*> temp(std::move(this->m));
		while (!temp.empty()) {
			auto t = temp.pop();
			if (t->is_variable() && t->to_string() == name) {
				this->m.push(new Number(val));
				delete t;
			} else
				this->m.push(t);
		}
		auto it = vars.begin();
		for (; it != vars.end() && *it != name; ++it);
		if (it != vars.end())
			vars.erase(it);
	}

	bool has_vars() {
		return !this->vars.empty();
	}
};

#endif //CALCULATOR_EXPRESSION_H
