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

#include "moneymarket.h"

#include "exceptions.h"
#include "conventions.h"
#include "fixedincome.h"

namespace final {

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TZeroCpn
//

floating TZeroCpn::DurationY( const TDate& asettle, floating aytm ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  return YearFrac(asettle,maturity,basis);
}

floating TZeroCpn::MDurationY( const TDate& asettle, floating aytm ) const
{
  return DurationY(asettle, aytm) / ( 1.0+YearFrac(asettle,maturity,basis)*aytm );
}

floating TZeroCpn::Yield( const TDate& asettle, floating apx ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );
  if( apx<=0 )
    throw TExInvalidPrice( apx );

  return MMktYield( asettle, apx, false, basis );
}

TDate TZeroCpn::StandardSettlement( const TDate& adate ) const
{
  return WorkDay( adate, 2 );
}

floating TZeroCpn::Price( const TDate& asettle, floating ayield ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  return MMktPrice( asettle, ayield, false, basis );
}

floating TZeroCpn::TrueYield( const TDate& asettle, floating apx ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );
  if( apx<=0 )
    throw TExInvalidPrice( apx );

  return MMktYield( asettle, apx, true, basis );
}

floating TZeroCpn::TruePrice( const TDate& asettle, floating ayield ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  return MMktPrice( asettle, ayield, true, basis );
}

TDate TZeroCpn::PrevCouponDate( const TDate& asettle ) const
{
//  May-05-2005
//  throw TExNoCouponCashflows();
  if( asettle<issued || asettle>maturity )
    throw TExInvalidSettlement( asettle );

  return issued;
}

TDate TZeroCpn::NextCouponDate( const TDate& asettle ) const
{
//  May-05-2005
//  throw TExNoCouponCashflows();
  if( asettle<issued || asettle>maturity )
    throw TExInvalidSettlement( asettle );

  return issued;
}

int TZeroCpn::NextCashflow( const TDate& asettle, TDate& anext,
                          floating& aamount ) const
{
  if( asettle.Serial()<maturity.Serial() )
  {
    anext = maturity;
    aamount = 100.0;
    return 1;
  }
  else
    return 0;
}

int TZeroCpn::CashflowsTillMaturity( const TDate& asettle ) const
{
  if( asettle.Serial()<maturity.Serial() )
  {
    return 1;
  }
  else
    return 0;
}

floating TZeroCpn::CashflowAmount( const TDate& asettle ) const
{
  if( asettle==maturity )
    return 100.0;
  else
    return 0.0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TUSB
//

TDate TUSB::StandardSettlement( const TDate& atradedate ) const
{
  return WorkDay( atradedate, 1 );
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TJGTB
//

TDate TJGTB::StandardSettlement( const TDate& atradedate ) const
{
  return WorkDay( atradedate, 4 );
};

floating TJGTB::MMktPrice( const TDate& atradedate, floating ammktyield,
                           int abumpholidays, int abasis ) const
{
  return RoundDown( TZeroCpn::MMktPrice(atradedate, ammktyield, abumpholidays, abasis), 4 );
}

floating TJGTB::MMktYield( const TDate& atradedate, floating aprice,
                           int abumpholidays, int abasis ) const
{
  return TZeroCpn::MMktYield(atradedate, aprice, abumpholidays, abasis);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TACTB
//

TDate TACTB::StandardSettlement( const TDate& atradedate ) const {
    return WorkDay( atradedate, 3 );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// TCd
//

TCd::TCd( const TDate& amaturity, const TDate& aissued, floating acoupon, int abasis,
          floating aamount ):
  TSecurity( amaturity, aissued, abasis )
{
  coupon = acoupon * 100;
  amount = aamount;

  matflow = YearFrac( aissued, amaturity, abasis ) * coupon;
  if( amount > 0.0 )
    matflow = Round( matflow*amount/100.0, 2 ) / amount * 100;

  matflow += 100.0;
//  matflow = 100 + YearFrac( aissued, amaturity, abasis ) * coupon;
}

TDate TCd::StandardSettlement( const TDate& adate ) const
{
  return WorkDay( adate, 2 );
}

floating TCd::DurationY( const TDate& asettle, floating aytm ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  return YearFrac(asettle, maturity, basis);
}

floating TCd::MDurationY( const TDate& asettle, floating aytm ) const
{
  return DurationY(asettle, aytm) / ( 1.0+YearFrac(asettle,maturity,basis)*aytm );
}

floating TCd::Yield( const TDate& asettle, floating apx ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );
  if( apx<=0 )
    throw TExInvalidPrice( apx );

  return MMktYield( asettle, apx, false, basis );
}

floating TCd::Price( const TDate& asettle, floating ayield ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  return MMktPrice( asettle, ayield, false, basis );
}

floating TCd::TrueYield( const TDate& asettle, floating apx ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );
  if( apx<=0 )
    throw TExInvalidPrice( apx );

  return MMktYield( asettle, apx, true, basis );
}

floating TCd::TruePrice( const TDate& asettle, floating ayield ) const
{
  if( asettle.Serial()>=maturity.Serial() ||
      asettle.Serial()<issued.Serial() )
    throw TExInvalidSettlement( asettle );

  return MMktPrice( asettle, ayield, true, basis );
}

TDate TCd::PrevCouponDate( const TDate& asettle ) const
{
//  May-05-2005
//  if( coupon == 0 )
//    throw TExNoCouponCashflows();

  if( asettle<issued || asettle>maturity )
    throw TExInvalidSettlement( asettle );

  return issued;
}

TDate TCd::NextCouponDate( const TDate& asettle ) const
{
//  May-05-2005
//  if( coupon == 0.0 )
//    throw TExNoCouponCashflows();

  if( asettle<issued || asettle>=maturity )
    throw TExInvalidSettlement( asettle );

  return maturity;
}

int TCd::NextCashflow( const TDate& asettle, TDate& anext, floating& aamount ) const
{
  if( asettle.Serial()<maturity.Serial() )
  {
    anext = maturity;
    aamount = matflow;
    return 1;
  }
  else
    return 0;
}

int TCd::CashflowsTillMaturity( const TDate& asettle ) const
{
  if( asettle.Serial()<maturity.Serial() )
  {
    return 1;
  }
  else
    return 0;
}

floating TCd::CashflowAmount( const TDate& asettle ) const
{
  if( asettle==maturity )
    return matflow;
  else
    return 0.0;
}

floating TCd::AccruedInterest( const TDate& asettle ) const
{
  if( asettle<issued || asettle>maturity )
    throw TExInvalidSettlement( asettle );

  return final::AccruedInterest( asettle, issued, maturity, coupon/100.0, 100.0, 1, basis );
}

} // end of namespace final

