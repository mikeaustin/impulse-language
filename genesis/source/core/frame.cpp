//
// core/frame.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#include "core/frame.h"
#include "core/protos/symbol.h"

namespace impulse {

 //
 // class Frame
 //

	// TODO: Is returning void faster?
	inline Value Frame::setSlot( const Symbol symbol, const Value value )
	{
		//_slots.at( symbol.getId() ) = value;
		//_slots[symbol.getId()] = value;
		//_slots.insert( _slots.find( symbol.getId() ), std::make_pair(symbol.getId(), value) );
		getSlots().insert( std::make_pair( symbol.getId(), value ) );

		return value;
	}

	inline Value Frame::getSlot( const Symbol symbol )
	{
		SlotMap::iterator iter = getSlots().find( symbol.getId() );
		
		if (iter != getSlots().end())
		{
			return iter->second;
		}

		return Value();
	}

	inline Value Frame::setSlot( const string name, const Value value )
	{
		return setSlot( SymbolProto::at( name ), value );
	}
	
	inline Value Frame::getSlot( const string name )
	{
		return getSlot( SymbolProto::at( name ) );
	}

	inline void Frame::incRef()
	{
		//if (debugGarbage) TRACE( "\t\t\t\t\t\t\t\t+ " << inspect( *this ) );
		//if (debugGarbage) TRACE( "+ " << inspect( *this ) );
	
		++_count;
	}

	inline void Frame::decRef()
	{
		--_count;

		//if (debugGarbage) TRACE( "\t\t\t\t\t\t\t\t- " << inspect( *this ) << (_refCount == 0 ? " free" : "") );
		//if (debugGarbage) TRACE( "- " << inspect( *this ) );

		if (_count < 0) TRACE( "deleting freed object " << inspect( *this ) );

		if (_count == 0)
		{
			//if (debugGarbage) TRACE( "freeing " << inspect( *this ) );

			TRACE( "freeing " << inspect( *this ) );
			
			delete this;
		}
	}

	std::vector< std::vector<Frame*> > Frame::_releasePoolStack;

}

