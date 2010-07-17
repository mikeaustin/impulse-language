//
// core/protos/string.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include <sstream>

#include "object.h"

namespace impulse {

 //
 // class String
 //

	class String : public Frame {

	 public:

		String( Frame& proto ) : Frame( proto )
		{
			setSlot( Symbol::at( "size" ), *new Method( size_, 0 ) );
			setSlot( Symbol::at( "++" ), *new Method( concat_, 1 ) );
		}

		String( string value ) : Frame( String::instance() ), _value( value ) { }

		string getValue() { return _value; }

		virtual string inspect( Value receiver ) const { return string( "\"" ) + _value + string( "\"" ); }

		static Frame& instance()
		{
			static GCValue string = *new String( Object::instance() );
			
			return string.getFrame();
		}

		static Value size_( Value receiver, const Array& args )
		{
			String& self = receiver.get<String>();
			
			return self._value.size();
		}

		static Value concat_( Value receiver, const Array& args )
		{
			String& self = (String&) receiver.getFrame();
			String& other = (String&) args[0].getFrame();
			
			return self._value + other._value;
		}

	 private:
	
		string _value;
		
	};

}

