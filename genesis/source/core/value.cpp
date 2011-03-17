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

	static const double min_float = std::numeric_limits<double>::min();
	static const double max_float = std::numeric_limits<double>::max();

 //
 // class Value
 //

	//inline Value::Value()               : Atom( VoidProto::instance(), 0.0 ) { }
	inline Value::Value()               : Atom( NULL, 0.0 ) { }
	inline Value::Value( Atom value )   : Atom( value ) { }
	inline Value::Value( Frame& frame ) : Atom( frame, max_float ) { }
	inline Value::Value( double value )	: Atom( NumberValue::instance(), value ) { }

	inline Value::Value( SelfMessage& value ) : Atom( value, min_float ) { }

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

	inline Frame& Value::getProto() { return _frame->getProto(); }

	inline Value Value::apply( Value receiver, const Array& args, Value locals )
	{
		// Optimization to return *this immediately if possible
		// If it's not garbage collected, it doesn't override apply()
		
		ENTER( "Value::apply( receiver = " << receiver << " )" );
		
		Value result;
		
		if (getFloat() < max_float)
		{
			if (_float > min_float)
				result = *this;
			else
				result = args.self();
		}
		else
			result = getFrame().apply( receiver, args, locals );
			
		LEAVE( result );
		
		return result;
	}

	inline Value Value::perform( const Symbol selector, const Array& args, Value locals )
	{
		ENTER( "Value::perform( selector = " << selector << " ) *this = " << *this );
		
		Value result = getFrame().perform( *this, selector, args, locals );
		
		LEAVE( result );
		
		return result;
	}

	inline Value Value::perform( const string name, const Array& args, Value locals )
	{
		return perform( SymbolProto::at( name ), args, locals );
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
		if (getFloat() == max_float)
			_frame->incrementReference();
	}

	inline GCValue::~GCValue()
	{
		if (_float == max_float)
			_frame->decrementReference();
	}

	inline GCValue::GCValue( const GCValue& value ) {
		_frame = value._frame;
		_float = value.getFloat();
		
		if (getFloat() == max_float)
			_frame->incrementReference();
	}

	inline GCValue::GCValue( const Value& value ) {
		_frame = value._frame;
		_float = value.getFloat();
		
		if (getFloat() == max_float)
			_frame->incrementReference();
	}

 	inline Value& GCValue::operator =( const Value& value )
 	{
		if (_frame != value._frame && _float != value._float)
		{
			if (value.getFloat() == max_float)
				value.getFrame().incrementReference();

			if (getFloat() == max_float)
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

