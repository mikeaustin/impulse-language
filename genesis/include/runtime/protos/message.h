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
			Array args; args.self( msgArgs.self() );

			for (unsigned int i = 0; i < _args.getFrame().size(); i++)
			{
				msgArgs[i] = _args.getFrame()[i].apply( locals, args, locals );
			}

			TRACE( "" );
		}

		virtual Value apply( Value receiver, const Array& args, Value locals )
		{
			ENTER( "Message::apply( receiver = " << receiver << " ) _name = " << _name );

//			if (_name.getFrame().getName() == "foo")
//				return 20;

			Array msgArgs; msgArgs.self( receiver );

			evaluateArgs( msgArgs, locals );

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
 	
 		virtual Value apply( Value receiver, const Array& args, Value locals )
 		{
 			ENTER( "SelfMessage::apply( receiver = " << receiver << " )" );
 			
 			//return locals.get<LocalsProto>().selfContext();

 			LEAVE( args.self() );

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
				Array msgArgs; msgArgs.self( args.self() ); evaluateArgs( msgArgs, locals );
				
				result = std::pow( receiver.getFloat(), msgArgs[Index::_0].getFloat() );
			}
			else result = MessageProto::apply( receiver, args, locals );

			LEAVE( result );

			return result;
		}
		
	};

}

#endif

