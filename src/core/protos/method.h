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

		typedef Value (* const Function)(Value, const Array&, Value context);

	 public:
	 
		Method( const string name, Function function, const int argsSize )
		 : _function( function ), _argsSize( argsSize ), _argTypes( NULL ) { }

		Method( const string funcName, Function function, const int argsSize, const Frame* argTypes[] )
		 : _function( function ), _argsSize( argsSize ), _argTypes( argTypes ), _funcName( funcName ) { }

		virtual string inspect( Value receiver ) const { return "<method \'" + _funcName + ">"; }

		Value eval( Value receiver, const Array& args, Value context )
		{
			ENTER( "Method::eval( this = " << inspect( *this ) << "," <<
								" receiver = " << receiver.inspect() << " )" );

			if (_argsSize != (unsigned int) -1 && args.size() != _argsSize)
			{
				cerr << "*** Wrong number of arguments" << endl;

				LEAVE( "" );

				return Value();
			}

			if (_argTypes)
			{
				for (unsigned int i = 0; i < _argsSize; ++i)
				{
					if (&args[i].getProto() != _argTypes[i])
					{
						cerr << "*** Argument types mismatch" << endl;

						LEAVE( "" );

						return Value();
					}
				}
			}

			Value result = _function( receiver, args, context );

			LEAVE( result );
			
			return result;
		}
	
	 private:

		const Function _function;
		unsigned int   _argsSize;
		const Frame**  _argTypes;
		const string   _funcName;
		
	};

}

#endif

