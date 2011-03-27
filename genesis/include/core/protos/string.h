//
// core/protos/string.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_STRING_H
#define IMPULSE_STRING_H

#include "core/frame.h"
#include "core/array.h"

#include "core/protos/symbol.h"
#include "core/protos/block.h"
#include "core/protos/method.h"
#include "core/protos/object.h"

namespace impulse {

 //
 // class StringProto
 //

	class StringProto : public Frame {

	 public:

	 //
	 // Lifecycle
	 //

		StringProto() : Frame( ObjectProto::instance() ) { }

		static StringProto& instance()
		{
			static StringProto string_;

			return string_;
		}

		static StringProto& create( string value ) { return *new StringProto( value ); }

		static void initSlots()
		{
/*			static std::vector<ArgType> powArgs, sinArgs;
			powArgs.push_back( ArgType( SymbolProto::at( "n" ), NumberProto::instance() ) );

			//instance().setSlot( "pow", *new MethodProto( *new BlockProto<NumberProto>( instance(), &NumberProto::pow2_, powArgs ) ) );
			instance().setSlot( "pow", *new MethodProto2( &pow_, powArgs ) );
			//instance().setSlot( "sin", *new MethodProto( *new BlockProto<NumberProto>( instance(), &NumberProto::sin, sinArgs ) ) );
			instance().setSlot( "sin", *new MethodProto2( &sin, sinArgs ) );*/
		}

		string getString() { return _string; }

	 //
	 // Inspection
	 //

		virtual string inspect( const Value self ) const;

	 //
	 // Methods
	 //

		//static Value pow_( Value self, const Array& args );
		//Value pow2_( Value self, const Array& args, Value locals );
		//static Value sin( Value self, const Array& args );

	 private:
	 
		StringProto( string string_ ) : _string( string_ ) { }

		string _string;

	};
	
}

#endif

