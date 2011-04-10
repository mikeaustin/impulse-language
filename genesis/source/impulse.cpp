//
// impulse.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

//#include "impulse.h"
//#include "core/all.h"
//#include "runtime/all.h"

#include <iostream>
#include <string>
#include <vector>
#include <numeric>

using std::string;
using std::vector;
using std::istream;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include "impulse.h"

#include "core/protos/symbol.cpp"
#include "core/protos/number.cpp"

#include "core/protos/locals.h"
#include "core/protos/method.h"
#include "core/protos/stream.h"
#include "runtime/protos/message.h"

#include "core/value.cpp"
#include "core/frame.cpp"

#include "parser/parser.h"

#include "parser/scanner.cpp"

#ifdef TEST
	#include "../tests/core.cpp"
	#include "../tests/block.cpp"
	#include "../tests/frame.cpp"
	#include "../tests/number.cpp"
#endif

namespace impulse {

	const char spaces[] = "                                                            ";
	int        indent = 0;
	
}

using namespace impulse;

Value receiver;
Array arguments;

int main( int argc, char* argv[] )
{
	if (0)
	{
		//std::cout << "------------------------------------------------------------" << std::endl;
		std::cout << "Impulse 0.2.0 — Copyright 2008-2011 Mike Austin" << std::endl;

		std::cout << std::endl;
	
		std::cout << "sizeof (double)   = " << sizeof (double)   << std::endl;
		std::cout << "sizeof (Value)    = " << sizeof (Value)    << std::endl;
		std::cout << "sizeof (Frame)    = " << sizeof (Frame)    << std::endl;
		std::cout << "sizeof (SymbolId) = " << sizeof (Frame::SymbolId) << std::endl;
		std::cout << "sizeof (Array)    = " << sizeof (Array)    << std::endl;
	}

	if (0)
	{
		Frame::ReleasePool releasePool;

		ObjectProto::initSlots();
		NumberProto::initSlots();

#ifdef TEST
		CoreTest().run();
		BlockTest().run();
		FrameTest().run();
		NumberTest().run();
#endif

#ifdef BENCH
		Array args();
	
		//for ( volatile int i = 0; i < 1000000000; i++ )
		{
			//Value( 10 ).apply( 10, args );
			//block.apply( 10, args );
		}
	
		std::cout << "Benchmark results:" << std::endl;
#endif

		Frame& lobby  = Frame::create();
		Value locals = *new LocalsProto( lobby );

		std::vector< std::vector<Value> > code;
		code.push_back( std::vector<Value>() );
		
		code.back().push_back( *new SelfMessage() );
		//code.back().push_back( *new MessageProto( SymbolProto::at( "mul" ), *new ArrayProto( *new SelfMessage() ) ) );
		code.back().push_back( *new OperatorMessage<mul_>( SymbolProto::at( "*" ), *new ArrayProto( *new SelfMessage() ) ) );
		//code.back().push_back( *new ConstOperatorMessage<mul_>( SymbolProto::at( "*", 5 ) ) );

		receiver = locals;
		arguments.self( 5 );

		//for (int i = 0; i < 200000000; i++)
		{
			std::vector< std::vector<Value> >::const_iterator line = code.begin();

			while (line != code.end())
			{
				std::vector<Value>::const_iterator message = line->begin();
				const std::vector<Value>::const_iterator end = line->end();

				TRACE( "" );

				receiver = locals;

				while (message != end)
				{
					receiver = message->apply( receiver, arguments, locals );

					TRACE( "" );

					++message;
				}
			
				++line;
			}
		}
		
		cout << "result = " << receiver << endl;
	}

	{
		Frame& lobby  = Frame::create();
		Value locals = *new LocalsProto( lobby );

		Scanner& scanner = *new Scanner( cin );

		Token token;
	
//		while (token = scanner.nextToken(), token.type() != NULL)
//		{
//			cout << "=> " << token.value().inspect() << endl;
//		}

		StatementParser parser( scanner );
	
		vector<GCValue> messages;
		parser.parse( messages );

		vector<GCValue>::const_iterator message = messages.begin();

		while (message != messages.end())
		{
			receiver = message->apply( receiver, arguments, locals );
		
			++message;
		}

		cout << receiver << endl;
	}
	
	return 0;
}

