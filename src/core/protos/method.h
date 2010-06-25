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
		 : _func( func ), _argsSize( argsSize ) { _eval2 = &eval2; }

		virtual string toString( Value receiver ) const { return "[Method]"; }

		inline Value eval( Value receiver, const Array& args, Value context )
		{
			ENTER( "Method::eval( receiver = " << receiver.toString() << " )" );

			if (args.size() == _argsSize)
			{
				LEAVE( "" );
				
				return _func( receiver, args );
			}

			cerr << "Wrong number of arguments" << endl;
			
			LEAVE( "" );

			return Void::instance();
		}

		inline static Value eval2( Frame* self_, Value receiver, const Array& args, Value context )
		{
			Method* self = (Method*) self_;
			
			ENTER( "Method::eval( receiver = " << receiver.toString() << " )" );

			if (args.size() == self->_argsSize)
			{
				LEAVE( "" );
				
				return self->_func( receiver, args );
			}

			cerr << "Wrong number of arguments" << endl;
			
			LEAVE( "" );

			return Void::instance();
		}
	
	 private:

		Value (*_func)(Value, const Array&);
		unsigned int  _argsSize;
	};

}

