/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 **/


#ifndef DONUT_COMMON
#define DONUT_COMMON


// Includes
#include <assert.h>
#include <iostream>
#include <cstdio>
#include "DebugPrinters.h"


namespace Donut
{
 	
#ifdef DEBUG
	#define AssertNoRelease(Enonce) assert(Enonce)
#elif RELEASE 
	#define AssertNoRelease(Enonce) {}
#endif

	#define AssertRelease(Enonce) assert(Enonce)

	#define CondAssertReleasePrint(Enonce,msg) if(!Enonce) {std::cout<< msg <<std::endl;assert(Enonce);}

#ifdef DEBUG
	#define AssertNoReleasePrint(Enonce, Msg) if(!Enonce)  {std::cout<< Msg <<std::endl; assert(Enonce);}
#else
	#define AssertNoReleasePrint(Enonce, Msg) {}
#endif

#ifdef DEBUG
	template<class T> inline void checked_delete(T * x)
 	{
 		typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
 		(void) sizeof(type_must_be_complete);
 		delete x;
 	}
#endif
 	
#ifdef RELEASE
	template<class T> inline void checked_delete(T * x)
 	{
 		delete x;
 	}
#endif


	#define foreach(IT,X) for ( typeof( X.begin() ) IT = X.begin(); IT != X.end(); ++IT )
	#define tryget(IT,LIST, ELEM) typeof( LIST.begin()) IT = LIST.find(ELEM);
}
#endif
