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

#ifndef moneymarketH
#define moneymarketH

/** \file moneymarket.h
    \brief Contatins class definitions of main money markter instruments.

    Probably two main classes contained in the file moneymarket.h
    are TZeroCpn and TCd (both descendants of TSecurity).
*/

#include "defs.h"
#include "security.h"
#include "conventions.h"
#include "exceptions.h"
#include "coupons.h"

#include "../utils/utils.h"
using namespace utils;

namespace final {
#pragma GCC diagnostic ignored "-Wwrite-strings"
    
class TZeroCpn: public TSecurity
{
public:
  TZeroCpn( const TDate& amaturity, const TDate& aissued, int abasis=ACT_360 ):
    TSecurity( amaturity, aissued, abasis ) {}

  virtual int      IsBill() const    { return true; }

  virtual char*    SecurityType() const { return "Zero coupon money market security"; }
  virtual int      SecurityTypeNumber() const { return SEC_BILL; }

  virtual floating Price( const TDate& asettle, floating aytm ) const;
  virtual floating Yield( const TDate& asettle, floating apx ) const;
  virtual floating TruePrice( const TDate& asettle, floating aytm ) const;
  virtual floating TrueYield( const TDate& asettle, floating apx ) const;

  virtual TDate    StandardSettlement( const TDate& atradedate ) const;

  virtual TDate    PrevCouponDate( const TDate& asettle ) const;
  virtual TDate    NextCouponDate( const TDate& asettle ) const;

  virtual int      NextCashflow( const TDate& asettle, TDate& anext,
                                 floating& aamount ) const;
  virtual int      CashflowsTillMaturity( const TDate& asettle ) const;
  virtual floating CashflowAmount( const TDate& asettle ) const;

  virtual floating AccruedInterest( const TDate& asettle ) const { return 0.0; }

  virtual floating DurationY( const TDate& asettle, floating aytm ) const;
  virtual floating MDurationY( const TDate& asettle, floating aytm ) const;
};

// TUSB - trida pro americke t-billy
class TUSB: public TZeroCpn
{
public:
  TUSB( const TDate& amaturity, const TDate& aissued, int abasis=ACT_360  ):
    TZeroCpn( amaturity, aissued, abasis ) {}

  virtual char*    SecurityType() const { return "US treasury bills"; }
  virtual int      SecurityTypeNumber() const { return SEC_USB; }

  virtual TDate    StandardSettlement( const TDate& atradedate ) const;
};

// TJGTB - trida pro praci s japonskymi t-billy a financing billy
class TJGTB: public TZeroCpn
{
public:
  TJGTB( const TDate& amaturity, const TDate& aissued, int abasis=ACT_ACT  ):
    TZeroCpn( amaturity, aissued, abasis ) {}

  virtual char*    SecurityType() const { return "Japanese government bills (JGTB)"; }
  virtual int      SecurityTypeNumber() const { return SEC_JGTB; }

  virtual floating MMktPrice( const TDate& atradedate, floating ayield,
                              int abumpholidays=true, int abasis=ACT_ACT ) const;
  virtual floating MMktYield( const TDate& atradedate, floating aprice,
                              int abumpholidays=true, int abasis=ACT_ACT ) const;
  virtual TDate    StandardSettlement( const TDate& atradedate ) const;
};

// TACTB - trida pro australske t-billy
class TACTB: public TZeroCpn
{
public:
  TACTB( const TDate& amaturity, const TDate& aissued, int abasis=ACT_365  ):
    TZeroCpn( amaturity, aissued, abasis ) {}

  virtual char*    SecurityType() const { return "Australia treasury bills"; }
  virtual int      SecurityTypeNumber() const { return SEC_ACTB; }

  virtual TDate    StandardSettlement( const TDate& atradedate ) const;
};

// Certificate of Deposit, Commercial Programme
// maji pouze jedno cashflow, ale mohou mit kupon pri maturite kupon
class TCd: public TSecurity
{
protected:
  floating  coupon;
  floating  matflow;
  floating  amount;
public:
  TCd( const TDate& amaturity, const TDate& aissued, floating acoupon,
    int abasis=ACT_360, floating aamount=0 );

  virtual char*    SecurityType() const { return "Certicate of deposit (CD, ECD)"; }
  virtual int      SecurityTypeNumber() const { return SEC_CD; }

  virtual int      IsCD() const { return true; }
  virtual int      HasCoupon() const { return coupon!=0.0; }

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

  virtual floating DurationY( const TDate& asettle, floating aytm ) const;
  virtual floating MDurationY( const TDate& asettle, floating aytm ) const;
};


} // end of namespace final

#endif
