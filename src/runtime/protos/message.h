//
// runtime/protos/message.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_MESSAGE_H
#define IMPULSE_MESSAGE_H

namespace impulse {

 //
 // class Message
 //

	class Message : public Frame {

	 public:

		Message( Symbol& selector, const Array& msgArgs )
		 : _selector( selector ), _msgArgs( msgArgs ) { }

		virtual string inspect( Value receiver ) const { return "<message>"; }

		inline Value eval( Value receiver, const Array& args, Value context )
		{
			ENTER( "Message::eval( receiver = " << receiver.inspect() << "," <<
								" _selector = " << _selector.inspect(_selector) << " )" );

			Array msgArgs( _msgArgs.size() );
			
			switch (_msgArgs.size())
			{
				case 5: msgArgs[4] = _msgArgs[4].eval( context, args, context );
				case 4: msgArgs[3] = _msgArgs[3].eval( context, args, context );
				case 3: msgArgs[2] = _msgArgs[2].eval( context, args, context );
				case 2: msgArgs[1] = _msgArgs[1].eval( context, args, context );
				case 1: msgArgs[0] = _msgArgs[0].eval( context, args, context );
				case 0: ;
			}
			
			//Value result = receiver.send( (Symbol&) _selector.getFrame(), msgArgs, context );
			Value result = receiver.send( _selector, msgArgs, context );
			
			LEAVE( result.inspect() );
			
			return result;
		}

		Array evalArgs( Value context )
		{
			Array msgArgs( _msgArgs.size() );
			Array args;

			switch (_msgArgs.size())
			{
				case 5: msgArgs[4] = _msgArgs[4].eval( context, args, context );
				case 4: msgArgs[3] = _msgArgs[3].eval( context, args, context );
				case 3: msgArgs[2] = _msgArgs[2].eval( context, args, context );
				case 2: msgArgs[1] = _msgArgs[1].eval( context, args, context );
				case 1: msgArgs[0] = _msgArgs[0].eval( context, args, context );
				case 0: ;
			}
			
			return msgArgs;
		}

//	 protected:

		Symbol& _selector;
		Array   _msgArgs;

	};

 //
 // class MulMessage
 //

	class MulMessage : public Message {

	 public:

		MulMessage( const Array& msgArgs )
		 : Message( Symbol::at( "*" ), msgArgs ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			if (_msgArgs[0].getFloat() != numeric_limits<double>::max())
			{
				return receiver.getFloat() * _msgArgs[0].getFloat();
			}
			
			return Message::eval( receiver, args, context );
		}

	};

	class ConstMulMessage : public Message {

	 public:

		ConstMulMessage( const double value, const Array& msgArgs )
		 : Message( Symbol::at( "*" ), msgArgs ), _value( value ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			return receiver.getFloat() * _value;
		}

	 private:
	
		double _value;

	};

 //
 // class AssignMessage
 //

	class AssignMessage : public Message {

	 public:

		AssignMessage( Symbol& name, const Array& msgArgs )
		 : Message( Symbol::at( "=" ), msgArgs ), _name( name ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			Value expr = _msgArgs[0].eval( receiver, args, context );

			return receiver.setSlot( _name, expr );
		}
		
	 private:
	
		Symbol& _name;

	};

 //
 // class StringMessage
 //

	class StringMessage : public Message {

	 public:

		StringMessage( const Array& msgArgs )
		 : Message( Symbol::at( "string" ), msgArgs ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			String& string_ = *new String( _msgArgs[0].get<String>().getValue() );

			autorelease( string_ );

			return string_;
		}

	};

 //
 // class ArrayMessage
 //

	class ArrayMessage : public Message {

	 public:

		ArrayMessage( const Array& msgArgs )
		 : Message( Symbol::at( "array" ), msgArgs ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			Array msgArgs = evalArgs( context );

			GCArray& values = *new ArrayValue( msgArgs );
			//values._proto = &Array::instance();

			autorelease( values );

			return values;
		}

	};

 //
 // class BlockMessage
 //

	class BlockMessage : public Message {

	 public:

		BlockMessage( const Array& msgArgs, Frame& body )
		 : Message( Symbol::at( "array" ), msgArgs ), _body( body ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			Lambda& block = *new Lambda( _msgArgs, _body, context );

			autorelease( block );
			
			return block;
		}

	 private:
	 
	 	Frame& _body;

	};
	
}

#endif

