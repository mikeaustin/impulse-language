//
// core/protos/expression.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_EXPRESSION_H
#define IMPULSE_EXPRESSION_H

#include "core/frame.h"
#include "core/array.h"

#include "message.h"

namespace impulse {

 //
 // class Function
 //

	class Expression : public Frame {

	 public:

		Expression() { }

		Expression(const Expression& expression)
		{
			_messages = expression._messages;
		}

		size_t size() { return _messages.size(); }

		void push_back( Value message )
		{
			_messages.push_back( message );
		}

		void concat( Expression& expression )
		{
			_messages.insert( _messages.end(), expression._messages.begin(), expression._messages.end() );
		}

		Value operator []( size_t index ) { return _messages.at( index ); }

		virtual Value apply( Value receiver, const Array& args, Value locals ) const
		{
 			ENTER( "Expression::apply( receiver = " << receiver << " )" );

			vector<GCValue>::const_iterator message = _messages.begin();

			if (message == _messages.end())
			{
				return Value();
			}

			while (message != _messages.end())
			{
				receiver = message->apply( receiver, args, locals );
		
				++message;
			}
			
			LEAVE( receiver );
			
			return receiver;
		}
	
	 private:

		vector<GCValue> _messages;

	};

}

#endif

