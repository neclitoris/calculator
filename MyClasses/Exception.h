#ifndef CALCULATOR_EXCEPTION_H
#define CALCULATOR_EXCEPTION_H

#include <string>

class Exception {
private:
	std::string _nani;

public:
	Exception(std::string s) : _nani(s) {}

	std::string nani() {
		return this->_nani;
	}
};

class Not_A_Name : public Exception {
public:
	Not_A_Name() : Exception("") {}

	Not_A_Name(std::string q) : Exception(std::move(q)) {}
};

class Not_A_Number : public Exception {
public:
	Not_A_Number() : Exception("") {}

	Not_A_Number(std::string q) : Exception(std::move(q)) {}
};

class Calculation_Error : public Exception {
public:
	Calculation_Error(std::string q) : Exception(std::move(q)) {}
};

#endif //CALCULATOR_EXCEPTION_H
