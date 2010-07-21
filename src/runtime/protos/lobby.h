//
// runtime/protos/lobby.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_LOBBY_H
#define IMPULSE_LOBBY_H

#include "../../core/protos/object.h"

namespace impulse {

 //
 // class Lobby
 //

	class Lobby : public Frame {

	 public:

		Lobby() : Frame( Object::instance() )
		{
			setSlot( Symbol::at( "print:" ), *new Method( "print:", print_, 1 ) );
			setSlot( Symbol::at( "help" ),   *new Method( "help:",  help,   0 ) );
			setSlot( Symbol::at( "exit" ),   *new Method( "exit",   exit,   0 ) );
		}

		virtual string inspect( Value receiver ) const { return "<lobby>"; }

		static Frame& instance()
		{
			static GCValue lobby = *new Lobby();
			
			return lobby.getFrame();
		}

		static Value print_( Value receiver, const Array& args, Value context )
		{
			cout << args[0] << endl;
			
			return Value();
		}

		static Value help( Value receiver, const Array& args, Value context )
		{
			cout << "No help yet" << endl;
			
			return Value();
		}

		static Value exit( Value receiver, const Array& args, Value context )
		{
			exitMainLoop = true;
			
			return Value();
		}
		
	};

}

#endif

