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
	class Frame;
	class Void;
	class Symbol;
	class val;

	template <typename T>
	class array {
	
	 public:

		array()              : _size( 0 ) { }
		array( size_t size ) : _size( size ) { }

		unsigned int size() const { return _size; }

		T& operator []( size_t index ) { return (T&) _array[0][index]; }
		const T& operator []( size_t index ) const { return (T&) _array[0][index]; }
		//T& operator []( size_t index ) { return _array[index]; }
		//const T& operator []( size_t index ) const { return _array[index]; }

	 private:
	 
		char   _array[sizeof(T)][5];
		//T      _array[5];
		size_t _size;
	 
	};

	typedef unsigned short SymbolId;
	//typedef vector<Value> Array;
	typedef array<Value> Array;

 //
 // class Atom
 //

	class Atom {

	 public:

		Atom( Frame& frame, float value );
		//Atom( float value );

		Frame* _frame;
		float  _float;
	
	};

 //
 // class Value
 //

	class Value : public Atom {

	 public:

	 	Value();
	 	Value( Frame& frame );
		Value( float value );

		Value eval( Value receiver, const Array& args, Value context );
		//Value send( const Symbol& selector, const Array& args, Value context );
		Value send( const SymbolId selectorId, const Array& args, Value context );
		Value send( const Symbol& selector, const Array& args, Value context );
	
		Frame& getFrame() const;
		float  getFloat() const { return _float; }
//		template <typename T>
//		T& get() const { return *dynamic_cast<T*>( _frame ); }

		Frame& getProto();

		string toString() const;

		operator float() { return getFloat(); }

//	 protected:

//		Frame* _frame;
//		float  _float;

	};

 //
 // class CGValue
 //

	class GCValue : public Value {

	 public:

	 	GCValue();
	 	GCValue( Frame& frame );
		GCValue( float value );
		GCValue( const GCValue& value );
		
		~GCValue();
		
		Value& operator =( const Value& value );

	};

 //
 // class val
 //

	class val : public Value {

	 public:

		val( const Value& value ) : Value( value ) { }

	 	bool operator ==( const Value& value ) { return _float == value._float; }

	};
	
}

#endif

