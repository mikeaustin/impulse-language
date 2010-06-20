//
// symbol.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

namespace impulse {

 //
 // class Symbol
 //

	class Symbol : public Frame {

	 public:
	 
	 	Symbol( string name ) : _name( name ), _id( _nextId++ ) { }
	 
		virtual string toString( Value receiver ) const { return string("'") + _name; }

		SymbolId getId() const { return _id; }

	 private:

		string    _name;
		SymbolId  _id;

		static SymbolId _nextId;

	};

	SymbolId Symbol::_nextId = 0;

	Symbol& mul = *new Symbol( "*" );
	Symbol& add = *new Symbol( "+" );
	Symbol& x   = *new Symbol( "x" );
	//GCValue mul_( mul );
	//GCValue add_( add );

}

