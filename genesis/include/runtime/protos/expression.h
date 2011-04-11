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

		size_t size() { return _messages.size(); }

		void push( Value message )
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
			vector<GCValue>::const_iterator message = _messages.begin();

			while (message != _messages.end())
			{
				receiver = message->apply( receiver, args, locals );
		
				++message;
			}
			
			return receiver;
		}
	
	 private:

		vector<GCValue> _messages;

	};

}

#endif

