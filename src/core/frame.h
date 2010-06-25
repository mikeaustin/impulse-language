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

	class Frame {

	 public:

	 	Frame()               : _proto( NULL ), _refCount( 0 ) { _eval2 = &eval2; }
	 	Frame( Frame& proto ) : _proto( &proto ), _refCount( 0 ) { }

		Value setSlot( const SymbolId symbolId, const Value value );
		Value getSlot( const SymbolId symbolId );

		Value setSlot( const Symbol& symbol, const Value value );
		Value getSlot( const Symbol& symbol );

		virtual Value eval( Value receiver, const Array& args, Value context );
		static Value eval2( Frame* self, Value receiver, const Array& args, Value context );
		Value (*_eval2)( Frame* self, Value receiver, const Array& args, Value context );
		
		Value send( Value receiver, const SymbolId selectorId, const Array& args, Value context );

		virtual string toString( Value receiver ) const { return "[Frame]"; }

		Frame& getProto() const { return *_proto; }

		void incRef();
		void decRef();

//	 private:

		SlotMap _slots;
		Frame*  _proto;
		int     _refCount;

	};

 //
 // class Nil
 //

	class Nil : public Frame {

	 public:

		static Frame& instance()
		{
			static Nil nil;

			return nil;
		}

		virtual string toString( Value receiver ) const { return "[Nil]"; }

	};

 //
 // class Void
 //

	class Void : public Frame {

	 public:

		static Frame& instance()
		{
			static Void void_;

			return void_;
		}

		virtual string toString( Value receiver ) const { return "[Void]"; }

	};

}

#endif

