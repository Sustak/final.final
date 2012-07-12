
// Financial Analytics Library (FINAL)
// Copyright 2008 Marek Sestak
// Released under the GNU GPL 2.0 license.

#include "linkers.h"
#include "exceptions.h"

using namespace std;

namespace final {

void TDataSeries::Add( const TDate& adate, floating avalue )
{
	dataseries[ adate ] = avalue;
}

void TDataSeries::Clear() 
{
	dataseries.clear();
}

floating TDataSeries::GetValue( const TDate& adate ) const
{
	std::map< TDate, floating >::const_iterator i = dataseries.find( adate );

	if( i == dataseries.end() ) 
		throw TExFINAL( "Data series doesn't contain any value for given date." );
	
	return i->second;
}
	
floating TDataSeries::GetValue( int month, int year ) const
{
	std::map< TDate, floating >::const_iterator i = dataseries.lower_bound( TDate(1,month,year) );

	if( i == dataseries.end() )
		throw TExFINAL( "Data series doesn't contain a value for given month." );

	if( i->first.Month() == month && i->first.Year() == year )
		return i->second;
	
	throw TExFINAL( "Data series doesn't contain a value for given month." );
}

floating CPIIndexValue( LinkerType linkertype, const TDate& avaluedate, const TDataSeries& acpiseries ) {
	
	// month2, value2 etc ... values corresponding to date 2 months ago
	// month3, value3 etc ... values corresponging to date 3 months ago
	
	TDate month2 = avaluedate.EDate( -2 );
	TDate month3 = avaluedate.EDate( -3 );
	
	floating value2 = acpiseries.GetValue( month2.Month(), month2.Year() );
	floating value3 = acpiseries.GetValue( month3.Month(), month3.Year() );

	TDate endcurrentmonth = avaluedate.EOMonth( 0 );
	
	floating value = ( floating (avaluedate.Day()-1) )
					 / ( floating (endcurrentmonth.Day()) )
					 * ( value2-value3 ) + value3;

	if( linkertype == OATe or linkertype == OATi ) {
		
		// bez zaokrouhlovani hodnot CPI nefunguji napr. tyto pripady
		// Date: 14.10.06  IndexValue:     102.4019354839  IndexRatio:     1.0247000000
		//         Error: Should be:       102.4019400000                  1.0247100000
		// Date: 09.11.07  IndexValue:     104.2966666667  IndexRatio:     1.0436600000
		//         Error: Should be:       104.2966700000                  1.0436700000		
		
		value = floating_floor( value * 1000000 ) / 1000000;
		value = utils::Round( value, 5 );
	}
		
	return value;
}

floating CPIIndexRatio( LinkerType linkertype, const TDate& avaluedate,
	const TDataSeries& acpiseries, floating abasecpiindexvalue )
{
	floating value = CPIIndexValue( linkertype, avaluedate, acpiseries );
	floating ratio = value / abasecpiindexvalue;
	
	if( linkertype == TII || linkertype == OATi || linkertype == OATe || 
		linkertype == BTPSe || linkertype == DBRe || linkertype == GGBe )
	{
		ratio = utils::Round( ratio, 5 );
	}
	
	return ratio;
}

} // end of namespace final
