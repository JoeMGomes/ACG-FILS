#pragma once

class Counter
{
private:

	static int count;

public:

	static void DrawCall() {
		count++;
	}
	//static member function
	static int getCount()
	{
		return count;
	}

	static void resetCount() {
		count = 0;
	}
};

