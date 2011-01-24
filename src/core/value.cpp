//
// core/value.cpp
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include "value.h"

namespace impulse {

	class String;

 //
 // class Value
 //

	inline Value::Value()               : Atom( Void::instance(), 0 ) { }
	inline Value::Value( Atom value )   : Atom( value ) { }
	//inline Value::Value( Frame& value ) : Atom( value, &value == &Void::instance() ? 0 : numeric_limits<double>::max() ) { }
	inline Value::Value( Frame& value ) : Atom( value, numeric_limits<double>::max() ) { }
	inline Value::Value( Void& value )  : Atom( value, 0 ) { }
	inline Value::Value( Lobby& value ) : Atom( value, 0 ) { }

	inline Value::Value( string value )   : Atom( *new String( value ), numeric_limits<double>::max() ) { }
	inline Value::Value( bool value )     : Atom( BooleanValue::instance(), value ) { }
	inline Value::Value( double value )   : Atom( NumberValue::instance(), value ) { }
	inline Value::Value( int value )      : Atom( NumberValue::instance(), value ) { }
	inline Value::Value( long value )     : Atom( NumberValue::instance(), value ) { }
	inline Value::Value( unsigned value ) : Atom( NumberValue::instance(), value ) { }
	//inline Value::Value( size_t value )   : Atom( NumberValue::instance(), value ) { }

	inline Value Value::setSlot( const Symbol& symbol, const Value value ) { return getFrame().setSlot( symbol, value ); }
	inline Value Value::getSlot( const Symbol& symbol ) { return getFrame().getSlot( symbol ); }

	inline Value Value::eval( Value receiver, const Array& args, Value context )
	{
		if (getFloat() < numeric_limits<double>::max())
		{
			ENTER( "Value::eval( value = " << inspect() << ", receiver = " << receiver.inspect() << " )" );

			LEAVE( inspect() );
			
			return *this;
		}

		Value result = getFrame().eval( receiver, args, context );
		
		return result;
	}

	inline Value Value::send( const Symbol& selector, const Array& args, Value context )
	{
		//ENTER( "Value::send( value = " << inspect() << ","
		//				   " selector = " << selector.inspect( const_cast<Symbol&>( selector ) ) << " )" );
		
		Value result = getFrame().send( *this, selector, args, context );
		
		//LEAVE( result.inspect() );
		
		return result;
	}

	inline Frame& Value::getProto() const { return _frame->getProto(); }

	Value Value::clone()
	{
		if (getFloat() < numeric_limits<double>::max())
		{
			return *this;
		}

		Frame& frame = *new Frame();
		frame._proto = &getFrame();
		
		return frame;
	}

	string Value::inspect() const { return getFrame().inspect( *this ); }

	ostream& operator <<( ostream& stream, Value value )
	{
		if (&value._frame->getProto() == &Number::instance())
			stream << value.getFloat();
		else if (&value._frame->getProto() == &String::instance())
			stream << value.get<String>().getValue();
		else
			stream << value.inspect();
			
		return stream;
	}

 //
 // class GCValue
 //

	inline GCValue::GCValue()               : Value() { }
	inline GCValue::GCValue( Frame& value ) : Value( value )
	{
		//cout << "GCValue::GCValue( Frame& value )" << endl;
		
		if (_float == numeric_limits<double>::max())
			_frame->incRef();
	}
	inline GCValue::GCValue( const Value& value ) : Value( value )
	{
		//cout << "GCValue::GCValue( const Value& value )" << endl;

		if (_float == numeric_limits<double>::max())
			_frame->incRef();
	}
	inline GCValue::GCValue( const double value ) : Value( value ) { }
	inline GCValue::GCValue( const GCValue& value )
	{
		//cout << "GCValue::GCValue( const GCValue& value )" << endl;

		if (value._float == numeric_limits<double>::max())
			value._frame->incRef();

	 	_frame = value._frame;
	 	_float = value.getFloat();
	}

	inline GCValue::~GCValue()
	{
		//cout << "GCValue::~GCValue()" << endl;

		if (_float == numeric_limits<double>::max())
			_frame->decRef();
	}

 	inline Value& GCValue::operator =( const Value& value )
 	{
		//cout << "GCValue::operator =( const Value& value )" << endl;

		if (value._float == numeric_limits<double>::max())
			value._frame->incRef();
		if (_float == numeric_limits<double>::max())
			_frame->decRef();

	 	_frame = value._frame;
	 	_float = value.getFloat();
	 	
	 	//LEAVE( "" );
	 	
 		return *this;
 	}

}

