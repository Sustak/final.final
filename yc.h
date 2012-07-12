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

#ifndef ycH
#define ycH

#include <map>
#include <vector>
#include "defs.h"
#include "calendar.h"
#include "security.h"
#include "conventions.h"

namespace final {

// --------------------------------------------------------------------------

floating LinearInterpol( const TDate& date,
        const TDate& date1, floating value1, const TDate& date2, floating value2 );
floating ExponentialInterpol( const TDate& date, const TDate& basedate,
        const TDate& date1, floating value1, const TDate& date2, floating value2 );

// --------------------------------------------------------------------------

typedef enum { IM_Linear=0, IM_Exponential=1 } TInterpolMethod;

struct TRate
{
  TDate                 startDate;
  TDate                 endDate;
  floating              rate;
};

struct TTermRate
{
  TTermType             termType;
  int                   nTerms;
  floating              rate;
};

TTermRate GetTermRate( const char* term, floating rate );

typedef std::map<TDate, double > TYields;
typedef std::vector<TTermRate> TTermRateVector;
typedef std::vector<TRate> TRateVector;

// --------------------------------------------------------------------------

class TYieldCurve
{
protected:
  TDate                 spot;
  floating              cashrate;

  TYields               yields;
  int                   basis;
  int                   compounding;
  const TCalendar       *calendar;

  TInterpolMethod       interpolmethod;

  virtual void          _Recalc() {};

public:
                        TYieldCurve( const TDate& aspot, floating cashrate,
                                TInterpolMethod ainterpolmethod=IM_Linear,
                                int abasis = ACT_ACT, int acompounding = 1,
                                TCalendar* acalendar=NULL );
                        TYieldCurve( const TDate& aspot, floating cashrate,
                                const TYields& ayields,
                                TInterpolMethod ainterpolmethod=IM_Linear,
                                int abasis = ACT_ACT, int acompounding = 1,
                                TCalendar* acalendar=NULL );
  virtual               ~TYieldCurve()
                        {}

  virtual void          Recalc() const;

  virtual TDate         Spot() const;

  virtual floating      Yield( const TDate &adate ) const;
  virtual floating      SetYield( const TDate& adate, floating ayield );
  virtual void          Yields( TYields& yields ) const;

  virtual floating      YieldFromDF( const TDate &asettle, const TDate& amaturity,
                                     floating adf ) const;
  virtual floating      DFFromYield( const TDate &asettle, const TDate& amaturity,
                                     floating ayield ) const;

  virtual floating      DF( const TDate& adate ) const;
  virtual floating      SetDF( const TDate& adate, floating ayield );

  virtual floating      ZSpread( const TSecurity& asecurity, const TDate& asettle,
                                floating aprice ) const;
  virtual floating      PresentValue( const TDate &asettle, const TCashflows& cfs,
                                floating aspread = 0.0 ) const;

  virtual int           CompoundingFrequency() const
                                { return compounding; }
};

// --------------------------------------------------------------------------

class TSwapCurve: public TYieldCurve
{
protected:
  TYields               df;
  int                   floatingbasis;
  int                   fixedbasis;

  int                   swapfrequency;
  int                   payfrequency;

  TTermRateVector       libors, swaprates;
  TYields               futures;

  bool                  recalcneeded;
  virtual void          _Recalc();

public:
                        TSwapCurve( const TDate& aspot, floating acashrate,
                                const TTermRateVector& alibors,
                                const TYields& afutures, const TTermRateVector& aswaprates,
                                int fixedbasis = ISMA_30_360, int afloatingbasis = ACT_360,
                                int swapfrequency = 1, TCalendar *acalendar = NULL );

  virtual floating      DF( const TDate& adate ) const;
  virtual floating      Yield( const TDate &adate ) const;
  virtual void          DFs( TYields& adfs ) const;

  virtual void          Recalc() const;

//  virtual floating      YieldFromDF( const fil::TDate& adate, floating adf ) const;


  virtual floating      ASWSpread( const TSecurity& asecurity, const TDate& asettle,
                                floating aprice ) const;

};

// --------------------------------------------------------------------------

} // end of namespace

#endif
