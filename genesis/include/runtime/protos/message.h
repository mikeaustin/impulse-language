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
#include "core/protos/number.h"

namespace impulse {

 //
 // class MessageProto
 //

	class MessageProto : public Frame {

	 public:

		MessageProto( Symbol name, ArrayProto& args ) : _name( name ), _args( args ) { }

		const SymbolProto& getName() const { return _name.getFrame(); }
		const ArrayProto&  getArgs() const { return _args.getFrame(); }
		
		virtual Value apply( Value receiver, const Array& args, Value locals ) const
		{
			ENTER( "Message::apply( receiver = " << receiver << " ) _name = " << _name );

			Array msgArgs( receiver ); msgArgs.size( _args.getFrame().size() );

			switch (msgArgs.size())
			{
				case 5: msgArgs[4] = _args.getFrame()[4].apply( locals, args, locals );
				case 4: msgArgs[3] = _args.getFrame()[3].apply( locals, args, locals );
				case 3: msgArgs[2] = _args.getFrame()[2].apply( locals, args, locals );
				case 2: msgArgs[1] = _args.getFrame()[1].apply( locals, args, locals );
				case 1: msgArgs[0] = _args.getFrame()[0].apply( locals, args, locals );
			}

			Value result = receiver.perform( _name.getFrame(), msgArgs, locals );
						
			LEAVE( result );
			
			return result;
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
 	
 		virtual Value apply( Value receiver, const Array& args, Value locals ) const
 		{
 			ENTER( "SelfMessage::apply( receiver = " << receiver << " )" );

 			LEAVE( receiver );

			return args.self();
		}
 	
 	};

 //
 // class OperatorMessage
 //

	class OperatorMessage : public MessageProto {
	
	 public:
	
		OperatorMessage( Symbol selector, ArrayProto& args ) : MessageProto( selector, args ) { }
	
		virtual Value apply( Value receiver, const Array& args, Value locals ) const
		{
			Value result;

			if (&receiver.getFrame() == &NumberValue::instance())
			{
				const Array msgArgs( args.self(), getArgs()[0].apply( locals, args, locals ) );

				if (&msgArgs[Index::_0].getFrame() == &NumberValue::instance())
				{
					switch (getName().getId())
					{
						case SymbolProto::ADD: result = receiver.getFloat() + msgArgs[Index::_0].getFloat();
						case SymbolProto::MUL: result = receiver.getFloat() * msgArgs[Index::_0].getFloat();
						case SymbolProto::POW: result = std::pow( receiver.getFloat(), msgArgs[Index::_0].getFloat() );
					}
				}
				else result = MessageProto::apply( receiver, args, locals );
			}
			else result = MessageProto::apply( receiver, args, locals );
	
			return result;
		}
		
	};
	
}

#endif

