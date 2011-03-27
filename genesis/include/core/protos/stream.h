//
// core/protos/stream.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_STREAM_H
#define IMPULSE_STREAM_H

#include <cstdio>

#include "core/frame.h"
#include "core/array.h"

#include "core/protos/symbol.h"
#include "core/protos/block.h"
#include "core/protos/method.h"
#include "core/protos/object.h"

namespace impulse {

 //
 // class StreamProto
 //

	class StreamProto : public Frame {

	 public:

		enum Mode { ReadOnly, ReadWrite, OverWrite, Append };

	 //
	 // Lifecycle
	 //

		static StreamProto& instance()
		{
			static StreamProto stream;

			return stream;
		}

		static StreamProto& create( FILE* file ) { return *new StreamProto( file ); }

		static void initSlots()
		{
/*			static std::vector<ArgType> powArgs, sinArgs;
			powArgs.push_back( ArgType( SymbolProto::at( "n" ), NumberProto::instance() ) );

			//instance().setSlot( "pow", *new MethodProto( *new BlockProto<NumberProto>( instance(), &NumberProto::pow2_, powArgs ) ) );
			instance().setSlot( "pow", *new MethodProto2( &pow_, powArgs ) );
			//instance().setSlot( "sin", *new MethodProto( *new BlockProto<NumberProto>( instance(), &NumberProto::sin, sinArgs ) ) );
			instance().setSlot( "sin", *new MethodProto2( &sin, sinArgs ) );*/
		}

		FILE* getFile() { return _file; }

	 //
	 // Inspection
	 //

		virtual string inspect( const Value self ) const;

	 //
	 // Methods
	 //

		StreamProto& operator << ( double value ) { if (_file) fprintf( _file, "%f", value ); return *this; }

		//static Value pow_( Value self, const Array& args );
		//Value pow2_( Value self, const Array& args, Value locals );
		//static Value sin( Value self, const Array& args );

	 private:

	 
		StreamProto()             : Frame( ObjectProto::instance() ), _file( NULL ) { }
		StreamProto( FILE* file ) : Frame( ObjectProto::instance() ), _file( file ) { }
		StreamProto( const string name, Mode mode = ReadOnly )
		{
			const char* fmode = "";
			
			switch ( mode )
			{
				case ReadOnly:  fmode = "r"; break;
				case ReadWrite: fmode = "r+"; break;
				case OverWrite: fmode = "w"; break;
				case Append:    fmode = "a"; break;
			}
			
			_file = fopen( name.c_str(), fmode );
		}

		FILE* _file;

	};
	
}

#endif

