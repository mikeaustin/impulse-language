//
// core/protos/range.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_RANGE
#define IMPULSE_RANGE

#include "object.h"

namespace impulse {

 //
 // class Range
 //

	template <typename T>
	class range {
	 public:
		class range_iterator : public iterator<random_access_iterator_tag, T>
		{
		 public:
		 	range_iterator() { }
		 	range_iterator( T value ) : _value( value ) { }
		 	
		 	const T& operator *() const { return _value; }
			range_iterator& operator ++() { return _value++, *this; }
			range_iterator& operator ++(int) { T value = _value; return ++_value, value; }
			bool operator ==(range_iterator const& other) const { return _value == *other; }
			bool operator !=(range_iterator const& other) const { return _value != *other; }
	
		 private:
			T _value;
		};

		typedef range_iterator iterator;

		range( T from, T to ) : _from( from ), _to( to ) { }

		range_iterator begin() { return range_iterator( _from ); }
		range_iterator end() { return range_iterator( _to + 1 ); }
		
	 private:
		T _from, _to;
	};

	class Range : public Frame {

	 public:

		Range( Frame& proto );
		Range( int from, int to ) : Frame( Range::instance() ), _from( from ), _to( to ) { }

		void initSlots();

		int from() { return _from; }
		int to() { return _to; }

		virtual string inspect( const Value receiver ) const
		{
			Range& range = (Range&) receiver.getFrame();
			
			if (&range == &Range::instance())
				return "<range>";
				
			ostringstream stream;

			stream << range._from << ".." << range._to;
			
			return stream.str();

		}

		static Frame& instance()
		{
			static GCValue range = *new Range( Object::instance() );
			
			return range.getFrame();
		}

		static Value size( Value receiver, const Array& args, Value context )
		{
			Range& self = (Range&) receiver.getFrame();
			
			return self._to - self._from + 1;
		}

		static Value each_( Value receiver, const Array& args, Value context );
		static Value map_( Value receiver, const Array& args, Value context );

	 private:
	
		int _from, _to;
		
	};

}

#endif

