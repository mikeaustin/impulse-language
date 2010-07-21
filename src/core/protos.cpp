//
// core/protos/range.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

namespace impulse {

 //
 // class Range
 //

	Range::Range( Frame& proto ) : Frame( proto ) { }
	
	void Range::initSlots()
	{
		static const Frame* mapArgTypes[] = { &Lambda::instance() };

		setSlot( Symbol::at( "size" ),  *new Method( "size",  size,  0 ) );
		setSlot( Symbol::at( "map:" ),  *new Method( "map:",  map_,  1, mapArgTypes ) );
		setSlot( Symbol::at( "each:" ), *new Method( "each:", each_, 1, mapArgTypes ) );
	}

	Value Range::each_( Value receiver, const Array& args, Value context )
	{
		Range& range  = receiver.get<Range>();
		Lambda& block = args[0].get<Lambda>();

		Array blockArgs( 1 );

		for (int i = range._from; i <= range._to; ++i)
		{
			blockArgs[0] = i;
		
			Value value = block.eval_( receiver, blockArgs, context );
		}

		return Value();
	}

	Value Range::map_( Value receiver, const Array& args, Value context )
	{
		Range& range  = receiver.get<Range>();
		Lambda& block = args[0].get<Lambda>();

		Array blockArgs( 1 );
		GCArray& array = *new ArrayValue();

		for (int i = range._from; i <= range._to; ++i)
		{
			blockArgs[0] = i;
		
			Value value = block.eval_( receiver, blockArgs, context );
			array._array.push_back( value );
		}

		autorelease( array );
	
		return array;
	}

 //
 // class GCArray
 //

	GCArray::GCArray() { }
	GCArray::GCArray( Frame& proto ) : Frame( proto ) { }

	void GCArray::initSlots()
	{
		static const Frame* mapArgTypes[] = { &Lambda::instance() };
	
		setSlot( Symbol::at( "size" ), *new Method( "size", size_,  0 ) );
		setSlot( Symbol::at( "[]" ),   *new Method( "[]",   slice_, 1 ) );
		setSlot( Symbol::at( "map:" ), *new Method( "map:", map_,   1, mapArgTypes ) );
	}

	inline Frame& GCArray::instance()
	{
		static GCValue array = *new GCArray( Object::instance() );
			
		return array.getFrame();
	}

	Value GCArray::map_( Value receiver, const Array& args, Value context )
	{
		GCArray& self  = receiver.get<GCArray>();
		Lambda& block = args[0].get<Lambda>();

		Array blockArgs( 1 );
		GCArray& array = *new ArrayValue();

		vector<GCValue>::iterator iter = self._array.begin();
		
		while (iter != self._array.end())
		{
			blockArgs[0] = *iter;
			
			Value value = block.eval_( receiver, blockArgs, context );
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

}

