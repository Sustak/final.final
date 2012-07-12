
// Financial Analytics Library (FINAL)
// Copyright 2004 Marek Sestak
// Released under the GNU GPL 2.0 license.

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
