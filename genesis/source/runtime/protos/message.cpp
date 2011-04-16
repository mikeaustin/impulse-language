//
// core/runtime/message.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#include "runtime/protos/message.h"

namespace impulse {

	template <Value (*operation)(Value, const Array&)>
	inline Value OperatorMessage<operation>::apply( Value receiver, const Array& args, Value locals ) const
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
	
}

