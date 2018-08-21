#ifndef POINT_H
#define POINT_H

#include <windows.h>
#include <utility>

namespace Snake_game {

	class Point {
	public:
		int x, y;

	public:
		Point() {}

		Point(int x_in, int y_in) {
			x = x_in, y = y_in;
		}

		Point(COORD source) {
			x = source.X, y = source.Y;
		}

		Point operator-(Point &&rhs) const {
			return Point(x - rhs.x, y - rhs.y);
		}

		Point operator+(Point &&rhs) const {
			return Point(x + rhs.x, y + rhs.y);
		}

		Point operator-(Point &rhs) const {
			return Point(x - rhs.x, y - rhs.y);
		}

		Point operator+(Point &rhs) const {
			return Point(x + rhs.x, y + rhs.y);
		}

		Point &operator-=(Point &&rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Point &operator+=(Point &&rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Point &operator-=(Point &rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Point &operator+=(Point &rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		bool operator==(Point &rhs) const {
			return x == rhs.x && y == rhs.y;
		}

		bool operator==(Point &&rhs) const {
			return x == rhs.x && y == rhs.y;
		}

		operator COORD() const {
			return {static_cast<SHORT>(x), static_cast<SHORT>(y)};
		}
	};

}

#endif //POINT_H
