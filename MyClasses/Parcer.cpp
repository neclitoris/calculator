#include "Parcer.h"

bool is_func(std::string S) {
	return S == "sin" || S == "cos" || S == "exp" || S == "abs" || S == "ln" || S == "sqrt";
}

bool is_bin_func(std::string S) {
	return S == "log" || S == "root";
}

bool is_const(std::string S) {
	return S == "e" || S == "pi";
}

bool is_sign(std::string S) {
	return S == "*" || S == "^" || S == "/" || S == "+" || S == "-";
}

bool is_delim_class(std::string S) {
	return S == ")" || S == "," || S == "\n";
}

bool is_name(std::string S) {
	auto ptr = S.begin();
	while (*ptr == ' ') ++ptr;
	if (isalpha(*ptr) || *ptr == '_') {
		++ptr;
		while (ptr != S.end() && (isalpha(*ptr) || isdigit(*ptr) || *ptr == '_'))
			++ptr;
		if (ptr != S.end() && *ptr != ' ')
			return 0;
	} else return 0;
	return 1;
}

bool is_number(std::string S) {
	auto ptr = S.begin();
	while (*ptr == ' ') ++ptr;
	if (*ptr == '-' || *ptr == '+')
		ptr++;
	if (ptr == S.end())
		return 0;
	bool has_pt = 0;
	int after_pt = 0;
	if (!isdigit(*ptr))
		return 0;
	while (isdigit(*ptr) || *ptr == '.') {
		if (*ptr == '.') {
			if (has_pt)
				return 0;
			has_pt = 1;
		} else if (has_pt)
			after_pt++;
		++ptr;
	}
	if (has_pt != after_pt)
		return 0;
	if (ptr == S.end())
		return 1;
	bool exp = 0;
	if (*ptr == 'e' || *ptr == 'E') {
		++ptr;
		if (ptr == S.end())
			return 0;
		if (*ptr == '-' || *ptr == '+')
			++ptr;
		if (ptr == S.end())
			return 0;
		while (ptr != S.end() && isdigit(*ptr)) {
			++ptr;
			exp = 1;
		}
	} else exp = 1;
	if (exp == 0)
		return 0;
	return 1;
}

std::string::iterator name(std::string::iterator ptr, std::string &S, char delim) {
	if (*ptr != delim && (isalpha(*ptr) || *ptr == '_')) {
		++ptr;
		while (*ptr != delim && (isalpha(*ptr) || isdigit(*ptr) || *ptr == '_'))
			++ptr;
	} else throw Not_A_Name();
	if (*ptr != delim && *ptr != '(' && !is_sign(std::string(1, *ptr)) && *ptr != ' ') {
		std::string what = S;
		for (int i = 0; i < ptr - S.begin(); i++)
			what.push_back(' ');
		what.push_back('^');
		if (is_delim_class(std::string(1, *ptr))) {
			what.append("\nThis place is for \'");
			if (delim == '\n')
				what.append("end of line");
			else
				what.push_back(delim);
			what.append("\' only.\n");
		} else what.append("\nDo you even know what the \'name\' is?\n");
		throw Exception(what);
	}
	return ptr;
}

std::string::iterator number(std::string::iterator ptr, std::string &S, char delim) {
	if (!isdigit(*ptr))
		throw Not_A_Number();
	while (isdigit(*ptr) || *ptr == '.')
		ptr++;
	if (*ptr != delim) {
		bool exp = 0;
		if (*ptr == 'e' || *ptr == 'E') {
			++ptr;
			if (*ptr == '-' || *ptr == '+')
				++ptr;
			while (isdigit(*ptr)) {
				++ptr;
				exp = 1;
			}
		} else exp = 1;
		if (exp == 0) {
			std::string what = S;
			for (int i = 0; i < ptr - S.begin(); i++)
				what.push_back(' ');
			what.push_back('^');
			what.append("\nIs exponential form too difficult to enter?\n");
			throw Exception(what);
		}
		if (*ptr != delim && !is_sign(std::string(1, *ptr)) && *ptr != ' ') {
			std::string what = S;
			for (int i = 0; i < ptr - S.begin(); i++)
				what.push_back(' ');
			what.push_back('^');
			if (is_delim_class(std::string(1, *ptr))) {
				what.append("\nI expected to see \'");
				if (delim == '\n')
					what.append("end of line");
				else
					what.push_back(delim);
				what.append("\' here.\n");
			} else what.append("\nDo you even know what the \'number\' is?\n");
			throw Exception(what);
		}
	}
	return ptr;
}

void binary(std::string::iterator &ptr, std::string &S, Queue<Elem *> &ans) {
	++ptr;
	ans.push(new Open_brace());
	expr(ptr, S, ans, ',');
	++ptr;
	ans.push(new Delim());
	expr(ptr, S, ans, ')');
	ans.push(new Close_brace());
	++ptr;
}

void unary(std::string::iterator &ptr, std::string &S, Queue<Elem *> &ans) {
	++ptr;
	ans.push(new Open_brace());
	expr(ptr, S, ans, ')');
	ans.push(new Close_brace());
	++ptr;
}

void expr(std::string::iterator &ptr, std::string &S, Queue<Elem *> &ans, char delim) {
	std::string::iterator q;
	while (*ptr == ' ') ++ptr;
	if (*ptr == '-') {
		++ptr;
		ans.push(new Unary(Bignum::operator-));
	} else if (*ptr == '+') ++ptr;
	while (*ptr == ' ') ++ptr;
	try {
		q = name(ptr, S, delim);
		std::string t = S.substr(ptr - S.begin(), q - ptr);
		ptr = q;
		if (is_const(t)) {
			if (t == "pi")
				ans.push(new Number(Math::pi));
			else
				ans.push(new Number(Math::e));
		} else if (is_bin_func(t)) {
			if (t == "log")
				ans.push(new Binary(Math::log));
			if (t == "root")
				ans.push(new Binary(Math::root));
			binary(ptr, S, ans);
		} else if (is_func(t)) {
			if (t == "sin")
				ans.push(new Unary(Math::sin));
			if (t == "cos")
				ans.push(new Unary(Math::cos));
			if (t == "abs")
				ans.push(new Unary(Bignum::abs));
			if (t == "exp")
				ans.push(new Unary(Math::exp));
			if (t == "ln")
				ans.push(new Unary(Math::ln));
			if (t == "sqrt")
				ans.push(new Unary(Math::sqrt));
			unary(ptr, S, ans);
		} else {
			ans.push(new Variable(t));
		}
	} catch (Not_A_Name &not_a_name) {
		try {
			q = number(ptr, S, delim);
			std::string t = S.substr(ptr - S.begin(), q - ptr);
			ans.push(new Number(Bignum(t)));
			ptr = q;
		} catch (Not_A_Number &not_a_number) {
			if (*ptr == '(') {
				++ptr;
				ans.push(new Open_brace());
				expr(ptr, S, ans, ')');
				++ptr;
				ans.push(new Close_brace());
			} else {
				std::string what = S;
				for (int i = 0; i < ptr - S.begin(); i++)
					what.push_back(' ');
				what.push_back('^');
				what.append("\nI refuse to read it.\n");
				throw Exception(what);
			}
		}
	}
	while (*ptr == ' ')
		++ptr;
	while (*ptr != delim) {
		if (is_sign(std::string(1, *ptr))) {
			if (*ptr == '+')
				ans.push(new Binary(Bignum::operator+));
			if (*ptr == '-')
				ans.push(new Binary(Bignum::operator-));
			if (*ptr == '*')
				ans.push(new Binary(Bignum::operator*));
			if (*ptr == '/')
				ans.push(new Binary(Bignum::operator/));
			if (*ptr == '^')
				ans.push(new Binary(Math::pow));
			while (*++ptr == ' ');
		} else {
			std::string what = S;
			for (int i = 0; i < ptr - S.begin(); i++)
				what.push_back(' ');
			what.push_back('^');
			what.append("\nNo-no-no. Either sign, or ");
			if (delim == '\n')
				what.append("end of line");
			else
				what.push_back(delim);
			what.append(" here.\n");
			throw Exception(what);
		}
		expr(ptr, S, ans, delim);
	}
}

Queue<Elem *> parce(std::string &S) {
	S.push_back('\n');
	Queue<Elem *> ans;
	auto ptr = S.begin();
	ans.push(new Open_brace());
	expr(ptr, S, ans);
	ans.push(new Close_brace());
	S.pop_back();
	return ans;
}