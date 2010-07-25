//
// core/protos/boolean.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_BOOLEAN
#define IMPULSE_BOOLEAN

#include "object.h"

namespace impulse {

 //
 // class Boolean
 //

	class Boolean : public Frame {

	 public:

		Boolean( Frame& proto ) : Frame( proto ) { }
		
		void initSlots();

		Boolean( bool value ) : Frame( Boolean::instance() ), _value( value ) { }

		bool getValue() { return _value; }

		virtual string inspect( const Value receiver ) const
		{
			if (&receiver.getFrame() == &Boolean::instance())
				return "<boolean>";
			
			return _value ? "true" : "false";
		}

		static Frame& instance()
		{
			static GCValue object = *new Boolean( Object::instance() );
			
			return object.getFrame();
		}

		OPERATOR_METHOD( equal_, == );
		OPERATOR_METHOD( notEqual_, != );

		static Value ternary_( Value receiver, const Array& args, Value context );

	 private:
	
		bool _value;
		
	};

	class BooleanValue : public Frame {
	
	 public:
	 
	 	BooleanValue() : Frame( Boolean::instance() ) { }

		virtual string inspect( const Value receiver ) const
		{
			ostringstream stream; stream << (receiver.getBool() ? "true" : "false");
			
			return stream.str();
		}

		static Frame& instance()
		{
			//static GCValue number( *new NumberValue() );
			static BooleanValue object;
			
			//return number.getFrame();
			return object;
		}
	
	};

}

#endif

