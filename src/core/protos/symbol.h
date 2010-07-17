//
// core/protos/symbol.h
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
	 
	 	Symbol( string name ) : _name( name ), _id( _nextId++ )
	 	{
	 		incRef();
	 		
	 		_slotMap[name] = this;
	 	}
	 
		virtual string inspect( Value receiver ) const { return string("'") + _name; }

		SymbolId getId() const { return _id; }

		static Symbol& at( string name )
		{
			map<string, Symbol*>::iterator iter = _slotMap.find( name );
			
			if (iter != _slotMap.end())
			{
				return *(*iter).second;
			}
			
			return *new Symbol( name );
		}

	 private:

		string    _name;
		SymbolId  _id;

		static SymbolId _nextId;
		static map<string, Symbol*> _slotMap;

	};

	SymbolId Symbol::_nextId = 0;
	map<string, Symbol*> Symbol::_slotMap;

}

