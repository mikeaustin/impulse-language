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

	inline void *Frame::operator new( std::size_t size )
	{
		Frame* frame = (Frame *) ::operator new( size );

		if (_releasePoolStack.size() > 0)
			_releasePoolStack.back().push_back( frame );
		else
			TRACE( "No release pool available." );
		
		return frame;
	}

	inline Frame::Frame()               : _protoFrame( NULL ), _publicSlots( NULL ), _referenceCount( 1 ) { }

 	inline Frame::Frame( Frame& proto ) : _protoFrame( &proto ), _publicSlots( NULL ), _referenceCount( 1 )
 	{
 		getProto().incrementReference();
 	}

	inline Frame::~Frame()
	{
		if ( _protoFrame ) getProto().decrementReference();
		if ( _publicSlots ) delete _publicSlots;
	}

	inline string Frame::inspect( const Value receiver ) const
	{
		std::stringstream stream;
		
		stream << "<frame@" << this << ">";
		
		return stream.str();
	}

	inline Value Frame::setSlot( const Symbol symbol, const Value value )
	{
		// TODO: Why doesn't insert work the same as []?
		//_slots.at( symbol.getId() ) = value;
		getSlots()[symbol.getId()] = value;
		//_slots.insert( _slots.find( symbol.getId() ), std::make_pair(symbol.getId(), value) );
		//getSlots().insert( std::make_pair( symbol.getId(), value ) );

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

	inline void Frame::incrementReference()
	{
		//if (debugGarbage) TRACE( "\t\t\t\t\t\t\t\t+ " << inspect( *this ) );
		TRACE( "+ " << inspect( *this ) );
	
		++_referenceCount;
	}

	inline void Frame::decrementReference()
	{
		--_referenceCount;

		//if (debugGarbage) TRACE( "\t\t\t\t\t\t\t\t- " << inspect( *this ) << (_refCount == 0 ? " free" : "") );
		TRACE( "- " << inspect( *this ) );

		if (_referenceCount < 0) TRACE( "deleting freed object " << inspect( *this ) );

		if (_referenceCount == 0)
		{
			//if (debugGarbage) TRACE( "freeing " << inspect( *this ) );

			TRACE( "freeing " << inspect( *this ) );
			
			delete this;
		}
	}

	std::vector< std::vector<Frame*> > Frame::_releasePoolStack;

 //
 // ReleasePool
 //
 
	inline Frame::ReleasePool::ReleasePool()
	{
		TRACE( "Pushing new release pool..." );
		
		Frame::_releasePoolStack.push_back( std::vector<Frame*>() );
	}

	inline Frame::ReleasePool::~ReleasePool()
	{
		TRACE( "Poping old release pool..." );
		
		std::vector<Frame*> pool = _releasePoolStack.back();
		std::vector<Frame*>::iterator iter = pool.begin();
		
		while (iter != pool.end())
		{
			(*iter)->decrementReference();
		
			iter = pool.erase( iter );
		}
		
		_releasePoolStack.pop_back();
	}

}

