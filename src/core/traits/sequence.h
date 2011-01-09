//
// core/traits/sequence.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_SEQUENCE
#define IMPULSE_SEQUENCE

namespace impulse {

 //
 // class Sequence
 //

	template <typename T>
	class Sequence : public Frame {
	
	 public:

		Sequence();
		Sequence( const size_t size );
		Sequence( Frame& proto, const size_t size );

		static Value each_( Value receiver, const Array& args, Value context );

	 private:
	
		vector<T> _array;

	};

	template <typename T> Sequence<T>::Sequence() { }
	template <typename T> Sequence<T>::Sequence( size_t size ) : _array( size ) { }
	template <typename T> Sequence<T>::Sequence( Frame& proto, size_t size = 0 ) : Frame( proto ) { }

	template <typename T> Value Sequence<T>::each_( Value receiver, const Array& args, Value context )
	{
		Sequence& self = receiver.get<Sequence>();
		Block& block  = args[0].get<Block>();

		Array blockArgs( 1 );

		typename vector<T>::iterator iter = self._array.begin();
		
		while (iter != self._array.end())
		{
			blockArgs[0] = *iter;
			
			block.evalLoop_( receiver, blockArgs, context );
			
			++iter;
		}
		
		return Value();
	}

}

#endif

