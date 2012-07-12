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
