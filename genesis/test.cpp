#include <iostream>
#include <limits>

using namespace std;

int main()
{
	double x = numeric_limits<double>::quiet_NaN();

	cout << reinterpret_cast<unsigned long&>( x ) << endl;

	return 0;
}

