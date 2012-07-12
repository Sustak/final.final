
// Financial Analytics Library (FINAL)
// Copyright 2004 Marek Sestak
// Released under the GNU GPL 2.0 license.

#include "coupons.h"
#include "conventions.h"
#include "exceptions.h"

namespace final {

int CouponNum( const TDate& asettle, const TDate& amaturity, int afrequency,
               int abasis )
{
  int result;
  int nmonths;

  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency!=4
      && afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );

  asettle.Validate();
  amaturity.Validate();

  if( asettle.Serial()>=amaturity.Serial() )
    return 0;

  nmonths = (amaturity.Year()-asettle.Year())*12+(amaturity.Month()-asettle.Month());

  if( abasis>=NONEOM_start )
  {
    if( asettle.Day()>=amaturity.Day() )
      nmonths--;
    else
      if( asettle.IsMonthEnd() )
        nmonths--;
  }
  else
  {
    if( amaturity.IsMonthEnd() && asettle.IsMonthEnd() )
    {
        nmonths--;
    }
    else {
        if( amaturity.IsMonthEnd()==false ) { 
            if( asettle.Day()>=amaturity.Day() )
             nmonths--;
            else
            {
             if( asettle.IsMonthEnd() )
               nmonths--;
            }
        }
    }
  }  

  result = nmonths/(12/afrequency) + 1;

  return result;
}

TDate PrevCouponDate( const TDate& asettle, const TDate& amaturity,
                      int afrequency, int abasis )
{
  TDate result;
  int   ncpn;

  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency!=4
      && afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );

  asettle.Validate();
  amaturity.Validate();

  if( asettle.Serial()>amaturity.Serial() )
    throw TExInvalidSettlement( asettle );

  if( asettle==amaturity )
    return amaturity;

  ncpn = CouponNum( asettle,amaturity,afrequency,abasis )-1;
  result = amaturity.EDate( -(ncpn+1)*(12/afrequency) );

  if( abasis<NONEOM_start )
  {
    if( amaturity.IsMonthEnd() )
      result.SetToMonthEnd();
  }
  else
  {
    if( !result.IsValid() )
      result.SetToMonthEnd();
  }

  return result;
}

TDate NextCouponDate( const TDate &asettle, const TDate &amaturity,
                      int afrequency, int abasis )
{
  TDate result;
  int   ncpn;

  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency!=4
      && afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );

  asettle.Validate();
  amaturity.Validate();

  if( asettle.Serial()>=amaturity.Serial() )
    throw TExInvalidSettlement( asettle );

  ncpn = CouponNum( asettle,amaturity,afrequency,abasis )-1;
  result = amaturity.EDate( -(ncpn)*(12/afrequency) );

  if( abasis<NONEOM_start )
  {
    if( amaturity.IsMonthEnd() )
      result.SetToMonthEnd();
  }
  else
  {
    if( !result.IsValid() )
      result.SetToMonthEnd();
  }

  return result;
}

TDate NextCouponDate( const TDate &acouponDate, int afrequency )
{
  TDate result;
  int   ncpn;

  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency!=4
      && afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );

  acouponDate.Validate();

  result = acouponDate.EDate( 12/afrequency );

  return result;
}

int CouponDaysBS( const TDate& asettle, const TDate& amaturity,
                  int afrequency, int abasis )
{
  TDate beginning;
  beginning = PrevCouponDate( asettle, amaturity, afrequency, abasis );
  return NDays( beginning, asettle, abasis );
}

int CouponDays( const TDate& asettle, const TDate& amaturity,
                int afrequency, int abasis )
{
  TDate beginning, next;
  beginning = PrevCouponDate( asettle, amaturity, afrequency, abasis );
  next      = NextCouponDate( asettle, amaturity, afrequency, abasis );
  return NDays( beginning, next, abasis );
}

int CouponDaysNC( const TDate& asettle, const TDate& amaturity,
                  int afrequency, int abasis )
{
  TDate next;
  next = NextCouponDate( asettle, amaturity, afrequency, abasis );
  return NDays( asettle, next, abasis );
}

floating CouponPayment( const TDate& acoupondate, const TDate& amaturity,
           floating acoupon, floating apar, int afrequency, int abasis )
{
  if( acoupondate.Serial()>amaturity.Serial() )
    throw TExInvalidSettlement( acoupondate );

  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency!=4
      && afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );

  if( !acoupondate.IsValid() )
    throw TExInvalidDate( acoupondate );
  if( !amaturity.IsValid() )
    throw TExInvalidDate( amaturity, MSG_ExInvalidMaturity );

  TDate dt_prev = PrevCouponDate( acoupondate, amaturity, afrequency, abasis );

  if( dt_prev!=acoupondate )
    throw TExFINAL( "Cannot return coupon payments for non-coupon dates." );
  else
    return apar*acoupon/afrequency;
}

floating CouponPayment( const TDate& acoupondate, const TDate& aissued,
           const TDate& amaturity, floating acoupon, floating apar,
           int afrequency, int abasis )
{
  TDate next_cpn = NextCouponDate( aissued, amaturity, afrequency, abasis );
  return CouponPayment( acoupondate, aissued, amaturity, aissued, next_cpn,
                          acoupon, apar, afrequency, abasis );
}

floating CouponPayment( const TDate& acoupondate, const TDate& aissued,
           const TDate& amaturity, const TDate& aintaccr, const TDate& afirstcpn,
           floating acoupon, floating apar,
           int afrequency, int abasis )
{
  if( afrequency!=1 && afrequency!=2 && afrequency!=3 && afrequency!=4
      && afrequency!=6 && afrequency!=12 )
    throw TExInvalidCouponFrequency( afrequency );

  if( !acoupondate.IsValid() )
    throw TExInvalidDate( acoupondate );
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
    throw TExFINAL( "First coupon date should be or or before maturity date." );

  if( acoupondate>amaturity || acoupondate<afirstcpn )
    throw TExInvalidDate( acoupondate, "Invalid coupon date (%d.%d.%d)." );

  TDate cpndt_before_coupondate = PrevCouponDate( acoupondate, amaturity, afrequency, abasis );
  if( cpndt_before_coupondate != acoupondate ) {
    
    
    // special odd-coupon bonds
    
    if( acoupondate == afirstcpn )
        return YearFrac( aissued, afirstcpn, abasis ) * acoupon;
    
    throw TExFINAL( "Cannot return coupon payments for non-coupon dates." );
  }

  if( acoupondate != afirstcpn )
    return apar*acoupon/afrequency;

  TDate cpndt_after_intaccr = NextCouponDate( aintaccr, amaturity, afrequency, abasis );
  TDate cpndt_before_intaccr = PrevCouponDate( aintaccr, amaturity, afrequency, abasis );

  if( cpndt_before_intaccr == aintaccr )
    return apar*acoupon/afrequency*
           CouponNum( aintaccr, afirstcpn, afrequency, abasis );

  floating accrued = apar * acoupon / afrequency *
    NDays( aintaccr, cpndt_after_intaccr, abasis ) /
    Denum( cpndt_before_intaccr, cpndt_after_intaccr, afrequency, abasis );
  accrued += apar * acoupon / afrequency *
             CouponNum( cpndt_after_intaccr, afirstcpn, afrequency, abasis );
  return accrued;
}

} // end of namespace final
