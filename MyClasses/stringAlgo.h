#ifndef STRINGALGO_H
#define STRINGALGO_H

#include <vector>

std::vector<int> &&z_func(std::string &S) {
	int l, r = l = 0;
	std::vector<int> &ans = *new std::vector<int>(S.length());
	ans[0] = S.length();
	for (int i = 1; i < S.length(); i++) {
		if (i <= r)
			ans[i] = min(r - i + 1, ans[i - l]);
		while (i + ans[i] < S.length() && S[ans[i]] == S[i + ans[i]])
			++ans[i];
		if (i + ans[i] - 1 > r)
			l = i, r = i + ans[i] - 1;
	}
	return std::move(ans);
}

std::vector<int> &&pref_func(std::string &S) {
	std::vector<int> &ans = *new std::vector<int>(S.length());
	ans[0] = 0;
	for (int i = 1; i < S.length(); i++) {
		int k = ans[i - 1];
		while (k > 0 && S[i] != S[k]) k = ans[k - 1];
		if (S[i] == S[k]) ++k;
		ans[i] = k;
	}
	return std::move(ans);
}

void build_suf_arr(std::string s, std::vector<int> &suf_arr) {
	s += '\0';
	const int n = s.size();

	if (suf_arr.size() != n)
		suf_arr.resize(n);

	std::vector<int> cnt(max(256, n), 0);
	std::vector<int> eq_classes(n);

	/// сортируем первые символы поразрядной сортировкой
	for (uchar ch: s)
		cnt[ch]++;
	for (int i = 1; i < cnt.size(); i++)
		cnt[i] += cnt[i - 1];
	for (int i = n - 1; i >= 0; i--)
		suf_arr[--cnt[s[i]]] = i;

	int cl = 1;
	for (int i = 1; i < n; i++) {
		/// если символ в текущем элементе суффмассива не равен предыдущему - меняем текущий класс эквивалентности.
		if (s[suf_arr[i]] != s[suf_arr[i - 1]])
			cl++;
		eq_classes[suf_arr[i]] = cl - 1;
	}

	std::vector<int> t_suf_arr(n), t_eq_classes;
	/// пока блоки размером меньше n и не у каждого элемента класс эквивалентности уникален.
	for (int k = 0; (1 << k) < n && cl < s.size(); k++) {
		t_eq_classes.resize(n);

		cnt.assign(n, 0);

		/// добавляем перед каждым блоком блок такой же длины (для которого тоже определён текущий класс эквивалентности)
		/// обновляем cnt для очередной сортировки подсчётом.
		for (int i = 0; i < n; i++) {
			t_suf_arr[i] = suf_arr[i] - (1 << k);
			t_suf_arr[i] < 0 ? t_suf_arr[i] += n : t_suf_arr[i];
			cnt[eq_classes[i]]++;
		}
		/// сама сортировка. Сортируем блоки длины 2^(k + 1) по левой половине, т.к. по правой они уже отсортированы.
		for (int i = 1; i < cl; i++)
			cnt[i] += cnt[i - 1];
		for (int i = n - 1; i >= 0; i--)
			suf_arr[--cnt[eq_classes[t_suf_arr[i]]]] = t_suf_arr[i];

		/// обновляем список классов эквивалентности.
		t_eq_classes[suf_arr[0]] = 0;
		cl = 1;

		/// Каждый из упорядоченных теперь блоков можно разбить на две части, для которых классы эквивалентности уже известны.
		for (int i = 1; i < n; i++) {
			int m1 = suf_arr[i - 1] + (1 << k),
				m2 = suf_arr[i] + (1 << k);
			m1 >= n ? m1 -= n : m1;
			m2 >= n ? m2 -= n : m2;

			if (eq_classes[suf_arr[i]] != eq_classes[suf_arr[i - 1]] || eq_classes[m1] != eq_classes[m2]) cl++;

			t_eq_classes[suf_arr[i]] = cl - 1;
		}

		/// Заменяем список классов эквивалентности на новый.

		/// WARNING: стандарт C++11 или позже.
		eq_classes = std::move(t_eq_classes);
		/// С++ < 11
		//eq_classes.swap(t_eq_classes);
	}
	/// Убираем фиктивный элемент.
	suf_arr.erase(suf_arr.begin());
}

#endif //STRINGALGO_H
