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

#ifndef exceptionsH
#define exceptionsH

/** \file exceptions.h
    \brief Contains all exception classes used by FINAL.

    All FINAL's exception handling classes are derived from TExFIL.
*/

#include "defs.h"

#include "../utils/utils.h"
using namespace utils;

namespace final {

class TExFINAL: public TException
{
public:
                TExFINAL();
                TExFINAL( const TString &amsg );
                TExFINAL( const TDate& adate1, const TString& amsg );
                TExFINAL( const TDate& adate1, const TDate& adate2, const TString& amsg );
};

class TExInvalidDate : public TExFINAL
{
public:
                TExInvalidDate( const TDate& adate );
                TExInvalidDate( const TDate& adate, const TString &amsg );
};

class TExInvalidPrice : public TExFINAL
{
public:
                TExInvalidPrice( floating apx );
};

class TExInvalidCouponFrequency : public TExFINAL
{
public:
                TExInvalidCouponFrequency( int afq );
};

class TExInvalidSettlement: public TExFINAL
{
public:
                TExInvalidSettlement( const TDate& asettle );
};

class TExInvalidCoupon: public TExFINAL
{
public:
                TExInvalidCoupon( const floating acoupon );
};

class TExInvalidRedemption: public TExFINAL
{
public:
                TExInvalidRedemption( const floating aredeem );
};

class TExMoreThanOneCashflow: public TExFINAL
{
public:
                TExMoreThanOneCashflow( const TDate& asettle );
};

class TExTooExtremeValues: public TExFINAL
{
public:
                TExTooExtremeValues();
};

class TExNoCouponCashflows: public TExFINAL
{
public:
                TExNoCouponCashflows();
};

class TExCannotPerformOnFRN: public TExFINAL
{
public:
                TExCannotPerformOnFRN();
};

} // end of namespace final

#ifndef MSG_ExGeneral
#define MSG_GeneralException            "General FINAL exception"
#endif

#ifndef MSG_ExTooExtremeValues
#define MSG_ExTooExtremeValues          "Error while computing yield/price; probably too extreme values"
#endif

#ifndef MSG_ExInvalidDate
#define MSG_ExInvalidDate               "Invalid date format (%d.%d.%d)"
#endif

#ifndef MSG_ExInvalidSettlement
#define MSG_ExInvalidSettlement         "Invalid settlement date (%d.%d.%d)"
#endif

#ifndef MSG_ExInvalidMaturity
#define MSG_ExInvalidMaturity           "Invalid maturity date (%d.%d.%d)"
#endif

#ifndef MSG_ExInvalidIssueDate
#define MSG_ExInvalidIssueDate          "Invalid issue date (%d.%d.%d)"
#endif

#ifndef MSG_ExInvalidIntAccr
#define MSG_ExInvalidIntAccr            "Invalid interest accrual date (%d.%d.%d)"
#endif

#ifndef MSG_ExInvalidFirstCpn
#define MSG_ExInvalidFirstCpn           "Invalid first coupon date (%d.%d.%d)"
#endif

#ifndef MSG_ExInvalidPrice
#define MSG_ExInvalidPrice              "Invalid price (%g)"
#endif

#ifndef MSG_ExInvalidCouponFrequency
#define MSG_ExInvalidCouponFrequency    "Invalid coupon frequency (%d)"
#endif

#ifndef MSG_ExInvalidCoupon
#define MSG_ExInvalidCoupon             "Invalid coupon size (%g)"
#endif

#ifndef MSG_ExInvalidRedemption
#define MSG_ExInvalidRedemption         "Invalid redemption value (%g)"
#endif

#ifndef MSG_ExMoreThanOneCashflow
#define MSG_ExMoreThanOneCashflow       "There's more than one coupon/redemption payment after the settlement (%d.%d.%d)"
#endif

#ifndef MSG_ExUnableToConstructSecurity
#define MSG_ExUnableToConstructSecurity "Unable to create a security from the given data"
#endif

#ifndef MSG_ExNoCouponCashflows
#define MSG_ExNoCouponCashflows         "Security has no coupons."
#endif

#ifndef MSG_ExCannotPerformOnFRN
#define MSG_ExCannotPerformOnFRN        "This function cannot be performed on floating rate notes."
#endif

#ifndef MSG_ExNextCouponNotDefined
#define MSG_ExNextCouponNotDefined      "Next coupon date cannot be found. Probably no coupon has been defined after %d.%d.%d."
#endif

#endif
