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

#ifndef calendarH
#define calendarH

#include <time.h>
#include <set>
#include "defs.h"
#include "../utils/utils.h"

using namespace utils;

namespace final {

// forward declarations
class TCalendar;

// ---------------------------------------------------------------------------

const TCalendar* GetCalendar();
const TCalendar* GetCalendar( const TString& acalendarname );

// ---------------------------------------------------------------------------

#ifdef USE_NO_STD
typedef set<TDate, less<TDate> > TDateSet;
#else
typedef std::set<TDate, std::less<TDate> > TDateSet;
#endif

// --------------------------------------------------------------------------

class TCalendar
{
  private:
  TDateSet holidays;

  public:
                TCalendar( const TDate* aholidays, int anholidays );
                TCalendar() {};
                TCalendar( const TCalendar& acalendar );
  virtual       ~TCalendar();

  TCalendar&    operator=( const TCalendar& acalendar );

  int           AddHoliday( const TDate& adate );
                // vraci true pokud byl holiday pridan
                //       false pokud je holiday uz v kalendari
  int           DeleteHoliday( const TDate& adate );
                // vraci true pokud byl holiday vymazan
                //       false pokud kalendar holiday neobsahuje
  const TDateSet& Holidays() const { return holidays; }
                // vraci vsechny holidaye...

  TDate         WorkDay( const TDate& adate, int n ) const;
                // vraci n-ty pracovni den po adate
                // pokud je adate sobota, a n=3, vraci stredu
                // pokud je n=0, vraci adate

  //  TDate         NextWorkDay( const TDate& adate );  /* OBSOLETE! */
  TDate         BumpHolidays( const TDate& adate ) const;
                // pokud je adate svatek a/nebo sobota/nedele, vrati nasledujici
                // pracovni den, jinak vraci adate
                //
                // obe funkce delaji totez, nicmene NextWorkDay je pomerne
                // nestastne a prozatim je zachovana jen z duvodu zpetne
                // kompatibility
  int           IsHoliday( const TDate& adate ) const;
                // vraci true/false v zavislosti na tom, zda je dany den svatkem a/nebo
                // sobota/nedele
};


// ---------------------------------------------------------------------------

} // end of namespace final

//---------------------------------------------------------------------------
#endif
