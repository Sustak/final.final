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
