//
// core/value.cpp
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include "value.h"

namespace impulse {

 //
 // class Atom
 //
 
	Atom::Atom( Frame& frame, float value ) : _frame( &frame ), _float( value ) { }

 //
 // class Value
 //

	inline Value::Value()               : Atom( Void::instance(), 0 ) { }
	inline Value::Value( Frame& value ) : Atom( value, &value == &Void::instance() ? 0 : numeric_limits<float>::max() ) { }
	inline Value::Value( float value )  : Atom( NumberValue::instance(), value ) { }

	inline Value Value::eval( Value receiver, const Array& args, Value context )
	{
		//ENTER( "Value::eval( this = " << toString() << ", receiver = " << receiver.toString() << " )" );

		if (getFloat() < numeric_limits<float>::max())
		{
			return *this;
		}

		//return getFrame().eval( receiver, args, context );
		return (getFrame()._eval2)( &getFrame(), receiver, args, context );
	}

	inline Value Value::send( const SymbolId selectorId, const Array& args, Value context )
	{
		return getFrame().send( *this, selectorId, args, context );
	}

	inline Value Value::send( const Symbol& selector, const Array& args, Value context )
	{
		return getFrame().send( *this, selector.getId(), args, context );
	}

	inline Frame& Value::getFrame() const { return *_frame; }

	inline Frame& Value::getProto() { return _frame->getProto(); }

	string Value::toString() const { return getFrame().toString( *this ); }

 //
 // class GCValue
 //

	inline GCValue::GCValue()               : Value() { }
	inline GCValue::GCValue( Frame& value ) : Value( value )
	{
		if (_float == numeric_limits<float>::max())
			_frame->incRef();
	}
	inline GCValue::GCValue( float value ) : Value( value ) { }
	inline GCValue::GCValue( const GCValue& value )
	{
		if (value._float == numeric_limits<float>::max())
			value._frame->incRef();

	 	_frame = value._frame;
	 	_float = value.getFloat();
	}

	inline GCValue::~GCValue()
	{
		if (_float == numeric_limits<float>::max())
			_frame->decRef();
	}

 	inline Value& GCValue::operator =( const Value& value )
 	{
 		ENTER( "CGValue::operator =()" );

		if (value._float == numeric_limits<float>::max())
			value._frame->incRef();
		if (_float == numeric_limits<float>::max())
			_frame->decRef();

	 	_frame = value._frame;
	 	_float = value.getFloat();
	 	
	 	LEAVE( "" );
	 	
 		return *this;
 	}

}

