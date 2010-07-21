//
// core/protos/number.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_NUMBER_H
#define IMPULSE_NUMBER_H

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

		Number() : Frame( Object::instance() ) { }
		
		void initSlots()
		{
			static const Frame* addArgTypes[] = { &Number::instance() };
			static const Frame* powArgTypes[] = { &Number::instance() };

			setSlot( Symbol::at( "+" ),    *new Method( "+",    add_,   1, addArgTypes ) );
			setSlot( Symbol::at( "-" ),    *new Method( "-",    sub_,   1 ) );
			setSlot( Symbol::at( "*" ),    *new Method( "*",    mul_,   1 ) );
			setSlot( Symbol::at( "/" ),    *new Method( "/",    div_,   1 ) );
			setSlot( Symbol::at( "%" ),    *new Method( "%",    mod_,   1 ) );
			setSlot( Symbol::at( "sin" ),  *new Method( "sin",  sin,    0 ) );
			setSlot( Symbol::at( "cos" ),  *new Method( "cos",  cos,    0 ) );
			setSlot( Symbol::at( "pow:" ), *new Method( "pow:", pow_,   1, powArgTypes ) );
			setSlot( Symbol::at( ".." ),   *new Method( "..",   range_, 1 ) );
		}

		virtual string inspect( Value receiver ) const { return "<number>"; }

		static Frame& instance()
		{
			static GCValue number = *new Number();
			
			return number.getFrame();
		}

#define OPERATOR_METHOD( function, op ) \
		static Value function( Value receiver, const Array& args, Value context ) \
		{ \
			return receiver.getFloat() op args[0].getFloat(); \
		}

		OPERATOR_METHOD( add_, + );
		OPERATOR_METHOD( sub_, - );
		OPERATOR_METHOD( mul_, * );
		OPERATOR_METHOD( div_, / );

		static Value mod_( Value receiver, const Array& args, Value context )
		{
			return (long) receiver.getFloat() % (long) args[0].getFloat();
		}

		static Value sin( Value receiver, const Array& args, Value context )
		{
			return ::sin( receiver.getFloat() );
		}

		static Value cos( Value receiver, const Array& args, Value context )
		{
			return ::cos( receiver.getFloat() );
		}

		static Value pow_( Value receiver, const Array& args, Value context )
		{
			return ::pow( receiver.getFloat(), args[0].getFloat() );
		}

		static Value range_( Value receiver, const Array& args, Value context )
		{
			Range& range = *new Range( receiver.getFloat(), args[0].getFloat() );
			
			autorelease( range );
			
			return range;
		}
		
	};

	class NumberValue : public Frame {
	
	 public:
	 
	 	NumberValue() : Frame( Number::instance() ) { }

		virtual string inspect( Value receiver ) const
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

#endif

