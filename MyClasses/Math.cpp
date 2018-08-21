#include "Math.h"

namespace Math {
	Bignum sin(Bignum x) {
		Bignum ans = x;
		Bignum mult = x;
		x *= x;
		for (Bignum i = 3; mult.abs() > "1e-20"; i += 2) {
			mult *= -x / (i * (i - 1));
			ans += mult;
		}
		if (abs(ans.round() - ans) < "1e-9")
			ans = ans.round();
		return ans;
	}

	Bignum cos(Bignum x) {
		Bignum ans = 1;
		Bignum mult = 1;
		x *= x;
		for (Bignum i = 2; mult.abs() > "1e-20"; i += 2) {
			mult *= -x / (i * (i - 1));
			ans += mult;
		}
		if (abs(ans.round() - ans) < "1e-9")
			ans = ans.round();
		return ans;
	}

	Bignum exp(Bignum x) {
		Bignum ans = 1;
		Bignum mult = 1;
		for (Bignum i = 1; mult.abs() > "1e-10"; i += 1) {
			mult *= x / i;
			ans += mult;
		}
		if (abs(ans.round() - ans) < "1e-9")
			ans = ans.round();
		return ans;
	}

	Bignum ln(Bignum x) {
		if (x == 10)
			return "2.3025850929 9404568401 7991454684 3642076011 0148862877";
		if (x <= 0)
			throw Calculation_Error("Invalid argument for logarithm: positive required.");
		auto norm = x.normal_form();
		Bignum z = (Bignum(1) - norm.first) / (Bignum(1) + norm.first);
		Bignum mult = z;
		Bignum ans = z;
		z *= z;
		for (Bignum i = 3; mult.abs() > "1e-20"; i += 2) {
			mult *= (i - 2) * z / i;
			ans += mult;
		}
		ans = ans * -2 + ln(10) * norm.second;
		if (abs(ans.round() - ans) < "1e-9")
			ans = ans.round();
		return ans;
	}

	Bignum log(Bignum a, Bignum x) {
		Bignum ans = ln(x) / ln(a);
		if (abs(ans.round() - ans) < "1e-9")
			ans = ans.round();
		return ans;
	}

	Bignum pow(Bignum a, Bignum n) {
		Bignum ans = exp(ln(a) * n);
		if (abs(ans.round() - ans) < "1e-9")
			ans = ans.round();
		return ans;
	}

	Bignum root(Bignum a, Bignum n) {
		if (!n.is_integer() || n.even() && a <= 0)
			throw Calculation_Error("Invalid argument: positive required for even or rational root.");
		Bignum ans = pow(a, Bignum(1) / n);
		if (abs(ans.round() - ans) < "1e-9")
			ans = ans.round();
		return ans;
	}

	Bignum sqrt(Bignum a) {
		Bignum ans = root(a, 2);
		if (abs(ans.round() - ans) < "1e-9")
			ans = ans.round();
		return ans;
	}
};