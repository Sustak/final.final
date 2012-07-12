
// Financial Analytics Library (FINAL)
// Copyright 2004 Marek Sestak
// Released under the GNU GPL 2.0 license.

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
  NL_365 = 9, NL_360 = 10, NL_ACT = 11,

  NONEOM_start = 100,
  US_30_360_NONEOM = 100, ACT_ACT_NONEOM = 101, ACT_360_NONEOM = 102,
  ACT_365_NONEOM = 103, ISMA_30_360_NONEOM = 104, ISMA_30_365_NONEOM = 105,
  ISMA_30_ACT_NONEOM = 106, US_30_365_NONEOM = 107, US_30_ACT_NONEOM = 108
} TDaycountConvention;

floating YearFrac( const TDate& astart, const TDate& aend, int abasis );
int NDays( const TDate& afrom, const TDate& ato, int abasis );
floating Denum( const TDate& prevcpn, const TDate& nextcpn, int afrequency, int abasis );


} // end of namespace final

#endif
