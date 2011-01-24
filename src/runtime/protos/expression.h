//
// runtime/protos/expression.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_EXPRESSION_H
#define IMPULSE_EXPRESSION_H

namespace impulse {

 //
 // class Expression
 //

	class Expression : public Frame {

	 public:

		Expression( vector<Value> messages )
		 : _messages( messages ), _nextExpr( NULL ) { }

		Expression( const Expression& expression )
		 : _messages( expression._messages ), _nextExpr( expression._nextExpr ) { }

		Expression()
		 : _nextExpr( NULL ) { }

		Expression* setNextExpr( Expression& nextExpr ) { return _nextExpr = &nextExpr; }

		virtual string inspect( const Value receiver ) const { return "<expression>"; }

		inline Value eval( Value receiver_ /* hide */, const Array& args, Value context )
		{
			ENTER( "Expression::eval( receiver = " << receiver_.inspect() << "," <<
									" context = " << context.inspect() << " )" );

			Expression* expr = this;
			Value receiver;

			while (expr)
			{
				receiver = context;

				for (vector<Value>::iterator iter = expr->_messages.begin(); iter != expr->_messages.end(); ++iter)
				{
					receiver = iter->eval( receiver, args, context );
				}
				
				expr = expr->_nextExpr;
			}

			LEAVE( receiver.inspect() );
			
			return receiver;
		}
	
	 private:

		vector<Value> _messages;
		Expression*   _nextExpr;

	};
	
}

#endif

