//
// core/protos/number.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_NUMBER_H
#define IMPULSE_NUMBER_H

#include "core/frame.h"

namespace impulse {

 //
 // class NumberProto
 //

	class NumberProto : public Frame {

	 public:

		NumberProto() { TRACE( "NumberProto::NumberProto()" ); }

		static Frame& instance()
		{
			ENTER( "NumberProto::instance()" );

			static NumberProto number;

			LEAVE( "NumberProto::instance()" );

			return number;
		}

		static void initSlots()
		{
			instance().setSlot( "x", 20 );
		}

		virtual string inspect( const Value receiver ) const
		{
			return "<number>";
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

		virtual string inspect( const Value receiver ) const
		{
			std::ostringstream stream;
			
			stream << receiver.getFloat();
			
			return stream.str();
		}
	
	};
	
}

#endif

