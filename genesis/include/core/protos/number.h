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

namespace impulse {

 //
 // class NumberProto
 //

	class NumberProto : public Frame {

	 public:

	 //
	 // Lifecycle
	 //

		NumberProto() { TRACE( "NumberProto::NumberProto()" ); }

		static NumberProto& instance()
		{
			ENTER( "NumberProto::instance()" );

			static NumberProto number;

			LEAVE( "NumberProto::instance()" );

			return number;
		}

		static void initSlots()
		{
			static std::vector<ArgType> argtypes;
			argtypes.push_back( ArgType( SymbolProto::at( "n" ), NumberProto::instance() ) );

			instance().setSlot( "pow", *new BlockProto<NumberProto>( instance(), &NumberProto::pow_, argtypes ) );
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
			return std::pow( self.getFloat(), args[Index::_0].getFloat() );
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

