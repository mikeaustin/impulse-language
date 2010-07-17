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
		
		void initSlots()
		{
			setSlot( Symbol::at( "proto" ), *new Method( proto, 0 ) );
			setSlot( Symbol::at( "clone" ), *new Method( clone, 0 ) );
		}

		static Frame& instance()
		{
			static GCValue object = *new Object;
			//static Object object2;
			
			return object.getFrame();
			//return object2;
		}

		static Value proto( Value receiver, const Array& args )
		{
			return receiver.getProto();
		}

		static Value clone( Value receiver, const Array& args )
		{
			return receiver.clone();
		}

		virtual string inspect( Value receiver ) const { return "<bject>"; }

	};

}

#endif

