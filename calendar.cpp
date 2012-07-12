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

#include <math.h>

#include "calendar.h"
#include "exceptions.h"
#include "conventions.h"

namespace final {

// --------------------------------------------------------------------------

int days_in_month[] = {31,28,31,30,31, 30, 31, 31, 30, 31, 30, 31};
int days_in_month_cum[] = {0, 31,59,90,120,151,181,212,243,273,304,334};

// --------------------------------------------------------------------------

const TDate italian_holidays[] = {
  // 2001
  TDate(1,1,2001), TDate(6,1,2001),
  TDate(13,4,2001), TDate(16,4,2001), TDate(25,4,2001),
  TDate(1,5,2001), TDate(15,8,2001), TDate(1,11,2001),
  TDate(8,12,2001), TDate(25,12,2001), TDate(26,12,2001), TDate(31,12,2001),
  // 2002
  TDate(1,1,2002), TDate(6,1,2002),
  TDate(1,4,2002), TDate(25,4,2002),
  TDate(1,5,2002), // TDate(15,8,2002), // TDate(1,10,2002),
  TDate(8,12,2002), TDate(25,12,2002), TDate(26,12,2002),
  // 2003
  TDate(1,1,2003), TDate(6,1,2003),
  TDate(21,4,2003), TDate(25,4,2003),
  TDate(1,5,2003), // TDate(15,8,2003), // TDate(1,10,2003),
  TDate(8,12,2003), TDate(25,12,2003), TDate(26,12,2003),
  // 2004
  TDate(1,1,2004), TDate(6,1,2004),
  TDate(12,4,2004), TDate(25,4,2004),
  TDate(1,5,2004), // TDate(15,8,2004), // TDate(1,10,2004),
  TDate(8,12,2004), TDate(25,12,2004), TDate(26,12,2004),
  // 2005
  TDate(1,1,2005), TDate(6,1,2005),
  TDate(28,3,2005), TDate(25,4,2005),
  TDate(1,5,2005), // TDate(15,8,2005), // TDate(1,10,2005),
  TDate(8,12,2005), TDate(25,12,2005), TDate(26,12,2005),
  // 2006
  TDate(1,1,2006), TDate(6,1,2006),
  TDate(17,4,2006), TDate(25,4,2006),
  TDate(1,5,2006), // TDate(15,8,2006), // TDate(1,10,2006),
  TDate(8,12,2006), TDate(25,12,2006), TDate(26,12,2006),
  // 2007
  TDate(1,1,2007), TDate(6,1,2007),
  TDate(9,4,2007), TDate(25,4,2007),
  TDate(1,5,2007), // TDate(15,8,2007), // TDate(1,10,2007),
  TDate(8,12,2007), TDate(25,12,2007), TDate(26,12,2007),
  // 2008
  TDate(1,1,2008), TDate(6,1,2008),
  TDate(24,3,2008), TDate(25,4,2008),
  TDate(1,5,2008), // TDate(15,8,2008), // TDate(1,10,2008),
  TDate(8,12,2008), TDate(25,12,2008), TDate(26,12,2008),
  // 2009
  TDate(1,1,2009), TDate(6,1,2009),
  TDate(13,4,2009), TDate(25,4,2009),
  TDate(1,5,2009), // TDate(15,8,2009), // TDate(1,10,2009),
  TDate(8,12,2009), TDate(25,12,2009), TDate(26,12,2009),
  // 2010
  TDate(1,1,2010), TDate(6,1,2010),
  TDate(5,4,2010), TDate(25,4,2010),
  TDate(1,5,2010), // TDate(15,8,2010), // TDate(1,10,2010),
  TDate(8,12,2010), TDate(25,12,2010), TDate(26,12,2010),
  // 2011
  TDate(1,1,2011), TDate(6,1,2011),
  TDate(25,4,2011),
  TDate(1,5,2011),
  TDate(8,12,2011), TDate(25,12,2011), TDate(26,12,2011),
  // 2012
  TDate(1,1,2012), TDate(6,1,2012),
  TDate(25,4,2012),
  TDate(1,5,2012),
  TDate(8,12,2012), TDate(25,12,2012), TDate(26,12,2012),
  // 2013
  TDate(1,1,2013), TDate(6,1,2013),
  TDate(25,4,2013),
  TDate(1,5,2013),
  TDate(8,12,2013), TDate(25,12,2013), TDate(26,12,2013),
};

TCalendar CalendarItaly( italian_holidays, sizeof(italian_holidays)/sizeof(italian_holidays[0]) );
TCalendar DefaultCalendar;

// --------------------------------------------------------------------------

const TCalendar* GetCalendar()
{
  return &DefaultCalendar;
}

const TCalendar* GetCalendar( const TString& acalendarname )
{
  TString calname;
  calname = acalendarname.UpperCase();
  if( calname=="ITALY" )
    return &CalendarItaly;

  return &DefaultCalendar;
}

// --------------------------------------------------------------------------


// --------------------------------------------------------------------------

TCalendar::TCalendar( const TDate* aholidays, int anholidays )
{
  for( int i=0; i<anholidays; i++ )
    if( aholidays[i].IsValid() )
      AddHoliday( aholidays[i] );
}

TCalendar::TCalendar( const TCalendar& acalendar )
{
  *this = acalendar;
}

TCalendar& TCalendar::operator=( const TCalendar& acalendar )
{
  holidays = acalendar.holidays;
  return *this;
}

TCalendar::~TCalendar()
{
}

int TCalendar::IsHoliday( const TDate& adate ) const
{
  adate.Validate();

  if( adate.WeekDay()>=6 )
    return true;

  if( holidays.count( adate ) > 0 )
    return true;
  else
    return false;
}

TDate TCalendar::WorkDay( const TDate& adate, int n ) const
{
  adate.Validate();

  TDate  dt;
  int    start,end;
  int    i;

  if( n==0 ) return adate;
  if( n>0 )
  {
    dt = adate;
    while( IsHoliday(dt) )
    {
      dt.Set( dt.Serial()+1 );
    }
    if( dt!=adate )
      n--;
    start = dt.Serial();

    int newdt = start + (n/5)*7 + n%5;
    int weekdays = n%5 + dt.WeekDay();
    if( weekdays>5 )
      newdt += 2;

    end = newdt;

    TDateSet::const_iterator i;
    i = holidays.upper_bound(TDate(start));

    TDate hol;
    while( i!=holidays.end() ) {
      hol = *i;
      if( hol.WeekDay()<6 && hol.Serial()>=start && hol.Serial()<=end ) {
        end++;
        dt.Set(end);
        if( dt.WeekDay()==6 )
          end+=2;
      }
      i++;
    }

    dt.Set(end);
  }
  else
  {
    n = -n;

    dt = adate;
    while( IsHoliday(dt) )
    {
      dt.Set( dt.Serial()-1 );
    }
    if( dt!=adate )
      n--;
    start = dt.Serial();

    dt.Set( start - (n/5)*7 - n%5 );
    if( dt.WeekDay()>=6 )
      end = dt.Serial()-2;
    else
      end = dt.Serial();

    TDateSet::const_iterator i;
    i = holidays.lower_bound(TDate(end));

    TDate hol;
    while( i!=holidays.end() ) {
      hol = *i;
      if( hol.WeekDay()<6 && hol.Serial()<=start && hol.Serial()>=end ) {
        end--;
        dt.Set(end);
        if( dt.WeekDay()==7 )
          end-=2;
      }
      if( i==holidays.begin() )
        i = holidays.end();
      else
        i--;
    }

    dt.Set(end);
  }

  return dt;
}

TDate TCalendar::BumpHolidays( const TDate& adate ) const
{
  adate.Validate();

  TDate dt,dt1;
  dt1.Set( adate.Serial()-1 );

  dt = WorkDay( dt1, 1 );
  return dt;
}

int TCalendar::AddHoliday( const TDate& adate )
{
  adate.Validate();
  if( holidays.count( adate )==0 ) {
    holidays.insert( adate );
    return true;
  }
  return false;
}

int TCalendar::DeleteHoliday( const TDate& adate )
{
  adate.Validate();
  return holidays.erase(adate);
}


// --------------------------------------------------------------------------

} // end of namespace final
