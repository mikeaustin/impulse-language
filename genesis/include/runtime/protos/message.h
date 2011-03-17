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

		SymbolProto& getName() { return _name.getFrame(); }
		ArrayProto&  getArgs() { return _args.getFrame(); }
		
		void evaluateArgs( Array& msgArgs, Value locals )
		{
			msgArgs.size( _args.getFrame().size() );
			Array args( msgArgs.self() );

			switch (_args.getFrame().size())
			{
				case 5: msgArgs[4] = _args.getFrame()[4].apply( locals, args, locals );
				case 4: msgArgs[3] = _args.getFrame()[3].apply( locals, args, locals );
				case 3: msgArgs[2] = _args.getFrame()[2].apply( locals, args, locals );
				case 2: msgArgs[1] = _args.getFrame()[1].apply( locals, args, locals );
				case 1: msgArgs[0] = _args.getFrame()[0].apply( locals, args, locals );
			}

			TRACE( "" );
		}

		virtual Value apply( Value receiver, const Array& args, Value locals )
		{
			ENTER( "Message::apply( receiver = " << receiver << " ) _name = " << _name );

			switch (_args.getFrame().size())
			{
				case 2: {
					const Array msgArgs( receiver, _args.getFrame()[0].apply( locals, args, locals ),
												   _args.getFrame()[1].apply( locals, args, locals ) );
					return receiver.perform( _name.getFrame(), msgArgs, locals );
				} break;
				case 1: {
					const Array msgArgs( receiver, _args.getFrame()[0].apply( locals, args, locals ) );
					return receiver.perform( _name.getFrame(), msgArgs, locals );
				} break;
				default: return Value();
			}

//			Array msgArgs( receiver );

//			evaluateArgs( msgArgs, locals );

//			Value result = receiver.perform( _name.getFrame(), msgArgs, locals );
			
//			LEAVE( result );
			
//			return result;
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
 	
 		virtual Value apply( Value receiver, const Array& args, Value locals )
 		{
 			ENTER( "SelfMessage::apply( receiver = " << receiver << " )" );

 			LEAVE( receiver );

			return args.self();
		}
 	
 	};

	class PowMessage : public MessageProto {
	
	 public:
	 
	 	PowMessage( ArrayProto& args ) : MessageProto( SymbolProto::at( "pow" ), args ) { }
	 
		virtual Value apply( Value receiver, const Array& args, Value locals )
		{
 			ENTER( "PowMessage::apply( receiver = " << receiver << " )" );
			
			Value result;

			if (&receiver.getProto() == &NumberProto::instance())
			{
				const Array msgArgs( args.self(), getArgs()[0].apply( locals, args, locals ) );
				
				result = std::pow( receiver.getFloat(), msgArgs[Index::_0].getFloat() );
			}
			else result = MessageProto::apply( receiver, args, locals );

			LEAVE( result );

			return result;
		}
		
	};

}

#endif

