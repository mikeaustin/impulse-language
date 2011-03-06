//
// core/frame.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_FRAME_H
#define IMPULSE_FRAME_H

#include <sstream>
#include <vector>
#include <map>

#include "core/value.h"

namespace impulse {

	typedef unsigned short SymbolId;
	typedef std::map<const SymbolId, Value> SlotMap;

	class Frame {

	 public:
	
		void *operator new( std::size_t size )
		{
			Frame* frame = (Frame *) ::operator new( size );

			if (_releasePoolStack.size() > 0)
				_releasePoolStack.back().push_back( frame );
			else
				std::cerr << "No release pool available." << std::endl;
			
			return frame;
		}
	
		Frame()               : _proto( NULL ), _slots( NULL ), _count( 1 ) { }
	 	Frame( Frame& proto ) : _proto( &proto ), _slots( NULL ), _count( 1 ) { }

		Value setSlot( const Symbol symbol, const Value value );
		Value getSlot( const Symbol symbol );

		Value setSlot( const string name, const Value value );
		Value getSlot( const string name );

		Frame& getProto() { return *_proto; }

		SlotMap& getSlots() { if (!_slots) _slots = new SlotMap(); return *_slots; }

		virtual string inspect( const Value receiver ) const
		{
			std::stringstream stream;
			
			stream << "<frame@" << this << ">";
			
			return stream.str();
		}

		void incRef();
		void decRef();

		class ReleasePool {
		
		 public:
		 
			ReleasePool()
			{
				std::cout << "Pushing new release pool..." << std::endl;
				
				Frame::_releasePoolStack.push_back( std::vector<Frame*>() );
			}

			~ReleasePool()
			{
				std::cout << "Poping old release pool..." << std::endl;
				
				std::vector<Frame*> pool = _releasePoolStack.back();
				std::vector<Frame*>::iterator iter = pool.begin();
				
				while (iter != pool.end())
				{
					(*iter)->decRef();
				
					iter = pool.erase( iter );
				}
				
				_releasePoolStack.pop_back();
			}

			long size()
			{
				return _releasePoolStack.back().size();
			}
			
		};

	 private:

		// TODO: Change this to a GCValue
		Frame*   _proto;
		SlotMap* _slots;
		short    _count;

		static std::vector< std::vector<Frame*> > _releasePoolStack;

		//long padding[1024];

	};
	
}

#endif

