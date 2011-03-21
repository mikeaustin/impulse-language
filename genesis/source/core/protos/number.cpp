//
// core/protos/number.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#include "core/frame.h"
#include "core/protos/number.h"

namespace impulse {

 //
 // Inspection
 //

	string NumberProto::inspect( const Value self ) const
	{
		return Frame::inspect( self, "number" );
	}

 //
 // Methods
 //

	Value NumberProto::pow_( Value self, const Array& args )
	{
		ENTER( "NumberProto::pow_( self = " << self << ", args[0] = " << args[Index::_0] << " )" );
		
		Value result = std::pow( self.getFloat(), args[Index::_0].getFloat() );
		
		LEAVE( result );
		
		return result;
	}

	Value NumberProto::sin( Value self, const Array& args )
	{
		return std::sin( self.getFloat() );
	}

}

