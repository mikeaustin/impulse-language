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

	class Foo : public Frame {

	 public:

		Foo( std::vector< GCValue::Type<SymbolProto> > argnames, std::vector<GCValue> code )
		 : _argnames( argnames ), _code( code ) { }

		Value value_( Value receiver, const Array& args, Value locals )
		{
			static const Array msgArgs;

			switch (args.size())
			{
				case 5:  locals.setSlot( _argnames[4].getFrame(), args[Index::_4] );
				case 4:  locals.setSlot( _argnames[3].getFrame(), args[Index::_3] );
				case 3:  locals.setSlot( _argnames[2].getFrame(), args[Index::_2] );
				case 2:  locals.setSlot( _argnames[1].getFrame(), args[Index::_1] );
				case 1:  locals.setSlot( _argnames[0].getFrame(), args[Index::_0] );
			}

			std::vector<GCValue>::iterator message = _code.begin();

			while (message != _code.end())
			{
				receiver = (*message++).apply( receiver, msgArgs, locals );
			}

			return receiver;
		}

	 private:

		std::vector< GCValue::Type<SymbolProto> >
		                           _argnames;
		std::vector<GCValue>       _code;
	
	};

	template <typename T = Foo>
	class BlockProto2 : public Frame {

		typedef Value (T::*const Function)(Value, const Array&, Value);
	
	 public:
	 
		BlockProto2( T& object, Function function, std::vector< GCValue::Type<SymbolProto> > argnames, Value locals )
		 : _object( object ), _function( function ), _argnames( argnames ), _locals( locals ) { }

		BlockProto2( std::vector<GCValue> code, std::vector< GCValue::Type<SymbolProto> > argnames, Value locals )
		 : _object( *new Foo( argnames, code ) ), _function( &Foo::value_ ), _argnames( argnames ), _locals( locals ) { }

		short arity() { return _argnames.size(); }

		Value value( Value receiver, const Array& args )
		{
			if (args.size() == _argnames.size())
				return (_object.*_function)( receiver, args, _locals.getFrame() );
			else
				return Value();
		}

	 	T&                         _object;
		Function                   _function;

	 private:
	 
		std::vector< GCValue::Type<SymbolProto> >
		                           _argnames;
		GCValue  _locals;
		
	};

 //
 // class BlockProto
 //

/*
	class BlockProto : public Frame {

		typedef Value (*const Function)(Value, const Array&, LocalsProto&);

	 public:

		BlockProto( Function function, std::vector< GCValue::Type<SymbolProto> > argnames, LocalsProto& locals )
		 : _function( function ), _argnames( argnames ), _locals( locals ) { }

		BlockProto( std::vector<GCValue> code, std::vector< GCValue::Type<SymbolProto> > argnames, LocalsProto& locals )
		 : _function( value_ ), _code( code ), _argnames( argnames ), _locals( locals ) { }

		inline Value value( Value receiver, const Array& args )
		{
			if (args.size() == _argnames.size())
				return _function( receiver, args, _locals.getFrame() );
			else
				return Value();
		}

		inline short arity() { return _argnames.size(); }

		virtual string inspect( const Value self ) const
		{
			return Frame::inspect( self, "block" );
		}

		static Value value_( Value self, const Array& args, LocalsProto& locals )
		{
			BlockProto& block = self.get<BlockProto>();
			Value receiver = args[Index::_0];
			static const Array msgArgs;

			switch (args.size())
			{
				case 5:  block._locals.setSlot( block._argnames[4].getFrame(), args[Index::_4] );
				case 4:  block._locals.setSlot( block._argnames[3].getFrame(), args[Index::_3] );
				case 3:  block._locals.setSlot( block._argnames[2].getFrame(), args[Index::_2] );
				case 2:  block._locals.setSlot( block._argnames[1].getFrame(), args[Index::_1] );
				case 1:  block._locals.setSlot( block._argnames[0].getFrame(), args[Index::_0] );
			}

			std::vector<GCValue>::iterator message = block._code.begin();

			while (message != block._code.end())
			{
				receiver = (*message++).apply( receiver, msgArgs, block._locals.getFrame() );
			}

			return receiver;
		}

		Function                   _function;

	 private:
	 
		std::vector<GCValue>       _code;
		std::vector< GCValue::Type<SymbolProto> >
		                           _argnames;
		GCValue::Type<LocalsProto> _locals;

	};
*/
}

#endif

