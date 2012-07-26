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

#include "bonds.h"

#include "conventions.h"
#include "exceptions.h"
#include "coupons.h"
#include "fixedincome.h"

namespace final {
    
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TBond
//

TBond::TBond( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ):
  TSecurity( amaturity, aissued, abasis )
{
  amaturity.Validate();
  aissued.Validate();
  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency !=4 &&
      afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );
  if( acoupon<0 )
    throw TExInvalidCoupon( acoupon );
  if( aredeem<=0 )
    throw TExInvalidRedemption( aredeem );

  coupon = acoupon*100;
  frequency = afrequency;
  redeem = aredeem;
  firstcpn = afirstcpn;
  intaccr = aintaccr;
  cashflow_dates = 0;
  cashflow_amounts = 0;
  exdivdays = 0;

  if( areadjust )
    _Readjust();
}

TBond::TBond( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ):
  TSecurity( amaturity, aissued, abasis )
{
  amaturity.Validate();
  aissued.Validate();
  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency !=4 &&
      afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );
  if( acoupon<0 )
    throw TExInvalidCoupon( acoupon );
  if( aredeem<=0 )
    throw TExInvalidRedemption( aredeem );

  coupon = acoupon*100;
  frequency = afrequency;
  redeem = aredeem;
  firstcpn = afirstcpn;
  intaccr = aintaccr;
  cashflow_dates = 0;
  cashflow_amounts = 0;
  exdivdays = 0;

  _Readjust();
}

TBond::~TBond()
{
  if( cashflow_dates ) delete[] cashflow_dates;
  if( cashflow_amounts ) delete[] cashflow_amounts;
}

void TBond::_Readjust()
{
  int i;
  if( cashflow_dates ) delete[] cashflow_dates;
  if( cashflow_amounts ) delete[] cashflow_amounts;
  cashflow_dates = NULL;
  cashflow_amounts = NULL;

  TDate prevCouponDate = final::PrevCouponDate( firstcpn, maturity, frequency, basis );
  if( prevCouponDate != firstcpn ) {
      
      // special odd-coupon case

      // let's estimate how many cashflows the bond has (and allocate a bit more)...
      int maxi;
      maxi = (int) ((final::YearFrac( issued, maturity, ACT_ACT ) + 2) * frequency)+1;
      
      // allocate it
      cashflow_dates = new TDate[ maxi ];
      cashflow_amounts = new floating[ maxi ];
      
      i = 0;
      cashflow_dates[0] = issued;
      cashflow_amounts[0] = 0;
      
      TDate dt = firstcpn;
      while( dt <= maturity ) {
             i++;
             if( i>=maxi )
                 throw TExFINAL( "FINAL: internal error in odd-coupon section of Bond::_Readjust()" );
             dt = final::NextCouponDate( dt, frequency );
             cashflow_dates[i] = dt;
             cashflow_amounts[i] = final::YearFrac( cashflow_dates[i-1], dt, basis ) * coupon;
      }
      ncashflows = i;
      cashflow_amounts[ncashflows] += redeem;
  }    
  else {
      
      // standard coupon bonds... 
       
      ncashflows = CouponNum( firstcpn, maturity, frequency, basis ) + 1;
    
      cashflow_dates = new TDate[ncashflows+1];
      cashflow_amounts = new floating[ncashflows+1];
    
      cashflow_dates[0] = issued;
      cashflow_amounts[0] = 0;
    
      for( i=1; i<=ncashflows; i++ )
      {
    
        if( i!=1 )
        {
          cashflow_dates[i] = final::NextCouponDate( cashflow_dates[i-1],maturity,frequency,basis);
          cashflow_amounts[i] = coupon/frequency;
        }
        else
        {
          cashflow_dates[1] = firstcpn;
          cashflow_amounts[1] = final::CouponPayment( firstcpn,issued,maturity,
                                  intaccr,firstcpn,coupon/100.0,100.0,frequency,basis );
    //      fil::AccruedInterest( firstcpn,intaccr,
    //                              maturity,coupon/100.0,100.0,frequency,basis );
        }
      }
    
      cashflow_amounts[ncashflows] += redeem;
    
      if( cashflow_dates[ncashflows]!=maturity )
        throw TExFINAL("TBond::_Readjust() : posledni cashflow se nerovna maturite");
  }
}

void TBond::Readjust()
{
  TSecurity::Readjust();
  _Readjust();
}

TString TBond::SecurityDescriptor() const
{
  TString ret = TSecurity::SecurityDescriptor() + TString(",");
  ret += TString(SEC_DES_COUPON) + TString(":") + TString(coupon) + TString(",");
  ret += TString(SEC_DES_FREQUENCY) + TString(":") + TString(frequency) + TString(",");
  ret += TString(SEC_DES_REDEEMS) + TString(":") + TString(redeem) + TString(",");
  ret += TString(SEC_DES_INTACCR) + TString(":") + intaccr.DateString() + TString(",");
  ret += TString(SEC_DES_FIRSTCPN) + TString(":") + firstcpn.DateString() + TString(",");
  ret += TString(SEC_DES_EXDIV) + TString(":") + TString(exdivdays);
}

TDate TBond::StandardSettlement( const TDate& adate ) const
{
  return WorkDay( adate, 3 );
}

floating TBond::DurationY( const TDate& asettle, floating aytm ) const
{
  asettle.Validate();
  if( asettle.Serial()<issued.Serial() || asettle.Serial()>maturity.Serial() )
    throw TExInvalidSettlement( asettle );

  if( asettle==maturity ) return 0.0;
  if( asettle==maturity ) return 0.0;

  floating p0, p1, p2;
  floating diff = 0.001;
  floating dur;

  p0 = ConvPrice( asettle, aytm, false );
  p1 = ConvPrice( asettle, aytm+diff, false );
  p2 = ConvPrice( asettle, aytm-diff, false);

  dur = ( (-(p1-p2)/(2*diff)) / (p0+AccruedInterest(asettle)) ) * ( 1+aytm/frequency );
  return dur;
}

floating TBond::MDurationY( const TDate& asettle, floating aytm ) const
{
  return DurationY( asettle, aytm ) / ( 1.0+aytm/frequency ) ;
}

floating TBond::_Price( const TDate& asettle, floating aytm ) const
{

  floating result;

  int      i;
  floating power=0;
  TDate    nextcpn;
  floating ytmfq;

  ytmfq = ( 1.0 + aytm/frequency );
  if( ytmfq<0 )
    throw TExTooExtremeValues();

  i = _FindFirstCouponDate( asettle );

  nextcpn = final::NextCouponDate( asettle, maturity, frequency, basis );
  if( i!=-1 )
    if( cashflow_dates[i].Serial()>nextcpn.Serial() )
    // pokud je splnena tato podminka, znamena to, ze bond ma dlouhy
    // prvni kupon!
      power = 1;

  power += (floating) NDays(asettle,nextcpn,basis)/
                      CouponDays( asettle, maturity, frequency, basis );
  power = floating_pow( ytmfq, power );

  if( i!=-1 )
  {
    result = 0;
    for( ; i<=ncashflows; i++ )
    {
      result += cashflow_amounts[i]/power;
      power *= ytmfq;
    }
  }
  else
  {
    result = redeem/power;
  }

  result -= AccruedInterest( asettle );
  return result;
}

floating TBond::_TruePrice( const TDate& asettle, floating aytm ) const
{
  floating   result;

  TDate      next_dt, nextcpn;
  int        i;
  int        ndaysfirstcp;
  floating   power=0.0;
  floating   ytmfq;

  ytmfq = ( 1.0 + aytm/frequency );
  if( ytmfq<0 )
    throw TExTooExtremeValues();

  const TCalendar* cal=Calendar();

  i=_FindFirstCouponDate( asettle );

  ndaysfirstcp = CouponDays( asettle, maturity, frequency, basis );
  
  nextcpn = final::NextCouponDate( asettle, maturity, frequency, basis );
  if( i!=-1 )
    if( cashflow_dates[i].Serial()>nextcpn.Serial() )
      power = 1.0;

  power += (floating) (nextcpn.Serial()-asettle.Serial())/
           ndaysfirstcp;

  if( i!=-1 )
  {
    result = 0.0;
    for( ; i<=ncashflows; i++ )
    {
      floating diff;

      next_dt = cal->BumpHolidays( cashflow_dates[i] );
      if( next_dt != cashflow_dates[i] )
        diff = (floating)(next_dt.Serial()-cashflow_dates[i].Serial())
                 / ndaysfirstcp;
      else
        diff = 0.0;

      result += cashflow_amounts[i]/floating_pow(ytmfq,power+diff);
      power++;
    }
  }
  else
  {
    floating diff;
    next_dt = cal->BumpHolidays( maturity );
    if( next_dt != maturity )
      diff = (floating)(next_dt.Serial()-maturity.Serial())
             / ndaysfirstcp;
    else
      diff = 0.0;
    result = redeem/floating_pow(ytmfq,power+diff);
  }

  result -= AccruedInterest(asettle);

  return result;
}

floating TBond::_Yield( const TDate& asettle, floating apx ) const
{
  floating low,high;
  floating r,ro;
  floating lowpx, highpx, rpx;

//  r = coupon/apx*100.0;
  r = ( coupon*frequency + ( redeem-apx ) / YearFrac( asettle, maturity, ACT_ACT ) ) /
      ( ( redeem+apx ) / 2 ) * 100.0;
  ro = 0.5;

  while( (lowpx=_Price(asettle,(r-ro)/100.0))<apx
         || (highpx=_Price(asettle,(r+ro)/100.0))>apx )
    ro *= 2;

  low = r-ro;
  high = r+ro;

  for( int i=0; i<MAX_ITERATIONS; i++ )
  {
    r = (lowpx-apx)/(lowpx-highpx)*(high-low)+low;
    double diff = (rpx=_Price( asettle,r/100 )) - apx;
    if( floating_fabs(diff)<YIELD_PREC )
      return r/100;

    if( diff>0.0 )
    {
      low=r;
      lowpx=rpx;
    }
    else
    {
      high=r;
      highpx=rpx;
    }
  }
  return r/100;
}

floating TBond::_TrueYield( const TDate& asettle, floating apx ) const
{
  floating low,high;
  floating r,ro;
  floating lowpx, highpx, rpx;

//  r = coupon/apx*100.0;
  r = ( coupon*frequency + ( redeem-apx ) / YearFrac( asettle, maturity, ACT_ACT ) ) /
      ( ( redeem+apx ) / 2 ) * 100.0;
  ro = 0.5;

  while( (lowpx=_TruePrice(asettle,(r-ro)/100.0))<apx
         || (highpx=_TruePrice(asettle,(r+ro)/100.0))>apx )
    ro *= 2;

  low = r-ro;
  high = r+ro;

  for( int i=0; i<MAX_ITERATIONS; i++ )
  {
    r = (lowpx-apx)/(lowpx-highpx)*(high-low)+low;
    double diff = (rpx=_TruePrice( asettle,r/100 )) - apx;
    if( floating_fabs(diff)<YIELD_PREC )
      return r/100;

    if( diff>0.0 )
    {
      low=r;
      lowpx=rpx;
    }
    else
    {
      high=r;
      highpx=rpx;
    }
  }
  return r/100;

}

floating TBond::_PriceSimple( const TDate& asettle, floating aytm ) const
{
  if( CouponNum( asettle, maturity, frequency, basis )>1 )
    throw TExMoreThanOneCashflow( asettle );

  floating apx;
  floating redemption;

  redemption = redeem;
  if( exdivdays>0 )
  {
    TDate exdivdate;
    ExDivDate( asettle, exdivdate );
    if( exdivdate.Serial()>asettle.Serial() )
      redemption += cashflow_amounts[ncashflows]-redeem;
  }
  else
    redemption += cashflow_amounts[ncashflows]-redeem;

  apx = ( redemption ) /
        ( 1 + aytm/frequency
              * CouponDaysNC( asettle, maturity, frequency, basis )
              / CouponDays( asettle, maturity, frequency, basis ) )
        - AccruedInterest( asettle );

  return apx;
}

floating TBond::_YieldSimple( const TDate& asettle, floating apx ) const
{
  if( CouponNum( asettle, maturity, frequency, basis )>1 )
    throw TExMoreThanOneCashflow( asettle );
  if( apx<0 )
    throw TExInvalidPrice( apx );

  floating aytm;
  floating fullpx;
  floating redemption;

  redemption = redeem;
  if( exdivdays>0 )
  {
    TDate exdivdate;
    ExDivDate( asettle, exdivdate );
    if( exdivdate.Serial()>asettle.Serial() )
      redemption += cashflow_amounts[ncashflows]-redeem;
  }
  else
    redemption += cashflow_amounts[ncashflows]-redeem;


  fullpx = apx+AccruedInterest( asettle );

  aytm = ( redemption - fullpx )/fullpx*frequency
         * CouponDays( asettle, maturity, frequency, basis )
         / CouponDaysNC( asettle, maturity, frequency, basis );

  return aytm;
}

floating TBond::_TruePriceSimple( const TDate& asettle, floating aytm ) const
{
  if( CouponNum( asettle, maturity, frequency, basis )>1 )
    throw TExMoreThanOneCashflow( asettle );

  floating apx;
  TDate    realmat;
  floating redemption;

  redemption = redeem;
  if( exdivdays>0 )
  {
    TDate exdivdate;
    ExDivDate( asettle, exdivdate );
    if( exdivdate.Serial()>asettle.Serial() )
      redemption += cashflow_amounts[ncashflows]-redeem;
  }
  else
    redemption += cashflow_amounts[ncashflows]-redeem;

  const TCalendar *cal = Calendar();
  realmat = cal->BumpHolidays( maturity );

  apx = ( redemption ) /
        ( 1 + aytm/frequency
              * NDays( asettle, realmat, basis )
              / CouponDays( asettle, maturity, frequency, basis ) )
        - AccruedInterest( asettle );

  return apx;
}

floating TBond::_TrueYieldSimple( const TDate& asettle, floating apx ) const
{
  if( CouponNum( asettle, maturity, frequency, basis )>1 )
    throw TExMoreThanOneCashflow( asettle );
  if( apx<0 )
    throw TExInvalidPrice( apx );

  floating aytm;
  floating fullpx;
  TDate    realmat;
  floating redemption;

  redemption = redeem;
  if( exdivdays>0 )
  {
    TDate exdivdate;
    ExDivDate( asettle, exdivdate );
    if( exdivdate.Serial()>asettle.Serial() )
      redemption += cashflow_amounts[ncashflows]-redeem;
  }
  else
    redemption += cashflow_amounts[ncashflows]-redeem;

  const TCalendar *cal = Calendar();
  realmat = cal->BumpHolidays( maturity );

  fullpx = apx+AccruedInterest( asettle );
  aytm = ( redemption - fullpx )/fullpx*frequency
         * CouponDays( asettle, maturity, frequency, basis )
         / NDays( asettle, realmat, basis );

  return aytm;
}

void TBond::ExDivDate( const TDate& asettle, TDate& aexdivdate ) const
{
  asettle.Validate();
  if( asettle.Serial()<issued.Serial() || asettle.Serial()>maturity.Serial() )
    throw TExInvalidSettlement( asettle );

  TDate    nextcpn;
  int      i;

  if( asettle==maturity )
    nextcpn = maturity;
  else
  {
    i = 1;
    while( i<ncashflows && asettle>=cashflow_dates[i] ) i++;
    nextcpn = cashflow_dates[i];
  }
  //  nextcpn = _FindFirstCashflow( asettle );
//    nextcpn = NextCouponDate( asettle, maturity, frequency, basis );

  if( exdivdays==0 )
    aexdivdate = nextcpn;
  else
    if( calendar )
      if( exdivdays==30 )
      {
        aexdivdate = EDate( nextcpn, -1 );
      }
      else
        aexdivdate = calendar->WorkDay( nextcpn, -(exdivdays-1) );
    else
    {
      const TCalendar* cal = Calendar();
      if( exdivdays==30 )
      {
        aexdivdate = EDate( nextcpn, -1 );
      }
      else
        aexdivdate = cal->WorkDay( nextcpn, -(exdivdays-1) );
    }
}

floating TBond::AccruedInterest( const TDate& asettle ) const
{

  if( !asettle.IsValid() )
    throw TExInvalidSettlement( asettle );

  if( asettle.Serial()<issued.Serial() || asettle.Serial()>maturity.Serial() )
    throw TExInvalidSettlement( asettle );

  if( asettle.Serial()<=intaccr.Serial() ) return 0.0;
  if( asettle==maturity ) return 0.0;
  if( coupon==0.0 ) return 0.0;

  int i;
  i=_FindFirstCouponDate( asettle )-1;

  if( exdivdays>0 && i==-2 )
    i = ncashflows;

  if( i<0 || i>ncashflows )
    throw TExFINAL( "Unknown error in TBond::AccruedInterest." );

  if( exdivdays>0 )
  {
    TDate exdivdate;
    ExDivDate( asettle, exdivdate );

    if( asettle.Serial()>=exdivdate.Serial() )
      return final::AccruedInterest( asettle, cashflow_dates[0], maturity,
           intaccr, cashflow_dates[1], coupon/100.0, 100.0, frequency, basis )
           - final::CouponPayment(cashflow_dates[i],issued,maturity,intaccr,
           cashflow_dates[1],coupon/100.0,100.0,frequency,basis );
  }
  return final::AccruedInterest( asettle, cashflow_dates[0], maturity,
           intaccr, cashflow_dates[1], coupon/100.0, 100.0, frequency, basis );
}

int TBond::_FindFirstCouponDate( const TDate& asettle, int ignoreexdivs ) const
{
  if( asettle.Serial()>=maturity.Serial() )
    return -1;

  int i;
  int ndays;
  int result;

  if( ncashflows<1 )
    throw TExFINAL( "Bond has no cashflows." );

  if( asettle.Serial()<cashflow_dates[1].Serial() )
    result = 1;
  else
  {
    ndays = cashflow_dates[ncashflows].Serial()-cashflow_dates[1].Serial();
    if( ndays<=0 )
      throw TExFINAL( "Bond has just one cashflow and the settlement date is not prior to this date." );

    i = (int) ( 1+ (ncashflows-1)*
        ((floating)(asettle.Serial()-cashflow_dates[1].Serial()) / ndays) );

    for( ; i>0 && cashflow_dates[i].Serial()>=asettle.Serial(); i-- );
    for( ; i<ncashflows && asettle.Serial()>=cashflow_dates[i+1].Serial(); i++ );

    result = i+1;
  }

  if( exdivdays>0 && !ignoreexdivs )
  {
    TDate exdivdate;
    ExDivDate( asettle, exdivdate );
    if( exdivdate.Serial()<=asettle.Serial() )
    {
      result++;
      if( result>ncashflows )
        result = -1;
    }
  }

  return result;
}

floating TBond::CompYield( const TDate& asettle, floating apx, int acomp ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );
  if( acomp!=1 && acomp!=2 && acomp!=3 && acomp!=4
      && acomp!=6 && acomp!=12 )
    throw TExInvalidCouponFrequency( acomp );

  if( CouponNum( asettle, maturity, frequency, basis )>1 )
    if( acomp==frequency )
      return ConvYield( asettle, apx );
    else
      return ConvertCompounding( ConvYield( asettle, apx ), frequency, acomp );
  else
    return ConvYield( asettle, apx );
}

floating TBond::Price( const TDate& asettle, floating aytm ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  // rev 2009-Feb-03, Marek Sestak, condition changed
  //if( CouponNum( asettle, maturity, frequency, basis )>1 )
  if( asettle >= final::PrevCouponDate( maturity-1, maturity, frequency, basis ) )
    return _PriceSimple( asettle, aytm );


  return _Price( asettle, aytm );
}

floating TBond::Yield( const TDate& asettle, floating apx ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  // rev 2009-Feb-03, Marek Sestak, condition changed
  // if( ncashflows==1 )
  if( asettle >= final::PrevCouponDate( maturity-1, maturity, frequency, basis ) )
    return _YieldSimple( asettle, apx );

  // rev 2009-Feb-03, Marek Sestak, the following code commented out,
  //                  is useless, should be tested by the previous condition
  // if( asettle >= cashflow_dates[ncashflows-1] )
  //   return _YieldSimple( asettle, apx );

  return _Yield( asettle, apx );
}

floating TBond::TruePrice( const TDate& asettle, floating aytm ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  // rev 2009-Feb-03, Marek Sestak, condition changed
  // if( CouponNum( asettle, maturity, frequency, basis )>1 )
  if( asettle >= final::PrevCouponDate( maturity-1, maturity, frequency, basis ) )
    return _TruePriceSimple( asettle, aytm );
  else
    return _TruePrice( asettle, aytm );
}

floating TBond::TrueYield( const TDate& asettle, floating apx ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  // rev 2009-Feb-02, condition changed from:
  // if( CouponNum( asettle, maturity, frequency, basis )>1 )
  if( asettle < final::PrevCouponDate(maturity-1,maturity,frequency,basis) )
    return _TrueYield( asettle, apx );
  else
    return _TrueYieldSimple( asettle, apx );
}

TDate TBond::PrevCouponDate( const TDate& asettle ) const
{
  if( coupon == 0 )
    throw TExNoCouponCashflows();

  if( asettle<issued || asettle>maturity )
    throw TExInvalidSettlement( asettle );

  int i = _FindFirstCouponDate( asettle );
  if( i==-1 )
    throw TExFINAL( "Bond has no coupon cashflows." );

  if( i==1 )
    return intaccr;

  return cashflow_dates[i-1];
}

TDate TBond::NextCouponDate( const TDate& asettle ) const
{
  if( coupon == 0 )
    throw TExNoCouponCashflows();

  if( asettle<issued || asettle>=maturity )
    throw TExInvalidSettlement( asettle );

  int i = _FindFirstCouponDate( asettle );
  if( i==-1 )
    throw TExFINAL( "Bond has no coupon cashflows." );

  return cashflow_dates[i];
}

int TBond::NextCashflow( const TDate& asettle, TDate& anext,
                          floating& aamount ) const
{
  int i;

  if( asettle.Serial()>=maturity.Serial() )
  {
    return 0;
  }

  i = _FindFirstCouponDate( asettle );
  if( i==-1 )
  {
    anext = maturity;
    aamount = redeem;
    return 1;
  }
  anext = cashflow_dates[i];
  aamount = cashflow_amounts[i];

  return ( ncashflows - i + 1 );
}

int TBond::CashflowsTillMaturity( const TDate& asettle ) const
{
  int i;

  if( asettle.Serial()>=maturity.Serial() )
    return 0;

  i = _FindFirstCouponDate( asettle );

  if( i==-1 )
    return 1;
  return ( ncashflows - i + 1 );
}

floating TBond::CashflowAmount( const TDate& asettle ) const
{
  int i;
  TDate dt;

  if( asettle.Serial()>maturity.Serial() )
    return 0.0;

  dt.Set( asettle.Serial()-1 );
  i = _FindFirstCouponDate( dt );

  if( cashflow_dates[i]==asettle )
    return cashflow_amounts[i];
  else
    return 0.0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TUST
//

TUST::TUST( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{
}

TUST::TUST( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{
}

TDate TUST::StandardSettlement( const TDate& atradedate ) const
{
  return WorkDay( atradedate, 1 );
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TBondAnnualComp
//

TBondAnnualComp::TBondAnnualComp( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{}


TBondAnnualComp::TBondAnnualComp( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ):
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{}

TBondAnnualComp::~TBondAnnualComp()
{}

floating TBondAnnualComp::_Price( const TDate& asettle, floating aytm ) const
{
  int      i;
  floating result;
  floating power=0.0;
  TDate    nextcpn;
  floating ytmfq;

  ytmfq = 1.0+aytm;
  if( ytmfq<0 )
    throw TExTooExtremeValues();

  i=_FindFirstCouponDate( asettle );

  nextcpn = final::NextCouponDate( asettle, maturity, frequency, basis );
  if( nextcpn.Serial() < cashflow_dates[i].Serial() )
    power = 1.0;

  power += (floating) NDays( asettle, nextcpn, basis ) /
          CouponDays( asettle, maturity, frequency, basis ) / frequency;
  result = 0.0;

  for( ; i<=ncashflows; i++ )
  {
    result += cashflow_amounts[i]/floating_pow(ytmfq,power);
    power += 1.0/frequency;
  }

  result -= AccruedInterest( asettle );

  return result;
}

floating TBondAnnualComp::_TruePrice( const TDate& asettle, floating aytm ) const
{
  TDate      next_dt, nextcpn;
  int        i;
  int        ndaysfirstcp;
  floating   result;
  floating   power=0.0;
  floating   ytmfq;

  ytmfq = 1.0 + aytm;
  if( ytmfq<0 )
    throw TExTooExtremeValues();

  const TCalendar* cal = Calendar();

  i=_FindFirstCouponDate( asettle );

  nextcpn = final::NextCouponDate( asettle, maturity, frequency, basis );
  if( nextcpn.Serial()<cashflow_dates[i].Serial() )
      power = 1.0;

  ndaysfirstcp = CouponDays( asettle, maturity, frequency, basis );
  power += (floating) CouponDaysNC( asettle, maturity, frequency, basis )/
           ndaysfirstcp/frequency;
  result = 0.0;

  for( ; i<=ncashflows; i++ )
  {
    floating diff;

    next_dt = cal->BumpHolidays( cashflow_dates[i] );
    if( next_dt != cashflow_dates[i] )
      diff = (floating)(next_dt.Serial()-cashflow_dates[i].Serial())
               / ndaysfirstcp / frequency;
    else
      diff = 0.0;

    result += cashflow_amounts[i]/floating_pow(ytmfq,power+diff);
    power+=1.0/frequency;
  }

  result -= AccruedInterest(asettle);

  return result;
}

floating TBondAnnualComp::DurationY( const TDate& asettle, floating aytm ) const
{
  if( asettle==maturity ) return 0;

  floating p0, p1, p2;
  floating diff = 0.001;
  floating dur;

  p0 = ConvPrice( asettle, aytm, false );
  p1 = ConvPrice( asettle, aytm+diff, false );
  p2 = ConvPrice( asettle, aytm-diff, false );

  dur = ( (-(p1-p2)/(2*diff)) / (p0+AccruedInterest(asettle)) ) * ( 1+aytm );
  return dur;
}

floating TBondAnnualComp::MDurationY( const TDate& asettle, floating aytm ) const
{
  return DurationY( asettle, aytm ) / ( 1+aytm );
}

floating TBondAnnualComp::CompYield( const TDate& asettle, floating apx, int acomp ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );
  if( acomp!=1 && acomp!=2 && acomp!=3 && acomp!=4
      && acomp!=6 && acomp!=12 )
    throw TExInvalidCouponFrequency( acomp );

  if( CouponNum( asettle, maturity, frequency, basis )>1 )
    if( acomp==1 )
      return _Yield( asettle, apx );
    else
      return ConvertCompounding( _Yield( asettle, apx ), 1, acomp );
  else
    return _YieldSimple( asettle, apx );
}

///////////////////////////////////////////////////////////////////////////////

TBTPS::TBTPS( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBondAnnualComp(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{
  calendar = GetCalendar( "ITALY" );
}

TBTPS::TBTPS( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ) :
  TBondAnnualComp(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{
  calendar = GetCalendar( "ITALY" );
}

TBTPS::~TBTPS()
{
}

floating TBTPS::AccruedInterest( const TDate& asettle ) const
{
  return Round( TBond::AccruedInterest( asettle ), 5 );
}

///////////////////////////////////////////////////////////////////////////////

TSPGB::TSPGB( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBondAnnualComp(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{}

TSPGB::TSPGB( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ) :
  TBondAnnualComp(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{}

TSPGB::~TSPGB()
{}

///////////////////////////////////////////////////////////////////////////////

TFRTR::TFRTR( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{}

TFRTR::TFRTR( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{}

floating TFRTR::AccruedInterest( const TDate& asettle ) const
{
  floating accrued = TBond::AccruedInterest( asettle );
  if( asettle >= TDate(18,4,2005) )
    accrued = Round( accrued, 7 );
  else
    accrued = Round( accrued, 3 );
  return accrued;
}

floating TFRTR::ConvYield( const TDate& asettle, floating apx, int around ) const
{
  return Yield( asettle, apx );
}

floating TFRTR::ConvPrice( const TDate& asettle, floating apx, int around ) const
{
  return Price( asettle, apx );
}

floating TFRTR::MarketValue( floating anominal, const TDate& asettle, floating apx ) const
{
  if( asettle<issued || asettle>=maturity )
    return 0.0;

  return anominal/100.0*( apx + AccruedInterest(asettle) );
}

///////////////////////////////////////////////////////////////////////////////

TBTNS::TBTNS( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{}

TBTNS::TBTNS( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{}

floating TBTNS::ConvYield( const TDate& asettle, floating apx, int around ) const
{
  if( asettle.Serial()>=maturity.Serial() )
    throw TExInvalidSettlement( asettle );
  if( apx<0 )
    throw TExInvalidPrice( apx );

  return _Yield( asettle, apx );
}

floating TBTNS::ConvPrice( const TDate& asettle, floating aytm, int around ) const
{
  if( asettle.Serial()>=maturity.Serial() )
    throw TExInvalidSettlement( asettle );

  return _Price( asettle, aytm );
}

TDate TBTNS::StandardSettlement( const TDate& adate ) const
{
  return WorkDay( adate, BTNS_STD_SETTLEMENT );
}

floating TBTNS::AccruedInterest( const TDate& asettle ) const
{
  floating accrued = TBond::AccruedInterest( asettle );
  #ifdef BTNS_ACCRUED_ROUNDED
    if( asettle>=TDate(18,4,2005) )
      accrued = Round( accrued, 7 );
  #endif
  return accrued;
}

///////////////////////////////////////////////////////////////////////////////

TUKT::TUKT( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{
  exdivdays = 7;
  mmktconv = ACT_365;
}

TUKT::TUKT( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{
  exdivdays = 7;
  mmktconv = ACT_365;
}

floating TUKT::ConvYield( const TDate& asettle, floating apx, int around ) const
{
  if( asettle.Serial()>=maturity.Serial() )
    throw TExInvalidSettlement( asettle );
  if( apx<0 )
    throw TExInvalidPrice( apx );

  return _Yield( asettle, apx );
}

floating TUKT::ConvPrice( const TDate& asettle, floating aytm, int around ) const
{
  if( asettle.Serial()>=maturity.Serial() )
    throw TExInvalidSettlement( asettle );

  return _Price( asettle, aytm );
}

///////////////////////////////////////////////////////////////////////////////

TCZGB::TCZGB( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{
  exdivdays = 30;
}

TCZGB::TCZGB( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{
  exdivdays = 30;
}

///////////////////////////////////////////////////////////////////////////////

TJGB::TJGB( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,false)
// ve volani konstruktoru TBond je nejdulezitejsi predat areadjust=false !
// vrnitrni struktury si v TJGB::_Readjust nastavime sami
{
  mmktconv = ACT_365;
  if( areadjust )
    _Readjust();
}

TJGB::TJGB( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,false)
// ve volani konstruktoru TBond je nejdulezitejsi predat areadjust=false !
// vrnitrni struktury si v TJGB::_Readjust nastavime sami
{
  mmktconv = ACT_365;
  _Readjust();
}

floating TJGB::JapaneseSimpleYield( const TDate& asettle, floating apx ) const
{
  if( asettle.Serial()<issued.Serial() || asettle.Serial()>=maturity.Serial() )
    throw TExInvalidSettlement( asettle );

  return ( coupon + (redeem-apx) / (NDays(asettle,maturity,basis)/365.0) ) / apx;
}

floating TJGB::JapaneseSimplePrice( const TDate& asettle, floating ayld ) const
{
  if( asettle.Serial()<issued.Serial() || asettle.Serial()>=maturity.Serial() )
    throw TExInvalidSettlement( asettle );

  floating remlife;
  remlife = (floating) NDays( asettle, maturity, basis ) / 365.0;

  return (floating) ( coupon + redeem/remlife ) / ( ayld + 1.0/remlife );
}

floating TJGB::ConvYield( const TDate& asettle, floating apx, int around ) const
{
  if( around )
    return RoundDown( JapaneseSimpleYield( asettle, apx ), 5 );
  else
    return Yield( asettle, apx );
}

floating TJGB::ConvPrice( const TDate& asettle, floating ayld, int around ) const
{
  if( around )
    return RoundDown( JapaneseSimplePrice( asettle, ayld ), 3 );
  else
    return Price( asettle, ayld );
}

void TJGB::_Readjust()
{
  int i;
  TDate maturity20;

  maturity20 = maturity;
  maturity20.Day( 20 );

  ncashflows = CouponNum( firstcpn, maturity20, frequency, basis ) + 1;

  if( cashflow_dates ) delete[] cashflow_dates;
  if( cashflow_amounts ) delete[] cashflow_amounts;

  cashflow_dates = new TDate[ncashflows+1];
  cashflow_amounts = new floating[ncashflows+1];

  cashflow_dates[0] = issued;
  cashflow_amounts[0] = 0;

  for( i=1; i<=ncashflows; i++ )
  {

    if( i!=1 )
    {
      cashflow_dates[i] = final::NextCouponDate( cashflow_dates[i-1],maturity20,frequency,basis);
      cashflow_amounts[i] = coupon/frequency;
    }
    else
    {
      cashflow_dates[1] = firstcpn;
      cashflow_amounts[1] = final::AccruedInterest( firstcpn,intaccr,
                              maturity20,coupon/100.0,100.0,frequency,basis );
    }

  }

  cashflow_amounts[ncashflows] += redeem +
    RoundDown( coupon*( maturity.Serial()-maturity20.Serial())/365.0, 4 );

  if( cashflow_dates[ncashflows]!=maturity20 )
    throw TExFINAL("TBond::_Readjust() : posledni cashflow se nerovna maturite");

  cashflow_dates[ncashflows] = maturity;
}

TDate TJGB::StandardSettlement( const TDate& atradedate ) const
{
  return WorkDay( atradedate, 4 );
}

floating TJGB::AccruedInterest( const TDate& asettle ) const
{
  TDate beg = PrevCouponDate( asettle );
  return RoundDown( coupon*(asettle-beg)/365, 4 );
//  return coupon*(asettle-beg)/365;
}

///////////////////////////////////////////////////////////////////////////////

TACGB::TACGB( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              int areadjust ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{
    exdivdays = 7;
}

TACGB::TACGB( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{
    exdivdays = 7;
}

floating TACGB::AccruedInterest( const TDate& asettle ) const
{
    floating accrued = Round( TBond::AccruedInterest( asettle ), 3 );
    return accrued;
}

floating TACGB::ConvPrice( const TDate& asettle, floating aytm, int around ) const
{
    if( around==false ) {
        return Price( asettle, aytm );
    }
    else {
        return Round( Price( asettle, aytm ), 3 );
    }
}

void TACGB::ExDivDate(const TDate& asettle, TDate& aexdivdate) const {

  asettle.Validate();
  if( asettle.Serial()<issued.Serial() || asettle.Serial()>maturity.Serial() )
    throw TExInvalidSettlement( asettle );

  TDate    nextcpn;
  int      i;

  if( asettle==maturity )
    nextcpn = maturity;
  else
  {
    i = 1;
    while( i<ncashflows && asettle>=cashflow_dates[i] ) i++;
    nextcpn = cashflow_dates[i];
  }

  if( exdivdays==0 )
    aexdivdate = nextcpn;
  else
    aexdivdate = TDate( nextcpn.Serial()-7 );
  
}

///////////////////////////////////////////////////////////////////////////////

TACGBi::TACGBi( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              TDataSeries* acpi,
              int areadjust ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem,areadjust)
{
    exdivdays = 7;
    cpi = acpi;
}

TACGBi::TACGBi( const TDate& amaturity, const TDate& aissued,
              const TDate& aintaccr, const TDate& afirstcpn, int abasis,
              floating acoupon, int afrequency, floating aredeem,
              TDataSeries* acpi ) :
  TBond(amaturity,aissued,aintaccr,afirstcpn,abasis,acoupon,afrequency,aredeem)
{
    exdivdays = 7;
    cpi = acpi;
}

void TACGBi::SetCPI(TDataSeries* acpi) {
    cpi = acpi;
}

void TACGBi::ExDivDate(const TDate& asettle, TDate& aexdivdate) const {

  asettle.Validate();
  if( asettle.Serial()<issued.Serial() || asettle.Serial()>maturity.Serial() )
    throw TExInvalidSettlement( asettle );

  TDate    nextcpn;
  int      i;

  if( asettle==maturity )
    nextcpn = maturity;
  else
  {
    i = 1;
    while( i<ncashflows && asettle>=cashflow_dates[i] ) i++;
    nextcpn = cashflow_dates[i];
  }

  if( exdivdays==0 )
    aexdivdate = nextcpn;
  else
    aexdivdate = TDate( nextcpn.Serial()-7 );
  
}

floating TACGBi::AccruedInterest( const TDate& asettle ) const
{
    if( !cpi ) {
        throw TExFINAL("Failed to calculate accrued interest of an inflation linked bond. No CPI index provided.");
    }
    
    floating cpiRatio = final::CPIIndexRatio( ACGBi, asettle, *cpi, 100.0,
            InterestAccrualDate().Serial(),
            FirstCouponDate().Serial() );
    
    floating accrued = TBond::AccruedInterest( asettle );

    return Round( accrued*cpiRatio, 3 );
}

floating TACGBi::ConvPrice( const TDate& asettle, floating aytm, int around ) const
{
    floating accrued = AccruedInterest( asettle );
    floating cpiRatio = CPIIndexRatio(ACGBi,asettle,*cpi,100.0,
            InterestAccrualDate().Serial(), FirstCouponDate().Serial() );
    floating price = final::Price( asettle, maturity, coupon/100.0 * cpiRatio,
            aytm, 100.0 * cpiRatio, frequency, basis );
        
    if( around==false )
        return price + accrued;
    else
        return Round(price+accrued,3);
}

floating TACGBi::ConvYield(const TDate& asettle, double aprice, int around) const
{
    floating accrued = AccruedInterest( asettle );
    floating cpiRatio = CPIIndexRatio(ACGBi,asettle,*cpi,100.0,
            InterestAccrualDate().Serial(), FirstCouponDate().Serial() );
    floating yield = final::Yield( asettle, maturity, coupon/100.0 * cpiRatio, aprice - accrued, 
            100.0 * cpiRatio, frequency, basis );

    if( around==false )
        return yield;
    else
        return Round(yield,4);
}

///////////////////////////////////////////////////////////////////////////////
} // end of namespace final

