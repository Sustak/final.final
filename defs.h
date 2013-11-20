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

#ifndef defsH
#define defsH

/// The main version number of the library.
#define FINAL_VERSION_NUMBER 1.0
#define FINAL_VERSION "1.0"

/// Build date.
#ifdef __DATE__
  #define FINAL_RELEASE __DATE__
#else
  #define FINAL_RELEASE unknown_date    // for compilers w/o __DATE__ macro
#endif

#define FINAL "Financial analytics library (FINAL), version "FINAL_VERSION", release "FINAL_RELEASE

// --------------------------------------------------------------------------

// --------------------------------------------------------------------------

#include <math.h>

/** \brief Determines the default type for floating point value operations.

    FINAL by default uses <tt>double</tt> for all floating point
    math operations. If FINAL_USE_LONGDOUBLE is defined then
    all operations are performed on the <tt>long double</tt> instead.
*/

#ifdef FINAL_USE_LONGDOUBLE
  #define floating long double
  #define floating_pow powl
  #define floating_fabs fabsl
  #define floating_ceil ceill
  #define floating_floor floorl
  #define floating_frexp frexpl
  #define floating_minprec ((long double)1e-17 )
#else
  #define floating double
  #define floating_pow pow
  #define floating_fabs fabs
  #define floating_ceil ceil
  #define floating_floor floor
  #define floating_frexp frexp
  #define floating_minprec ((double)1e-14)
#endif

// --------------------------------------------------------------------------

/** \brief Maximum number of iterations when numerical methods are used.

    Some of the FINAL's functions aren't calculated by an explicitly given
    formula. In such a situation a numerical approximation approach is used.
    Macro MAX_ITERATIONS defines how many iterations can be used when
    using numerical methods. By default FINAL uses 50 iteration cycles.
*/
#ifndef MAX_ITERATIONS
#define MAX_ITERATIONS     50
#endif

// --------------------------------------------------------------------------

/** \brief Sufficient yield calculation precision.

    For most bonds (or other securities with more than one cashflow)
    when yields are calculated from prices,
    numerical methods are used.

    Macro YIELD_PREC defines sufficient yield precision that all
    FINAL's functions use when using approximation methods to calculate yields.
    For instance, such methods are used for most bonds
    (or other securities with more than one cashflow) in price to yield
    conversions.

    Generally, the higher the precision
    (i.e. the lower the number in YIELD_PREC), the more iteration cycles
    are needed in order to find the result.
*/
#ifndef YIELD_PREC
#define YIELD_PREC         1e-12
#endif

// --------------------------------------------------------------------------

/** \brief Defines the standard settlement of the French B.T.A.N. bonds.

    Standard settlement of French 2 and 5 year bonds (B.T.A.N., also called B.T.N.S.)
    according to market convention is just one day (bond trades settle
    one day after trade has been executed). If you however need to change
    this to any other number of days (for example 3, which is standard
    for all other european government bonds), redefine the macro
    BTNS_STD_SETTLEMENT.
*/

#ifndef BTNS_STD_SETTLEMENT
#define BTNS_STD_SETTLEMENT 1
#endif

// --------------------------------------------------------------------------

/** \brief Defines whether accrued interest of the French B.T.A.N. bonds is rounded to the 7th decimal.

    According to the decision of the CNO (Comite De Nomarmalisation Obligataire)
    accrued interest for all french domestic securities should be rounded
    to the 7th decimal. However the current market practice dictates no rounding
    to be applied at all... Define macro BTNS_ACCRUED_ROUNDED to enable the
    rounding.
*/

#define BTNS_ACCRUED_ROUNDED
#ifdef BTNS_ACCRUED_ROUNDED
#endif

// --------------------------------------------------------------------------

#define SEC_DES_MATURITY  "Maturity"
#define SEC_DES_COUPON    "Coupon"
#define SEC_DES_FREQUENCY "Frequency"
#define SEC_DES_BASIS     "Basis"
#define SEC_DES_INTACCR   "InterestAccruesFrom"
#define SEC_DES_ISSUED    "Issued"
#define SEC_DES_FIRSTCPN  "FirstCouponDate"
#define SEC_DES_SECTYPE   "SecurityType"
#define SEC_DES_REDEEMS   "Redemption"
#define SEC_DES_EXDIV     "ExDividendDays"

// --------------------------------------------------------------------------

namespace final {

// In some enviroments macro Yield is already defined! In order to be able to
// work properly, we'll undefine it now and define it back at the end of the
// header file...

#ifdef Yield
#define FINAL_ORIGINAL_Yield Yield
#undef Yield
#endif

} // end of namespace final

#endif
