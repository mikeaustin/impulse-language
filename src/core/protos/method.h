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

		Method( Frame& proto ) : Frame( proto ), _argsSize( 0 ) { }	 
		Method( const string funcName, Block& function, const int argsSize );
		Method( const string funcName, Block& function, const int argsSize, const Frame* argTypes[] );

		void initSlots();

		void addBlock( Block& block, const Frame* argTypes[] );

		virtual string inspect( const Value receiver ) const { return "<method " + _funcName + ">"; }

		static Frame& instance();

		static Value arity( Value receiver, const Array& args, Value context )
		{
			Method& self  = receiver.get<Method>();
			
			return self._argsSize;
		}

		Value eval( Value receiver, const Array& args, Value context );

	 private:

		GCValue        _function;
		const unsigned _argsSize;
		const Frame**  _argTypes;
		const string   _funcName;

		map<const Frame**, Block*> _argTypes2;
		
	};

}

#endif

