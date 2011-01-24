//
// core/value.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_VALUE_H
#define IMPULSE_VALUE_H

#include <iostream>
#include <vector>
#include <cstring>
#include <limits>

using namespace std;

namespace impulse {

	class Atom;
	class Value;
	class GCValue;
	class val;
	
	class Frame;
	class Void;
	class Symbol;
	class Array;
	class Block;
	class Lobby;

 //
 // class Atom
 //

	class Atom {

	 public:

		Atom() { }
		Atom( Frame& frame, double value )  : _frame( &frame ), _float( value ) { }

		Frame* _frame;
		double _float;
	
	};

 //
 // class Value
 //

	class Value : public Atom {

	 public:

	 	Value();
	 	Value( Atom atom );
	 	Value( Frame& frame );
	 	Value( Void& frame );
	 	Value( Lobby& frame );

		Value( string value );
		Value( double value );
		//Value( size_t value );
		Value( int value );
		Value( long value );
		Value( unsigned value );
		Value( bool value );

		Value setSlot( const Symbol& symbol, const Value value );
		Value getSlot( const Symbol& symbol );

		Value eval( Value receiver, const Array& args, Value context );
		Value send( const Symbol& selector, const Array& args, Value context );
	
		Frame& getFrame() const { return *_frame; }
		double  getFloat() const { return _float; }
		bool getBool() const { return _float; }
		template <typename T>
		T& get() const { return *static_cast<T*>( _frame ); }
		template <typename T>
		T& getDynamic() const { return *dynamic_cast<T*>( _frame ); }

		Frame& getProto() const;

		Value clone();

		string inspect() const;

		operator double() { return getFloat(); }

	};

	ostream& operator <<( ostream& stream, Value value );

 //
 // class CGValue
 //

	class GCValue : public Value {

	 public:

	 	GCValue();
	 	GCValue( Frame& frame );
	 	GCValue( const Value& value );
		GCValue( const double value );
		GCValue( const GCValue& value );
		
		~GCValue();
		
		Value& operator =( const Value& value );

	};

 //
 // class val
 //

	class val : public Value {

	 public:

		val()                     : Value() { }
		val( const Value& value ) : Value( value ) { }

	 	bool operator ==( const Value& value ) { return _float == value._float; }

	};

	typedef unsigned short SymbolId;
	
}

#endif

