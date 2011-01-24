//
// core/protos/array.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_ARRAY
#define IMPULSE_ARRAY

#include "../traits/enumerable.h"

namespace impulse {

 //
 // class GCArray
 //

	class GCArray : public Enumerable, public Frame {

	 public:

		class Iterator : public Enumerable::Iterator {
		
		 public:

			Iterator( vector<GCValue>& sequence )
			 : _sequence( sequence ), _iterator( sequence.begin() ) { }
		 
			virtual bool hasNext() { return _iterator != _sequence.end(); };
			virtual Value getValue() { return *_iterator++; }

		 private:

			vector<GCValue>&          _sequence;
			vector<GCValue>::iterator _iterator;
			
		};

		GCArray();
		GCArray( size_t size );
		GCArray( const Array& value );
		GCArray( Frame& proto, size_t size );

		void initSlots();

		unsigned int size() const { return _array.size(); }

		void push_back( Value value ) { _array.push_back( value ); }

		Value& operator []( size_t index ) { return _array[index]; }
		const Value& operator []( size_t index ) const { return _array[index]; }

		virtual Iterator& iterator()
		{
			return *new Iterator( _array );
		}

		string inspect( const Value receiver ) const
		{
			return "<array>";
		}

		static Frame& instance();

		static Value size_( Value receiver, const Array& args, Value context )
		{
			GCArray& self = receiver.get<GCArray>();
			
			return self._array.size();
		}

		static Value slice_( Value receiver, const Array& args, Value context )
		{
			GCArray& self  = receiver.get<GCArray>();
			unsigned index = (unsigned) args[0].getFloat() - 1;
			
			if (index >= 0 && index < self._array.size())
			{
				return self._array[index];
			}
			
			return Value();
		}

		static Value each_( Value receiver, const Array& args, Value context );
		static Value map_( Value receiver, const Array& args, Value context );
		static Value zip_( Value receiver, const Array& args, Value context );

//	 private:

		vector<GCValue> _array;
	 
	};

	class ArrayValue : public GCArray {
	
	 public:
	 
	 	ArrayValue();
		ArrayValue( size_t size );
	 	ArrayValue( const Array& value );

		string inspect( const Value receiver ) const
		{
			GCArray& array = receiver.get<GCArray>();
			ostringstream stream;

			stream << "[";
			
			for (unsigned int i = 0; i < array._array.size() && i < 10; i++)
			{
				stream << Value( array._array[i] ).inspect();
				
				if (i < array._array.size() - 1) stream << ", ";
			}
			
			if (array._array.size() > 10) stream << "...";
			
			stream << "]";
			
			return stream.str();
		}
	
	};

}

#endif

