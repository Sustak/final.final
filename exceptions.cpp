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

#include "exceptions.h"

namespace final {

TExFINAL::TExFINAL()
{
  Message =  MSG_GeneralException;
}

TExFINAL::TExFINAL( const TString &amsg ): TException( amsg )
{}

TExFINAL::TExFINAL( const TDate& adate1, const TString& amsg )
{
  Message.printf( amsg.c_str(), adate1.Day(), adate1.Month(), adate1.Year() );
}

TExFINAL::TExFINAL( const TDate& adate1, const TDate& adate2, const TString& amsg )
{
  Message.printf( amsg.c_str(), adate1.Day(), adate1.Month(), adate1.Year(),
        adate2.Day(), adate2.Month(), adate2.Year() );
}

// --------------------------------------------------------------------------

TExInvalidDate::TExInvalidDate( const TDate& adate, const TString& amsg )
{
  Message.printf( amsg.c_str(), adate.Day(), adate.Month(), adate.Year() );
}

TExInvalidDate::TExInvalidDate( const TDate& adate )
{
  Message.printf( MSG_ExInvalidDate, adate.Day(), adate.Month(), adate.Year() );
}

// --------------------------------------------------------------------------

TExInvalidPrice::TExInvalidPrice( const floating apx )
{
  Message.printf( MSG_ExInvalidPrice, apx );
}

// --------------------------------------------------------------------------

TExInvalidCouponFrequency::TExInvalidCouponFrequency( const int afq )
{
  Message.printf( MSG_ExInvalidCouponFrequency, afq );
}

// --------------------------------------------------------------------------

TExInvalidSettlement::TExInvalidSettlement( const TDate& asettle )
{
  Message.printf( MSG_ExInvalidSettlement, asettle.Day(), asettle.Month(), asettle.Year() );
}

// --------------------------------------------------------------------------

TExInvalidCoupon::TExInvalidCoupon( const floating acoupon )
{
  Message.printf( MSG_ExInvalidCoupon, acoupon );
}

// --------------------------------------------------------------------------

TExInvalidRedemption::TExInvalidRedemption( const floating aredeem )
{
  Message.printf( MSG_ExInvalidRedemption, aredeem );
}

// --------------------------------------------------------------------------

TExMoreThanOneCashflow::TExMoreThanOneCashflow( const TDate& asettle )
{
  Message.printf( MSG_ExMoreThanOneCashflow, asettle.Day(), asettle.Month(), asettle.Year() );
}

// --------------------------------------------------------------------------

TExTooExtremeValues::TExTooExtremeValues()
{
  Message = MSG_ExTooExtremeValues;
}

// --------------------------------------------------------------------------

TExNoCouponCashflows::TExNoCouponCashflows()
{
  Message = MSG_ExNoCouponCashflows;
}

// --------------------------------------------------------------------------

TExCannotPerformOnFRN::TExCannotPerformOnFRN()
{
  Message = MSG_ExCannotPerformOnFRN;
}

// --------------------------------------------------------------------------

} // end of namespace final
