#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class Value { };
class Symbol { };

ostream& operator <<( ostream& stream, const Value& value )
{
	return stream << 10, stream;
}

Value value( vector<Value>::iterator iter )
{
	return *iter;
}

Value value( map<Symbol, Value>::iterator iter )
{
	return (*iter).second;
}

template <typename T>
void foo( T seq )
{
	typename T::iterator iter = seq.begin();

	while (iter != seq.end())
	{
		cout << value( iter ) << endl;

		++iter;
	}
}

int main()
{
	vector<Value> a;
	map<Symbol, Value> b;

	a.push_back( Value() );

	foo( a );
	foo( b );

	return 0;
}

