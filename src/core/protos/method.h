//
// core/protos/method.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_METHOD
#define IMPULSE_METHOD

namespace impulse {

 //
 // class Method
 //

	class Method : public Frame {

		//typedef Value (* const Function)(Value, const Array&, Value context);

	 public:
	 
		Method( const string funcName, Block& function, const int argsSize );
		Method( const string funcName, Block& function, const int argsSize, const Frame* argTypes[] );

		void addBlock( Block& block, const Frame* argTypes[] );

		virtual string inspect( const Value receiver ) const { return "<method \'" + _funcName + ">"; }

		Value eval( Value receiver, const Array& args, Value context );

	 private:

		Block&   _function;
		const unsigned _argsSize;
		const Frame**  _argTypes;
		const string   _funcName;
		map<const Frame**, Block*> _argTypes2;
		
	};

}

#endif

