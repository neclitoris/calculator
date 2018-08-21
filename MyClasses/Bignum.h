#ifndef BIGNUM_H
#define BIGNUM_H

#include "List.h"
#include "Exception.h"
#include <limits>
#include <cmath>

class Bignum {
public:
	friend std::istream &operator>>(std::istream &is, Bignum &input) {
		char tmp;
		while ((tmp = is.peek()) == ' ' || tmp == '\n' || tmp == '\r') is.get();
		if (tmp == '-') {
			input._sign = -1;
			is.get();
			if (!isdigit(is.peek())) {
				is.unget();
				input._sign = 0;
				input.normalize();
				return is;
			}
		} else if (isdigit(tmp)) input._sign = 1;
		else throw Not_A_Number();
		bool has_pt = 0;
		while (tmp = is.peek(), isdigit(tmp) || tmp == ',' || tmp == '.') {
			if (isdigit(tmp)) {
				input.M.push_back(tmp - '0');
				if (!has_pt) ++input.E;
			} else has_pt = 1;
			is.get();
		}
		if (is.peek() == 'E' || is.peek() == 'e') {
			is.get();
			bool minus = 0;
			if (is.peek() == '+' || is.peek() == '-')
				minus = is.get() == '-';
			if (!isdigit(is.peek()))
				throw Not_A_Number();
			long long E;
			is >> E;
			if (input.E + E * (minus ? -1 : 1) <= std::numeric_limits<short>::min())
				input.E = std::numeric_limits<short>::min();
			else if (input.E + E * (minus ? -1 : 1) >= std::numeric_limits<short>::max())
				input.E = std::numeric_limits<short>::max();
			else input.E += E * (minus ? -1 : 1);
		} else if (is.peek() != ' ' && is.peek() != '\n')
			throw Not_A_Number();
		input.normalize();
		return is;
	}

	friend std::ostream &operator<<(std::ostream &os, Bignum output) {
		output.normalize();
		os << output.to_string();
	}

private:
	List<int> M;
	short _sign;
	short E;

public:
	static Bignum infinity() {
		return std::string("1e32767");
	}

	static Bignum lowest() {
		return std::string("1e-32768");
	}

private:
	void normalize() &{
		while (!this->M.empty() && this->M.back() == 0 || this->M.size() > 50)
			this->M.pop_back();
		while (!this->M.empty() && this->M.front() == 0) {
			this->M.pop_front();
			this->E--;
		}
		if (this->M.empty()) {
			this->M.push_front(0);
			this->_sign = 1;
			this->E = std::numeric_limits<short>::min();
		}
	}

	void equalize(Bignum &rhs) &{
		if (*this == 0) {
			this->E = rhs.E;
			return;
		}
		if (rhs == 0) {
			rhs.E = this->E;
			return;
		}
		if (this->E > rhs.E) {
			rhs.M.insert(List<int>(this->E - rhs.E, 0), rhs.M.begin());
			rhs.E = this->E;
		}
		if (this->E < rhs.E) {
			this->M.insert(List<int>(rhs.E - this->E, 0), this->M.begin());
			this->E = rhs.E;
		}
	}

public:
	Bignum() : M(), E(0), _sign(1) {}

	Bignum(long long a) : Bignum() {
		if (a == 0) {
			this->M.push_back(0);
			this->E = std::numeric_limits<short>::min();
			return;
		}
		this->_sign = (a > 0) - (a < 0);
		a *= this->_sign;
		while (a != 0) {
			this->M.push_front(a % 10);
			++this->E;
			a /= 10;
		}
	}

	Bignum(int a) : Bignum((long long) a) {}

	Bignum(double a) : Bignum((long long) (a * 1e15)) {
		*this >>= 15;
	}

	Bignum(const std::string s) : Bignum() {
		auto ptr = s.begin();
		bool has_pt = 0;
		while (*ptr != 'e' && *ptr != 'E' && ptr != s.end()) {
			if (ptr == s.begin() && *ptr == '-') {
				this->_sign = -1;
				ptr++;
				continue;
			} else if (isdigit(*ptr))
				this->M.push_back(*ptr - '0');
			else if (*ptr == '.' || *ptr == ',')
				has_pt = 1;
			else if (*ptr != ' ')
				throw Not_A_Number();
			if (!has_pt) this->E++;
			ptr++;
		}
		if (ptr != s.end()) {
			if (*ptr != 'e' && *ptr != 'E') {
				throw Not_A_Number();
			}
			int E = 0;
			++ptr;
			bool minus = 0;
			if (*ptr == '-') minus = 1;
			else ptr--;
			while (*(++ptr)) {
				E *= 10;
				E += *ptr - '0';
			}
			if (this->E + E * (minus ? -1 : 1) <= std::numeric_limits<short>::min())
				this->E = std::numeric_limits<short>::min();
			else if (this->E + E * (minus ? -1 : 1) >= std::numeric_limits<short>::max())
				this->E = std::numeric_limits<short>::max();
			else this->E += E * (minus ? -1 : 1);
		}
		this->normalize();
	}

	Bignum(const char *s) : Bignum() {
		const char *ptr = s;
		bool has_pt = 0;
		while (*ptr != 'e' && *ptr != 'E' && *ptr) {
			if (ptr == s && *ptr == '-') {
				this->_sign = -1;
				ptr++;
				continue;
			} else if (isdigit(*ptr))
				this->M.push_back(*ptr - '0');
			else if (*ptr == '.' || *ptr == ',')
				has_pt = 1;
			else if (*ptr != ' ')
				throw Not_A_Number();
			if (!has_pt) this->E++;
			ptr++;
		}
		if (*ptr) {
			if (*ptr != 'e' && *ptr != 'E') {
				throw Not_A_Number();
			}
			int E = 0;
			++ptr;
			bool minus = 0;
			if (*ptr == '-') minus = 1;
			else --ptr;
			while (*(++ptr)) {
				E *= 10;
				E += *ptr - '0';
			}
			if (this->E + E * (minus ? -1 : 1) <= std::numeric_limits<short>::min())
				this->E = std::numeric_limits<short>::min();
			else if (this->E + E * (minus ? -1 : 1) >= std::numeric_limits<short>::max())
				this->E = std::numeric_limits<short>::max();
			else this->E += E * (minus ? -1 : 1);
		}
		this->normalize();
	}

	bool operator==(const Bignum &rhs) const {
		if (rhs.M.size() == 1 && this->M.size() == 1 && this->M.front() == 0 && rhs.M.front() == 0)
			return 1;
		if (this->_sign != rhs._sign || this->E != rhs.E || this->M.size() != rhs.M.size())
			return 0;
		for (auto l = this->M.begin(), r = rhs.M.begin(); l != this->M.end(); ++l, ++r)
			if (*l != *r)
				return 0;
		return 1;
	}

	bool operator!=(const Bignum &rhs) const {
		return !(*this == rhs);
	}

	bool operator==(Bignum &&rhs) const {
		if (rhs.M.size() == 1 && this->M.size() == 1 && this->M.front() == 0 && rhs.M.front() == 0)
			return 1;
		if (this->_sign != rhs._sign || this->E != rhs.E || this->M.size() != rhs.M.size())
			return 0;
		for (auto l = this->M.begin(), r = rhs.M.begin(); l != this->M.end(); ++l, ++r)
			if (*l != *r)
				return 0;
		return 1;
	}

	bool operator!=(Bignum &&rhs) const {
		return !(*this == rhs);
	}

	bool operator<(const Bignum &rhs) const {
		if (this->_sign < rhs._sign)
			return 1;
		if (this->_sign > rhs._sign)
			return 0;
		if (this->E < rhs.E)
			return 1;
		if (this->E > rhs.E)
			return 0;
		for (auto l = this->M.begin(), r = rhs.M.begin(); l != this->M.end() || r != rhs.M.end(); ++l, ++r) {
			if ((l == this->M.end() ? 0 : *l) * this->_sign < (r == rhs.M.end() ? 0 : *r) * this->_sign)
				return 1;
			if ((l == this->M.end() ? 0 : *l) * this->_sign > (r == rhs.M.end() ? 0 : *r) * this->_sign)
				return 0;
		}
		return 0;
	}

	bool operator<(Bignum &&rhs) const {
		return *this < rhs;
	}

	bool operator>(const Bignum &rhs) const {
		return rhs < *this;
	}

	bool operator>(Bignum &&rhs) const {
		return rhs < *this;
	}

	bool operator>=(const Bignum &rhs) const {
		return !(*this < rhs);
	}

	bool operator>=(Bignum &&rhs) const {
		return !(*this < rhs);
	}

	bool operator<=(const Bignum &rhs) const {
		return !(rhs < *this);
	}

	bool operator<=(Bignum &&rhs) const {
		return !(rhs < *this);
	}

	Bignum operator<<(short n) const &{
		Bignum ans(*this);
		ans.E += n;
		ans.normalize();
		return ans;
	}

	Bignum operator>>(short n) const &{
		Bignum ans(*this);
		ans.E -= n;
		ans.normalize();
		return ans;
	}

	Bignum operator<<(short n) &&{
		this->E += n;
		this->normalize();
		return *this;
	}

	Bignum operator>>(short n) &&{
		this->E -= n;
		this->normalize();
		return *this;
	}

	Bignum &operator<<=(short n) &{
		this->E += n;
		this->normalize();
		return *this;
	}

	Bignum &operator>>=(short n) &{
		this->E -= n;
		this->normalize();
		return *this;
	}

	Bignum operator-() const {
		Bignum ans(*this);
		ans._sign *= -1;
		return ans;
	}

	Bignum operator+(Bignum rhs) const {
		Bignum lhs(*this);
		lhs.equalize(rhs);
		int t = 0;
		if (lhs.M.size() < rhs.M.size())
			lhs.swap(rhs);
		auto l = lhs.M.rbegin(), r = rhs.M.rbegin();
		std::advance(l, std::max(lhs.M.size() - rhs.M.size(), 0ll));
		for (; l != lhs.M.rend(); l++, r++) {
			*l += *r * (lhs._sign * rhs._sign) + t;
			if (l != lhs.M.begin()) {
				if (*l >= 10) {
					*l -= 10;
					t = 1;
				} else if (*l < 0) {
					*l += 10;
					t = -1;
				} else t = 0;
			}
		}
		if (lhs.M.front() < 0) {
			lhs.M.front() += 10;
			for (auto it = lhs.M.begin(); it != lhs.M.end(); it++)
				*it = 9 - *it;
			lhs.M.back()++;
			int t = 0;
			for (auto it = lhs.M.rbegin(); it != lhs.M.rend(); it++) {
				*it += t;
				if (*it >= 10) {
					*it -= 10;
					t = 1;
				} else if (*it < 0) {
					*it += 10;
					t = -1;
				} else t = 0;
			}
			lhs = -lhs;
		} else if (lhs.M.front() >= 10) {
			lhs.M.front() -= 10;
			lhs.M.push_front(1);
			lhs.E++;
		}
		lhs.normalize();
		return lhs;
	}

	Bignum operator-(Bignum rhs) const {
		return *this + -std::move(rhs);
	}

	Bignum &operator+=(Bignum rhs) {
		return *this = *this + rhs;
	}

	Bignum &operator-=(Bignum rhs) {
		return *this = *this - rhs;
	}

	Bignum operator*(int rhs) const {
		Bignum ans(*this);
		if (rhs < 0) {
			ans._sign *= -1;
			rhs *= -1;
		}
		int prop = 0;
		for (auto it = ans.M.rbegin(); it != ans.M.rend(); it++) {
			auto val = std::div(*it * rhs + prop, 10);
			*it = val.rem;
			prop = val.quot;
		}
		while (prop) {
			ans.M.push_front(prop % 10);
			ans.E++;
			prop /= 10;
		}
		ans.normalize();
		return ans;
	}

	Bignum &operator*=(int rhs) {
		return *this = *this * rhs;
	}

	Bignum operator*(Bignum rhs) const {
		/// Умножение Карацубы закомментировано, т.к. на заданном диапазоне работает медленнее.
//		Bignum lhs(*this);
//		lhs.normalize();
//		rhs.normalize();
//		if (lhs.M.size() == 1 && rhs.M.size() == 1) {
//			int a = lhs.M.front() * rhs.M.front();
//			lhs.M.front() = a / 10;
//			lhs.M.push_back(a % 10);
//			lhs.E += rhs.E;
//			lhs._sign *= rhs._sign;
//			lhs.normalize();
//			return lhs;
//		}
//		auto l = lhs.M.begin(), r = rhs.M.begin();
//		long long n = std::max(lhs.M.size(), rhs.M.size());
//		if (n & 1) n++;
//		std::advance(l, n / 2), std::advance(r, n / 2);
//		Bignum lhs_r, rhs_r;
//		lhs_r.E = lhs.E;
//		rhs_r.E = rhs.E;
//		lhs_r.M = lhs.M.take(l, lhs.M.end());
//		rhs_r.M = rhs.M.take(r, rhs.M.end());
//		lhs_r.normalize();
//		rhs_r.normalize();
//		Bignum tmp1 = lhs * rhs;
//		Bignum tmp2 = lhs_r * rhs_r;
//		Bignum tmp3 = (lhs + lhs_r) * (rhs + rhs_r);
//		Bignum ans = tmp1 + (tmp3 - tmp1 - tmp2 >> n / 2) + (tmp2 >> n);
		Bignum ans(0);
		rhs.normalize();
		int pow = rhs.E - 1;
		for (auto it = rhs.M.begin(); it != rhs.M.end(); ++it)
			ans += *this * (*it * rhs._sign) << pow--;
		return ans;
	}

	Bignum &operator*=(Bignum rhs) {
		return *this = *this * std::move(rhs);
	}

	Bignum operator/(Bignum rhs) const {
		static int count = 0;
		if (rhs == 0)
			throw Calculation_Error("Division by zero.");
		if (count == 60) {
			count = 0;
			return 0;
		}
		++count;
		Bignum lhs(*this);
		lhs.normalize();
		rhs.normalize();
		int move = rhs.E - lhs.E;
		rhs >>= move;
		if (rhs.abs() > lhs.abs()) {
			rhs >>= 1;
			move++;
		}
		int rb = 10, lb = -9;
		while (rb - lb != 1) {
			int m = (rb + lb) >> 1;
			if (rhs * m > lhs)
				rb = m;
			else
				lb = m;
		}
		return Bignum((long long) lb) + (lhs - rhs * lb) / rhs >> move;
	}

	Bignum &operator/=(Bignum rhs) {
		return *this = *this / rhs;
	}

	std::string to_string() const {
		std::string os;
		if (this->_sign == -1) os.push_back('-');
		if (*this == 0ll) {
			os.push_back('0');
			return os;
		}
		if (std::abs(this->E) <= 15) {
			short E = this->E;
			if (E <= 0) {
				os.push_back('0');
				os.push_back('.');
				while (E < 0) {
					os.push_back('0');
					E++;
				}
				--E;
			}
			for (auto it = this->M.begin(); it != this->M.end(); ++it, --E) {
				if (E < -15)
					break;
				if (E == 0) {
					if (it == this->M.begin())
						os.push_back('0');
					os.push_back('.');
				}
				os.push_back(*it + '0');
			}
			while (E > 0) {
				os.push_back('0');
				--E;
			}
		} else {
			short E = 15;
			os.push_back(this->M.front() + '0');
			for (auto it = ++this->M.begin(); it != this->M.end() && --E > 0; ++it) {
				if (it == ++this->M.begin())
					os.push_back('.');
				os.push_back(*it + '0');
			}
			if (this->E - 1 != 0) {
				os.push_back('e');
				os.push_back(this->E - 1 > 0 ? '+' : '-');
				os.append(std::to_string(std::abs(this->E - 1)));
			}
		}
		return os;
	}

	inline void swap(Bignum &b) &{
		this->M.swap(b.M);
		std::swap(this->_sign, b._sign);
		std::swap(this->E, b.E);
	}

	inline Bignum abs() const {
		Bignum ans(*this);
		ans._sign = 1;
		return ans;
	}

	inline short sign() const {
		return this->_sign;
	}

	inline Bignum trunc() const &{
		return Bignum(*this).trunc();
	}

	inline Bignum trunc() &&{
		auto it = this->M.begin();
		for (int i = 0; i < this->E; i++)
			++it;
		this->M.take(it, this->M.end());
		return *this;
	}

	inline Bignum rational() const &{
		return Bignum(*this).rational();
	}

	inline Bignum rational() &&{
		auto it = this->M.begin();
		while (this->E > 0) {
			++it;
			--this->E;
		}
		this->M.take(this->M.begin(), it);
		return *this;
	}

	inline std::pair<Bignum, Bignum> fraction() const &{
		return Bignum(*this).fraction();
	}

	inline std::pair<Bignum, Bignum> fraction() &&{
		if (*this == 0)
			return std::make_pair(Bignum(0), Bignum(1));
		if (this->E >= this->M.size())
			return std::make_pair(*this, Bignum(1));
		short E = this->M.size() - this->E;
		this->E = this->M.size();
		Bignum a(1);
		a <<= E;
		return std::make_pair(*this, a);
	}

	inline bool is_integer() const {
		return this->M.size() <= this->E;
	}

	inline bool even() const {
		if (!this->is_integer())
			throw std::runtime_error("Asking if rational is even");
		return !((this->E == this->M.size() ? this->M.back() : 0) & 1);
	}

	std::pair<Bignum, short> normal_form() const {
		Bignum m = *this;
		short p = m.E;
		m.E = 1;
		return std::make_pair(m, p - 1);
	}

	Bignum round() {
		Bignum a(this->trunc());
		a.normalize();
		if (this->rational() < 0.5) return a;
		else return a + 1;
	}
};

inline void swap(Bignum &a, Bignum &b) {
	a.swap(b);
}

inline Bignum abs(const Bignum &val) {
	return val.abs();
}

inline Bignum abs(Bignum &&val) {
	return std::move(val).abs();
}

inline short sign(const Bignum &val) {
	return val.sign();
}

inline short sign(Bignum &&val) {
	return std::move(val).sign();
}

namespace std {
	template<>
	struct numeric_limits<Bignum> {
		static constexpr bool is_specialized = true;
		static constexpr bool is_signed = true;
	};
}

#endif //BIGNUM_H
