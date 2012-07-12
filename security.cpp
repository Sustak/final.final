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

#include "security.h"
#include "exceptions.h"
#include "conventions.h"
#include "fixedincome.h"

namespace final {

TSecurity::TSecurity( const TDate& amaturity, const TDate& aissued, int abasis ):
    maturity(amaturity),issued(aissued),basis(abasis)
{
  calendar = 0;
  mmktconv = ACT_360;
}

TSecurity::~TSecurity()
{
}

TString TSecurity::SecurityDescriptor() const
{
  TString ret = TString(SEC_DES_SECTYPE) + TString(":") + TString( SecurityType() ) + TString(",");
  ret += TString(SEC_DES_MATURITY) + TString(":") + maturity.DateString() + TString(",");
  ret += TString(SEC_DES_ISSUED) + TString(":") + maturity.DateString() + TString(",");
  ret += TString(SEC_DES_BASIS) + TString(":") + TString(basis) + TString(",");
  return ret;
}

const TCalendar* TSecurity::Calendar() const
{
  if( calendar )
    return calendar;

  return GetCalendar();
}

floating TSecurity::MarketValue( floating anominal, const TDate& asettle, floating apx ) const
{
  if( asettle<issued || asettle>=maturity )
    return 0.0;
  return anominal/100.0*( apx + AccruedInterest(asettle) );
}

floating TSecurity::MarketValueY( floating anominal, const TDate& asettle, floating aytm ) const
{
  return MarketValue( anominal, asettle, Price(asettle,aytm) );
}

floating TSecurity::BpvAmount( floating anominal, const TDate& asettle, floating apx ) const
{
  floating risk = Risk( asettle, apx );
  return risk * anominal / 10000.0;
}

floating TSecurity::BpvAmountY( floating anominal, const TDate& asettle, floating aytm ) const
{
  floating risk = RiskY( asettle, aytm );
  return risk * anominal / 10000.0;
}

floating TSecurity::MMktYield( const TDate& asettle, floating apx, int bumpholidays, int abasis ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );
  if( apx<=0 )
    throw TExInvalidPrice( apx );

  TDate    mat;
  floating cashflow;
  int      n;

  n = NextCashflow( asettle, mat, cashflow );
  if( n==0 )
    throw TExInvalidSettlement( asettle );
  if( n>1 )
    throw TExMoreThanOneCashflow( asettle );

  if( bumpholidays )
    return TrueYieldSimple( asettle, mat, apx+AccruedInterest( asettle ),
                            cashflow, abasis==-1?mmktconv:abasis, calendar );
  else
    return YieldSimple( asettle, mat, apx+AccruedInterest( asettle ),
                        cashflow, abasis==-1?mmktconv:abasis );
}

floating TSecurity::MMktPrice( const TDate& asettle, floating ayield, int bumpholidays, int abasis ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  TDate    mat;
  floating cashflow;
  int      n;

  n = NextCashflow( asettle, mat, cashflow );
  if( n==0 )
    throw TExInvalidSettlement( asettle );
  if( n>1 )
    throw TExMoreThanOneCashflow( asettle );

  if( bumpholidays )
    return TruePriceSimple( asettle, mat, ayield, cashflow,
             abasis==-1?mmktconv:abasis, calendar ) - AccruedInterest( asettle );
  else
    return PriceSimple( asettle, mat, ayield, cashflow,
             abasis==-1?mmktconv:abasis ) - AccruedInterest( asettle );
}

floating TSecurity::Discount( const TDate& asettle, floating apx, int abasis ) const
{
  TDate    mat;
  floating cashflow;
  int      n;

  n = NextCashflow( asettle, mat, cashflow );
  if( n==0 )
    throw TExInvalidSettlement( asettle );
  if( n>1 )
    throw TExMoreThanOneCashflow( asettle );

  floating fullprice = apx+AccruedInterest(asettle);
  return (cashflow-fullprice)/YearFrac(asettle, mat, abasis==-1?mmktconv:abasis)/100.0;
}

floating TSecurity::DiscountToPrice( const TDate& asettle, floating adisc, int abasis ) const
{
  TDate    mat;
  floating cashflow;
  int      n;

  n = NextCashflow( asettle, mat, cashflow );
  if( n==0 )
    throw TExInvalidSettlement( asettle );
  if( n>1 )
    throw TExMoreThanOneCashflow( asettle );

  floating fullprice = cashflow - ((100.0 * adisc) * YearFrac(asettle, mat, abasis==-1?mmktconv:abasis));
  return fullprice - AccruedInterest( asettle );
}

floating TSecurity::ConvexityY( const TDate& asettle, floating aytm ) const
{
  floating dy=0.001;
  floating conv;
  floating px,pxup,pxdown;

  px = ConvPrice( asettle, aytm, false );
  pxup = ConvPrice( asettle, aytm+dy, false );
  pxdown = ConvPrice( asettle, aytm-dy, false );

  conv = 1/(dy*dy)*(pxup-px+pxdown-px)/(px+AccruedInterest(asettle));

  return conv;
}

floating TSecurity::RiskY( const TDate& asettle, floating aytm ) const
{
  return ( ConvPrice( asettle, aytm-0.000005, false ) -
           ConvPrice( asettle, aytm+0.000005, false ) ) * 1000;
}

TDate TSecurity::StandardSettlement() const
{
  return StandardSettlement( Today() );
}

TDate TSecurity::WorkDay( const TDate& adate, int n ) const
{
  if( calendar )
  {
    return calendar->WorkDay( adate, n );
  }
  else
  {
    TCalendar cal;
    return cal.WorkDay( adate, n );
  }
}

void TSecurity::Set( const TDate& amaturity, const TDate& aissued, int abasis )
{
  maturity = amaturity;
  issued = aissued;
  basis = abasis;

  Readjust();
}

void TSecurity::SetCalendar( const TCalendar* acalendar )
{
  calendar = acalendar;
  Readjust();
}

floating TSecurity::DurationY( const TDate& asettle, floating aytm ) const
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

floating TSecurity::MDurationY( const TDate& asettle, floating aytm ) const
{
  return DurationY( asettle, aytm ) / ( 1+aytm );
}

floating TSecurity::SumCashflows( const TDate& afrom, const TDate& ato,
  floating areinvestrate, int abasis )
{
  if( ato<=afrom )
    return 0.0;

  floating ret = 0.0;
  int nr;
  floating aamount = 0.0;
  TDate nextone, current;
  current = afrom;
  nextone = afrom;

  while( current<=ato )
  {
//    current = nextone;
    ret += aamount * ( 1.0 + areinvestrate * YearFrac( current, ato, abasis ) );
    nr = NextCashflow( current, nextone, aamount );
    if( nr==0 )
      break;
    current = nextone;
  }

  return ret;
}

floating TSecurity::GetNextCashflow( const TDate& asettle ) const
{
  TDate nextone;
  floating aamount;
  int nr = NextCashflow( asettle, nextone, aamount );
  if( nr>0 )
    return aamount;
  else
    throw TExInvalidDate( asettle, "No cashflow after %d.%d.%d." );
}

int TSecurity::RealNextCashflow( const TDate& asettle, TDate& anextone, floating& aamount) const
{
  TDate nextone;
  TDate realsettle;
  int nr;

  realsettle = asettle + 1;
  if( calendar )
  {
    realsettle = calendar->WorkDay( realsettle, -1 );
  }
  else
  {
    TCalendar cal;
    realsettle = cal.WorkDay( realsettle, -1 );
  }

  nr = NextCashflow( realsettle, nextone, aamount );
  anextone = RealSettlementDate( nextone );

  return nr;
}

int TSecurity::RealCashflowsTillMaturity( const TDate& asettle ) const
{
  TDate nextone;
  floating amount;

  return RealNextCashflow( asettle, nextone, amount );
}

floating TSecurity::RealCashflowAmount( const TDate& asettle ) const
{
  floating amount;
  const TCalendar *cal = Calendar();

  if( cal->IsHoliday( asettle ) )  // o svatku nebo vikendu nemuze
    return 0.0;                    // zadne cashflow nastat

  amount = CashflowAmount( asettle );

  TDate i(asettle-1);
  while ( cal->IsHoliday(i) )
  {
    amount += CashflowAmount( i );
    i--;
  }

  return amount;
}

TDate TSecurity::RealSettlementDate( const TDate& asettle ) const
{
  TDate ret;
  if( calendar )
  {
    ret = calendar->BumpHolidays( asettle );
  }
  else
  {
    TCalendar cal;
    ret = cal.BumpHolidays( asettle );
  }
  return ret;
}

floating TSecurity::NextCouponSize( const TDate& asettle ) const
{
  if( HasCoupon() )
  {
    if( asettle.Serial()>=maturity.Serial() ||
        asettle.Serial()<issued.Serial() )
    {
      throw TExInvalidSettlement( asettle );
    }
    TDate next;
    floating amount;
    int n = NextCashflow( asettle, next, amount );
    if( n<1 )
      throw TExInvalidSettlement( asettle );
    if( next==maturity )
      return amount-Redemption();
    else
      return amount;
  }
  throw TExNoCouponCashflows();
}

void TSecurity::GetCashflows( const TDate& asettle, TCashflows &cfs,
        floating aamount ) const
{
  cfs.clear();
  TDate nextone, settle;
  floating anextamount;
  int nr = NextCashflow( asettle, nextone, anextamount );
  while( nr>0 ) {
    cfs[nextone] = anextamount * aamount / 100.0;
    settle = nextone;
    nr = NextCashflow( settle, nextone, anextamount );
  }
}

} // end of namespace final

