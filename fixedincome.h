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

#ifndef fixedincomeH
#define fixedincomeH

#include "defs.h"
#include "conventions.h"
#include "../utils/maths.h"
#include "security.h"
#include "bonds.h"
#include "moneymarket.h"

namespace final {

// --------------------------------------------------------------------------

TSecurity* GetSecurity( const TString &aticker, floating acoupon, int amaturity,
                        int aissued,
                        int afrequency=1, int abasis=ACT_ACT,
                        int aintaccr=-1, int afirstcpn=-1, floating aredemption=100.0 );

// --------------------------------------------------------------------------

floating AccruedInterest( const TDate& asettle, const TDate& amaturity,
           floating acoupon, floating apar, int afrequency, int abasis );

floating AccruedInterest( const TDate& asettle, const TDate& aissued,
           const TDate& amaturity,
           floating acoupon, floating apar, int afrequency, int abasis );

floating AccruedInterest( const TDate& asettle, const TDate& aissued,
           const TDate& amaturity, const TDate& aintaccr, const TDate& afirstcpn,
           floating acoupon, floating apar, int afrequency, int abasis );

// --------------------------------------------------------------------------

// prevod z ytm do ciste ceny pro bondy
floating Price( const TDate& asettle, const TDate& amaturity, floating acoupon,
           floating aytm, floating aredem=100.0, int afrequency=1, int abasis=ACT_ACT );
floating TruePrice( const TDate& asettle, const TDate& amaturity, floating acoupon,
           floating aytm, floating aredem=100.0, int afrequency=1, int abasis=ACT_ACT,
           const TCalendar* cal=0 );

// prevod z ciste ceny do ytm pro bondy
floating Yield( const TDate& asettle, const TDate& amaturity, floating acoupon,
           floating aprice, floating aredem=100.0, int afrequency=1, int abasis=ACT_ACT );
floating TrueYield( const TDate& asettle, const TDate& amaturity, floating acoupon,
           floating aprice, floating aredem=100.0, int afrequency=1, int abasis=ACT_ACT,
           const TCalendar* cal=0 );

// jednoduchy yield/celkova cena a naopak
floating PriceSimple( const TDate& asettle, const TDate& amaturity,
           floating ayield, floating redeem=100.0, int abasis=ACT_360 );
floating YieldSimple( const TDate& asettle, const TDate& amaturity,
           floating apx, floating redeem=100.0, int abasis=ACT_360 );
floating TruePriceSimple( const TDate& asettle, const TDate& amaturity,
           floating ayield, floating redeem=100.0, int abasis=ACT_360,
           const TCalendar* acalendar=0 );
floating TrueYieldSimple( const TDate& asettle, const TDate& amaturity,
           floating apx, floating redeem=100.0, int abasis=ACT_360,
           const TCalendar* acalendar=0 );

// obecne funkce pro vypocet yieldu/ceny/accrued interestu/durace/
//                           modifikovane durace/convexity/risk faktoru
floating FILPrice( int asettle, int amaturity, floating acoupon,
           floating ayield, floating aredeem=100.0, int afrequency=1, int abasis=ACT_ACT,
           const char* aticker=0, int issued=-1, int intaccr=-1, int firstcpn=-1 );
floating FILYield( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem=100.0, int afrequency=1, int abasis=ACT_ACT,
           const char* aticker=0, int issued=-1, int intaccr=-1, int firstcpn=-1 );
floating FILAccruedInterest( int asettle, int amaturity, floating acoupon,
           int afrequency=1, int abasis=ACT_ACT,
           const char* aticker=0, int issued=-1, int intaccr=-1, int firstcpn=-1 );
floating FILDuration( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem=100.0, int afrequency=1, int abasis=ACT_ACT,
           const char* aticker=0, int issued=-1, int intaccr=-1, int firstcpn=-1 );
floating FILMDuration( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem=100.0, int afrequency=1, int abasis=ACT_ACT,
           const char* aticker=0, int issued=-1, int intaccr=-1, int firstcpn=-1 );
floating FILConvexity( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem=100.0, int afrequency=1, int abasis=ACT_ACT,
           const char* aticker=0, int issued=-1, int intaccr=-1, int firstcpn=-1 );
floating FILRisk( int asettle, int amaturity, floating acoupon,
           floating aprice, floating aredeem=100.0, int afrequency=1, int abasis=ACT_ACT,
           const char* aticker=0, int issued=-1, int intaccr=-1, int firstcpn=-1 );

// --------------------------------------------------------------------------

floating ConvertCompounding( floating ayld, int acomp, int anewcomp );

// --------------------------------------------------------------------------

} // end of namespace final

#endif
