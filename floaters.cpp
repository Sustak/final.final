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

#include "floaters.h"
#include "exceptions.h"

using namespace std;

namespace final {

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TFRN
//

TFRN::TFRN( const TDate& amaturity, const TDate& aissued, int abasis, int afrequency ):
  TSecurity( amaturity, aissued, abasis )
{
  mmktstdsettle = 2;
  frequency = afrequency;
}

TFRN::~TFRN()
{
  rates.clear();
  payments.clear();
}

void TFRN::AddRate( const TDate &afrom, const TDate &ato, floating arate )
{
//  if( afrom<issued )
//    throw TExInvalidDate( afrom, "Cannot set interest rate, %d.%d.%d is before the FRN was issued." );

  if( ato<issued )
    throw TExInvalidDate( afrom, "End of the period must be after issue date." );

  if( afrom>=maturity )
    throw TExInvalidDate( afrom, "Cannot set interest rate, %d.%d.%d is on or after the FRN matures." );

  if( ato<=afrom )
    throw TExFINAL( "Cannot set interest rate for the FRN, beginning of the period must preceed the end of period." );

  if( ato>maturity )
    throw TExInvalidDate( afrom, "Cannot set interest rate, %d.%d.%d is past maturity date." );

  if( rates.size()==0 )
  {
    rates.push_back( TFRNRate( afrom, ato, arate ) );
    return;
  }

  if( ato <= rates.begin()->from )
  {
    rates.insert( rates.begin(), TFRNRate( afrom, ato, arate ) );
    return;
  }

  for( TFRNRates::iterator i=rates.end()-1; i>=rates.begin(); i-- )
  {
    if( afrom >= i->to )
    {
      i++;
      if( i!=rates.end() )
      {
        if( ato > i->from )
          throw TExInvalidDate( ato, "Cannot set interest rate for period ending %d.%d.%d, it overlaps with some already defined period." );
      }
      rates.insert( i, TFRNRate( afrom, ato, arate ) );
      return;
    }
  }

  if( ato > rates.begin()->from )
    throw TExInvalidDate( ato, "Cannot set interest rate for period ending %d.%d.%d, it overlaps with some already defined period." );

  rates.insert( rates.begin(), TFRNRate( afrom, ato, arate ) );
  return;
}

void TFRN::AddPayment( const TDate &avaluedt, floating a1mioamt )
{
  if( avaluedt<issued )
    throw TExInvalidDate( avaluedt, "Cannot set payment on %d.%d.%d because it's before the FRN was issued." );
  if( avaluedt>maturity )
    throw TExInvalidDate( avaluedt, "Cannot set payment on %d.%d.%d because it's after the FRN matures." );

  TFRNPayments::const_iterator exist = find( payments.begin(), payments.end(), avaluedt );
  if( exist!=payments.end() )
    throw TExInvalidDate( avaluedt, "Payment valid for %d.%d.%d has already been set, cannot be redefined." );

  payments.push_back( TFRNPayment( avaluedt, a1mioamt ) );
  sort( payments.begin(), payments.end() );
}

void TFRN::AddCoupon( const TDate &afrom, const TDate &ato, floating arate )
{
  AddRate( afrom, ato, arate );
  AddPayment( ato, CalcPayment( 100.0, afrom, ato ) );
}

void TFRN::DeletePayment( const TDate &avaluedt )
{
  TFRNPayments::iterator exist = find( payments.begin(), payments.end(), avaluedt );
  if( exist!=payments.end() )
  {
    payments.erase( exist );
  }
  else
    throw TExInvalidDate( avaluedt, "Cannot delete a FRN payment, no payment for %d.%d.%d has previously been defined." );
}

void TFRN::DeleteRate( const TDate &afrom, const TDate &ato )
{
  TFRNRates::iterator exist = find( rates.begin(), rates.end(), afrom );
  if( exist!=rates.end() )
  {
    if( exist->To() == ato )
      rates.erase( exist );
    else
      throw TExFINAL( afrom, ato, "Cannot delete interest rate for period %d.%d.%d to %d.%d.%d; "
                                "mismatch in the end date." );
  }
  else
    throw TExFINAL( afrom, ato, "Cannot delete interest rate for period %d.%d.%d to %d.%d.%d; "
                              "mismatch in the beginnig date." );
}

void TFRN::DeleteCoupon( const TDate &afrom, const TDate &ato )
{
  DeleteRate( afrom, ato );
  DeletePayment( ato );
}

void TFRN::ClearRatesAndPayments()
{
  rates.clear();
  payments.clear();
}

floating TFRN::CalcPaymentFromLastPay( floating anomamount, const TDate& ato ) const
{
  TDate lastpay = issued;

  TFRNPayments::const_iterator payment = payments.begin();
  while( payment!=payments.end() )
  {
    if( payment->value>ato )
      break;
    lastpay = payment->value;
    payment++;
  }
  return TFRN::CalcPayment( anomamount, lastpay, ato );
}

floating TFRN::CalcPayment( floating anomamount, const TDate& afrom,
                            const TDate& ato ) const
{
  if( afrom>ato )
    throw TExFINAL( afrom, ato, "Invalid period from %d.%d.%d to %d.%d.%d." );
//  if( afrom<issued )
//    throw TExInvalidDate( afrom, "Beginning of the period should start after issue date." );
  if( ato<issued )
    throw TExInvalidDate( afrom, "End of the period must be after issue date." );
  if( ato>maturity )
    throw TExInvalidDate( ato, "End of the period should be on or before maturity date." );

  TFRNRates::const_iterator rate = rates.begin();
  while( rate != rates.end() )
  {
    if( afrom>=rate->from && afrom<rate->to )
      break;
    rate++;
  }
  if( rate == rates.end() )
    throw TExInvalidDate( afrom, "No interest rate defined from %d.%d.%d." );
  floating cum_interest = 0.0;

  TDate lastpaydt = afrom;
  int afq = frequency;
  if( !afq )
    afq = 1;

  do
  {
    if( ato<=rate->to )
    {

      int num = NDays( lastpaydt, ato, basis );
      floating denum = Denum( lastpaydt, rate->to, afq, basis );
      cum_interest += anomamount*rate->rate/100.0*num/denum/afq;

      return cum_interest;
    }


    int num = NDays( lastpaydt, rate->to, basis );
    floating denum = Denum( lastpaydt, rate->to, afq, basis );
    cum_interest += anomamount*rate->rate/100.0*num/denum/afq;

    if( (rate+1)==rates.end() )
      throw TExInvalidDate( rate->to, "No interest rate defined from %d.%d.%d." );
    if( rate->to < (rate+1)->from )
      throw TExFINAL( rate->to, (rate+1)->from, "No interest rate defined "
                    "between %d.%d.%d and %d.%d.%d." );
    lastpaydt = rate->to;
    rate++;
  }
  while( true );
}

floating TFRN::Price( const TDate& asettle, floating aytm ) const
{
  throw TExCannotPerformOnFRN();
}

floating TFRN::Yield( const TDate& asettle, floating apx ) const
{
  throw TExCannotPerformOnFRN();
}

floating TFRN::TruePrice( const TDate& asettle, floating aytm ) const
{
  throw TExCannotPerformOnFRN();
}

floating TFRN::TrueYield( const TDate& asettle, floating apx ) const
{
  throw TExCannotPerformOnFRN();
}

TDate TFRN::StandardSettlement( const TDate& atradedate ) const
{
  return WorkDay( atradedate, 3 );
}

TDate TFRN::PrevCouponDate( const TDate& asettle ) const
{
  if( asettle<issued || asettle>=maturity )
    throw TExInvalidSettlement( asettle );

  for( TFRNPayments::const_iterator i = payments.end();
       i!=payments.begin(); i-- )
  {
    if( (i-1)->value<asettle )
      return (i-1)->value;
  }

  return issued;
}

TDate TFRN::NextCouponDate( const TDate& asettle ) const
{
  if( asettle<issued || asettle>=maturity )
    throw TExInvalidSettlement( asettle );

  for( TFRNPayments::const_iterator i = payments.begin();
       i!=payments.end(); i++ )
  {
    if( (i)->value>asettle )
      return (i)->value;
  }

  throw TExInvalidDate( asettle, MSG_ExNextCouponNotDefined );
}

int TFRN::NextCashflow( const TDate& asettle, TDate& anext, floating& aamount ) const
{
  if( asettle>=maturity )
    return 0;

  if( payments.size()==0 )
  {
    anext = maturity;
    aamount = 100.0;
    return 1;
  }

  if( asettle>=(payments.end()-1)->value )
  {
    anext = maturity;
    aamount = 100.0;
    return 1;
  }

  for( TFRNPayments::const_iterator i = payments.begin();
       i!=payments.end(); i++ )
  {
    if( (i)->value>asettle )
    {
      anext = i->value;
      aamount = i->amount;
      int ret = payments.end()-i;
      if( (payments.end()-1)->value==maturity )
        return ret;
      else
        return ret+1;
    }
  }
  return 1;
}

int TFRN::CashflowsTillMaturity( const TDate& asettle ) const
{
  TDate nextdate;
  floating nextsize;
  int n = NextCashflow( asettle, nextdate, nextsize );
  return n;
}

floating TFRN::CashflowAmount( const TDate& asettle ) const
{
  if( asettle>=maturity )
    return 0.0;

  TFRNPayments::const_iterator i = find( payments.begin(), payments.end(), asettle );
  if( i==payments.end() )
    return 0.0;
  else
    return i->amount;
}

floating TFRN::AccruedInterest( const TDate& asettle ) const
{
  return CalcPaymentFromLastPay( 100.0, asettle );
}

TDate TFRN::Spot( const TDate& adate ) const
{
  if( calendar )
  {
    return calendar->WorkDay( adate, mmktstdsettle );
  }
  else
  {
    TCalendar cal;
    return cal.WorkDay( adate, mmktstdsettle );
  }
}

floating TFRN::DurationY( const TDate& asettle, floating aytm ) const
{
  if( asettle<issued || asettle>=maturity )
    throw TExInvalidSettlement( asettle );

  if( payments.size()==0 )
    throw TExInvalidDate( asettle, MSG_ExNextCouponNotDefined );

  TDate spot = Spot( asettle );
  if( spot>=(rates.end()-1)->to )
    throw TExInvalidDate( asettle, MSG_ExNextCouponNotDefined );

  TFRNRates::const_iterator i = rates.end();
  while( i > rates.begin() )
  {
    i--;
    if( spot>=i->from && spot<i->to )
      return YearFrac( asettle, i->to, basis );
  }
  throw TExInvalidDate( asettle, "No interest rate period containing %d.%d.%d found." );
}

floating TFRN::MDuration( const TDate& asettle, floating, int ) const
{
  if( asettle<issued || asettle>=maturity )
    throw TExInvalidSettlement( asettle );

  if( payments.size()==0 )
    throw TExInvalidDate( asettle, MSG_ExNextCouponNotDefined );

  TDate spot = Spot( asettle );
  if( spot>=(rates.end()-1)->to )
    throw TExInvalidDate( asettle, MSG_ExNextCouponNotDefined );

  TFRNRates::const_iterator i = rates.end()-1;
  while( i >= rates.begin() )
  {
    if( spot>=i->from && spot<i->to )
    {
      floating yf = YearFrac( asettle, i->to, basis );
      return yf/(1.0+i->rate*yf);
    }
    i--;
  }
  throw TExInvalidDate( asettle, "No interest rate period containing %d.%d.%d found." );
}

floating TFRN::MDurationY( const TDate& asettle, floating aytm ) const
{
  if( asettle<issued || asettle>=maturity )
    throw TExInvalidSettlement( asettle );

  if( payments.size()==0 )
    throw TExInvalidDate( asettle, MSG_ExNextCouponNotDefined );

  TDate spot = Spot( asettle );
  if( spot>=(rates.end()-1)->to )
    throw TExInvalidDate( asettle, MSG_ExNextCouponNotDefined );

  TFRNRates::const_iterator i = rates.end()-1;
  while( i >= rates.begin() )
  {
    if( spot>=i->from && spot<i->to )
    {
      floating yf = YearFrac( asettle, i->to, basis );
      return yf/(1.0+aytm*yf);
    }
    i--;
  }
  throw TExInvalidDate( asettle, "No interest rate period containing %d.%d.%d found." );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TCCTS
//

floating TCCTS::CalcPayment( floating anomamount,
                     const TDate& afrom, const TDate& ato ) const
{
  if( afrom>=ato )
    throw TExFINAL( afrom, ato, "Invalid period from %d.%d.%d to %d.%d.%d." );
  if( afrom<issued )
    throw TExInvalidDate( afrom, "Beginning of the period should start after issue date." );
  if( ato>maturity )
    throw TExInvalidDate( ato, "End of the period should be on or before maturity date." );

  TFRNRates::const_iterator rate = rates.begin();
  while( rate != rates.end() )
  {
    if( afrom>=rate->from && afrom<rate->to )
      break;
    rate++;
  }
  if( rate == rates.end() )
    throw TExInvalidDate( afrom, "No interest rate defined from %d.%d.%d." );

  floating cum_interest = 0.0;
  TDate lastpaydt = afrom;

  do
  {
    if( ato<=rate->to )
    {
      cum_interest += CouponPayment( ato, lastpaydt, maturity, rate->rate,
                                     anomamount, frequency, basis );
//      anomamount*rate->rate*YearFrac(lastpaydt,ato,basis);

      return cum_interest;
    }
    cum_interest += CouponPayment( rate->to, lastpaydt, maturity, rate->rate,
                                   anomamount, frequency, basis );
//    anomamount*rate->rate*YearFrac(lastpaydt,rate->to,basis);
    if( (rate+1)==rates.end() )
      throw TExInvalidDate( rate->to, "No interest rate defined from %d.%d.%d." );
    if( rate->to < (rate+1)->from )
      throw TExFINAL( rate->to, (rate+1)->from, "No interest rate defined "
                    "between %d.%d.%d and %d.%d.%d." );
    lastpaydt = rate->to;
    rate++;
  }
  while( true );
}

floating TCCTS::AccruedInterest( const TDate& asettle ) const
{
  return Round( TFRN::AccruedInterest( asettle ), 5 );
}

// -----------------------------------------------------------------------------

} // end of namespace

