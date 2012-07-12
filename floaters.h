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

#ifndef floatersH
#define floatersH

#include <vector>
#include "defs.h"
#include "calendar.h"
#include "security.h"
#include "conventions.h"
#include "coupons.h"

#include "../utils/utils.h"
using namespace utils;

//---------------------------------------------------------------------------

namespace final {

// forward declaration
class TFRN;

class TFRNRate
{
friend class TFRN;
friend class TCCTS;
protected:
  TDate    from;
  TDate    to;
  floating rate;
public:
               TFRNRate( const TDate& afrom, const TDate& ato, floating arate ):
                 from(afrom), to(ato), rate( arate ) {}
               TFRNRate( const TFRNRate& arate ):
                 from(arate.from), to(arate.to), rate(arate.rate) {}
  TDate        From() const { return from; }
  TDate        To()   const { return to; }
  floating     Rate() const { return rate; }
  bool         operator<( const TFRNRate& b ) const { return from<b.from; }
  bool         operator==( const TDate& b ) const { return from==b; }
  bool         operator!=( const TDate& b ) const { return from!=b; }
  TFRNRate&    operator=( const TFRNRate& b ) { from=b.from; rate=b.rate; return *this; }
};

class TFRNPayment
{
friend class TFRN;
friend class TCCTS;

protected:
  TDate    value;
  floating amount;

public:
  TFRNPayment( const TDate& aat, floating a1mioamt ): value(aat), amount( a1mioamt ) {}
  TFRNPayment( const TFRNPayment& apay ): value(apay.value), amount(apay.amount) {}
  TDate        ValueDate() { return value; }
  floating     Amount()    { return amount; }
  bool         operator<( const TFRNPayment& b ) const { return value<b.value; }
  bool         operator==( const TFRNPayment& b ) const { return value==b.value; }
  bool         operator==( const TDate& b ) const { return value==b; }
  bool         operator!=( const TDate& b ) const { return value!=b; }
  TFRNPayment& operator=( const TFRNPayment& b )
               { value=b.value; amount=b.amount; return *this; }
};

typedef std::vector<TFRNRate> TFRNRates;
typedef std::vector<TFRNPayment> TFRNPayments;

class TFRN: public TSecurity
{
protected:
  TFRNRates        rates;
  TFRNPayments     payments;
  int              mmktstdsettle;
  int              frequency;
                   // frekvence se pouziva pouze jako "voditko" ve funkcich
                   // GetNextCoupon
public:
                   TFRN( const TDate& amaturity, const TDate& aissued, int abasis, int afrequency=0 );
  virtual          ~TFRN();

  virtual char*    SecurityType() const { return "Floating rate note"; }
  virtual int      SecurityTypeNumber() const { return SEC_FRN; }

  virtual void     AddRate( const TDate &afrom, const TDate &ato, floating arate );
  virtual void     AddPayment( const TDate &avaluedt, floating a1mioamt );
  virtual void     AddCoupon( const TDate &afrom, const TDate &ato, floating arate );

  virtual void     DeletePayment( const TDate &avaluedt );
  virtual void     DeleteRate( const TDate &afrom, const TDate &ato );
  virtual void     DeleteCoupon( const TDate &afrom, const TDate &ato );
                   // DeletePayment maze cashflows floateru (nekontroluje objem!)
                   // podobne DeleteRate maze urokovou sazbu (nekontroluje jeji vysi!)
                   //
                   // POZOR!!! Pouziti techto funkci muze vest ke ztrate konzistence
                   // dat obsazenych ve vnitrnich strukturach tridy TFRN!!!
                   // Bezpecnejsi je pouzit ClearRatesAndPayments a nasledne
                   // pridavat AddRate/AddPayment/AddCoupon...

  virtual void     ClearRatesAndPayments();
                   // maze vsechny cashflows a urokove sazby bondu

  virtual int      IsFloater() const { return true; }
  virtual int      HasCoupon() const { return true; }

          void     SetStandardMmktSettlementDays( int andays )
                   { mmktstdsettle = andays; }
  virtual TDate    Spot( const TDate& adate ) const;

  virtual floating CalcPayment( floating anomamount,
                     const TDate& afrom, const TDate& ato ) const;
  virtual floating CalcPaymentFromLastPay( floating anomamount,
                     const TDate& ato ) const;

  virtual floating Price( const TDate& asettle, floating aytm ) const;
  virtual floating Yield( const TDate& asettle, floating apx ) const;
  virtual floating TruePrice( const TDate& asettle, floating aytm ) const;
  virtual floating TrueYield( const TDate& asettle, floating apx ) const;

  virtual TDate    StandardSettlement( const TDate& atradedate ) const;

  virtual TDate    PrevCouponDate( const TDate& asettle ) const;
  virtual TDate    NextCouponDate( const TDate& asettle ) const;

  virtual int      NextCashflow( const TDate& asettle, TDate& anext, floating& aamount ) const;
  virtual int      CashflowsTillMaturity( const TDate& asettle ) const;
  virtual floating CashflowAmount( const TDate& asettle ) const;

  virtual floating AccruedInterest( const TDate& asettle ) const;

  virtual floating Duration( const TDate& asettle, floating ) const
                   { return DurationY( asettle, 0.0 ); }
  virtual floating DurationY( const TDate& asettle, floating aytm ) const;
  virtual floating MDuration( const TDate& asettle, floating aprice, int adjust=false ) const;
  virtual floating MDurationY( const TDate& asettle, floating aytm ) const;
};


class TCCTS: public TFRN
{
public:
                   TCCTS( const TDate& amaturity, const TDate& aissued,
                          int abasis=ACT_ACT, int afrequency=2):
                          TFRN( amaturity, aissued, abasis, afrequency ) {}

  virtual char*    SecurityType() const { return "Italian government floaters (CCTS)"; }
  virtual int      SecurityTypeNumber() const { return SEC_CCTS; }

  virtual floating CalcPayment( floating anomamount,
                     const TDate& afrom, const TDate& ato ) const;
//  virtual floating CalcPaymentFromLastPay( floating anomamount,
//                     const TDate& ato ) const;
  virtual floating AccruedInterest( const TDate& asettle ) const;
};

//----------------------------------------------------------------------------

} // end of namespace

#endif
