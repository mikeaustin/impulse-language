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

		MessageProto( Symbol name, Array args ) : _name( name ), _args( args ) { }

		virtual Value apply( Value receiver, const Array& args )
		{
			std::cout << "Message::apply()" << std::endl;
			std::cout << _name << std::endl;
			std::cout << _args[Index::_0] << std::endl;

			if (_name.getName() == "foo")
				return _args[Index::_0].getFloat() * 2;
				
			return Value();
		}
		
	 private:

		Symbol _name;
		Array  _args;

	};

}

#endif

