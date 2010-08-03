//
// core/frame.cpp
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include "frame.h"

namespace impulse {

 //
 // class Frame
 //

	inline Value Frame::setSlot( const Symbol& symbol, const Value value )
	{
		_publicSlots[symbol.getId()] = value;
		
		return value;
	}

	inline Value Frame::getSlot( const Symbol& symbol )
	{
		SlotMap::iterator iter = _publicSlots.find( symbol.getId() );
		
		if (iter != _publicSlots.end())
		{
			return iter->second;
		}

		return Value();
	}

	inline Value Frame::eval( Value receiver, const Array& args, Value context )
	{
		ENTER( "Frame::eval( receiver = " << receiver.inspect() << " )" );
		
		LEAVE( inspect( *this ) );
		
		return *this;
	}

	inline Value Frame::send( Value receiver, const Symbol& selector, const Array& args, Value context )
	{
		ENTER( "Frame::send( this = "     << inspect( *this ) << ","
						   " receiver = " << receiver.inspect() << ","
						   " selector = " << selector.inspect( const_cast<Symbol&>( selector ) ) << ","
						   " args = "     << args.inspect() << " )" );


		if (methodCaching && selector.getId() == _cache[0].selectorId)
		{
			return Value( _cache[0].value ).eval( receiver, args, context );
		}
//cout << selector.inspect( const_cast<Symbol&>( selector ) ) << endl;

		Frame* frame = this;

		while (frame)
		{
			SlotMap::iterator iter = frame->_publicSlots.find( selector.getId() );

			if (iter != frame->_publicSlots.end())
			{
				Value value = iter->second;

				if (methodCaching)
				{
					static unsigned index = 0;

					_cache[index].selectorId = selector.getId();
					_cache[index].value      = value;

					//index = (index + 1) & 0x01;
				}

				Value result = value.eval( receiver, args, context );

				LEAVE( result.inspect() );
				
				return result;
			}

			frame = frame->_proto;
		}

		cout << "*** Slot not found: " << receiver.inspect() << "." << selector.getName() << endl;

		LEAVE( "" );

		return Value();
	}

	inline void Frame::incRef()
	{
		//if (debugGarbage) TRACE( "\t\t\t\t\t\t\t\t+ " << inspect( *this ) );
		//if (debugGarbage) TRACE( "+ " << inspect( *this ) );
		
		++_refCount;
	}

	inline void Frame::decRef()
	{
		--_refCount;

		//if (debugGarbage) TRACE( "\t\t\t\t\t\t\t\t- " << inspect( *this ) << (_refCount == 0 ? " free" : "") );
		//if (debugGarbage) TRACE( "- " << inspect( *this ) );

		//if (_refCount < 0) TRACE( "deleting freed object " << inspect( *this ) );

		if (_refCount == 0)
		{
			if (debugGarbage) TRACE( "freeing " << inspect( *this ) );

			delete this;
		}
	}

	vector<Frame*> Frame::_releasePool;

 //
 // class Array
 //
/*
	inline Array::Array( const Array& value )
	{
		_array[0] = value._array[0];
		_array[1] = value._array[1];
		_array[2] = value._array[2];
		_array[3] = value._array[3];
		_array[4] = value._array[4];
			
		_size = value._size;
	}
*/
}

