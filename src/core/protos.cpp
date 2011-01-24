//
// core/protos.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include <algorithm>

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
		//Invokable& block = dynamic_cast<Invokable&>( args[0].getFrame() );
		Block& block = args[0].get<Block>();

		Array blockArgs( 1 );
		// Create another autorelease pool

		for (int i = range._from; i <= range._to; ++i)
		{
			blockArgs[0] = i;
		
			block.evalLoop_( receiver, blockArgs, context );

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
		Range& range = receiver.get<Range>();
		Block& block = args[0].get<Block>();

		Array blockArgs( 1 );
		GCArray& array = *new ArrayValue( range._to - range._from + 1 );

		for (int i = range._from; i <= range._to; ++i)
		{
			blockArgs[0] = i;

			Value value = block.evalLoop_( receiver, blockArgs, context );
			
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
	GCArray::GCArray( size_t size ) : _array( size ) { }
	GCArray::GCArray( Frame& proto, size_t size = 0 ) : Frame( proto ) { }

	void GCArray::initSlots()
	{
		static const Frame* mapArgTypes[] = { &Block::instance() };
		static const Frame* sliceArgTypes[] = { &Number::instance() };
	
		setSlot( Symbol::at( "size" ),  *new Method( "size",  *new Block( size_ ),  0 ) );
		setSlot( Symbol::at( "[]" ),    *new Method( "[]",    *new Block( slice_ ), 1, sliceArgTypes ) );
		setSlot( Symbol::at( "each:" ), *new Method( "each:", *new Block( each_ ),  1, mapArgTypes ) );
		setSlot( Symbol::at( "map:" ),  *new Method( "map:",  *new Block( map_ ),   1, mapArgTypes ) );
		setSlot( Symbol::at( "zip:" ),  *new Method( "zip:",  *new Block( zip_ ),  -1 ) );
	}

	inline Frame& GCArray::instance()
	{
		static GCValue array = *new GCArray( Object::instance() );
			
		return array.getFrame();
	}

	Value GCArray::each_( Value receiver, const Array& args, Value context )
	{
		GCArray& self = receiver.get<GCArray>();
		Block& block  = args[0].get<Block>();

		Array blockArgs( 1 );

		vector<GCValue>::iterator iter = self._array.begin();
		
		while (iter != self._array.end())
		{
			blockArgs[0] = *iter;
			
			block.evalLoop_( receiver, blockArgs, context );
			
			++iter;
		}
		
		return Value();
	}

	Value GCArray::map_( Value receiver, const Array& args, Value context )
	{
		GCArray& self = receiver.get<GCArray>();
		Block& block  = args[0].get<Block>();

		Array blockArgs( 1 );
		GCArray& array = *new ArrayValue( self._array.size() );

		vector<GCValue>::iterator iter = self._array.begin();
		
		while (iter != self._array.end())
		{
			blockArgs[0] = *iter;
			
			Value value = block.evalLoop_( receiver, blockArgs, context );
			array._array.push_back( value );
			
			++iter;
		}
		
		return array;
	}

	static bool hasNext( Enumerable::Iterator* iter )
	{
		return iter->hasNext();
	}

	static bool notHasNext( Enumerable::Iterator* iter )
	{
		return !iter->hasNext();
	}
  
	Value GCArray::zip_( Value receiver, const Array& args, Value context )
	{
		GCArray& self = receiver.get<GCArray>();
		Block& block  = args[0].get<Block>();

		vector<Enumerable::Iterator*> iters( self.size() );

		for (unsigned int i = 0; i < self.size(); ++i)
			iters[i] = &self[i].getDynamic<Enumerable>().iterator();

		Array blockArgs( iters.size() );
		GCArray& array = *new ArrayValue();

		if (args.size() == 1)
		{
			while (find_if( iters.begin(), iters.end(), notHasNext ) == iters.end())
			{
				for (unsigned int i = 0; i < iters.size(); ++i)
					blockArgs[i] = iters[i]->getValue();
			
				Value value = block.evalLoop_( receiver, blockArgs, context );
			
				if (&value.getFrame() != &Void::instance())
					array._array.push_back( value );
			}
		}
		else
		{
			Value fill = args[1];
			
			while (find_if( iters.begin(), iters.end(), hasNext ) != iters.end())
			{
				for (unsigned int i = 0; i < iters.size(); ++i)
					blockArgs[i] = iters[i]->hasNext() ? iters[i]->getValue() : fill;
			
				Value value = block.evalLoop_( receiver, blockArgs, context );
			
				if (&value.getFrame() != &Void::instance())
					array._array.push_back( value );
			}
		}

		return autorelease( array );
	}
	
 //
 // class ArrayValue
 //

	inline ArrayValue::ArrayValue()                     : GCArray( GCArray::instance() ) { }
	inline ArrayValue::ArrayValue( size_t size )        : GCArray( GCArray::instance(), size ) { }
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
	 : Frame( Method::instance() ),
	   _function( function ), _argsSize( argsSize ), _argTypes( NULL ), _funcName( funcName ) { }

	Method::Method( const string funcName, Block& function, const int argsSize, const Frame* argTypes[] )
	 : Frame( Method::instance() ),
	   _function( function ), _argsSize( argsSize ), _argTypes( argTypes ), _funcName( funcName ) { }

	void Method::addBlock( Block& block, const Frame* argTypes[] )
	{
		_argTypes2[argTypes] = &block;
	}

	Frame& Method::instance()
	{
		static GCValue method = *new Method( Object::instance() );
			
		return method.getFrame();
	}

	void Method::initSlots()
	{
		setSlot( Symbol::at( "arity" ), *new Method( "arity", *new Block( arity ), 0 ) );
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
		//Value result = _function.eval_( receiver, args, context );
		//Value result = _function.get<Block>().eval_( receiver, args, context );
		Value result = _function.get<Block>().eval_( receiver, args, receiver );

		LEAVE( result );
		
		return result;
	}

 //
 // Object
 //

	void Object::initSlots()
	{
		static const Frame* blockArgTypes[] = { &Block::instance() };
		
		setSlot( Symbol::at( "proto" ), *new Method( "proto", *new Block( proto ), 0 ) );
		setSlot( Symbol::at( "clone" ), *new Method( "clone", *new Block( clone ), 0 ) );
		setSlot( Symbol::at( "async:" ), *new Method( "async:", *new Block( async_ ), 1, blockArgTypes ) );
		setSlot( Symbol::at( "method:" ), *new Method( "method:", *new Block( method_ ), 2 ) );
		setSlot( Symbol::at( "methods" ), *new Method( "methods", *new Block( methods ), 0 ) );
	}

	Value Object::method_( Value receiver, const Array& args, Value context )
	{
		String& name  = args[0].get<String>();
		Block& block  = args[1].get<Block>();
		Frame& object = *new Method( name.getValue(), block, block.arity() );
		
		receiver.setSlot( Symbol::at( name.getValue() ), object );
		
		return Value();
	}

	Future::Future( Block& block, Value receiver ) : Frame( Future::instance() ),
	 _receiver( receiver ), _block( block ) { }

	Frame& Future::instance()
	{
		static GCValue future = *new Future( Object::instance() );
			
		return future.getFrame();
	}

	void Future::initSlots()
	{
		setSlot( Symbol::at( "value" ), *new Method( "value", *new Block( value ), 0 ) );
	}

	Value Future::value( Value receiver, const Array& args, Value context )
	{
		Future& self = receiver.get<Future>();
		Block& block = self._block.get<Block>();

		Array blockArgs(1); blockArgs[0] = self._receiver;

		self._result = block.eval_( receiver, blockArgs, Value() );

		return self._result;
	}

 //
 // Lobby
 //  

	Lobby::Lobby() : Frame( Object::instance() )
	{
		static const Frame* fooNumberArgTypes[] = { &Number::instance(), &Number::instance() };
		static const Frame* fooObjectArgTypes[] = { &Number::instance(), &Object::instance() };

		setSlot( Symbol::at( "lobby" ), *this );

		setSlot( Symbol::at( "nil" ), Nil::instance() );

		setSlot( Symbol::at( "print:" ),  *new Method( "print:",  *new Block( print_ ), -1 ) );
		setSlot( Symbol::at( "object:" ), *new Method( "object:", *new Block( object_ ), 2 ) );
		//setSlot( Symbol::at( "method:" ), *new Method( "method:", *new Block( method_ ), 2 ) );
		setSlot( Symbol::at( "help" ),    *new Method( "help",    *new Block( help ),    0 ) );
		setSlot( Symbol::at( "exit" ),    *new Method( "exit",    *new Block( exit ),    0 ) );

		//Method& foo = *new Method( "foo:",  *new Block( foo_with_ ), 2, fooNumberArgTypes );
		//foo.addBlock( *new Block( foo_$number$_with_$object$_, 2, fooObjectArgTypes ) );

		setSlot( Symbol::at( "foo-a:" ), *new Method( "foo:", *new Block( foo_with_ ), 2, fooNumberArgTypes ) );
		setSlot( Symbol::at( "foo-b:" ), *new Method( "foo:", *new Block( foo_with_ ), 2, fooObjectArgTypes ) );

		setSlot( Symbol::at( "true" ), true );
		setSlot( Symbol::at( "false" ), false );

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
		const static Array exprArgs;

		if (receiver.getBool() == true)
		{
			Expression& expr = args[0].get<Expression>();
			
			return expr.eval( context, exprArgs, context );
		}
		else if (args.size() == 2)
		{
			Expression& expr = args[1].get<Expression>();
			
			return expr.eval( context, exprArgs, context );
		}
		
		return Value();
	}

	inline Value TernaryMessage::eval( Value receiver, const Array& args, Value context )
	{
		const static Array exprArgs;

		if (&receiver.getProto() == &Boolean::instance())
		{
			if (receiver.getBool() == true)
			{
				Expression& expr = (Expression&) _msgArgs[0].getFrame();
		
				return expr.eval( context, exprArgs, context );
			}
			else if (_msgArgs.size() == 2)
			{
				Expression& expr = (Expression&) _msgArgs[1].getFrame();
		
				return expr.eval( context, exprArgs, context );
			}
			else return Value();
		}
		
		return Message::eval( receiver, args, context );
	}

}

