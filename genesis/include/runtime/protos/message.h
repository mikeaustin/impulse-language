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

		SymbolProto& getName() const { return _name.getFrame(); }
		ArrayProto&  getArgs() const { return _args.getFrame(); }
		
		virtual string inspect( const Value self ) const
		{
			MessageProto& message = *dynamic_cast<MessageProto*>( &self.getFrame() );
			
			std::ostringstream stream; stream << "message:" << message.getName().getName();

			return Frame::inspect( self, stream.str() );
		}

		virtual Value apply( Value receiver, const Array& args, Value locals ) const
		{
			ENTER( "Message::apply( receiver = " << receiver << " ) _name = " << _name.getFrame() );

			Array msgArgs; msgArgs.size( _args.getFrame().size() ).self( receiver );

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

	Value add_( Value receiver, const Array& msgArgs ) { return receiver.getFloat() + msgArgs[Index::_0].getFloat(); }
	Value sub_( Value receiver, const Array& msgArgs ) { return receiver.getFloat() - msgArgs[Index::_0].getFloat(); }
	Value mul_( Value receiver, const Array& msgArgs ) { return receiver.getFloat() * msgArgs[Index::_0].getFloat(); }
	Value div_( Value receiver, const Array& msgArgs ) { return receiver.getFloat() / msgArgs[Index::_0].getFloat(); }
	Value pow_( Value receiver, const Array& msgArgs ) { return pow( receiver.getFloat(), msgArgs[Index::_0].getFloat() ); }

	template <Value (*operation)(Value, const Array&)>
	class OperatorMessage : public MessageProto {
	
	 public:
	
		OperatorMessage( Symbol selector, ArrayProto& args ) : MessageProto( selector, args ) { }
		
		virtual Value apply( Value receiver, const Array& args, Value locals ) const
		{
			Value result;

			Array msgArgs( getArgs()[0].apply( locals, args, locals ) ); msgArgs.self( args.self() );

			if (&receiver.getFrame() == &NumberValue::instance() && &msgArgs[Index::_0].getFrame() == &NumberValue::instance())
			{
				result = (*operation)( receiver, msgArgs );
			}
			else result = receiver.perform( getName(), msgArgs, locals );
	
			return result;
		}
		
	};

}

#endif

