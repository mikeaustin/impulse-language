//
// core/protos/method.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_METHOD_H
#define IMPULSE_METHOD_H

#include "core/frame.h"
#include "core/array.h"

#include "core/protos/locals.h"
#include "core/protos/block.h"

namespace impulse {

 //
 // class MethodProto
 //

	class MethodProto : public Frame {

	 public:

		MethodProto( Block& block ) : _block( block ) { }

		//short arity() { return _block.get< BlockProto<Function> >().arity(); }

		virtual Value apply( Value receiver, const Array& args, Value locals )
		{
			ENTER( "MethodProto::apply( receiver = " << receiver << " )" );
			
			Value result = _block.getFrame().value( receiver, args );
			
			LEAVE( result );
			
			return result;
		}

	 private:
	 
		GCValue::Type<Block> _block;
		
	};

}

#endif

