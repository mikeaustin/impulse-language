//
// core/frame.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_FRAME_H
#define IMPULSE_FRAME_H

#include <sstream>
#include <map>

using namespace std;

namespace impulse {

	typedef map<const SymbolId, GCValue> SlotMap;

 //
 // class Array
 //

	class Array {
	
	 public:

		Array()              : _size( 0 ) { }
		Array( size_t size ) : _size( size ) { }

		unsigned int size() const { return _size; }

		void push_back( Value value ) { _array[_size++] = value; }

		void clear() { _size = 0; }

		Value& operator []( size_t index ) { return (Value&) _array[index]; }
		const Value& operator []( size_t index ) const { return (Value&) _array[index]; }

		string inspect( const Value receiver ) const
		{
			return "<array>";
		}

		string inspect() const
		{
			ostringstream stream;

			stream << "[";
			
			for (unsigned int i = 0; i < _size; i++)
			{
				stream << Value( _array[i] ).inspect();
				
				if (i < _size - 1) stream << ", ";
			}
			
			stream << "]";
			
			return stream.str();
		}

	 private:

		Atom   _array[5];
		size_t _size;
	 
	};

 //
 // class Frame
 //

	typedef Value (*EvalFunc)( Frame& self, Value receiver, const Array& args, Value context );

	class Frame {

	 public:

	 	Frame()               : _proto( NULL ), _refCount( 1 ) { }
	 	Frame( Frame& proto ) : _proto( &proto ), _refCount( 1 ) { _proto->incRef(); }

		virtual ~Frame() { if (_proto) _proto->decRef(); }

		virtual void initSlots() { }

		Value setSlot( const Symbol& symbol, const Value value );
		Value getSlot( const Symbol& symbol );

		virtual Value eval( Value receiver, const Array& args, Value context );
		Value send( Value receiver, const Symbol& selector, const Array& args, Value context );

		virtual string inspect( const Value receiver ) const
		{
			stringstream stream;
			
			stream << "<frame@" << this << ">";
			
			return stream.str();
		}

		Frame& getProto() const { return *_proto; }

		static void autorelease( Frame& frame ) { _releasePool.push_back( &frame ); }

		void incRef();
		void decRef();

		struct Cache {

		 	Cache() : selectorId( -1 ) { }
		 	
			SymbolId selectorId;
			Atom	 value;

		};

//	 private:

		SlotMap  _publicSlots;
		Frame*   _proto;
		short    _refCount;
		Value  (*_eval3)( Frame*, const int, Value, const Array&, Value );
		Cache    _cache[1];
		vector<Value> _locals;

		static vector<Frame*> _releasePool;

	};

}

#endif

