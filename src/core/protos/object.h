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
 // class Future
 //

	class Future : public Frame {

	 public:

		Future( Frame& proto ) : Frame( proto ) { }
		Future( Block& block, Value receiver );

		void initSlots();

		static Frame& instance();

		virtual string inspect( const Value receiver ) const { return "<future>"; }

		static Value value( Value receiver, const Array& args, Value context );
		
		GCValue _receiver;
		GCValue _block;
		GCValue _result;

	};

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

		static Value async_( Value receiver, const Array& args, Value context )
		{
			return autorelease( *new Future( args[0].get<Block>(), receiver ) );
		}

		static Value method_( Value receiver, const Array& args, Value context );

		static Value methods( Value receiver, const Array& args, Value context )
		{
			Object& self = receiver.get<Object>();
			GCArray& array = *new ArrayValue();
			
			SlotMap::iterator iter = self._publicSlots.begin();
			for (; iter != self._publicSlots.end(); ++iter)
			{
				array.push_back((*iter).second);
				
				if (&self.getProto())
				{
					//method( self.getProto(), args, context );
				}
			}

			return autorelease( array );
		}

		virtual string inspect( const Value receiver ) const { return "<object>"; }

	};

}

#endif

