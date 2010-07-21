//
// core/frame.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_FRAME_H
#define IMPULSE_FRAME_H

#include <map>

using namespace std;

namespace impulse {

	typedef map<const SymbolId, GCValue> SlotMap;

 //
 // class Frame
 //

	typedef Value (*EvalFunc)( Frame* self, Value receiver, const Array& args, Value context );

	class Frame {

	 public:

	 	Frame()               : _proto( NULL ), _refCount( 1 ) { }
	 	Frame( Frame& proto ) : _proto( &proto ), _refCount( 1 ) { }

		virtual ~Frame() { }

		virtual void initSlots() { }

		Value setSlot( const Symbol& symbol, const Value value );
		Value getSlot( const Symbol& symbol );

		virtual Value eval( Value receiver, const Array& args, Value context );
		
		Value send( Value receiver, const Symbol& selector, const Array& args, Value context );

		virtual string inspect( Value receiver ) const { return "<frame>"; }

		Frame& getProto() const { return *_proto; }

		static void autorelease( Frame& frame ) { _releasePool.push_back( &frame ); }

		void incRef();
		void decRef();

		struct Cache {

		 	Cache() : selectorId( -1 ) { }
		 	
			SymbolId selectorId;
			Frame*   frame;

		};

//	 private:

		SlotMap  _publicSlots;
		Frame*   _proto;
		short    _refCount;
		Value (*_eval3)( Frame*, const int, Value, const Array&, Value );
		Cache    _cache;

		static vector<Frame*> _releasePool;

	};

}

#endif

