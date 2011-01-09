//
// core/protos/vector.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_VECTOR
#define IMPULSE_VECTOR

#include "../traits/enumerable.h"
#include "../traits/sequence.h"

namespace impulse {

 //
 // class Vector
 //

	class Vector : public Enumerable, public Sequence<double> {

	 public:

	 	Vector();
	 	Vector( const size_t size );
		Vector( Frame& proto, const size_t size );

		void initSlots();

	 private:

		vector<double> _array;

	};

	Vector::Vector() { }
	Vector::Vector( const size_t size ) : _array( size ) { }
	Vector::Vector( Frame& proto, const size_t size = 0 ) : Sequence<double>( proto ) { }

	void Vector::initSlots()
	{
		static const Frame* mapArgTypes[] = { &Block::instance() };
		//static const Frame* sliceArgTypes[] = { &Number::instance() };

		setSlot( Symbol::at( "each:" ), *new Method( "each:", *new Block( each_ ),  1, mapArgTypes ) );
	}

}

#endif

