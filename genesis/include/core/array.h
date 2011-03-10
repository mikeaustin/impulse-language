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

		Array() : _size( 0 ) { }
		Array( Value arg0 ) : _size( 1 ), _0( arg0 ) { }
		Array( Value arg0, Value arg1 ) : _size( 1 ), _0( arg0 ), _1( arg1 ) { }

		size_t size() const { return _size; }

		Value& operator []( Index index ) { return (Value&) *(&_0 + index.index()); }
		const Value& operator []( Index index ) const { return (Value&) *(&_0 + index.index()); }

		virtual string inspect( const Value self ) const
		{
			return "<args>";
		}
 
	 private:

		size_t _size;
		Atom _0, _1, _2, _3, _4;
		
	};

	class ArrayProto : public Frame {
	
	 public:
	 
		//ArrayProto( Value arg0 ) : _vector( 1 ) { _vector[0] = arg0; }
		//ArrayProto( Value arg0, Value arg1 ) : _vector( 2 ) { _vector[0] = arg0; _vector[1] = arg1; }
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
	 
		std::vector<GCValue> _vector;
	
	};

}

#endif

