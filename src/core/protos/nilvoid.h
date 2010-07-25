//
// core/protos/nilvoid.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_NILVOID_H
#define IMPULSE_NILVOID_H

namespace impulse {

 //
 // class Nil
 //

	class Nil : public Frame {

	 public:

		static Frame& instance()
		{
			static Nil nil;

			return nil;
		}

		virtual string inspect( const Value receiver ) const { return "nil"; }

	};

 //
 // class Void
 //

	class Void : public Frame {

	 public:

		static Frame& instance()
		{
			static Void void_;

			return void_;
		}

		virtual string inspect( const Value receiver ) const { return "void"; }

	};

}

#endif

