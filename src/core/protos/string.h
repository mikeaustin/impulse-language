//
// core/protos/string.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_STRING
#define IMPULSE_STRING

#include <sstream>

#include "object.h"

namespace impulse {

 //
 // class String
 //

	class String : public Frame {

	 public:

		String( Frame& proto ) : Frame( proto ) { }
		
		void initSlots()
		{
			setSlot( Symbol::at( "size" ), *new Method( "size", *new Block( size ),    0 ) );
			setSlot( Symbol::at( "++" ),   *new Method( "++",   *new Block( concat_ ), 1 ) );
		}

		String( string value ) : Frame( String::instance() ), _value( value ) { }

		string getValue() { return _value; }

		virtual string inspect( const Value receiver ) const
		{
			if (&receiver.getFrame() == &String::instance())
				return "<string>";
			
			return string( "\"" ) + _value + string( "\"" );
		}

		static Frame& instance()
		{
			static GCValue string = *new String( Object::instance() );
			
			return string.getFrame();
		}

		static Value size( Value receiver, const Array& args, Value context )
		{
			String& self = receiver.get<String>();
			
			return self._value.size();
		}

		static Value concat_( Value receiver, const Array& args, Value context )
		{
			String& self = (String&) receiver.getFrame();
			String& other = (String&) args[0].getFrame();
			
			return self._value + other._value;
		}

	 private:
	
		string _value;
		
	};

}

#endif

