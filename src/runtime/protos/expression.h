//
// runtime/protos/expression.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_EXPRESSION_H
#define IMPULSE_EXPRESSION_H

namespace runtime {

 //
 // class Expression
 //

	class Expression : public Frame {

	 public:

		Expression( vector<Value> messages )
		 : _messages( messages ), _nextExpr( NULL ) { _eval2 = &eval2; }

		void setNextExpr( Expression& nextExpr ) { _nextExpr = &nextExpr; }

		virtual string toString( Value receiver ) const { return "[Expression]"; }

		inline Value eval( Value receiver, const Array& args, Value context )
		{
			ENTER( "Expression::eval( receiver = " << receiver.toString() << " )" );

			Value value;
			
			for (vector<Value>::iterator iter = _messages.begin(); iter != _messages.end(); ++iter)
			{
				value = (*iter).eval( value, args, context );
			}
			
			LEAVE( "" );
			
			return value;
		}

		inline static Value eval2( Frame* self_, Value receiver, const Array& args, Value context )
		{
			ENTER( "Expression::eval( receiver = " << receiver.toString() << " )" );

			Expression* self = (Expression*) self_;
			Value value = receiver;

			while (self)
			{
				for (vector<Value>::iterator iter = self->_messages.begin(); iter != self->_messages.end(); ++iter)
				{
					value = (*iter).eval( value, args, context );
				}
				
				self = self->_nextExpr;
			}
						
			LEAVE( "" );
			
			return value;
		}
	
	 private:

		vector<Value> _messages;
		Expression*   _nextExpr;

	};
	
}

#endif

