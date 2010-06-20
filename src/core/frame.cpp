//
// frame.cpp
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include "frame.h"

namespace impulse {

 //
 // class Frame
 //

	inline Value Frame::setSlot( SymbolId symbolId, const Value value )
	{
		ENTER( "Frame::setSlot()" );

		//_slots[&name] = value;
		_slots[symbolId] = value;
		
		LEAVE( "" );
		
		return value;
	}

	inline Value Frame::getSlot( SymbolId symbolId )
	{
		if (_slots.find( symbolId ) != _slots.end())
		{
			return _slots[symbolId];
		}

		return Void::instance();
	}

	inline Value Frame::eval( Value receiver, const Array& args, Value context )
	{
		ENTER( "Frame::eval( receiver = " << receiver.toString() << " )" );
		
		LEAVE( "" );
		
		return *this;
	}

	inline Value Frame::eval2( Frame* self, Value receiver, const Array& args, Value context )
	{
		ENTER( "Frame::eval( receiver = " << receiver.toString() << " )" );
		
		LEAVE( "" );
		
		return *self;
	}

	inline Value Frame::send( Value receiver, const SymbolId selectorId, const Array& args, Value context )
	{
		//ENTER( "Frame::send( receiver = " << receiver.toString() << ", selector = "
		//       << Value((Symbol&) selector).toString() << " )" );

		Frame* frame = this;

		while (frame)
		{
			SlotMap::iterator iter = frame->_slots.find( selectorId );

			if (iter != _slots.end())
			{
				Value value = iter->second;

				LEAVE( "" );

				return value.eval( receiver, args, context );
			}
			
			frame = frame->_proto;
		}

		cout << "Slot not found" << endl;

		LEAVE( "" );

		return Void::instance();
	}

	inline void Frame::incRef()
	{
		cout << "                                                  + " << toString( *this ) << endl;

		++_refCount;
	}

	inline void Frame::decRef()
	{
		--_refCount;

		cout << "                                                  - " << toString( *this );
		
		if (_refCount == 0)
		{
			cout << "\r\t\t\t\t\t\t\t\tdeleting";

			cout << endl;
			
			delete this;
		}
		else cout << endl;
	}

}

