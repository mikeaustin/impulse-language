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
		
		void initSlots();

		static Frame& instance()
		{
			static GCValue object = *new Object;
			//static Object object2;
			
			return object.getFrame();
			//return object2;
		}

		static Value proto( Value receiver, const Array& args, Value context )
		{
			if (&receiver.getProto())
			{
				return receiver.getProto();
			}
			
			return Nil::instance();
		}

		static Value clone( Value receiver, const Array& args, Value context )
		{
			return receiver.clone();
		}

		virtual string inspect( const Value receiver ) const { return "<object>"; }

	};

}

#endif

