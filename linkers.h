// Financial Analytics Library (FINAL)
// Copyright (c) 2004 - 2012 by Marek Sestak, marek.sestak@gmail.com
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef linkersH
#define linkersH

#include <map>
#include "defs.h"
#include "calendar.h"

//---------------------------------------------------------------------------

namespace final {

class TDataSeries {
private:
	std::map< TDate, floating > dataseries;

public:
				TDataSeries() {}
virtual				~TDataSeries() {}

void				Add( const TDate& adate, floating avalue );
void				Clear();

virtual floating                GetValue( const TDate& adate ) const;
virtual floating                GetValue( int month, int year ) const;

}; // end of class TDataSeries

typedef enum { OATi, OATe, BTPSe, DBRe, GGBe, TII, SGBi, CANi, ACGBi } LinkerType;

floating CPIIndexValue( LinkerType linkertype, const TDate& avaluedate, 
	const TDataSeries& acpiseries );
floating CPIIndexRatio( LinkerType linkertype, const TDate& avaluedate,
	const TDataSeries& acpiseries, floating abasecpiindexvalue );

} // end of namespace final

//---------------------------------------------------------------------------

#endif // linkersH
