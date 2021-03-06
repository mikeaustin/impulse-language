//
// core/protos/block.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_BLOCK_H
#define IMPULSE_BLOCK_H

#include "core/frame.h"
#include "core/array.h"

#include "core/protos/locals.h"
#include "runtime/protos/expression.h"

namespace impulse {

 //
 // class ArgTyp
 //

	class ArgType {

	 public:
	 
		ArgType( Symbol name, Value type ) : _name( name ), _type( type ) { }

		Symbol getName() { return _name.getFrame(); }

	 private:
	 
		GCValue::Type<SymbolProto> _name;
		GCValue                    _type;
	
	};

 //
 // class Function
 //

	class Function : public Frame {

	 public:

		Function( vector<ArgType> argtypes, vector<GCValue> code )
		 : _argtypes( argtypes ), _code( code ) { }

		Value value_( Value receiver, const Array& args, Value locals )
		{
			ENTER( "Function::value_( receiver = " << receiver << " )" );

			Array msgArgs; msgArgs.self( args.self() );

			switch (args.size())
			{
				case 5: locals.setSlot( _argtypes[4].getName(), args[Index::_4] );
				case 4: locals.setSlot( _argtypes[3].getName(), args[Index::_3] );
				case 3: locals.setSlot( _argtypes[2].getName(), args[Index::_2] );
				case 2: locals.setSlot( _argtypes[1].getName(), args[Index::_1] );
				case 1: locals.setSlot( _argtypes[0].getName(), args[Index::_0] );
			}

			vector<GCValue>::iterator message = _code.begin();

			while (message != _code.end())
			{
				receiver = (*message++).apply( locals, msgArgs, locals );
			}

			LEAVE( receiver );

			return receiver;
		}

	 private:

		vector<ArgType> _argtypes;
		vector<GCValue> _code;
		vector< vector<Value> > code2;
	
	};


	class Block : public Frame {
	
	 public:

		Block( vector<ArgType> argtypes, Value locals ) : _argtypes( argtypes ), _locals( locals ) { }

		short arity() { return _argtypes.size(); }
	 
		virtual Value value( Value receiver, const Array& args ) = 0;

	 //private:
	 
		vector<ArgType> _argtypes;
		GCValue         _locals;
		
	};

 //
 // class BlockProto
 //

	template <typename T>
	class BlockProto : public Block {

		typedef Value (T::*const Method)(Value, const Array&, Value);
	
	 public:
	 
		BlockProto( T& object, Method method, vector<ArgType> argtypes )
		 : Block( argtypes, Value() ), _object( object ), _method( method ) { }

		BlockProto( vector<GCValue> code, vector<ArgType> argtypes, Value locals )
		 : Block( argtypes, locals ), _object( *new Function( argtypes, code ) ), _method( &Function::value_ ) { }

		virtual Value value( Value receiver, const Array& args )
		{
			ENTER( "BlockProto::value( receiver = " << receiver << " )" );
			
			Value result;
			
			if (args.size() == _argtypes.size())
				result = (_object.*_method)( receiver, args, _locals.getFrame() );
			else
				result = Value();
				
			LEAVE( result );
			
			return result;
		}

	 private:

	 	T&     _object;
		Method _method;
		
	};

	typedef Value (*const Function2)(Value, const Array&);

	class Function3 : public Frame {
	
	 public:
	 
		Function3( Function2 function, vector<ArgType> argtypes )
		 : _function( function ), _argtypes( argtypes ) { }

		short arity() { return _argtypes.size(); }

		Value value( Value self, const Array& args )
		{
			ENTER( "Function3::value( self = " << self << " )" );

			Value result = (*_function)( self, args );
			
			LEAVE( result );
			
			return result;
		}

	 protected:

		Function2       _function;
		vector<ArgType> _argtypes;
	
	};

	class Block2 : public Function3 {
	
	 public:

		//Block2( vector< vector<GCValue> > code, vector<ArgType> argtypes, Value locals )
		// : Function3( value_, argtypes ), _code( code ), _locals( locals ) { }

		Block2( vector<Expression> expressions, vector<ArgType> argtypes, Value locals )
		 : Function3( value_, argtypes ), _expressions( expressions ), _locals( locals ) { }

		virtual string inspect( const Value self ) const
		{
			return Frame::inspect( self, "block" );
		}

		static Value value_( Value self, const Array& args )
		{
			Block2& block = self.get<Block2>();

			Array msgArgs; msgArgs.self( args.self() );

			switch (args.size())
			{
				case 5: block._locals.setSlot( block._argtypes[4].getName(), args[Index::_4] );
				case 4: block._locals.setSlot( block._argtypes[3].getName(), args[Index::_3] );
				case 3: block._locals.setSlot( block._argtypes[2].getName(), args[Index::_2] );
				case 2: block._locals.setSlot( block._argtypes[1].getName(), args[Index::_1] );
				case 1: block._locals.setSlot( block._argtypes[0].getName(), args[Index::_0] );
			}

			vector<Expression>::const_iterator expression = block._expressions.begin();
			Value receiver;

			while (expression != block._expressions.end())
			{
				receiver = block._locals;
				
				receiver = expression->apply( block._locals, msgArgs, block._locals );
				
				++expression;
			}

			return receiver;		
		}
		
	 private:
	 
		vector<Expression> _expressions;
		GCValue            _locals;
		
	};

}

#endif

