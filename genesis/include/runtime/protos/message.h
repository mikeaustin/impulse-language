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

		virtual Value apply( Value receiver, const Array& args )
		{
			std::cout << "Message::apply()" << std::endl;
			std::cout << _name << std::endl;
			std::cout << _args.get<ArrayProto>()[0] << std::endl;

			if (_name.getFrame().getName() == "foo")
				return _args.get<ArrayProto>()[0].getFloat() * 2;
				
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
 	
 	};

}

#endif

