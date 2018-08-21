#ifndef CALCULATOR_MATH_H
#define CALCULATOR_MATH_H

#include "Bignum.h"

namespace Math {
	Bignum pi = "3,1415926535 8979323846 2643383279 5028841971 6939937510",
		e = "2,7182818284 5904523536 0287471352 6624977572 4709369995";

	Bignum sin(Bignum x);

	Bignum cos(Bignum x);

	Bignum exp(Bignum x);

	Bignum ln(Bignum x);

	Bignum log(Bignum a, Bignum x);

	Bignum pow(Bignum a, Bignum n);

	Bignum root(Bignum a, Bignum n);

	Bignum sqrt(Bignum a);
};

#include "Math.cpp"

#endif //CALCULATOR_MATH_H
