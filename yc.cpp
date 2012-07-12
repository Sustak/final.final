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

#include "yc.h"
#include "exceptions.h"
#include "coupons.h"
#include "fixedincome.h"

namespace final
{

//---------------------------------------------------------------------------

floating LinearInterpol( const TDate& date,
        const TDate& date1, floating value1, const TDate& date2, floating value2 )
{
  if( date1==date2 )
    throw TExFINAL( "Interpolation failed, range dates must be different." );

  return ((value2-value1)/(date2.Serial()-date1.Serial())) *
        (date.Serial()-date1.Serial())+value1;
}
/*
floating ExponentialInterpol( floating x,
        floating x1, floating value1, floating x2, floating value2 )
{
  if( x1==x2 )
    throw Exception( "Interpolation failed, range end points may not be equal." );
  if( x1==0 || x2==0 )
    throw Exception( "Exponential interpolation failed, range end points can't be zero." );

  floating d21 = x2-x1;
  return pow( value1, (floating) (x2-x) * x / (d21*x1) )
        * pow( value2, (floating) (x-x1) * x / (d21*x2) );
}
*/

floating ExponentialInterpol( const TDate& date, const TDate& basedate,
        const TDate& date1, floating value1, const TDate& date2, floating value2 )
{
  if( date1==date2 )
    throw TExFINAL( "Interpolation failed, range dates must be different." );
  if( date1==basedate || date2==basedate )
    throw TExFINAL( "Exponential interpolation failed, end dates can't be equal to a base date." );

  floating d21 = date2.Serial() - date1.Serial();
  floating d0 = date.Serial() - basedate.Serial();
  return pow( value1, (floating) (date2.Serial()-date.Serial()) * d0
                / (d21*(date1.Serial()-basedate.Serial())) )
        * pow( value2, (floating) (date.Serial()-date1.Serial()) * d0
                / (d21*(date2.Serial()-basedate.Serial())) );
}

//---------------------------------------------------------------------------

TTermRate GetTermRate( const char* aterm, floating arate )
{
  int nperiods;
  char typeofperiod;
  int n = sscanf( aterm, "%d%c", &nperiods, &typeofperiod );
  if( n<2 )
    throw TExFINAL( "Term identifier not recognized." );

  TTermRate rate;
  switch( typeofperiod ) {
    case 'd':
    case 'D': rate.termType = TT_Day; break;
    case 'w':
    case 'W': rate.termType = TT_Week; break;
    case 'm':
    case 'M': rate.termType = TT_Month; break;
    case 'y':
    case 'Y': rate.termType = TT_Year; break;
    default:
      throw TExFINAL( "Term identifier not recognized." );
  }
  rate.nTerms = nperiods;
  rate.rate = arate;

  return rate;
}

//---------------------------------------------------------------------------

TYieldCurve::TYieldCurve( const TDate& aspot,
        floating acashrate, TInterpolMethod ainterpolmethod,
        int abasis, int acompounding, TCalendar *acalendar )
{
  spot = aspot;
  basis = abasis;
  cashrate = acashrate;
  interpolmethod = ainterpolmethod;
  compounding = acompounding;
  if( acalendar )
    calendar = acalendar;
  else
    calendar = GetCalendar();
}

TYieldCurve::TYieldCurve( const TDate& aspot, floating acashrate,
        const TYields& ayields, TInterpolMethod ainterpolmethod,
        int abasis, int acompounding, TCalendar *acalendar )
{
  spot = aspot;
  basis = abasis;
  cashrate = acashrate;
  interpolmethod = ainterpolmethod;
  yields = ayields;
  compounding = acompounding;
  if( acalendar )
    calendar = acalendar;
  else
    calendar = GetCalendar();
}

TDate TYieldCurve::Spot() const
{
  return spot;
}

floating TYieldCurve::SetYield( const TDate &adate, floating ayield )
{
  TYields::iterator i = yields.find( adate );
  if( i==yields.end() ) {
    yields[ adate ] = ayield;
    return 0.0;
  }
  floating ret = i->second;
  i->second = ayield;
  return ret;
}

floating TYieldCurve::SetDF( const TDate &adate, floating adf )
{
  floating yld = YieldFromDF( spot, adate, adf );
  yld = SetYield( adate, yld );
  return DFFromYield( spot, adate, yld );
}

floating TYieldCurve::Yield( const TDate &adate ) const
{
  Recalc();
  if( adate<=spot || yields.size()==0 )
    return cashrate;

  if( yields.size()==1 )
    return yields.begin()->second;

  TYields::const_iterator i, j;
  i = yields.begin();
  if( adate<=i->first ) {
    j = i;
    j++;
  }
  else {
    j = yields.end();
    j--;
    if( adate>=j->first ) {
      i = j;
      i--;
    }
    else {
      i = yields.lower_bound( adate );
      j = i;
      j++;
      if( j==yields.end() ) {
        j = i;
        i--;
      }
    }
  }

  switch( interpolmethod ) {
    case IM_Linear:
      return LinearInterpol( adate, i->first, i->second, j->first, j->second );
    case IM_Exponential:
      return ExponentialInterpol( adate, spot, i->first, i->second, j->first, j->second );
    default:
      throw TExFINAL( "Interpolation type not recognized." );
  }
}

void TYieldCurve::Yields( TYields& ayields ) const
{
  Recalc();
  ayields = yields;
}

floating TYieldCurve::DF( const TDate& adate ) const
{
  Recalc();
  floating ayield;
  if( adate<=spot )
    ayield = cashrate;
  else
    ayield = Yield( adate );

  switch( basis ) {
    case ACT_360:
    case ACT_365:
      return 1.0 / ( 1.0 + YearFrac( spot, adate, basis ) * ayield );
    default:
      return 1.0 / pow ( 1.0 + ayield/compounding, compounding*YearFrac( spot, adate, basis ) );
  }
}

void TYieldCurve::Recalc() const
{
  ((TYieldCurve*)this)->_Recalc();
}

floating TYieldCurve::ZSpread( const TSecurity& asecurity, const TDate& asettle,
        floating aprice ) const
{
  TCashflows cfs;
  floating amount = 1000000.0;
  floating yldprec = YIELD_PREC * 10000.0;

  asecurity.GetCashflows( asettle, cfs, amount );
  floating mv = asecurity.MarketValue( amount, asettle, aprice );
  floating pv = PresentValue( asettle, cfs );
  if( pv<=0 )
    throw TExFINAL( "Security's present value is zero." );
  floating risk = asecurity.Risk( asettle, aprice );
  floating spread = (pv-mv)/(risk*(amount/10000.0));
  floating spreadchange;

  for( int i=0; i<MAX_ITERATIONS; i++ ) {
    pv = PresentValue( asettle, cfs, spread );
    spreadchange = (pv-mv)/(risk*(amount/10000.0));
    spread += spreadchange;
    if( fabs(spreadchange)<yldprec )
      return spread;
  }
  return spread;
}

floating TYieldCurve::PresentValue( const TDate& asettle, const TCashflows& cfs,
        floating aspread ) const
{

  floating ret = 0.0;
  floating basedf = DF( asettle );
  floating horizondf;
  floating yld;
  TCashflows::const_iterator i = cfs.begin();
  TCashflows::const_iterator n = cfs.end();
  TDate valuedate;
  while( i!=n ) {
    valuedate = i->first;
    horizondf = DF( valuedate )/basedf;
    if( aspread!=0 ) {
      yld = YieldFromDF( asettle, i->first, horizondf ) + aspread/10000.0;
      horizondf = DFFromYield( asettle, i->first, yld );
    }
    ret += i->second * horizondf;
    i++;
  }
  return ret;
}

floating TYieldCurve::YieldFromDF( const TDate &asettle, const TDate& amaturity, floating adf ) const
{
  return final::Yield( asettle, amaturity, 0.0, adf*100.0, 100.0, CompoundingFrequency(), ACT_ACT );
}

floating TYieldCurve::DFFromYield( const TDate &asettle, const TDate& amaturity, floating ayield ) const
{
  return final::Price( asettle, amaturity, 0.0, ayield, 100.0, CompoundingFrequency(), ACT_ACT )/100.0;
}


//---------------------------------------------------------------------------

TSwapCurve::TSwapCurve( const TDate& aspotdate, floating acashrate,
        const TTermRateVector& alibors,
        const TYields& afutures, const TTermRateVector& aswaprates,
        int afixedbasis, int afloatingbasis,
        int aswapfrequency, TCalendar *acalendar ):
        TYieldCurve( aspotdate, acashrate, IM_Linear, ACT_ACT, aswapfrequency, acalendar )
{
//  libors = alibors;
  libors.insert( libors.begin(), alibors.begin(), alibors.end() );
  futures = afutures;
//  swaprates = aswaprates;
  swaprates.insert( swaprates.begin(), aswaprates.begin(), aswaprates.end() );
  floatingbasis = afloatingbasis;
  fixedbasis = afixedbasis;
  recalcneeded = true;
  swapfrequency = aswapfrequency;
  payfrequency = swapfrequency * 2;
}

void TSwapCurve::_Recalc()
{
  int i, i1, j, n, cpns, swapcoupons;
  TDate enddate, lastdate, firstswapdate;
  TTermRate *r;
  TYields::iterator iter, iterend;
  floating begdf;

  yields.clear();
  df.clear();

  recalcneeded = false;
  // priznak, ze nepotrebujeme jiz pred dokoncenim vlastni rekalkulace
  // (jinak hrozi zacykleni, napr. pri volani DF)
  // nastavime jiz takhle brzy, nicmene
  // v pripade neuspechu rekalkulace, tedy pred vyhozenim jakekoliv vyjimky
  // je potreba jej opet nastavit!

  lastdate = spot;

  n = libors.size();
  for( i=0; i<n; i++ ) {
    r = &(libors[i]);
    enddate = calendar->BumpHolidays( spot.AddTerm( r->nTerms, r->termType ) );
    df[ enddate ] = 1.0 /
        ( 1.0 + YearFrac( spot, enddate, floatingbasis ) * r->rate );
    if( enddate>lastdate )
      lastdate = enddate;
  }

  iter = futures.begin();
  iterend = futures.end();
  while( iter!=iterend ) {
    begdf = DF( iter->first );
    enddate = calendar->BumpHolidays( iter->first.EDate(3) );
    if( enddate>lastdate ) {
      df[ enddate ] = begdf /
          ( 1.0 + YearFrac( iter->first, enddate, floatingbasis ) * iter->second );
      lastdate = enddate;
    }
    iter++;
  }

  n = swaprates.size();
  int nswapcoupons = 0;
  
  for( i=0; i<n; i++ ) {
    r = &(swaprates[i]);
    switch( r->termType ) {
      case TT_Year:
        cpns = r->nTerms * swapfrequency;
        break;
      case TT_Month:
        if( (r->nTerms % (12/swapfrequency)) != 0 ) {
          recalcneeded = true;
          throw TExFINAL( "Bad swap tenor." );
        }
        cpns = r->nTerms * swapfrequency / 12;
      default:
        recalcneeded = true;
        throw TExFINAL( "Bad swap tenor; tenor expected to be expressed in months or years only." );
    }
    if( cpns>nswapcoupons )
      nswapcoupons = cpns;
  }

  bool *defined = new bool[ nswapcoupons ];
  // true = swap rate k danemu dni definovan
  double *dfs = new double[ nswapcoupons ];
  // diskontni faktory
  double *ts = new double[ nswapcoupons ];
  // delka od predchoziho swap date k soucasnemu
  double *swaps = new double[ nswapcoupons ];
  // swapove raty (ohled je bran az na ty, co jsou delsi nez nejdelsi maturity
  // definovane libory/futures)
  TDate *dates = new TDate[ nswapcoupons ];
  // datumy kuponu
  int lastalreadydefdf = 0;
  // maximalni cislo kuponu stale definovaneho libory/futures

  try {

    // nejprve si predvyplnime pole dates a ts
    // spocitame diskontni faktory tam, kde to uz jde z liboru, futures...
    for( i=0; i<nswapcoupons; i++ ) {
      defined[i] = false;
      dates[i] = calendar->BumpHolidays( spot.EDate( (i+1)*12/swapfrequency ) );
      if( i==0 )
        ts[0] = YearFrac( spot, dates[0], fixedbasis );
      else
        ts[i] = YearFrac( dates[i-1], dates[i], fixedbasis );
      if( dates[i]<=lastdate ) {
        dfs[i] = DF( dates[i] );
        lastalreadydefdf = i;
      }
    }

    // vyplnime swapove raty do pole swaprates
    for( i=0; i<n; i++ ) {
      r = &(swaprates[i]);
      enddate = calendar->BumpHolidays( spot.AddTerm( r->nTerms, r->termType ) );

      // lastdate contains the last discount factor defined by libors or futures
      // we'll ignore all shorter swaprates
      if( enddate>lastdate ) {
        cpns = ( r->nTerms * swapfrequency / ( r->termType==TT_Year?1:12 ) ) - 1;
        if( cpns>=nswapcoupons ) {
          recalcneeded = true;
          throw TExFINAL( "Internal error in TSwapCurve::Recalc." );
        }
        defined[ cpns ] = true;
        swaps[ cpns ] = r->rate;
      }
    }

    // nyni projdeme swaprates a zkontrolujeme nadefinovane swaprates:
    // - pokud chybi jen jeden, je to ok, algoritmus pro vypocet df si s tim poradi
    // - pokud jich chybi vic, linearni interpolaci se swaprates "doplni"
    double yf1, yf2, yfd;
    for( i=lastalreadydefdf+1; i<nswapcoupons; i++ ) {
      j = i;
      while( defined[j] == false ) {
        if( (j+1)==nswapcoupons ) {
          recalcneeded = true;
          throw TExFINAL( "Internal error in TSwapCurve::Recalc." );
          // nikdy by jsme se sem nemeli dostat, protoze posledni clen pole
          // defined je vzdy true (posledni swaprate)
        }
        j++;
      }
      if( (j-i)>=2 ) {
        yf2 = 0.0;
        for( i1 = i+2; i1<=j; i1++ )
          yf2 += ts[i1];
        yf1 = ts[i] + ts[i+1];

        floating begswaprate, endswaprate;
        if( defined[i-1] )
          begswaprate = swaps[i-1];
        else
          begswaprate = Yield( dates[i-1] );

        endswaprate = swaps[j];

        for( i1 = i+1; i1<j; i1+=2 ) {
          if( i1>(i+1) ) {
            yfd = ts[i1-1] + ts[i];
            yf1 += yfd;
            yf2 -= yfd;
          }
          swaps[i1] = (yf1*endswaprate+yf2*begswaprate)/(yf1+yf2);
          defined[i1] = true;
        }
      }
      i = j;
    }

    floating sumprod = 0.0;
    floating a, b, c, mt, mi, mi1, S, XS, d;

    if( lastalreadydefdf==0 )
      dfs[0] = DF( dates[0] );

    for( i=0; i<=lastalreadydefdf; i++ )
      sumprod += dfs[i]*ts[i];

    for( i=lastalreadydefdf+1; i<nswapcoupons; i++ ) {
      if( defined[i] ) {
        // jednodussi pripad: spocteme
        // df(i) = (1-swaprate(i)*SUM(df(x)*t(x))/(1+swaprate(i)*ts(i))
        dfs[i] = (1-swaps[i]*sumprod)/(1+swaps[i]*ts[i]);
        df[ dates[i] ] = dfs[i];

        sumprod += dfs[i]*ts[i];
      } else if( defined[i+1] )
      {
        // jeden ze swaprates neni definovan:
        // pouzijeme metodu, kdy predpokladame, ze urok od
        // dates[i-1] do dates[i] je prumer urok od dates[i-2] do dates[i-1]
        // a dates[i] do dates[i+1]... podrobnejsi popis je mozne nalezt
        // v dokumentaci knihovny
        S = ( 1 + swaps[i+1]*ts[i+1] );
        XS = ( 1 - swaps[i+1]*sumprod ) / S;
        if( i==1 )
          mt = (1.0/dfs[i-1]-1.0)/ts[i-1];
        else
          mt = (dfs[i-2]/dfs[i-1]-1.0)/ts[i-1];
        a = 0.5 * ts[i] * ts[i+1] * XS;
        b = ts[i+1]*XS + mt*a - ts[i+1]*swaps[i+1]*ts[i]*dfs[i-1]/S +
            ts[i]*0.5*XS;
        c = (1.0 + 0.5*mt*ts[i])*XS - swaps[i+1]*ts[i]*dfs[i-1]/S - dfs[i-1];
        d = b*b - 4*a*c;
        if( d<=0 ) {
          recalcneeded = true;
          throw TExFINAL( "Zero coupon curve cannot be constructed." );
        }
        mi1 = (-b + sqrt(d))/(2.0*a);
        mi = (mi1 + mt)/2.0;
        dfs[i] = dfs[i-1]/(1.0+mi*ts[i]);
        dfs[i+1] = dfs[i]/(1.0+mi1*ts[i+1]);

        df[ dates[i] ] = dfs[i];
        df[ dates[i+1] ] = dfs[i+1];

        sumprod += dfs[i]*ts[i] + dfs[i+1]*ts[i+1];
        i++;
      }
    }
  }
  catch( ... )
  {
    delete[] dates;
    delete[] dfs;
    delete[] ts;
    delete[] swaps;
    delete[] defined;
    recalcneeded = true;
    throw;
  }
  delete[] dates;
  delete[] dfs;
  delete[] ts;
  delete[] swaps;
  delete[] defined;

  iter = df.begin();
  iterend = df.end();
  while( iter!=iterend ) {
    yields[ iter->first ] = YieldFromDF( spot, iter->first, iter->second );
    iter++;
  }
  recalcneeded = false;
}

void TSwapCurve::DFs( TYields& adfs ) const
{
  Recalc();
  adfs = df;
}

floating TSwapCurve::Yield( const TDate &adate ) const
{
  Recalc();
  if( adate<=spot )
    return cashrate;
  return YieldFromDF( spot, adate, DF(adate) );
}

floating TSwapCurve::DF( const TDate &adate ) const
{
  Recalc();

  if( adate<spot || df.size()==0 )
    return 1.0+cashrate*YearFrac( adate, spot, floatingbasis );

  if( df.size()==1 )
    return ExponentialInterpol( adate, spot, spot, 1.0,
      df.begin()->first, df.begin()->second );

  TYields::const_iterator i, j;
  i = df.begin();
  if( adate<=i->first ) {
    j = i;
    j++;
  }
  else {
    j = df.end();
    j--;
    if( adate>=j->first ) {
      i = j;
      i--;
    }
    else {
      i = df.lower_bound( adate );
      j = i;
      j++;
      if( j==df.end() ) {
        j = i;
        i--;
      }
    }
  }
  return ExponentialInterpol( adate, spot, i->first, i->second, j->first, j->second );
}

void TSwapCurve::Recalc() const
{
  if( recalcneeded ) {
    ((TSwapCurve*)this)->_Recalc();
    ((TSwapCurve*)this)->recalcneeded = false;
  }
}


floating TSwapCurve::ASWSpread( const TSecurity& asecurity, const TDate& asettle,
        floating aprice ) const
{
  TCashflows bondcfs, swapcfs;
  TYields swapylds;

  floating amount = 1000000.0;
  floating yldprec = YIELD_PREC * 10000.0;

  asecurity.GetCashflows( asettle, bondcfs, amount );
  floating bondmv = asecurity.MarketValue( amount, asettle, aprice );
  floating bondcfpv = PresentValue( asettle, bondcfs );
  if( bondcfpv<=0 )
    throw TExFINAL( "Security's present value is zero." );
  floating risk = asecurity.Risk( asettle, aprice );
  floating spread = (bondcfpv-bondmv)/(risk*(amount/10000.0));
  floating spreadchange;

  TDate matdate = asecurity.Maturity();
//  TDate prevswapdate = calendar->BumpHolidays(PrevCouponDate( asettle, matdate, payfrequency, fixedbasis ));
  TDate prevswapdate = PrevCouponDate( asettle, matdate, payfrequency, fixedbasis );
  TDate dt = asettle;
  TDate prevdt;
  floating df1, df2;
  df2 = DF( asettle );
  int nswapcfs;

  while( dt<matdate ) {
    prevdt = dt;
//    dt = calendar->BumpHolidays( NextCouponDate( prevdt, matdate, payfrequency, fixedbasis ) );
    dt = NextCouponDate( prevdt, matdate, payfrequency, fixedbasis );
    df1 = df2;
    df2 = DF( dt );
    swapylds[ dt ] = (df1/df2-1.0)/YearFrac( prevdt, dt, floatingbasis );
    swapcfs[ dt ] = 0.0;
  }

  nswapcfs = swapcfs.size();
  TCashflows::iterator cfiter;
  TYields::iterator ylditer;
  floating swappv, cashonsettle;
  floating firstyld = (swapylds.begin()->second);

  for( int i=0; i<MAX_ITERATIONS; i++ ) {

    dt = prevswapdate;
    cfiter = swapcfs.begin();
    ylditer = swapylds.begin();
    for( int j=0; j<nswapcfs; j++ ) {
      prevdt = dt;
      dt = cfiter->first;
      cfiter->second = (ylditer->second+spread/10000.0) *
        YearFrac( prevdt, dt, floatingbasis ) * amount
        + ( ((j+1)==nswapcfs)?amount:0 );
      cfiter++;
      ylditer++;
    }

    swappv = PresentValue( asettle, swapcfs );
    cashonsettle = bondmv -
        amount*(1+(firstyld+spread/10000.0)*YearFrac(prevswapdate,asettle,floatingbasis));

    spreadchange = (bondcfpv-swappv-cashonsettle)/(risk*(amount/10000.0));
    spread += spreadchange;
    if( fabs(spreadchange)<yldprec )
      return spread;
  }
  return spread;
}

//---------------------------------------------------------------------------

} // end of namespace
