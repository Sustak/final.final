
// Financial Analytics Library (FINAL)
// Copyright 2004 Marek Sestak
// Released under the GNU GPL 2.0 license.

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
