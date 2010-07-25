//
// core/traits/invokable.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_INVOKABLE
#define IMPULSE_INVOKABLE

namespace impulse {

 //
 // class Invokable
 //

	class Invokable : public Frame {
	
	 public:

		Invokable()               : Frame() { }
		Invokable( Frame& proto ) : Frame( proto ) { }

		virtual Value eval_( Value receiver, const Array& args, Value context ) = 0;

	};

}

#endif

