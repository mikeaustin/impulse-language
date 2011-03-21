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
		
		virtual Value apply( Value receiver, const Array& args, Value locals )
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

/*			if (_args.getFrame().size() <= 5)
			{
				for (unsigned int i = 0; i < _args.getFrame().size(); ++i)
				{
					msgArgs[i] = _args.getFrame()[i].apply( locals, args, locals );
				}
			}
*/
			Value result = receiver.perform( _name.getFrame(), static_cast<const Array>( msgArgs ), locals );
						
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

