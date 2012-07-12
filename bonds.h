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

#ifndef bondsH
#define bondsH

/** \file bonds.h
    \brief Contains classes representing bonds.

    TBond is the main class for operations with generic bonds.
    All other classes are its descendants and represent
    main types of government bonds for which calculations are performed
    in a different way (in most cases it's accrued interest and a conventional
    yield).
*/

/* List of changes:
    04/22/05 - TFRTR a TBTNS: new rules applicable to calculation of accrued
               interest on French government bonds
*/

#include "defs.h"
#include "security.h"

namespace final {

class TBond: public TSecurity
{
protected:
  floating  coupon;                // coupon rate (p.a.)
  int       frequency;             // coupon frequency
  floating  redeem;                // redemption value (in %s, eg. default=100)

  int       ncashflows;            // number of cashflows in life of bond
  TDate*    cashflow_dates;        // dates of cashflows
  floating* cashflow_amounts;      // amounts paid out
                                   // note: cashflow_dates[0] is set equal to
                                   // issued!

  TDate     intaccr;               // interest accrual date
  TDate     firstcpn;              // first coupon payment

  int       exdivdays;             // number of days bond trades ex-dividend
                                   // just one exception: 30 has meaning of
                                   // 'one ex-dividend month' which is a case
                                   // for CZGS

protected:
                   TBond( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
                   // parametr readjust udava, jestli se maji inicializovat
                   // vsechny interni datove struktury (typicky nastavene
                   // ve funkci Readjust()). pokud je tedy readjust=false,
                   // neni v konstruktoru zavolana funkce Readjust() a
                   // je treba naplnit parametry cashflow_dates a
                   // cashflow_amounts.
public:
                   TBond( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency=1, floating aredeem=100.0 );
                   // maturita, issue dt, interest accrual dt, first coupon dt,
                   // basis, kupon, frekvence, redemption amount

  virtual          ~TBond();

  virtual char*    SecurityType() const { return "Generic Bond"; }
  virtual int      SecurityTypeNumber() const { return SEC_BOND; }
  virtual TString  SecurityDescriptor() const;

  virtual int      IsBond() const { return true; }
  virtual int      HasCoupon() const { return true; }

  virtual floating Yield( const TDate& asettle, floating apx ) const;
  virtual floating Price( const TDate& asettle, floating aytm ) const;
  virtual floating TrueYield( const TDate& asettle, floating apx ) const;
  virtual floating TruePrice( const TDate& asettle, floating aytm ) const;

  virtual floating CompYield( const TDate& asettle, floating apx, int acomp ) const;
                   // funkce CompYield umoznuje vypocist yield s pozadovanym
                   // compoundingem acomp.
                   // Yield() normalne pouziva compounding shodny
                   // s frekvenci kuponu.

  virtual TDate    StandardSettlement( const TDate& atradedate ) const;

  virtual TDate    PrevCouponDate( const TDate& asettle ) const;
  virtual TDate    NextCouponDate( const TDate& asettle ) const;

  virtual int      NextCashflow( const TDate& asettle, TDate& anext,
                                 floating& aamount ) const;
  virtual int      CashflowsTillMaturity( const TDate& asettle ) const;
  virtual floating CashflowAmount( const TDate& adate ) const;

  virtual floating DurationY( const TDate& asettle, floating aytm ) const;
  virtual floating MDurationY( const TDate& asettle, floating aytm ) const;

  virtual void     ExDivDate( const TDate& asettle, TDate& aexdivdate ) const;
                   // k zadanemu settlementu vrati v aexdivdate
                   // pristi ex-dividend datum (tedy den, od kdy zacne nabihat
                   // dalsi kupon). pokud je exdivdays=0, vraci hodnotu
                   // navracenou NextCouponDate(asettle)...
  virtual floating AccruedInterest( const TDate& asettle ) const;

          TDate    InterestAccrualDate() const { return intaccr; }
          TDate    FirstCouponDate() const { return firstcpn; }
          int      ExDividendDays() const { return exdivdays; }
          floating Coupon() const { return coupon; }
  virtual floating Redemption() const { return redeem; }
          int      Frequency() const { return frequency; }

protected:
  virtual floating _Price( const TDate& asettle, floating aytm ) const;
  virtual floating _Yield( const TDate& asettle, floating apx ) const;
  virtual floating _YieldSimple( const TDate& asettle, floating apx ) const;
  virtual floating _PriceSimple( const TDate& asettle, floating aytm ) const;

  virtual floating _TrueYield( const TDate& asettle, floating apx ) const;
  virtual floating _TruePrice( const TDate& asettle, floating aytm ) const;
  virtual floating _TrueYieldSimple( const TDate& asettle, floating apx ) const;
  virtual floating _TruePriceSimple( const TDate& asettle, floating ayield ) const;

  virtual void     Readjust();
          void     _Readjust();

          int      _FindFirstCouponDate( const TDate& asettle, int ignoreexdivs = false ) const;

};

class TBondAnnualComp : public TBond
{
protected:
                   TBondAnnualComp( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TBondAnnualComp( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem=100.0 );
  virtual          ~TBondAnnualComp();

  virtual char*    SecurityType() const { return "Generic Bond with annual compounding"; }
  virtual int      SecurityTypeNumber() const { return SEC_BOND_ANNUAL; }

  virtual floating DurationY( const TDate& asettle, floating aytm ) const;
  virtual floating MDurationY( const TDate& asettle, floating aytm ) const;
  virtual floating CompYield( const TDate& asettle, floating apx, int acomp ) const;

protected:
  virtual floating _Price( const TDate& asettle, floating aytm ) const;
  virtual floating _TruePrice( const TDate& asettle, floating aytm ) const;
};

/* TODO : for some reason BTPS yield/price conversions are about hundred times slower than for generic TBond */
class TBTPS : public TBondAnnualComp
{
protected:
                   TBTPS( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TBTPS( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency=2, floating aredeem=100.0 );
  virtual          ~TBTPS();

  virtual char*    SecurityType() const { return "Italian government bond (BTPS)"; }
  virtual int      SecurityTypeNumber() const { return SEC_BTPS; }

  virtual floating ConvYield( const TDate& asettle, floating apx,
                              int around=true ) const
                   { return TrueYield( asettle, apx ); }
  virtual floating ConvPrice( const TDate& asettle, floating aytm,
                              int around=true ) const
                   { return TruePrice( asettle, aytm ); }

  virtual floating AccruedInterest( const TDate& asettle ) const;
};

class TSPGB : public TBondAnnualComp
{
protected:
                   TSPGB( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TSPGB( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem=100.0 );
  virtual          ~TSPGB();

  virtual char*    SecurityType() const { return "Spanish government bond (SPGB)"; }
  virtual int      SecurityTypeNumber() const { return SEC_SPGB; }

  virtual floating ConvYield( const TDate& asettle, floating apx,
                              int around=true ) const
                   { return TrueYield( asettle, apx ); }
  virtual floating ConvPrice( const TDate& asettle, floating aytm,
                              int around=true ) const
                   { return TruePrice( asettle, aytm ); }
};

class TFRTR : public TBond
{
protected:
                   TFRTR( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TFRTR( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency=1, floating aredeem=100.0 );

  virtual char*    SecurityType() const { return "French government bond (FRTR)"; }
  virtual int      SecurityTypeNumber() const { return SEC_FRTR; }

  virtual floating ConvYield( const TDate& asettle, floating apx, int around=true ) const;
  virtual floating ConvPrice( const TDate& asettle, floating aytm, int around=true ) const;
  virtual floating AccruedInterest( const TDate& asettle ) const;
  virtual floating MarketValue( floating anominal, const TDate& asettle, floating apx ) const;
};

class TBTNS : public TBond
{
protected:
                   TBTNS( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TBTNS( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency=1, floating aredeem=100.0 );

  virtual TDate    StandardSettlement( const TDate& atradedate ) const;
  virtual floating AccruedInterest( const TDate& asettle ) const;

  virtual char*    SecurityType() const { return "French government bond (BTNS)"; }
  virtual int      SecurityTypeNumber() const { return SEC_BTNS; }

  virtual floating ConvYield( const TDate& asettle, floating apx, int around=true ) const;
  virtual floating ConvPrice( const TDate& asettle, floating aytm, int around=true ) const;
};

class TUKT : public TBond
{
protected:
                   TUKT( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TUKT( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency=2, floating aredeem=100.0 );

  virtual char*    SecurityType() const { return "UK government bond (Gilts, UKT)"; }
  virtual int      SecurityTypeNumber() const { return SEC_UKT; }

  virtual floating ConvYield( const TDate& asettle, floating apx, int around=true ) const;
  virtual floating ConvPrice( const TDate& asettle, floating aytm, int around=true ) const;
};

class TCZGB : public TBond
{
protected:
                   TCZGB( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TCZGB( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency=1, floating aredeem=100.0 );

  virtual char*    SecurityType() const { return "Czech government bond (CZGB)"; }
  virtual int      SecurityTypeNumber() const { return SEC_CZGB; }
};

class TJGB : public TBond
{
protected:
                   TJGB( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TJGB( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency=1, floating aredeem=100.0 );

  virtual char*    SecurityType() const { return "Japanese government bond (JGB)"; }
  virtual int      SecurityTypeNumber() const { return SEC_JGB; }

  virtual floating JapaneseSimpleYield( const TDate& asettle, floating apx ) const;
  virtual floating JapaneseSimplePrice( const TDate& asettle, floating ayld ) const;
  /* TODO : co settlementy na 29.2.???! */
  virtual floating ConvYield( const TDate& asettle, floating apx, int around=true ) const;
  virtual floating ConvPrice( const TDate& asettle, floating ayld, int around=true ) const;

  virtual floating AccruedInterest( const TDate& asettle ) const;
  virtual TDate    StandardSettlement( const TDate& atradedate ) const;

protected:
          void     _Readjust();
};

class TUST : public TBond
{
protected:
                   TUST( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TUST( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency=2, floating aredeem=100.0 );

  virtual char*    SecurityType() const { return "US treasury bond"; }
  virtual int      SecurityTypeNumber() const { return SEC_UST; }

  virtual TDate    StandardSettlement( const TDate& atradedate ) const;
};

class TACGB : public TBond
{
protected:
                   TACGB( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency, floating aredeem,
                          int readjust );
public:
                   TACGB( const TDate& amaturity, const TDate& aissued,
                          const TDate& aintaccr, const TDate& afirscpn, int abasis,
                          floating acoupon, int afrequency=1, floating aredeem=100.0 );

  virtual floating ConvPrice( const TDate& asettle, floating aytm, int around=true ) const;
  virtual floating AccruedInterest( const TDate& asettle ) const;
  
  virtual char*    SecurityType() const { return "Australian government bond (ACGB)"; }
  virtual int      SecurityTypeNumber() const { return SEC_ACGB; }
};



} // end of namespace final

#endif
