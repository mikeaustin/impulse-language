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

		return Void::instance();
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

		if (methodCaching && selector.getId() == _cache.selectorId)
		{
			return _cache.frame->eval( receiver, args, context );
		}

		Frame* frame = this;

		while (frame)
		{
			SlotMap::iterator iter = frame->_publicSlots.find( selector.getId() );

			if (iter != frame->_publicSlots.end())
			{
				Value value = iter->second;

				if (methodCaching)
				{
					_cache.selectorId = selector.getId();
					_cache.frame      = &value.getFrame();
				}

				Value result = value.eval( receiver, args, context );

				LEAVE( result.inspect() );
				
				return result;
			}
			
			frame = frame->_proto;
		}

		cout << "*** Slot not found: " << receiver.inspect() << "." << selector.inspect( const_cast<Symbol&>( selector ) ) << endl;

		LEAVE( "" );

		return Void::instance();
	}

	inline void Frame::incRef()
	{
		if (debugGarbage) TRACE( "\t\t\t\t\t\t\t\t+ " << inspect( *this ) );
		//if (debugGarbage) TRACE( "+ " << inspect( *this ) );
		
		++_refCount;
	}

	inline void Frame::decRef()
	{
		--_refCount;

		if (debugGarbage) TRACE( "\t\t\t\t\t\t\t\t- " << inspect( *this ) << (_refCount == 0 ? " free" : "") );
		//if (debugGarbage) TRACE( "- " << inspect( *this ) );

		//if (_refCount < 0) TRACE( "deleting freed object " << inspect( *this ) );

		if (_refCount == 0)
		{
			//if (debugGarbage) TRACE( "\r\t\t\t\t\t\t\t\t\t\tdeleting" );
			//if (debugGarbage) TRACE( "freeing " << inspect( *this ) );

			delete this;
		}
	}

	vector<Frame*> Frame::_releasePool;

 //
 // class Array
 //

	Array::Array( const Array& value )
	{
		_array[0] = value._array[0];
		_array[1] = value._array[1];
		_array[2] = value._array[2];
		_array[3] = value._array[3];
		_array[4] = value._array[4];
			
		_size = value._size;
	}

	Array::Array( Frame& proto ) : Frame( proto ) {	}

	void Array::initSlots()
	{
		setSlot( Symbol::at( "size" ), *new Method( size_, 0 ) );
	}

	Frame& Array::instance()
	{
		static Value array = *new Array( Object::instance() );
			
		return array.getFrame();
	}

 //
 // class GCArray
 //

	GCArray::GCArray() { }
	GCArray::GCArray( Frame& proto ) : Frame( proto ) { }

	void GCArray::initSlots()
	{
		setSlot( Symbol::at( "size" ), *new Method( size_, 0 ) );
		setSlot( Symbol::at( "[]" ), *new Method( slice_, 1 ) );
		setSlot( Symbol::at( "map:" ), *new Method( map_, 1, mapArgTypes ) );
	}

	inline Frame& GCArray::instance()
	{
		static GCValue array = *new GCArray( Object::instance() );
			
		return array.getFrame();
	}

	Value GCArray::map_( Value receiver, const Array& args )
	{
		GCArray& self  = receiver.get<GCArray>();
		Lambda& block = args[0].get<Lambda>();

		Array blockArgs( 1 );
		GCArray& array = *new ArrayValue();

		vector<GCValue>::iterator iter = self._array.begin();
		
		while (iter != self._array.end())
		{
			blockArgs[0] = *iter;
			
			Value value = block.eval_( receiver, blockArgs, receiver );
			array._array.push_back( value );
			
			++iter;
		}
		
		return array;
	}

 //
 // class ArrayValue
 //

	inline ArrayValue::ArrayValue()                     : GCArray( GCArray::instance() ) { }
	inline ArrayValue::ArrayValue( const Array& value ) : GCArray( GCArray::instance() )
 	{
 		for (unsigned int i = 0; i < value.size(); ++i)
 		{
 			_array.push_back( value[i] );
 		}
 	}

	const Frame* Number::addArgTypes[] = { &Number::instance() };
	const Frame* Number::powArgTypes[] = { &Number::instance() };
	const Frame* GCArray::mapArgTypes[] = { &Lambda::instance() };

}

