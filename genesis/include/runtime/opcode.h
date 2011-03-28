
			if (&receiver.getFrame() == &NumberValue::instance())
			{
				Array msgArgs( getArgs()[0].apply( locals, args, locals ) ); msgArgs.self( args.self() );

				if (&msgArgs[Index::_0].getFrame() == &NumberValue::instance())
				{
					switch (getName().getId())
					{
						case SymbolProto::ADD: result = receiver.getFloat() + msgArgs[Index::_0].getFloat(); break;
						case SymbolProto::MUL: result = receiver.getFloat() * msgArgs[Index::_0].getFloat(); break;
						case SymbolProto::POW: result = std::pow( receiver.getFloat(), msgArgs[Index::_0].getFloat() ); break;
					}
				}
				else result = MessageProto::apply( receiver, args, locals );
			}
			else result = MessageProto::apply( receiver, args, locals );




	class Opcode : public Value {

	 public:

		Opcode( Value value ) : Value( value ) { }

		Value apply( Value receiver, Array& args, Value locals )
		{
			switch (_opcode)
			{
				case MESSAGE_ADD:  result = receiver.getFloat() + msgArgs[Index::_0].getFloat(); break;
				case MESSAGE_SEND: result = getFrame().apply( receiver, args, locals );
			}
		}

	 private:

		unsigned short _opcode;

	};

