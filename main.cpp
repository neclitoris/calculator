#include <iostream>
#include "MyClasses/Expression.h"

int main() {
	std::pair<int, char> p1, p2({8, 'a'});
	std::string expr;
	getline(std::cin, expr);
	Expression a;
	try {
		a = Expression(parce(expr));
	} catch (Exception &omae_wa_mou_shindeiru) {
		std::cout << omae_wa_mou_shindeiru.nani() << "\n";
		system("pause");
		return 0;
	}
	a.print();
	try {
		a.calculate();
	} catch (Exception &omae_wa_mou_shindeiru) {
		std::cout << omae_wa_mou_shindeiru.nani() << "\n";
		system("pause");
		return 0;
	}
	a.print();
	while (true) {
		if (!a.has_vars()) {
			system("pause");
			return 0;
		}
		std::cout << "Enter an equation:\n";
		std::string eq;
		getline(std::cin, eq);
		auto ptr = eq.begin();
		while (ptr != eq.end() && *ptr != '=')
			++ptr;
		if (ptr == eq.end()) {
			std::cout << "It's not an equation.\n";
			continue;
		}
		auto var = eq.substr(0, ptr - eq.begin());
		while (var.back() == ' ')
			var.pop_back();
		while (var.front() == ' ')
			var.erase(var.begin());
		auto num = eq.substr(ptr - eq.begin() + 1);
		while (num.back() == ' ')
			num.pop_back();
		while (num.front() == ' ')
			num.erase(num.begin());
		if (!is_name(var)) {
			std::cout << "No name found to the left.\n";
			continue;
		}
		if (!is_number(num) && !is_const(num)) {
			std::cout << "No number found to the right.\n";
			continue;
		}
		if (is_const(num))
			if (num == "e")
				num = Math::e.to_string();
			else
				num = Math::pi.to_string();
		a.replace_var(var, num);
		try {
			a.print();
			a.calculate();
		} catch (Exception &omae_wa_mou_shindeiru) {
			std::cout << omae_wa_mou_shindeiru.nani() << "\n";
			system("pause");
			return 0;
		}
		a.print();
	}
	return 0;
}