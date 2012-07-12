// --------------------------------------------------------------------------
// Financial Analytics Library (FINAL)
// Copyright 2004 Marek Sestak
// Released under the GNU GPL 2.0 license.
//
// * This program is free software;
// * you can redistribute it and/or modify it under the terms of the
// * GNU General Public License as published by the Free Software Foundation;
// * either version 2 of the License, or (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// * See the GNU General Public License for more details.
// *
// * You should have received a copy of the
// * GNU General Public License along with this program;
// * if not, write to the Free Software Foundation, Inc.,
// * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
// * The license can also be found at
// * http://www.opensource.org/licenses/gpl-license.php
// --------------------------------------------------------------------------

#ifndef finalH
#define finalH

/*! \file final.h
    \brief Financial Analytics Library's main header file.

    Header file final.h is a hub for all other subcomponents
    (i.e. fixedincome.h, calendar.h etc) and in most cases it's the
    only header file, that a user program needs to include in order to be
    able to use all the functions, that FINAL offers.
*/
// --------------------------------------------------------------------------

#include "../utils/utils.h"
using namespace utils;

#ifdef USE_VCL
#include <vcl.h>
#pragma hdrstop

typedef AnsiString TString;

#endif


#include "defs.h"
#include "exceptions.h"
#include "calendar.h"
#include "conventions.h"
#include "coupons.h"
#include "fixedincome.h"
#include "security.h"
#include "moneymarket.h"
#include "bonds.h"
#include "floaters.h"
#include "yc.h"
#include "linkers.h"

#endif
