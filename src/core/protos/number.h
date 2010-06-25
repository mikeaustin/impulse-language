//
// core/protos/number.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include <sstream>
#include <cmath>

#include "object.h"

namespace impulse {

	struct MethodCache {

		Symbol* selector;
		Frame*  method;

	};

 //
 // class Number
 //

	class Number : public Frame {

	 public:

		Number() : Frame( Object::instance() )
		{
			setSlot( add.getId(), *new Method( add_, 1 ) );
			setSlot( mul.getId(), *new Method( mul_, 1 ) );
			setSlot( _sin, *new Method( sin, 0 ) );
		}
/*
		virtual Value Xsend( Value receiver, const Symbol& selector, const Array& args, Value context )
		{
			static MethodCache cache;

			if (&selector == cache.selector)
			{
				return cache.method->eval( receiver, args, context );
			}

			SlotMap::iterator iter = _slots.find( &selector );

			if (iter != _slots.end())
			{
				Value method = iter->second;

				cache.selector = (Symbol*) &selector;
				cache.method   = &method.getFrame();
			}

			return Frame::send( receiver, selector, args, context );
		}
*/
		virtual string toString( Value receiver ) const
		{
			return "[Number]";
		}

		static Frame& instance()
		{
			static GCValue number = *new Number();
			
			return number.getFrame();
		}

		static Value add_( Value receiver, const Array& args )
		{
			return receiver.getFloat() + args[0].getFloat();
		}
	
		static Value mul_( Value receiver, const Array& args )
		{
			return receiver.getFloat() * args[0].getFloat();
		}

		static Value sin( Value receiver, const Array& args )
		{
			return ::sinf( receiver.getFloat() );
		}
		
	};

	class NumberValue : public Frame {
	
	 public:
	 
	 	NumberValue() : Frame( Number::instance() ) { }

		virtual string toString( Value receiver ) const
		{
			ostringstream stream; stream << receiver.getFloat();
			
			return stream.str();
		}

		static Frame& instance()
		{
			//static GCValue number( *new NumberValue() );
			static NumberValue number;
			
			//return number.getFrame();
			return number;
		}
	
	};

}

