//
// core/protos/lambda.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_LAMBDA_H
#define IMPULSE_LAMBDA_H

#include "../../core/protos/object.h"

namespace impulse {

 //
 // class Lambda
 //

	class Lambda : public Frame {

	 public:

		Lambda( Frame& proto ) : Frame( proto ), _body( Void::instance() ) { }
		Lambda( Array& args, Frame& body, Value context ) : Frame( Lambda::instance() ),
		 _args( args ), _body( body ), _context( context ) { _eval3 = &eval3; }

		void initSlots()
		{
			setSlot( Symbol::at( "[]" ), *new Method( slice_, -1 ) );
		}

		virtual string inspect( Value receiver ) const { return "<block>"; }

		static Frame& instance()
		{
			static GCValue lambda = *new Lambda( Object::instance() );
			
			return lambda.getFrame();
		}

		GCValue eval_( Value receiver, const Array& args, Value context )
		{
			ENTER( "Block::eval_( receiver = " << receiver.inspect() << " )" );

			Array blockArgs;

			for (unsigned int i = 0; i < _args.size(); ++i)
			{
				context.setSlot( (Symbol&) _args[i].getFrame(), args[i] );
			}

			Value result = _body.eval( context, blockArgs, context );			
			
			LEAVE( "" );
			
			return result;
		}

		static Value slice_( Value receiver, const Array& args )
		{
			Lambda& self  = receiver.get<Lambda>();

			Value context = *new Frame( self._context.getFrame() );

			return self.eval_( receiver, args, context );
		}

		inline static Value eval3( Frame* self_, const int range, Value receiver, const Array& args, Value context )
		{
			ENTER( "Block::eval3( receiver = " << receiver.inspect() << " )" );

			Lambda* self = (Lambda*) self_;

			for (int i = 1; i < range - 1; ++i)
			{
				self->_body.eval( receiver, args, context );			
			}

			Value result = self->_body.eval( receiver, args, context );			

			LEAVE( "" );
			
			return result;
		}
	
	 private:

		Array  _args;
		Frame& _body;
		Value  _context;

	};
	
}

#endif

