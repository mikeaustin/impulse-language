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

		Function( std::vector<ArgType> argtypes, std::vector<GCValue> code )
		 : _argtypes( argtypes ), _code( code ) { }

		Value value_( Value receiver, const Array& args, Value locals )
		{
			ENTER( "Function::value_( receiver = " << receiver << " )" );
			
			static const Array msgArgs;

			switch (args.size())
			{
				case 5:  locals.setSlot( _argtypes[4].getName(), args[Index::_4] );
				case 4:  locals.setSlot( _argtypes[3].getName(), args[Index::_3] );
				case 3:  locals.setSlot( _argtypes[2].getName(), args[Index::_2] );
				case 2:  locals.setSlot( _argtypes[1].getName(), args[Index::_1] );
				case 1:  locals.setSlot( _argtypes[0].getName(), args[Index::_0] );
			}

			std::vector<GCValue>::iterator message = _code.begin();

			while (message != _code.end())
			{
				receiver = (*message++).apply( receiver, msgArgs, locals );
			}

			LEAVE( receiver );

			return receiver;
		}

	 private:

		std::vector<ArgType> _argtypes;
		std::vector<GCValue> _code;
	
	};


	class Block : public Frame {
	
	 public:

		Block( std::vector<ArgType> argtypes, GCValue locals ) : _argtypes( argtypes ), _locals( locals ) { }

		short arity() { return _argtypes.size(); }
	 
		virtual Value value( Value receiver, const Array& args ) = 0;

	 //private:
	 
		std::vector<ArgType> _argtypes;
		GCValue              _locals;
		
	};

 //
 // class BlockProto
 //

	template <typename T>
	class BlockProto : public Block {

		typedef Value (T::*const Method)(Value, const Array&, Value);
	
	 public:
	 
		BlockProto( T& object, Method method, std::vector<ArgType> argtypes )
		 : Block( argtypes, Value() ), _object( object ), _method( method ) { }

		BlockProto( std::vector<GCValue> code, std::vector<ArgType> argtypes, Value locals )
		 : Block( argtypes, locals ), _object( *new Function( argtypes, code ) ), _method( &Function::value_ ) { }

		/*virtual Value apply( Value receiver, const Array& args, Value locals )
		{
			return value( receiver, args );
		}*/

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

}

#endif

