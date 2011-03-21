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

	class MethodProto2 : public Frame {

	 public:

		MethodProto2( Function3& function ) : _function( function ) { }

		MethodProto2( Function2 function, std::vector<ArgType> argtypes )
		 : _function( *new Function3( function, argtypes ) ) { }

		//short arity() { return _block.get< BlockProto<Function> >().arity(); }

		virtual Value apply( Value receiver, const Array& args, Value locals )
		{
			ENTER( "MethodProto2::apply( receiver = " << receiver << " )" );

			Value result = _function.getFrame().value( receiver, args );
			
			LEAVE( result );
			
			return result;
		}

	 private:
	 
		GCValue::Type<Function3> _function;
		
	};

}

#endif

