//
// core/traits/enumerable.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_ENUMERABLE
#define IMPULSE_ENUMERABLE

namespace impulse {

 //
 // class Enumerable
 //

	class Enumerable {
	
	 public:

		virtual Value map_( Value receiver, const Array& args, Value context );

		virtual Value each_( Value receiver, const Array& args, Value context ) = 0;

	};

}

#endif

