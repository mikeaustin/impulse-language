//
// core/protos/lambda.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_LAMBDA_H
#define IMPULSE_LAMBDA_H

namespace impulse {

 //
 // class Lambda
 //

	class Lambda : public Frame {

	 public:

		Lambda( Frame& expressions )
		 : _expressions( expressions ) { _eval2 = &eval2; }

		virtual string toString( Value receiver ) const { return "[Block]"; }

		inline Value eval( Value receiver, const Array& args, Value context )
		{
			ENTER( "Block::eval( receiver = " << receiver.toString() << " )" );

			Value result = _expressions.eval( receiver, args, context );			
			
			LEAVE( "" );
			
			return result;
		}

		inline static Value eval2( Frame* self_, Value receiver, const Array& args, Value context )
		{
			ENTER( "Block::eval( receiver = " << receiver.toString() << " )" );

			Lambda* self = (Lambda*) self_;

			Value result = (self->_expressions._eval2)( &self->_expressions, receiver, args, context );			
			
			LEAVE( "" );
			
			return result;
		}
	
	 private:

		Frame& _expressions;

	};
	
}

#endif

