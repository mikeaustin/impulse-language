//
// core/protos/number.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_NUMBER_H
#define IMPULSE_NUMBER_H

#include <cmath>

#include "core/frame.h"
#include "core/array.h"

#include "core/protos/symbol.h"
#include "core/protos/block.h"
#include "core/protos/method.h"
#include "core/protos/object.h"

namespace impulse {

 //
 // class NumberProto
 //

	class NumberProto : public Frame {

	 public:

	 //
	 // Lifecycle
	 //

		NumberProto() : Frame( ObjectProto::instance() ) { }

		static NumberProto& instance()
		{
			static NumberProto number;

			return number;
		}

		static void initSlots()
		{
			static std::vector<ArgType> powArgs, sinArgs;
			powArgs.push_back( ArgType( SymbolProto::at( "n" ), NumberProto::instance() ) );

			instance().setSlot( "pow", *new MethodProto( *new BlockProto<NumberProto>( instance(), &NumberProto::pow_, powArgs ) ) );
			instance().setSlot( "sin", *new BlockProto<NumberProto>( instance(), &NumberProto::sin, sinArgs ) );
			
		}

	 //
	 // Inspection
	 //

		virtual string inspect( const Value self ) const
		{
			return Frame::inspect( self, "number" );
		}

	 //
	 // Methods
	 //

		Value pow_( Value self, const Array& args, Value locals )
		{
			ENTER( "NumberProto::pow_( self = " << self << ", args[0] = " << args[Index::_0] << " )" );
			
			Value result = std::pow( self.getFloat(), args[Index::_0].getFloat() );
			
			LEAVE( result );
			
			return result;
		}

		Value sin( Value self, const Array& args, Value locals )
		{
			return std::sin( self.getFloat() );
		}

	};

 //
 // class NumberValue
 //

	class NumberValue : public Frame {
	
	 public:
	 
	 	NumberValue() : Frame( NumberProto::instance() ) { }

		static Frame& instance()
		{
			static NumberValue number;
			
			return number;
		}

		virtual string inspect( const Value self ) const
		{
			std::ostringstream stream;
			
			stream << self.getFloat();
			
			return stream.str();
		}
	
	};
	
}

#endif

