//
// message.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_MESSAGE_H
#define IMPULSE_MESSAGE_H

namespace runtime {

 //
 // class Message
 //

	class Message : public Frame {

	 public:

		Message( Symbol& selector, const Array& msgArgs )
		 : _selector( selector ), _selectorId( selector.getId() ), _msgArgs( msgArgs ) { _eval2 = &eval2; }

		virtual string toString( Value receiver ) const { return "[Message]"; }

		inline Value eval( Value receiver, const Array& args, Value context )
		{
			//ENTER( "Message::eval( receiver = " << receiver.toString() << ", _selector = " << _selector.toString() << " )" );

			Array msgArgs( _msgArgs.size() );
			
			switch (_msgArgs.size())
			{
				case 3: msgArgs[2] = _msgArgs[2].eval( receiver, args, context );
				case 2: msgArgs[1] = _msgArgs[1].eval( receiver, args, context );
				case 1: msgArgs[0] = _msgArgs[0].eval( receiver, args, context );
			}
			
			//Value result = receiver.send( _selector.get<Symbol>(), msgArgs, context );
			//Value result = receiver.send( (Symbol&) _selector.getFrame(), msgArgs, context );
			return receiver.send( _selector.getId(), msgArgs, context );
		}

		inline static Value eval2( Frame* self_, Value receiver, const Array& args, Value context )
		{
			//ENTER( "Message::eval( receiver = " << receiver.toString() << ", _selector = " << _selector.toString() << " )" );

			Message* self = (Message*) self_;

			Array msgArgs( self->_msgArgs.size() );
			Array& _msgArgs = self->_msgArgs;

			switch (self->_msgArgs.size())
			{
				case 5: msgArgs[4] = _msgArgs[4].eval( receiver, args, context );
				case 4: msgArgs[3] = _msgArgs[3].eval( receiver, args, context );
				case 3: msgArgs[2] = _msgArgs[2].eval( receiver, args, context );
				case 2: msgArgs[1] = _msgArgs[1].eval( receiver, args, context );
				case 1: msgArgs[0] = _msgArgs[0].eval( receiver, args, context );
			}

			//Value result = receiver.send( _selector.get<Symbol>(), msgArgs, context );
			//Value result = receiver.send( (Symbol&) _selector.getFrame(), msgArgs, context );
			return receiver.send( self->_selectorId, msgArgs, context );
		}
	
//	 protected:

		Symbol&  _selector;
		SymbolId _selectorId;
		Array    _msgArgs;

	};

 //
 // class MulMessage
 //

	class MulMessage : public Message {

	 public:

		MulMessage( const Array& msgArgs )
		 : Message( mul, msgArgs ) { _eval2 = &eval2; }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			if (_msgArgs[0].getFloat() != numeric_limits<float>::max())
			{
				return receiver.getFloat() * _msgArgs[0].getFloat();
			}
			
			return Message::eval( receiver, args, context );
		}

		static inline Value eval2( Frame* self_, Value receiver, const Array& args, Value context )
		{
			Message* self = (Message*) self_;
			
			//if (self->_msgArgs[0].getFloat() < numeric_limits<float>::max())
			{
				return receiver.getFloat() * self->_msgArgs[0].getFloat();
			}
			
			return Message::eval2( self, receiver, args, context );
		}

	};
	
}

#endif

