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

#ifndef conventionsH
#define conventionsH

#include "defs.h"

#include "../utils/utils.h"
using namespace utils;

namespace final {

/** \brief Day-count conventions used in FINAL's operations.

    Day-count conventions (also a day-count basis, or simply a basis)
    describe methods used to determine time span between two given dates.
*/

typedef enum // --------------------------------------------------------------
{
  US_30_360 = 0,  ACT_ACT = 1, ACT_360 = 2, ACT_365 = 3, ISMA_30_360 = 4,
  ISMA_30_365 = 5, ISMA_30_ACT = 6, US_30_365 = 7, US_30_ACT = 8,
  NL_365 = 9, NL_360 = 10, NL_ACT = 11, ACT_ACT_ISDA = 12,

  NONEOM_start = 100,
  US_30_360_NONEOM = 100, ACT_ACT_NONEOM = 101, ACT_360_NONEOM = 102,
  ACT_365_NONEOM = 103, ISMA_30_360_NONEOM = 104, ISMA_30_365_NONEOM = 105,
  ISMA_30_ACT_NONEOM = 106, US_30_365_NONEOM = 107, US_30_ACT_NONEOM = 108,
  ACT_ACT_ISDA_NONEOM = 112
} TDaycountConvention;

floating YearFrac( const TDate& astart, const TDate& aend, int abasis );
int NDays( const TDate& afrom, const TDate& ato, int abasis );
floating Denum( const TDate& prevcpn, const TDate& nextcpn, int afrequency, int abasis );


} // end of namespace final

#endif
