//
// core/protos/array.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

//#include "object.h"
#include <sstream>

namespace impulse {

 //
 // class Array
 //

	class Array : public Frame {
	
	 public:

		Array( const Array& value );
		Array( Frame& proto );
		Array()              : _size( 0 ) { }
		Array( size_t size ) : _size( size ) { }

		void initSlots();

		unsigned int size() const { return _size; }

		void push_back( Value value ) { _array[_size++] = value; }

		Value& operator []( size_t index ) { return (Value&) _array[index]; }
		const Value& operator []( size_t index ) const { return (Value&) _array[index]; }

		string inspect( Value receiver ) const
		{
			return "<array>";
		}

		static Frame& instance();

		string inspect() const
		{
			ostringstream stream;

			stream << "[";
			
			for (unsigned int i = 0; i < _size; i++)
			{
				stream << Value( _array[i] ).inspect();
				
				if (i < _size - 1) stream << ", ";
			}
			
			stream << "]";
			
			return stream.str();
		}

		static Value size_( Value receiver, const Array& args )
		{
			Array& self = receiver.get<Array>();
			
			return self._size;
		}

//	 private:

		Atom   _array[5];
		size_t _size;
	 
	};

	class GCArray : public Frame {

		static const Frame* mapArgTypes[];
	
	 public:

		GCArray();
		GCArray( const Array& value );
		GCArray( Frame& proto );
		//GCArray()              : _size( 0 ) { }
		//GCArray( size_t size ) : _size( size ) { }

		void initSlots();

		unsigned int size() const { return _array.size(); }

		void push_back( Value value ) { _array.push_back( value ); }

		Value& operator []( size_t index ) { return _array[index]; }
		const Value& operator []( size_t index ) const { return _array[index]; }

		string inspect( Value receiver ) const
		{
			return "<array>";
		}

		static Frame& instance();

		static Value size_( Value receiver, const Array& args )
		{
			GCArray& self = receiver.get<GCArray>();
			
			return self._array.size();
		}

		static Value slice_( Value receiver, const Array& args )
		{
			GCArray& self  = receiver.get<GCArray>();
			unsigned index = args[0].getFloat() - 1;
			
			if (index >= 0 && index < self._array.size())
			{
				return self._array[index];
			}
			
			return Value();
		}

		static Value map_( Value receiver, const Array& args );

//	 private:

		vector<GCValue> _array;
		//GCValue _array[5];
		//size_t  _size;
	 
	};

	class ArrayValue : public GCArray {
	
	 public:
	 
	 	ArrayValue();
	 	ArrayValue( const Array& value );

		string inspect( Value receiver ) const
		{
			GCArray& array = receiver.get<GCArray>();
			ostringstream stream;

			stream << "[";
			
			for (unsigned int i = 0; i < array._array.size(); i++)
			{
				stream << Value( array._array[i] ).inspect();
				
				if (i < array._array.size() - 1) stream << ", ";
			}
			
			stream << "]";
			
			return stream.str();
		}
	
	};

}

