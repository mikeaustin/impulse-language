//
// core/protos/range.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_RANGE
#define IMPULSE_RANGE

#include "object.h"
#include "../traits/enumerable.h"

namespace impulse {

 //
 // class Range
 //
 
	class Range : public Enumerable, public Frame {

	 public:

		class Iterator : public Enumerable::Iterator {
		
		 public:

			Iterator( int from, int to )
			 : _from( from ), _to( to ), _i( from ) { }
		 
			virtual bool hasNext() { return _i <= _to; };
			virtual Value getValue() { if (_i <= _to) return _i++; return 0; }

		 private:

			int _from, _to;
			int _i;
			
		};

		Range( Frame& proto );
		Range( int from, int to ) : Frame( Range::instance() ), _from( from ), _to( to ) { }

		void initSlots();

		int from() { return _from; }
		int to() { return _to; }

		virtual Iterator& iterator()
		{
			return *new Iterator( _from, _to );
		}

		virtual string inspect( const Value receiver ) const
		{
			//Range& range = (Range&) receiver.getFrame();
			Range& self = (Range&) receiver.get<Range>();
			
			if (&self == &Range::instance())
				return "<range>";
				
			ostringstream stream;

			stream << self._from << ".." << self._to;
			
			return stream.str();

		}

		static Frame& instance()
		{
			static GCValue range = *new Range( Object::instance() );
			
			return range.getFrame();
		}

		static Value size( Value receiver, const Array& args, Value context )
		{
			//Range& self = (Range&) receiver.getFrame();
			Range& self = (Range&) receiver.get<Range>();
			
			return self._to - self._from + 1;
		}

		static Value each_( Value receiver, const Array& args, Value context );
		static Value map_( Value receiver, const Array& args, Value context );

	 private:
	
		int _from, _to;
		
	};

}

#endif

