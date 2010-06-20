//
// impulse.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifdef DEBUG
 const char spaces[] = "                                                            ";
 int        indent = 0;

 #define ENTER( expr ) cout << &spaces[sizeof(spaces) - indent] << "> " << expr << endl; indent += 2
 #define LEAVE( expr ) indent -= 2; cout << &spaces[sizeof(spaces) - indent] << "< " << expr << endl
#else
 #define ENTER( expr )
 #define LEAVE( expr )
#endif

