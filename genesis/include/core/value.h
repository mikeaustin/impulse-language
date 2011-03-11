//
// core/value.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_VALUE_H
#define IMPULSE_VALUE_H

#include <ostream>

namespace impulse {

	class Frame;
	class Array;

 //
 // class Atom
 //

	class Atom {

	 public:

		Atom() { }
		Atom( Frame& frame, double value ) : _frame( &frame ), _float( value ) { }

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
		Value( double value );

		Value setSlot( const Symbol symbol, const Value value );
		Value getSlot( const Symbol symbol );

		Value setSlot( const string name, const Value value );
		Value getSlot( const string name );

		double getFloat() const { return _float; }
		Frame& getFrame() const { return *_frame; }

		template <typename T> T& get() const { return *static_cast<T*>( _frame ); }
		template <typename T> T& getDynamic() const { return *dynamic_cast<T*>( _frame ); }

		Value apply( Value receiver, const Array& args );
		Value apply( const Array& args ) { return apply( *this, args ); }

		string inspect() const;

		template <typename T> class Type;
	 
	};

	template <typename T>
	class Value::Type : public Value {
	
	 public:
	 
		Type( T& frame ) : Value( frame ) { }

		T& getFrame() { return get<T>(); }
	 
	};

 //
 // class GCValue
 //

	class GCValue : public Value {
	
	 public:
	 
	 	GCValue();
	 	GCValue( Atom atom );
	 	GCValue( Frame& frame );
		GCValue( double value );
		~GCValue();

		inline GCValue( const GCValue& value );
		inline GCValue( const Value& value );

 		Value& operator =( const Value& value );

		template <typename T> class Type;
	
	};

	template <typename T>
	class GCValue::Type : public GCValue {
	
	 public:
	 
		Type( Frame& frame ) : GCValue( frame ) { }

		T& getFrame() { return get<T>(); }
	 
	};

}

#endif

