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

	class Range : public Frame {

	 public:

		Range( Frame& proto );
		Range( int from, int to ) : Frame( Range::instance() ), _from( from ), _to( to ) { }

		void initSlots();

		virtual string inspect( Value receiver ) const
		{
			Range& range = (Range&) receiver.getFrame();
			
			//if (&range == &Range::instance())
			//	return "<range>";
				
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

