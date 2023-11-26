#pragma once
class gaming
{
public:
	void gaming15(int x, int y) {
		_x = x;
		_y = y;
	}
	int result() {
		return _x + _y;
	}
private:
	int _x;
	int _y;
};

