#ifndef CALCULATOR_PARCER_H
#define CALCULATOR_PARCER_H

#include <string>
#include "Queue.h"
#include "Exception.h"
#include "Elements.h"

bool is_func(std::string S);
bool is_bin_func(std::string S);
bool is_const(std::string S);
bool is_sign(std::string S);
bool is_delim_class(std::string S);
bool is_name(std::string S);
bool is_number(std::string S);

std::string::iterator name(std::string::iterator ptr, std::string &S, char delim);
std::string::iterator number(std::string::iterator ptr, std::string &S, char delim);

void binary(std::string::iterator &ptr, std::string &S, Queue<Elem*> &ans);
void unary(std::string::iterator &ptr, std::string &S, Queue<Elem*> &ans);
void expr(std::string::iterator &ptr, std::string &S, Queue<Elem*> &ans, char delim = '\n');

Queue<Elem*> parce(std::string &S);

#include "Parcer.cpp"

#endif //CALCULATOR_PARCER_H
