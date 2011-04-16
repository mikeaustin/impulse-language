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

 //
 // class Array
 //

	class Array {

		static const size_t maxSize = 5;

	 public:

		Array() : _size( 0 ) { }
		Array( Value arg0 ) : _size( 1 ), _0( arg0 ) { }
		Array( Value arg0, Value arg1 ) : _size( 1 ), _0( arg0 ), _1( arg1 ) { }

		Array& size( size_t size ) { return _size = size, *this; }
		size_t size() const { return _size; }

		Value& operator []( Index index ) { return (Value&) *(&_0 + index.index()); }
		const Value& operator []( Index index ) const { return (Value&) *(&_0 + index.index()); }

		virtual string inspect( const Value self ) const
		{
			return "<args>";
		}

		Array& self( Value self ) { return _self = self, *this; }
		Value self() const { return _self; }

	 private:

		// For efficiency, MessageProto is allowed to index the array directly

		Value& operator []( int index ) { return (Value&) *(&_0 + index); }
		const Value& operator []( int index ) const { return (Value&) *(&_0 + index); }

		friend class MessageProto;

		Value _self;
	 
		size_t _size;
		Atom _0, _1, _2, _3, _4;
		
	};

	class ArrayProto : public Frame {
	
	 public:

		ArrayProto() { }
		ArrayProto( Value arg0 ) { _vector.push_back( arg0 ); }
		ArrayProto( Value arg0, Value arg1 ) { _vector.push_back( arg0 ); _vector.push_back( arg1 ); }

		size_t size() { return _vector.size(); }

		Value& operator []( size_t index ) { return _vector[index]; }
		const Value& operator []( size_t index ) const { return _vector[index]; }

		virtual string inspect( const Value self ) const
		{
			return Frame::inspect( self, "array" );
		}
		
	 private:
	 
		vector<GCValue> _vector;
	
	};

}

#endif

