//---------------------------------------------------------------------------

// Financial Analytics Library (FINAL)
// Copyright 2004 Marek Sestak
// Released under the GNU GPL 2.0 license.

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

virtual floating	GetValue( const TDate& adate ) const;
virtual floating	GetValue( int month, int year ) const;

}; // end of class TDataSeries

typedef enum { OATi, OATe, BTPSe, DBRe, GGBe, TII } LinkerType;

floating CPIIndexValue( LinkerType linkertype, const TDate& avaluedate, 
	const TDataSeries& acpiseries );
floating CPIIndexRatio( LinkerType linkertype, const TDate& avaluedate,
	const TDataSeries& acpiseries, floating abasecpiindexvalue );

} // end of namespace final

//---------------------------------------------------------------------------

#endif // linkersH
