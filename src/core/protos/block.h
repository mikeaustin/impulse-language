//
// core/protos/block.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_BLOCK_H
#define IMPULSE_BLOCK_H

#include "object.h"

namespace impulse {

 //
 // class Block
 //

	class Block : public Invokable {

		typedef Value (* const Function)(Value, const Array&, Value context);

	 public:

		Block( Frame& proto ) : Invokable( proto ), _function( NULL ), _body( Void::instance() ) { }
		Block( const Array& args, Frame& body, Value context ) : Invokable( Block::instance() ),
		   _args( args ), _function( NULL ), _body( body ), _context( context ) { _eval3 = &eval3; }
		Block( Function function ) : Invokable( Block::instance() ),
		 _function( function ), _body( Void::instance() ), _context() { _eval3 = &eval3; }

		void initSlots()
		{
			setSlot( Symbol::at( "[]" ), *new Method( "[]", *new Block( slice_ ), -1 ) );
			setSlot( Symbol::at( "arity" ), *new Method( "arity", *new Block( arity ), 0 ) );
		}

		virtual string inspect( const Value receiver ) const { return "<block>"; }

		static Frame& instance()
		{
			static GCValue block = *new Block( Object::instance() );
			
			return block.getFrame();
		}

		int arity()
		{
			return _args.size();
		}

		Value eval_( Value receiver, const Array& args, Value context /* hide */ )
		{
			ENTER( "Block::eval_( receiver = " << receiver.inspect() << " )" );
/* Move method arg checking here
			if (args.size() != _args.size())
			{
				cerr << "*** Wrong number of arguments" << endl;
				
				return Value();
			}
*/
			if (_function)
			{
				Value result = _function( receiver, args, _context );

				LEAVE( "" );
		
				return result;
			}

			const unsigned int argsSize = _args.size();
			static Array bodyArgs;
			bodyArgs.push_back( context );
			
			Frame& blockContext = *new Frame( _context.getFrame() );
			blockContext._cache[0] = _context.getFrame()._cache[0];
			//Frame& blockContext = _context.getFrame();

			for (unsigned int i = 0; i < argsSize; ++i)
			{
				if (localsAccessEnabled)
					blockContext._locals.push_back( args[i] );

				blockContext.setSlot( _args[i].get<Symbol>(), args[i] );
			}

			//Value result = _body.eval( blockContext, bodyArgs, blockContext );
			Value result = _body.eval( receiver, bodyArgs, blockContext );

			blockContext.decRef();

			LEAVE( "" );
		
			return result;
		}

		Value evalLoop_( Value receiver, const Array& args, Value context )
		{
			ENTER( "Block::eval_( receiver = " << receiver.inspect() << " )" );

			if (_function)
			{
				Value result = _function( receiver, args, context );

				LEAVE( "" );
		
				return result;
			}
			
			const unsigned int argsSize = _args.size();
			const static Array bodyArgs;

			for (unsigned int i = 0; i < argsSize; ++i)
			{
				if (localsAccessEnabled)
					_context.getFrame()._locals.push_back( args[i] );

				_context.setSlot( (Symbol&) _args[i].getFrame(), args[i] );
			}

			Value result = _body.eval( _context, bodyArgs, _context );

			_context.getFrame()._locals.clear();

			LEAVE( "" );
		
			return result;
		}

		static Value slice_( Value receiver, const Array& args, Value context )
		{
			Block& self  = receiver.get<Block>();

			//Value context = *new Frame( self._context.getFrame() );
			//Value context = self._context;

			Value result = self.eval_( receiver, args, context );
			
			return result;
		}

		inline static Value eval3( Frame* self_, const int range, Value receiver, const Array& args, Value context )
		{
			ENTER( "Block::eval3( receiver = " << receiver.inspect() << " )" );

			Block* self = (Block*) self_;
			const static Array bodyArgs;

			for (int i = 1; i < range; ++i)
			{
				for (unsigned int i = 0; i < self->_args.size(); ++i)
				{
					context.setSlot( (Symbol&) self->_args[i].getFrame(), args[i] );
				}

				self->_body.eval( receiver, bodyArgs, context );			
			}

			LEAVE( "" );
			
			return Value();
		}

		static Value arity( Value receiver, const Array& args, Value context )
		{
			Block& self  = receiver.get<Block>();
			
			return self._args.size();
		}
		
	 private:

		Array    _args;
		Function _function;
		Frame&   _body;
		GCValue  _context;

	};
	
}

#endif

