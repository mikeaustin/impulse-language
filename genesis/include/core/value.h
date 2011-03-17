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

	class GCValue;
	class Frame;
	class Array;
	class LocalsProto;
	class SelfMessage;

 //
 // class Atom
 //

	class Atom {

	 public:

		Atom() { }
		Atom( void* frame, double value ) : _frame( NULL ), _float( value ) { }
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
		Value( SelfMessage& value );

		Value setSlot( const Symbol symbol, const Value value );
		Value getSlot( const Symbol symbol );

		Value setSlot( const string name, const Value value );
		Value getSlot( const string name );

		Frame& getProto();

		Frame& getFrame() const { return *_frame; }
		double getFloat() const { return _float; }

		template <typename T> T& get() const { return *static_cast<T*>( _frame ); }
		template <typename T> T& getDynamic() const { return *dynamic_cast<T*>( _frame ); }

		Value apply( Value receiver, const Array& args, Value locals );

		Value perform( const Symbol selector, const Array& args, Value locals );
		Value perform( const string name, const Array& args, Value locals );

		string inspect() const;

		template <typename T> class Type;
	 
	};

	template <typename T>
	class Value::Type : public Value {
	
	 public:
	 
		Type( T& frame ) : Value( frame ) { }

		T& getFrame() { return get<T>(); }
	 
	};

	std::ostream& operator <<( std::ostream& stream, Value value );


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

