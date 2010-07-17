//
// core/protos/method.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

namespace impulse {

 //
 // class Method
 //

	class Method : public Frame {

	 public:
	 
		Method( Value (* const func)(Value, const Array&), const unsigned int argsSize )
		 : _func( func ), _argsSize( argsSize ), _argTypes( NULL ) { }

		Method( Value (* const func)(Value, const Array&), const unsigned int argsSize, const Frame* argTypes[] )
		 : _func( func ), _argsSize( argsSize ), _argTypes( argTypes ) { }

		virtual string inspect( Value receiver ) const { return "<method>"; }

		inline Value eval( Value receiver, const Array& args, Value context )
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

			Value result = _func( receiver, args );

			LEAVE( result );
			
			return result;
		}
	
	 private:

		Value (* const _func)(Value, const Array&);
		unsigned int _argsSize;
		const Frame** _argTypes;
		
	};

}

