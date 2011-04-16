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

using std::map;

#include "core/value.h"

namespace impulse {

	class Frame {

	 public:

	 //
	 // Typedefs
	 //

		typedef unsigned short SymbolId;
		typedef map<const SymbolId, GCValue> SlotMap;

	 //
	 // Constructors
	 //
	
		void *operator new( size_t size );

		static Frame& create();
		static Frame& create( Frame& proto );
	
		virtual ~Frame();

	 //
	 // Slot Access
	 //

		Value setSlot( const Symbol symbol, const Value value );
		Value getSlot( const Symbol symbol );

		Value setSlot( const string name, const Value value );
		Value getSlot( const string name );

		Frame&   getProto() { return *_protoFrame; }
		SlotMap& getSlots() { if (_publicSlots == NULL) _publicSlots = new SlotMap();
							  return *_publicSlots; }

	 //
	 // Messaging
	 //

		virtual Value apply( Value receiver, const Array& args, Value locals ) const { return receiver; }

		Value perform( Symbol selector, const Array& args, Value locals ) { return perform( *this, selector, args, locals ); }
		Value perform( Value receiver, const Symbol selector, const Array& args, Value locals );

	 //
	 // Inspection
	 //

		virtual string inspect( const Value self ) const;
		string inspect( const Value self, const string name ) const;

	 //
	 // Garbage Collection
	 //

		long referenceCount() { return _referenceCount; }

		void incrementReference();
		void decrementReference();

		class ReleasePool {
		
		 public:

		 //
		 // Constructors
		 //
		 
			ReleasePool();
			~ReleasePool();

		 //
		 // Methods
		 //

			long size()  { return _releasePoolStack.back().size(); }
			long depth() { return _releasePoolStack.size(); }
			
		};

	 protected:
	 
 		Frame();
	 	Frame( Frame& proto );

	 private:

		Frame*   _protoFrame;
		SlotMap* _publicSlots;
		short    _referenceCount;

		static vector< vector<Frame*> > _releasePoolStack;

	};
	
}

#endif

