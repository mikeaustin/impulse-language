//
// core/value.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#include "core/value.h"

#include "core/protos/void.h"
#include "core/protos/number.h"
#include "core/protos/symbol.h"

namespace impulse {

 //
 // class Value
 //

	inline Value::Value()               : Atom( VoidProto::instance(), 0 ) { }
	inline Value::Value( Atom value )   : Atom( value ) { }
	//inline Value::Value( Frame& value ) : Atom( value, numeric_limits<double>::max() ) { }
	inline Value::Value( Frame& frame ) : Atom( frame, 0 ) { }

	inline Value::Value( double value )	 : Atom( NumberValue::instance(), value ) { }

	inline Value Value::setSlot( const Symbol symbol, const Value value )
	{
		return getFrame().setSlot( symbol, value );
	}
	
	inline Value Value::getSlot( const Symbol symbol )
	{
		return getFrame().getSlot( symbol );
	}

	inline Value Value::setSlot( const string name, const Value value )
	{
		return setSlot( SymbolProto::at( name ), value );
	}
	
	inline Value Value::getSlot( const string name )
	{
		return getSlot( SymbolProto::at( name ) );
	}

	string Value::inspect() const { return getFrame().inspect( *this ); }

 //
 // class GCValue
 //

	inline GCValue::GCValue()               : Value() { }
	inline GCValue::GCValue( Atom value )   : Value( value ) { }
	//inline Value::Value( Frame& value ) : Atom( value, numeric_limits<double>::max() ) { }
	inline GCValue::GCValue( Frame& frame ) : Value( frame )
	{
		_frame->incRef();
	}

	inline GCValue::~GCValue()
	{
		//cout << "GCValue::~GCValue()" << endl;

		//if (_float == numeric_limits<double>::max())
			_frame->decRef();
	}


	std::ostream& operator <<( std::ostream& stream, Value value )
	{
		stream << value.inspect();
			
		return stream;
	}
	
}	

