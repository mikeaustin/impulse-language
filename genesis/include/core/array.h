//
// core/array.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_ARRAY_H
#define IMPULSE_ARRAY_H

namespace impulse {

 //
 // class Index
 //

	class Index {

	 public:

		const size_t index() const { return _index; }

		static const Index _0;
		static const Index _1;
		static const Index _2;
		static const Index _3;
		static const Index _4;
		
	 private:

		Index( const size_t index ) : _index( index ) { }
	 
		const size_t _index;

	};

	const Index Index::_0( 0 );
	const Index Index::_1( 1 );
	const Index Index::_2( 2 );
	const Index Index::_3( 3 );
	const Index Index::_4( 4 );

	// Experimental
	//template <size_t index>
	//class Index : public Index<index - 1> { };

 //
 // class Array
 //

	class Array {

		static const size_t maxSize = 5;

	 public:

		Array()              : _size( 0 ) { }
		Array( size_t size ) : _size( size ) { }

		void push_back( Value value ) { _array[_size] = value; if (_size < maxSize) ++_size; }
		void pop_back() { if (_size > 0) _size--; }

		size_t size() const { return _size; }

		Value& operator []( Index index ) { return (Value&) _array[index.index()]; }
		const Value& operator []( Index index ) const { return (Value&) _array[index.index()]; }
 
	 private:

		size_t _size;
		Atom   _array[maxSize];

	};

}

#endif

