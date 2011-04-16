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
#include "core/protos/string.h"

#include <limits>

using std::ostream;

namespace impulse {

	//static const double min_float = __DBL_MIN__;
	//static const double max_float = __DBL_MAX__;

#define min_float __DBL_MIN__
#define max_float __DBL_MAX__

	static const long min_float_cast = 0x10000000000000;
	static const long max_float_cast = 0x7fefffffffffffff;
	static const unsigned long nan_float_cast  = 9221120237041090560;

 //
 // class Value
 //

	// TODO: Faster, but don't dereference void's _frame!
	//inline Value::Value()               : Atom( VoidProto::instance(), 0.0 ) { }
	inline Value::Value()               : Atom( NULL, 0.0 ) { }
	inline Value::Value( Atom value )   : Atom( value ) { }
	inline Value::Value( Frame& frame ) : Atom( frame, max_float ) { }
	inline Value::Value( double value )	: Atom( NumberValue::instance(), value ) { }
	inline Value::Value( string value ) : Atom( StringProto::create( value ), 0.0 ) { }

	inline Value::Value( SelfMessage& value ) : Atom( value, min_float ) { }
	//inline Value::Value( OperatorMessage& value ) : Atom( value, nan_float_cast ) { }

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

	inline Value Value::apply( Value receiver, const Array& args, Value locals ) const
	{
		// Optimization to return *this immediately if possible
		// If it's not garbage collected, it doesn't override apply()
		
		ENTER( "Value::apply( receiver = " << receiver << " )" );
		
		Value result;

		if (getFloat() != max_float)
		{
			if (_float != min_float)
				result = *this;
			else
				result = args.self();
		}
		else
			result = getFrame().apply( receiver, args, locals );

/*		switch (reinterpret_cast<const unsigned long&>( _float ))
		{
			case nan_float_cast: result = static_cast<OperatorMessage&>( getFrame() ).apply( receiver, args, locals ); break;
			case max_float_cast: result = getFrame().apply( receiver, args, locals ); break;
			case min_float_cast: result = args.self(); break;
			default:             result = *this; break;
		}
*/
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

	string Value::inspect() const
	{
		return &getFrame() == NULL ? VoidProto::instance().inspect( *this )
		                           : getFrame().inspect( *this );
	}

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

	ostream& operator <<( ostream& stream, const Value value )
	{
		stream << value.inspect();
			
		return stream;
	}
	
}	

