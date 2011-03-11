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

	typedef Value (*const Function)(Value, const Array&, Value);

 //
 // class BlockProto
 //

	class BlockProto : public Frame {

	 public:

		BlockProto( Function function, std::vector< GCValue::Type<SymbolProto> > argnames, GCValue::Type<LocalsProto> locals )
		 : _function( function ), _argnames( argnames ), _locals( locals ) { }

		BlockProto( std::vector<GCValue> code, std::vector< GCValue::Type<SymbolProto> > argnames, GCValue::Type<LocalsProto> locals )
		 : _code( code ), _function( value_ ), _argnames( argnames ), _locals( locals ) { }

		inline Value value( Value self, const Array& args )
		{
			if (args.size() == _argnames.size())
				return _function( self, args, _locals );
			else
				return Value();
		}

		inline short arity() { return _argnames.size(); }

		virtual string inspect( const Value self ) const
		{
			return Frame::inspect( self, "block" );
		}

		static Value value_( Value self, const Array& args, Value locals )
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
				receiver = (*message++).apply( receiver, msgArgs );
			}

			return receiver;
		}

	 private:
	 
		std::vector<GCValue>       _code;
		Function                   _function;
		std::vector< GCValue::Type<SymbolProto> >
		                           _argnames;
		GCValue::Type<LocalsProto> _locals;

	};

}

#endif

