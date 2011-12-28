#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

int main()
{
	cout << numeric_limits<double>::has_quiet_NaN << endl;
	cout << numeric_limits<double>::has_signaling_NaN << endl;

	double d = numeric_limits<double>::signaling_NaN();
	cout << d << endl;
	cout << isnan(d) << endl;
	cout << 0/0 << endl;

	return 0;
}

