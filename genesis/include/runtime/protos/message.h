//
// core/runtime/message.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_MESSAGE_H
#define IMPULSE_MESSAGE_H

#include "core/frame.h"
#include "core/array.h"
#include "core/protos/symbol.h"

namespace impulse {

 //
 // class MessageProto
 //

	class MessageProto : public Frame {

	 public:

		MessageProto( Symbol name, ArrayProto& args ) : _name( name ), _args( args ) { }

		virtual Value apply( Value receiver, const Array& args, Value locals )
		{
			TRACE( "Message::apply()" );
			TRACE( "  _name = " <<  _name );

			if (_name.getFrame().getName() == "foo")
				return 20;

			return Value();
		}
		
	 private:

		GCValue::Type<SymbolProto> _name;
		GCValue::Type<ArrayProto>  _args;

	};

 //
 // class SelfMessage
 //
 
 	class SelfMessage : public Frame {
 	
 	 public:
 	
 		virtual Value apply( Value receiver, const Array& args, Locals locals )
 		{
 			std::cout << locals.selfContext() << std::endl;
 			
 			return Value();
 		}
 	
 	};

}

#endif

