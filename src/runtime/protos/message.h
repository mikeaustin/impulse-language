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

		virtual string inspect( const Value receiver ) const { return "<message>"; }

		inline Value eval( Value receiver, const Array& args, Value context )
		{
			ENTER( "Message::eval( receiver = " << receiver.inspect() <<
							   ", _selector = " << _selector.inspect(_selector) <<
								  " context = " << context.inspect() << " )" );

			Array msgArgs( _msgArgs.size() );

			for (unsigned int i = 0, argsSize = _msgArgs.size(); i < argsSize; ++i)
			{
				msgArgs[i] = _msgArgs[i].eval( context, args, context );
			}

			Value result = receiver.send( _selector, msgArgs, context );
			
			LEAVE( result.inspect() );
			
			return result;
		}

		Array evalArgs( Value context )
		{
			Array msgArgs( _msgArgs.size() );
			Array args;

			for (unsigned int i = 0, argsSize = _msgArgs.size(); i < argsSize; ++i)
			{
				msgArgs[i] = _msgArgs[i].eval( context, args, context );
			}
			
			return msgArgs;
		}

//	 protected:

		Symbol& _selector;
		Array   _msgArgs;

	};

 //
 // class <Operator>Message
 //

#define OPERATOR_MESSAGE( message, op ) \
	class message##Message : public Message { \
	 public: \
		message##Message( const Array& msgArgs ) \
		 : Message( Symbol::at( #op ), msgArgs ) { } \
		virtual Value eval( Value receiver, const Array& args, Value context ) { \
			if (&receiver.getFrame() == &NumberValue::instance()) { \
				Array msgArgs = evalArgs( context ); \
				if (&msgArgs[0].getFrame() == &NumberValue::instance()) { \
					return receiver.getFloat() op msgArgs[0].getFloat(); \
				} \
			} \
			return Message::eval( receiver, args, context ); \
		} \
	}; \
	class Const##message##Message : public Message { \
	 public: \
		Const##message##Message( const double value, const Array& msgArgs ) \
		 : Message( Symbol::at( #op ), msgArgs ), _value( value ) { } \
		virtual Value eval( Value receiver, const Array& args, Value context ) { \
			if (&receiver.getFrame() == &NumberValue::instance()) { \
				return receiver.getFloat() op _value; \
			} \
			return Message::eval( receiver, args, context ); \
		} \
	 private: \
		double _value; \
	}

	OPERATOR_MESSAGE( Mul,   * );
	OPERATOR_MESSAGE( Div,   / );
	OPERATOR_MESSAGE( Add,   + );
	OPERATOR_MESSAGE( Sub,   - );
	
	OPERATOR_MESSAGE( Equal, == );
	OPERATOR_MESSAGE( LessThan, < );
	OPERATOR_MESSAGE( LessEqual, <= );
	OPERATOR_MESSAGE( GreaterThan, > );
	OPERATOR_MESSAGE( GreaterEqual, >= );

 //
 // class ModMessage
 //

	class ModMessage : public Message {

	 public:

		ModMessage( const Array& msgArgs )
		 : Message( Symbol::at( "%" ), msgArgs ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			if (&receiver.getFrame() == &NumberValue::instance()) {
				Array msgArgs = evalArgs( context );
				if (&msgArgs[0].getFrame() == &NumberValue::instance()) {
					return (int) receiver.getFloat() % (int) msgArgs[0].getFloat();
				}
			}
			
			return Message::eval( receiver, args, context );
		}

	};

	class ConstModMessage : public Message {

	 public:

		ConstModMessage( const double value, const Array& msgArgs )
		 : Message( Symbol::at( "%" ), msgArgs ), _value( value ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			if (&receiver.getFrame() == &NumberValue::instance()) {
				return (int) receiver.getFloat() % (int) _value;
			}
			
			return Message::eval( receiver, args, context );
		}

	 private:
	
		double _value;

	};

 //
 // TernaryMessage
 //
	
	class TernaryMessage : public Message {

	 public:

		TernaryMessage( const Array& msgArgs )
		 : Message( Symbol::at( "?" ), msgArgs ) { }

		Value eval( Value receiver, const Array& args, Value context );

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
			Frame& blockContext = *new Frame( context.getFrame() );
			
			autorelease( blockContext );
			
			Block& block = *new Block( _msgArgs, _body, blockContext );

			autorelease( block );
			
			return block;
		}

	 private:
	 
	 	Frame& _body;

	};
/*
	class ExpressionMessage : public Message {

	 public:

		ExpressionMessage( const Array& msgArgs, Frame& expression )
		 : Message( Symbol::at( "expression" ), msgArgs ), _expression( expression ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			return _expression;
		}

	 private:
	 
	 	Frame& _expression;

	};
*/
 //
 // class LocalMessage
 //

	class LocalMessage : public Message {

	 public:

		LocalMessage( int index, const Array& msgArgs )
		 : Message( Symbol::at( "local" ), msgArgs ), _index( index ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			/*if (_index > context.getFrame()._locals.size() - 1)
			{
				cerr << "*** Local out of range" << endl;
				
				return Value();
			}*/
			
			Value result = context.getFrame()._locals[_index];

			return result;
		}

	 private:
	 
		unsigned int _index;

	};

	class SliceMessage : public Message {

	 public:

		SliceMessage( const Array& msgArgs )
		 : Message( Symbol::at( "[]" ), msgArgs ) { }

		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			//cout << "SlideMessage::eval( receiver = " << receiver << ", " << "context = " << context.inspect() << " )" << endl;

			if (&receiver.getProto() == &Block::instance()) {
				Array msgArgs = evalArgs( context );
				
				//return Block::slice_( receiver, msgArgs, context );
				return Block::slice_( receiver, msgArgs, args[0] );
			}
			
			return Message::eval( receiver, args, context );
		}

	};
	
	class SelfMessage : public Message {
	
	 public:
	
		SelfMessage()
		 : Message( Symbol::at( "self" ), Array() ) { }
		 
		virtual Value eval( Value receiver, const Array& args, Value context )
		{
			return args[0];
		}

	};

}

#endif

