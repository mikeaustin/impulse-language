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
		static const Frame* mapArgTypes[] = { &Block::instance() };

		setSlot( Symbol::at( "size" ),  *new Method( "size",  *new Block( size ),  0 ) );
		setSlot( Symbol::at( "map:" ),  *new Method( "map:",  *new Block( map_ ),  1, mapArgTypes ) );
		setSlot( Symbol::at( "each:" ), *new Method( "each:", *new Block( each_ ), 1, mapArgTypes ) );
	}

	Value Range::each_( Value receiver, const Array& args, Value context )
	{
		Range& range  = receiver.get<Range>();
		Invokable& block = dynamic_cast<Invokable&>( args[0].getFrame() );

		Array blockArgs( 1 );
		// Create another autorelease pool

		for (int i = range._from; i <= range._to; ++i)
		{
			blockArgs[0] = i;
		
			block.eval_( receiver, blockArgs, context );

/*			vector<Frame*>::iterator iter = Frame::_releasePool.begin();
			while (iter != Frame::_releasePool.end())
			{
				(*iter)->decRef();
				
				iter = Frame::_releasePool.erase( iter );
			}*/
		}

		return Value();
	}

	Value Range::map_( Value receiver, const Array& args, Value context )
	{
		Range& range  = receiver.get<Range>();
		Block& block = args[0].get<Block>();

		Array blockArgs( 1 );
		GCArray& array = *new ArrayValue();

		for (int i = range._from; i <= range._to; ++i)
		{
			blockArgs[0] = i;
		
			Value value = block.eval_( receiver, blockArgs, context );
			
			if (&value.getFrame() != &Void::instance())
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
		static const Frame* mapArgTypes[] = { &Block::instance() };
		static const Frame* sliceArgTypes[] = { &Number::instance() };
	
		setSlot( Symbol::at( "size" ), *new Method( "size", *new Block( size_ ),  0 ) );
		setSlot( Symbol::at( "[]" ),   *new Method( "[]",   *new Block( slice_ ), 1, sliceArgTypes ) );
		setSlot( Symbol::at( "map:" ), *new Method( "map:", *new Block( map_ ),   1, mapArgTypes ) );
	}

	inline Frame& GCArray::instance()
	{
		static GCValue array = *new GCArray( Object::instance() );
			
		return array.getFrame();
	}

	Value GCArray::map_( Value receiver, const Array& args, Value context )
	{
		GCArray& self  = receiver.get<GCArray>();
		Block& block = args[0].get<Block>();

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

 //
 // class Method
 //

	Method::Method( const string funcName, Block& function, const int argsSize )
	 : _function( function ), _argsSize( argsSize ), _argTypes( NULL ) { }

	Method::Method( const string funcName, Block& function, const int argsSize, const Frame* argTypes[] )
	 : _function( function ), _argsSize( argsSize ), _argTypes( argTypes ), _funcName( funcName ) { }

	void Method::addBlock( Block& block, const Frame* argTypes[] )
	{
		_argTypes2[argTypes] = &block;
	}

	Value Method::eval( Value receiver, const Array& args, Value context )
	{
		ENTER( "Method::eval( this = " << inspect( *this ) << "," <<
							" receiver = " << receiver.inspect() << " )" );

		if (_argsSize != (unsigned int) -1 && args.size() != _argsSize)
		{
			cerr << "*** Wrong number of arguments" << endl;

			LEAVE( "" );

			return Value();
		}

		if (_argTypes)
		{
			for (unsigned int i = 0; i < _argsSize; ++i)
			{
				Frame* proto = &args[i].getProto();
				
				while (proto && proto != _argTypes[i])
				{
					proto = &proto->getProto();
				}
				
				if (proto == NULL)
				{
					cerr << "*** Invalid argument: expected " << (*_argTypes[i]).inspect( const_cast<Frame&>( *_argTypes[i] ) ) <<
												   ", found " << args[i].getProto().inspect( args[i].getProto() ) << endl;

					LEAVE( "" );

					return Value();
				}
			}
		}

		//Value result = _function( receiver, args, context );
		Value result = _function.eval_( receiver, args, context );

		LEAVE( result );
		
		return result;
	}

	void Object::initSlots()
	{
		setSlot( Symbol::at( "proto" ), *new Method( "proto", *new Block( proto ), 0 ) );
		setSlot( Symbol::at( "clone" ), *new Method( "clone", *new Block( clone ), 0 ) );
	}

	Lobby::Lobby() : Frame( Object::instance() )
	{
		static const Frame* fooNumberArgTypes[] = { &Number::instance(), &Number::instance() };
		static const Frame* fooObjectArgTypes[] = { &Number::instance(), &Object::instance() };

		setSlot( Symbol::at( "print:" ), *new Method( "print:", *new Block( print_ ), 1 ) );
		setSlot( Symbol::at( "help" ),   *new Method( "help:",  *new Block( help ),   0 ) );
		setSlot( Symbol::at( "exit" ),   *new Method( "exit",   *new Block( exit ),   0 ) );

		//Method& foo = *new Method( "foo:",  *new Block( foo_with_ ), 2, fooNumberArgTypes );
		//foo.addBlock( *new Block( foo_$number$_with_$object$_, 2, fooObjectArgTypes ) );

		setSlot( Symbol::at( "foo-a:" ), *new Method( "foo:", *new Block( foo_with_ ), 2, fooNumberArgTypes ) );
		setSlot( Symbol::at( "foo-b:" ), *new Method( "foo:", *new Block( foo_with_ ), 2, fooObjectArgTypes ) );

		setSlot( Symbol::at( "<object>" ), Object::instance() );
	}

 //
 // class Boolean
 //

	void Boolean::initSlots()
	{
		setSlot( Symbol::at( "==" ), *new Method( "==", *new Block( equal_ ),    1 ) );
		setSlot( Symbol::at( "!=" ), *new Method( "!=", *new Block( notEqual_ ), 1 ) );
		setSlot( Symbol::at( "?" ),  *new Method( "?",  *new Block( ternary_ ), -1 ) );
	}

	Value Boolean::ternary_( Value receiver, const Array& args, Value context )
	{
		if (receiver.getBool() == true)
		{
			Block& block = args[0].get<Block>();
			const static Array blockArgs;
			
			return block.eval_( context, blockArgs, context );
		}
		else if (args.size() == 2)
		{
			Block& block = args[1].get<Block>();
			const static Array blockArgs;
			
			return block.eval_( context, blockArgs, context );
		}
		
		return Value();
	}

}

