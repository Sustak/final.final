
// Financial Analytics Library (FINAL)
// Copyright 2004 Marek Sestak
// Released under the GNU GPL 2.0 license.

#ifndef couponsH
#define couponsH

#include "defs.h"

#include "../utils/utils.h"
using namespace utils;

namespace final {

int      CouponNum( const TDate& asettle, const TDate& amaturity,
           int afrequency, int abasis );
TDate    PrevCouponDate( const TDate &asettle, const TDate &amaturity,
           int afrequency, int abasis );
TDate    NextCouponDate( const TDate &asettle, const TDate &amaturity,
           int afrequency, int abasis );
// vraci datum pristiho/minuleho kuponu
// nebere pritom v potaz abasis az na modifikator NON_EOM

TDate    NextCouponDate( const TDate &acouponDate, int afrequency );
// vraci datum pristiho kuponu v zavislosti na datumu kuponu predchoziho
// (tato verze je znacne zjednodusena v porovnani s tou, ktera bere v uvahu i 
//  basis a maturitu!)


int      CouponDaysBS( const TDate& asettle, const TDate& amaturity,
           int afrequency, int abasis );
// vraci pocet dnu od zacatku kuponoveho obdobi do settlementu

int      CouponDays( const TDate& asettle, const TDate& amaturity,
           int afrequency, int abasis );
// vraci pocet dnu kuponoveho obdobi obsahujiciho settlement

int      CouponDaysNC( const TDate& asettle, const TDate& amaturity,
           int afrequency, int abasis );
// vraci pocet dnu od settlementu do nasledujciho kuponu

floating CouponPayment( const TDate& acoupondate, const TDate& amaturity,
           floating acoupon, floating apar, int afrequency, int abasis );
floating CouponPayment( const TDate& acoupondate, const TDate& aissued,
           const TDate& amaturity, floating acoupon, floating apar,
           int afrequency, int abasis );
floating CouponPayment( const TDate& acoupondate, const TDate& aissued,
           const TDate& amaturity, const TDate& aintaccr, const TDate& afirstcpn,
           floating acoupon, floating apar, int afrequency, int abasis );
// vyplata kuponu k dni acoupondate bondu s pevnym kuponem
// prvni verze navic pocita s "jednoduchym" bondem, kterej je vydanej v den
//   kuponu a nema short/long prvni kupon
// druha verze predpoklada, ze kupon zacina nabihat v den vydani

} // end of namespace final

#endif
