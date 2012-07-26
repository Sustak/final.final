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

#include "fixedincome.h"

namespace final {

// --------------------------------------------------------------------------

TSecurity* GetSecurity( const TString& aticker, floating acoupon, int amaturity,
                        int aissued,
                        int afrequency, int abasis,
                        int aintaccr, int afirstcpn, floating aredemption )
{
  TSecurity* ret;
  TDate      issued, maturity, intaccr, firstcpn;

  if( amaturity<0 )
  {
    throw TExFINAL( amaturity, "Invalid maturity date (serial number %d)" );
  }

  maturity.Set( amaturity );
  if( !maturity.IsValid() )
    throw TExInvalidDate( maturity, "Invalid maturity date (%d.%d.%d)" );

  if( aissued<0 )
  {
    throw TExFINAL( "Invalid issue date." );
  }
  issued.Set( aissued );
  if( !issued.IsValid() )
    throw TExInvalidDate( issued, "Invalid issue date (%d.%d.%d)" );

  if( aintaccr==-1 )
    intaccr = PrevCouponDate( issued, maturity, afrequency, abasis );
  else
  {
    intaccr.Set( aintaccr );
    if( !intaccr.IsValid() )
      throw TExInvalidDate( intaccr, "Invalid interest accrual date (%d.%d.%d)" );
  }

  if( afirstcpn==-1 )
    firstcpn = NextCouponDate( issued, maturity, afrequency, abasis );
  else
  {
    firstcpn.Set( afirstcpn );
    if( !firstcpn.IsValid() )
      throw TExInvalidDate( firstcpn, "Invalid first coupon date (%d.%d.%d)" );
  }

  if( aticker=="CD" || aticker=="CP" )
  {
    return new TCd( amaturity, aissued, acoupon, abasis );
  }

  if( aticker=="S" || aticker=="SP" ||
      aticker=="SPX" || aticker=="SPY" )
  {
    if( !IsZero(acoupon) )
      throw TExFINAL( "Cannot create a strip with a non-zero coupon rate." );
    ret = new TUST( maturity, issued, intaccr, firstcpn, abasis, 0.0, afrequency, aredemption );
    return ret;
  }
  if( aticker=="STRIP" ||
      aticker=="DBRS" || aticker=="DBRR" ||
      aticker=="FRTRS" || aticker=="FRTRR" ||
      aticker=="NETHRR" || aticker=="NETHRS" )
  {
    if( acoupon>=YIELD_PREC )
      throw TExFINAL( "Cannot create a strip with a non-zero coupon rate." );
    ret = new TBond( maturity, issued, intaccr, firstcpn, abasis, 0.0, afrequency, aredemption );
    return ret;
  }

  if( aticker=="BKO" ||
      aticker=="OBL" ||
      aticker=="DBRUF" ||
      aticker=="THA" ||
      aticker=="DBR" ||
      aticker=="GERP" ||
      aticker=="BOND" ||
      aticker=="GENERIC" ||
      aticker=="" )
  {
    if( afrequency==-1 )
      afrequency = 1;
    if( abasis==-1 )
      abasis = 1;

    return new TBond( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="BTPS" )
  {
    if( afrequency==-1 )
      afrequency = 2;
    if( abasis==-1 )
      abasis = 1;

    return new TBTPS( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="FRTR" )
  {
    if( afrequency==-1 )
      afrequency = 1;
    if( abasis==-1 )
      abasis = 1;

    return new TFRTR( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="BTNS" )
  {
    if( afrequency==-1 )
      afrequency = 1;
    if( abasis==-1 )
      abasis = 1;

    return new TBTNS( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="SPGB" )
  {
    if( afrequency==-1 )
      afrequency = 1;
    if( abasis==-1 )
      abasis = 1;

    return new TSPGB( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="UKT" )
  {
    if( afrequency==-1 )
      afrequency = 2;
    if( abasis==-1 )
      abasis = 1;

    return new TUKT( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="T" || aticker=="S" )
  {
    if( afrequency==-1 )
      afrequency = 2;
    if( abasis==-1 )
      abasis = ACT_ACT;

    return new TUST( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="CZGB" )
  {
    if( afrequency==-1 )
      afrequency = 1;
    if( abasis==-1 )
      abasis = ISMA_30_360;

    return new TCZGB( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="JGB" )
  {
    if( afrequency==-1 )
      afrequency = 2;
    if( abasis==-1 )
      abasis = NL_365;

    return new TJGB( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="ACGB" )
  {
    if( afrequency==-1 )
      afrequency = 2;
    if( abasis==-1 )
      abasis = 1;

    return new TACGB( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
  }

  if( aticker=="ACGBi" )
  {
    if( afrequency==-1 )
      afrequency = 2;
    if( abasis==-1 )
      abasis = 1;

    return new TACGBi( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption, NULL );
  }

  
  if( IsZero(acoupon) )
  {
    if( aticker=="B" )
      ret = new TUSB( maturity, issued );
    if( aticker=="ACTB" )
      ret = new TACTB( maturity, issued );
    else
      ret = new TZeroCpn( maturity, issued, abasis );
    
    return ret;
  }

  if( afrequency==-1 )
    afrequency = 1;
  if( abasis==-1 )
    abasis = 1;

  return new TBond( maturity, issued, intaccr, firstcpn, abasis, acoupon, afrequency, aredemption );
}

// --------------------------------------------------------------------------

floating AccruedInterest( const TDate& asettle, const TDate& amaturity,
           floating acoupon, floating apar, int afrequency, int abasis )
{
  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency!=4
      && afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );

  asettle.Validate();
  amaturity.Validate();

  if( asettle.Serial()>amaturity.Serial() )
    throw TExInvalidSettlement( asettle );

  if( asettle==amaturity )
    return 0.0;

  TDate dt_prev = PrevCouponDate( asettle, amaturity, afrequency, abasis );

  floating accrued = 0.0;

  if( dt_prev!=asettle )
  {
    int num = NDays( dt_prev, asettle, abasis );
    TDate dt_next = NextCouponDate( asettle, amaturity, afrequency, abasis );
    floating denum = Denum( dt_prev, dt_next, afrequency, abasis );
    accrued = apar*acoupon/afrequency*num/denum;
  }

  return accrued;
}

floating AccruedInterest( const TDate& asettle, const TDate& aissued,
           const TDate& amaturity,
           floating acoupon, floating apar, int afrequency, int abasis )
{
  TDate next_cpn = NextCouponDate( aissued, amaturity, afrequency, abasis );
  return AccruedInterest( asettle, aissued, amaturity, aissued, next_cpn,
                          acoupon, apar, afrequency, abasis );
}

floating AccruedInterest( const TDate& asettle, const TDate& aissued,
           const TDate& amaturity, const TDate& aintaccr, const TDate& afirstcpn,
           floating acoupon, floating apar, int afrequency, int abasis )
{
  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency!=4
      && afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );

  if( !asettle.IsValid() )
    throw TExInvalidSettlement( asettle );
  if( !amaturity.IsValid() )
    throw TExInvalidDate( amaturity, MSG_ExInvalidMaturity );
  if( !aissued.IsValid() )
    throw TExInvalidDate( aissued, MSG_ExInvalidIssueDate );
  if( !aintaccr.IsValid() )
    throw TExInvalidDate( aintaccr, MSG_ExInvalidIntAccr );
  if( !afirstcpn.IsValid() )
    throw TExInvalidDate( afirstcpn, MSG_ExInvalidFirstCpn );

  if( aissued>=amaturity )
    throw TExFINAL( "Issue date should be before maturity date." );
  if( aintaccr>=afirstcpn )
    throw TExFINAL( "Interest accrual date should be before the first coupon date." );
  if( afirstcpn>amaturity )
    throw TExFINAL( "First coupon date should be on or before maturity date." );

  if( asettle.Serial()>amaturity.Serial() ||
      asettle.Serial()<aissued.Serial() )
    throw TExInvalidSettlement( asettle );

  if( asettle==amaturity || asettle==afirstcpn )
    return 0.0;

  if( asettle>afirstcpn )
    return AccruedInterest( asettle, amaturity, acoupon, apar, afrequency, abasis );

  if( asettle<=aintaccr )
    return 0.0;
    // jo, i takhle hovadsky pripady uz jsem videl....

  TDate cpndt_before_firstcpn = PrevCouponDate( afirstcpn, amaturity, afrequency, abasis );
  if( afirstcpn!=cpndt_before_firstcpn )
    throw TExInvalidDate( afirstcpn, "Provided date of first coupon payment (%d.%d.%d) should be a real coupon date." );

  TDate cpndt_after_intaccr = NextCouponDate( aintaccr, amaturity, afrequency, abasis );
  TDate cpndt_before_intaccr = PrevCouponDate( aintaccr, amaturity, afrequency, abasis );

  if( asettle<=cpndt_after_intaccr )
  {
    return apar * acoupon / afrequency * NDays( aintaccr, asettle, abasis ) /
              Denum( cpndt_before_intaccr, cpndt_after_intaccr, afrequency, abasis );
  }

  floating accrued = apar * acoupon / afrequency *
    NDays( aintaccr, cpndt_after_intaccr, abasis ) /
    Denum( cpndt_before_intaccr, cpndt_after_intaccr, afrequency, abasis );

  TDate cpndt_before_settle = PrevCouponDate( asettle, amaturity, afrequency, abasis );
  TDate cpndt_after_settle = NextCouponDate( asettle, amaturity, afrequency, abasis );

  accrued += apar * acoupon / afrequency * CouponNum( cpndt_after_intaccr, cpndt_before_settle, afrequency, abasis );
  accrued += apar * acoupon / afrequency *
             NDays( cpndt_before_settle, asettle, abasis ) /
             Denum( cpndt_before_settle, cpndt_after_settle, afrequency, abasis );

  return accrued;
}

floating Price( const TDate& asettle, const TDate& amaturity, floating acoupon,
           floating aytm, floating aredem, int afrequency, int abasis )
{
  TBond *bnd;
  TDate issued;
  floating result;

  issued = PrevCouponDate( asettle, amaturity, afrequency, abasis );

  bnd = new TBond( amaturity, issued, issued,
                   NextCouponDate( issued, amaturity, afrequency, abasis ),
                   abasis, acoupon, afrequency, aredem );
  result = bnd->Price( asettle, aytm );
  delete bnd;

  return result;
}

floating TruePrice( const TDate& asettle, const TDate& amaturity, floating acoupon,
           floating aytm, floating aredem, int afrequency, int abasis,
           const TCalendar* cal )
{
  TBond *bnd;
  TDate issued;
  floating result;

  issued = PrevCouponDate( asettle, amaturity, afrequency, abasis );

  bnd = new TBond( amaturity, issued, issued,
                   NextCouponDate( issued, amaturity, afrequency, abasis ),
                   abasis, acoupon, afrequency, aredem );
  if( cal )
    bnd->SetCalendar( cal );
  result = bnd->TruePrice( asettle, aytm );
  delete bnd;

  return result;
}

floating Yield( const TDate& asettle, const TDate& amaturity, floating acoupon,
           floating aprice, floating aredem, int afrequency, int abasis )
{
  TBond *bnd;
  TDate issued;
  floating result;

  issued = PrevCouponDate( asettle, amaturity, afrequency, abasis );

  // TODO : všechno schovat do try {} catch (...) bloku
  bnd = new TBond( amaturity, issued, issued,
                   NextCouponDate( issued, amaturity, afrequency, abasis ),
                   abasis, acoupon, afrequency, aredem );
  result = bnd->Yield( asettle, aprice );
  delete bnd;

  return result;
}

floating TrueYield( const TDate& asettle, const TDate& amaturity, floating acoupon,
           floating aprice, floating aredem, int afrequency, int abasis,
           const TCalendar* cal )
{
  TBond *bnd;
  TDate issued;
  floating result;

  issued = PrevCouponDate( asettle, amaturity, afrequency, abasis );

  bnd = new TBond( amaturity, issued, issued,
                   NextCouponDate( issued, amaturity, afrequency, abasis ),
                   abasis, acoupon, afrequency, aredem );
  if( cal )
    bnd->SetCalendar( cal );
  result = bnd->TrueYield( asettle, aprice );
  delete bnd;

  return result;
}

floating ConvertCompounding( floating ayld, int acomp, int anewcomp )
{
  if( acomp!=1 && acomp!=2 && acomp!=3 && acomp!=4
      && acomp!=6 && acomp!=12 )
    throw TExInvalidCouponFrequency( acomp );
  if( anewcomp!=1 && anewcomp!=2 && anewcomp!=3 && anewcomp!=4
      && anewcomp!=6 && anewcomp!=12 )
    throw TExInvalidCouponFrequency( anewcomp );

  if( (1.0+ayld/acomp) < 0 )
    throw TExTooExtremeValues();

  return ( floating_pow(1.0+ayld/acomp,(double)acomp/anewcomp)-1 )*anewcomp;
}

floating PriceSimple( const TDate& asettle, const TDate& amaturity,
           floating ayield, floating redeem, int abasis )
{
  if( asettle.Serial()>=amaturity.Serial() )
    throw TExInvalidSettlement( asettle );

  return (floating)redeem/(1+ayield*YearFrac(asettle,amaturity,abasis));
}

floating YieldSimple( const TDate& asettle, const TDate& amaturity,
           floating apx, floating redeem, int abasis )
{
  if( asettle.Serial()>=amaturity.Serial() )
    throw TExInvalidSettlement( asettle );

  return (floating)(redeem/apx-1)/YearFrac(asettle,amaturity,abasis);
}

floating TruePriceSimple( const TDate& asettle, const TDate& amaturity,
           floating ayield, floating redeem, int abasis,
           const TCalendar* acalendar )
{
  if( asettle.Serial()>=amaturity.Serial() )
    throw TExInvalidSettlement( asettle );

  TDate realmat;

  if( !acalendar )
    acalendar = GetCalendar();

  realmat = acalendar->BumpHolidays( amaturity );

  return (floating)redeem/(1+ayield*YearFrac(asettle,realmat,abasis));
}

floating TrueYieldSimple( const TDate& asettle, const TDate& amaturity,
           floating apx, floating redeem, int abasis,
           const TCalendar* acalendar )
{
  if( asettle.Serial()>=amaturity.Serial() )
    throw TExInvalidSettlement( asettle );

  TDate realmat;

  if( acalendar==0 )
  {
    TCalendar cal;
    realmat = cal.BumpHolidays( amaturity );
  }
  else
    realmat = acalendar->BumpHolidays( amaturity );

  return (floating)(redeem/apx-1)/YearFrac(asettle,realmat,abasis);
}

floating FILYield( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem, int afrequency, int abasis,
           const char* aticker, int aissued, int aintaccr, int afirstcpn )
{
  TSecurity* sec;
  if( aissued==-1 )
    aissued = PrevCouponDate( TDate(asettle), TDate(amaturity), afrequency, abasis ).Serial();
    
  sec = GetSecurity( aticker, acoupon, amaturity, aissued,
                     afrequency, abasis, aintaccr, afirstcpn,
                     aredeem );
  if( sec )
  {
    TDate asettle_dt(asettle);
    floating ret=sec->ConvYield( asettle, aprice );
    delete sec;
    return ret;
  }
  else
    throw TExFINAL( MSG_ExUnableToConstructSecurity );
}

floating FILPrice( int asettle, int amaturity, floating acoupon,
           floating aytm, floating aredeem, int afrequency, int abasis,
           const char* aticker, int aissued, int aintaccr, int afirstcpn )
{
  TSecurity* sec;
  if( aissued==-1 )
    aissued = PrevCouponDate( TDate(asettle), TDate(amaturity), afrequency, abasis ).Serial();
  sec = GetSecurity( aticker, acoupon, amaturity, aissued,
                     afrequency, abasis, aintaccr, afirstcpn,
                     aredeem );
  if( sec )
  {
    TDate asettle_dt(asettle);
    floating ret=sec->ConvPrice( asettle, aytm );
    delete sec;
    return ret;
  }
  else
    throw TExFINAL( MSG_ExUnableToConstructSecurity );
}

floating FILAccruedInterest( int asettle, int amaturity, floating acoupon,
           int afrequency, int abasis,
           const char* aticker, int aissued, int aintaccr, int afirstcpn )

{
  TSecurity* sec;
  if( aissued==-1 )
    aissued = PrevCouponDate( TDate(asettle), TDate(amaturity), afrequency, abasis ).Serial();
  sec = GetSecurity( aticker, acoupon, amaturity, aissued,
                     afrequency, abasis, aintaccr, afirstcpn,
                     100.0 );
  if( sec )
  {
    TDate asettle_dt(asettle);
    floating ret=sec->AccruedInterest( asettle );
    delete sec;
    return ret;
  }
  else
    throw TExFINAL( MSG_ExUnableToConstructSecurity );
}

floating FILDuration( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem, int afrequency, int abasis,
           const char* aticker, int aissued, int aintaccr, int afirstcpn )
{
  TSecurity* sec;
  if( aissued==-1 )
    aissued = PrevCouponDate( TDate(asettle), TDate(amaturity), afrequency, abasis ).Serial();
  sec = GetSecurity( aticker, acoupon, amaturity, aissued,
                     afrequency, abasis, aintaccr, afirstcpn,
                     aredeem );
  if( sec )
  {
    TDate asettle_dt(asettle);
    floating ret=sec->Duration( asettle, aprice );
    delete sec;
    return ret;
  }
  else
    throw TExFINAL( MSG_ExUnableToConstructSecurity );
}

floating FILMDuration( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem, int afrequency, int abasis,
           const char* aticker, int aissued, int aintaccr, int afirstcpn )
{
  TSecurity* sec;
  if( aissued==-1 )
    aissued = PrevCouponDate( TDate(asettle), TDate(amaturity), afrequency, abasis ).Serial();
  sec = GetSecurity( aticker, acoupon, amaturity, aissued,
                     afrequency, abasis, aintaccr, afirstcpn,
                     aredeem );
  if( sec )
  {
    TDate asettle_dt(asettle);
    floating ret=sec->MDuration( asettle, aprice );
    delete sec;
    return ret;
  }
  else
    throw TExFINAL( MSG_ExUnableToConstructSecurity );
}

floating FILConvexity( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem, int afrequency, int abasis,
           const char* aticker, int aissued, int aintaccr, int afirstcpn )
{
  TSecurity* sec;
  if( aissued==-1 )
    aissued = PrevCouponDate( TDate(asettle), TDate(amaturity), afrequency, abasis ).Serial();
  sec = GetSecurity( aticker, acoupon, amaturity, aissued,
                     afrequency, abasis, aintaccr, afirstcpn,
                     aredeem );
  if( sec )
  {
    TDate asettle_dt(asettle);
    floating ret=sec->Convexity( asettle, aprice );
    delete sec;
    return ret;
  }
  else
    throw TExFINAL( MSG_ExUnableToConstructSecurity );
}

floating FILRisk( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem, int afrequency, int abasis,
           const char* aticker, int aissued, int aintaccr, int afirstcpn )
{
  TSecurity* sec;
  if( aissued==-1 )
    aissued = PrevCouponDate( TDate(asettle), TDate(amaturity), afrequency, abasis ).Serial();
  sec = GetSecurity( aticker, acoupon, amaturity, aissued,
                     afrequency, abasis, aintaccr, afirstcpn,
                     aredeem );
  if( sec )
  {
    TDate asettle_dt(asettle);
    floating ret=sec->Risk( asettle, aprice );
    delete sec;
    return ret;
  }
  else
    throw TExFINAL( MSG_ExUnableToConstructSecurity );
}

} // end of namespace final
