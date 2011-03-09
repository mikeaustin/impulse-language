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
	typedef std::map<const SymbolId, GCValue> SlotMap;

	class Frame {

	 public:
	
		void *operator new( std::size_t size );
	
		Frame();
	 	Frame( Frame& proto );
		~Frame();

		Value setSlot( const Symbol symbol, const Value value );
		Value getSlot( const Symbol symbol );

		Value setSlot( const string name, const Value value );
		Value getSlot( const string name );

		Frame&   getProto() { return *_protoFrame; }
		SlotMap& getSlots() { if (_publicSlots == NULL) _publicSlots = new SlotMap();
							  return *_publicSlots; }

		virtual Value apply( Value receiver, const Array& args ) { return receiver; }
		
		Value perform( Symbol selector, Array& args ) { return Value(); }

		virtual string inspect( const Value self ) const;

	 //
	 // Garbage Collection
	 //

		long referenceCount() { return _referenceCount; }

		void incrementReference();
		void decrementReference();

		class ReleasePool {
		
		 public:
		 
			ReleasePool();
			~ReleasePool();

			long size()  { return _releasePoolStack.back().size(); }
			long depth() { return _releasePoolStack.size(); }
			
		};

	 private:

		Frame*   _protoFrame;
		SlotMap* _publicSlots;
		short    _referenceCount;

		static std::vector< std::vector<Frame*> > _releasePoolStack;

	};
	
}

#endif

