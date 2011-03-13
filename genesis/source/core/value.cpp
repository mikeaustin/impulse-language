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

#include <limits>

namespace impulse {

 //
 // class Value
 //

	inline Value::Value()               : Atom( VoidProto::instance(), 0 ) { }
	inline Value::Value( Atom value )   : Atom( value ) { }
	inline Value::Value( Frame& frame ) : Atom( frame, std::numeric_limits<double>::max() ) { }
	inline Value::Value( double value )	: Atom( NumberValue::instance(), value ) { }

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

	inline Value Value::apply( Value receiver, const Array& args, Value locals )
	{
		// Optimization to return *this immediately if possible
		// If it's not garbage collected, it doesn't override value()
		
		if ( getFloat() < std::numeric_limits<double>::max() )
			return receiver;
		else
			return getFrame().apply( receiver, args, locals );
	}

	inline Value Value::perform( const Symbol selector, const Array& args, Value locals )
	{
		return getFrame().perform( *this, selector, args, locals );
	}

	string Value::inspect() const { return getFrame().inspect( *this ); }

 //
 // class GCValue
 //

	inline GCValue::GCValue()               : Value() { }
	inline GCValue::GCValue( Atom value )   : Value( value ) { }
	inline GCValue::GCValue( double value ) : Value( value ) { }
	inline GCValue::GCValue( Frame& frame ) : Value( frame )
	{
		if (getFloat() == std::numeric_limits<double>::max())
			_frame->incrementReference();
	}

	inline GCValue::~GCValue()
	{
		if (_float == std::numeric_limits<double>::max())
			_frame->decrementReference();
	}

	inline GCValue::GCValue( const GCValue& value ) {
		_frame = value._frame;
		_float = value.getFloat();
		
		if (getFloat() == std::numeric_limits<double>::max())
			_frame->incrementReference();
	}

	inline GCValue::GCValue( const Value& value ) {
		_frame = value._frame;
		_float = value.getFloat();
		
		if (getFloat() == std::numeric_limits<double>::max())
			_frame->incrementReference();
	}

 	inline Value& GCValue::operator =( const Value& value )
 	{
		if (_frame != value._frame && _float != value._float)
		{
			if (value.getFloat() == std::numeric_limits<double>::max())
				value.getFrame().incrementReference();

			if (getFloat() == std::numeric_limits<double>::max())
				getFrame().decrementReference();
		}

	 	_frame = value._frame;
	 	_float = value.getFloat();
	 	
 		return *this;
 	}

	std::ostream& operator <<( std::ostream& stream, Value value )
	{
		stream << value.inspect();
			
		return stream;
	}
	
}	

