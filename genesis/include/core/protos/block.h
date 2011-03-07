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

namespace impulse {

	typedef Value (* const Function)(Value, const Array&, Value);

 //
 // class BlockProto
 //


	class BlockProto : public Frame {

	 public:

		BlockProto( Function function, size_t argCount ) : _function( function ), _argCount( argCount ) { }

		virtual Value apply( Value receiver, Array& args )
		{
			if (args.size() == _argCount)
				return _function( receiver, args, _locals );
			else
				return Value();
		}

	 private:
	 
		Function _function;
		GCValue  _locals;
		size_t   _argCount;

	};

}

#endif

