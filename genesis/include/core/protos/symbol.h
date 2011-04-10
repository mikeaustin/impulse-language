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
 
	class SymbolProto : public Frame {

	 public:

	 //
	 // Typedefs
	 //

		typedef std::map<string, Value> SymbolMap;
		
		const static SymbolId ADD = 100;
		const static SymbolId MUL = 101;
		const static SymbolId POW = 102;

	 //
	 // Methods
	 //

		string getName() const { return _name; }

		SymbolId getId() const { return _id; }
		
		static const Symbol at( const string name )
		{
			SymbolMap::iterator iter = _symbolMap.find( name );
			
			if (iter != _symbolMap.end())
			{
				return (*iter).second.get<SymbolProto>();
			}

			return *new SymbolProto( name );
		}

	 //
	 // Inspection
	 //

		virtual string inspect( const Value self ) const
		{
			SymbolProto& symbol = *dynamic_cast<SymbolProto*>( &self.getFrame() );
			
			return "'" + symbol.getName();
			
			//std::ostringstream stream; stream << "symbol:" << symbol.getName();

			//return Frame::inspect( self, stream.str() );
		}

		SymbolProto( SymbolId id ) : _name( "anonymous" ), _id( id ) { }

	 private:

	 	SymbolProto( string name ) : _name( name ), _id( _nextId++ )
	 	{
	 		_symbolMap[name] = Value( *this );
	 	}


		string   _name;
		SymbolId _id;

		static SymbolId  _nextId;
		static SymbolMap _symbolMap;

	};

	typedef SymbolProto &Symbol;

}

#endif

