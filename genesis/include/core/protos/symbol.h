//
// core/protos/symbol.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_SYMBOL_H
#define IMPULSE_SYMBOL_H

#include "core/frame.h"

namespace impulse {

 //
 // class SymbolProto
 //

	typedef std::map<string, SymbolProto*> SymbolMap;
 
	class SymbolProto : public Frame {

	 public:

	 	SymbolProto( string name ) : _name( name ), _id( _nextId++ )
	 	{
	 		_symbolMap[name] = this;
	 	}
	 
		string getName() { return _name; }

		SymbolId getId() { return 0; }
		
		static const Symbol at( const string name )
		{
			SymbolMap::iterator iter = _symbolMap.find( name );
			
			if (iter != _symbolMap.end())
			{
				return *(*iter).second;
			}
			
			return *new SymbolProto( name );
		}

		virtual string inspect( const Value receiver ) const
		{
			SymbolProto& symbol = *dynamic_cast<SymbolProto*>( &receiver.getFrame() );
			
			std::ostringstream stream;
			
			stream << "<symbol:" << symbol.getName() << "@" << this << ">";
			
			return stream.str();
		}

	 private:

		string    _name;
		SymbolId  _id;

		static SymbolId  _nextId;
		static SymbolMap _symbolMap;

	};

	typedef SymbolProto &Symbol;

}

#endif

