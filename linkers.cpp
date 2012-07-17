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

floating CPIIndexValueACGBi( const TDate& avaluedate, const TDataSeries& acpiseries,
        int ainterestaccruesfrom, int afirstcoupondate ) {
    
    TDate prevDate = TDate( ainterestaccruesfrom );
    TDate cpnDate = TDate( afirstcoupondate );
    
    floating prevCapitalValue = 100.0;
    int month = prevDate.Month();
    int year = prevDate.Year();
    
    while( month%3!=0 ) month--;
    if( month==0 ) {
        month = 12;
        year--;
    }
    
    floating cpiChange = acpiseries.GetValue( month, year );
    floating capitalValue = Round( prevCapitalValue * (1.0+cpiChange/100.0), 2);
    
    while( avaluedate<=cpnDate ) {
        prevDate = cpnDate;
        cpnDate = cpnDate.EDate(3);
        month += 3;
        if( month>12 ) {
            month = 3;
            year++;
        }
        cpiChange = acpiseries.GetValue(month, year);
        prevCapitalValue = capitalValue;
        capitalValue = Round( prevCapitalValue * (1.0+cpiChange/100.0), 2);
    }
    
    floating cpiIndex = (avaluedate.Serial()-prevDate.Serial()) /
            (cpnDate.Serial()-prevDate.Serial()) * (capitalValue-prevCapitalValue) +
            prevCapitalValue;
    
    return cpiIndex;
}

floating CPIIndexValue( LinkerType linkertype, const TDate& avaluedate, 
        const TDataSeries& acpiseries,
        int ainterestaccruesfrom, int afirstcoupondate ) {

    // ACGBi is a special case, let's calculate the CPIIndexValue separately
    if( linkertype == ACGBi ) {
        return CPIIndexValueACGBi( avaluedate, acpiseries, 
                ainterestaccruesfrom, afirstcoupondate );
    }
    
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
	const TDataSeries& acpiseries, floating abasecpiindexvalue,
        int ainterestaccruesfrom, int afirstcoupondate ) {
    
	floating value = CPIIndexValue( linkertype, avaluedate, acpiseries, 
                                        ainterestaccruesfrom, afirstcoupondate );        
        floating ratio;
        
        if( linkertype = ACGBi ) {
            ratio = utils::Round( value/100.0,2 );
        }
        else {
            ratio = value / abasecpiindexvalue;

            if( linkertype == TII || linkertype == OATi || linkertype == OATe || 
                    linkertype == BTPSe || linkertype == DBRe || linkertype == GGBe )
            {
                    ratio = utils::Round( ratio, 5 );
            }
        }
	
	return ratio;
}

} // end of namespace final
