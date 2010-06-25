//
// core/protos/object.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_OBJECT_H
#define IMPULSE_OBJECT_H

namespace impulse {

 //
 // class Object
 //

	class Object : public Frame {

	 public:

		Object() { }

		static Frame& instance()
		{
			static GCValue object = *new Object;
			//static Object object2;
			
			return object.getFrame();
			//return object2;
		}

		virtual string toString( Value receiver ) const { return "[Object]"; }

	};

}

#endif

