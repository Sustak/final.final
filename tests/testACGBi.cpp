/* 
 * File:   testACGBi.cpp
 * Author: u03061
 *
 * Created on 17.7.2012, 14:16:28
 */

#include <stdlib.h>
#include <iostream>

#include "../final/final.h"
#include "../utils/datetime.h"

using namespace final;
using namespace utils;

/*
 * Simple C++ Test Suite
 */

namespace final {
       floating CPIIndexValueACGBi( const TDate& avaluedate, const TDataSeries& acpiseries,
        int ainterestaccruesfrom, int afirstcoupondate );
}

void testCPIIndexValueACGBi() {
    
    // this test replicates figures shown in excel sheet 
    //   'acgb inflation linkers calcs.xls' found in the docs folder...
    
    TDate avaluedate = TDate(19,7,2012);
    TDataSeries acpiseries;

    acpiseries.Add(TDate(31, 12, 1990), 1.71);
    acpiseries.Add(TDate(31, 3, 1991), 1.21);
    acpiseries.Add(TDate(30, 6, 1991), 0);
    acpiseries.Add(TDate(30, 9, 1991), 0.38);
    acpiseries.Add(TDate(31, 12, 1991), 0.75);
    acpiseries.Add(TDate(31, 3, 1992), 0.47);
    acpiseries.Add(TDate(30, 6, 1992), -0.14);
    acpiseries.Add(TDate(30, 9, 1992), -0.09);
    acpiseries.Add(TDate(31, 12, 1992), 0.28);
    acpiseries.Add(TDate(31, 3, 1993), 0.7);
    acpiseries.Add(TDate(30, 6, 1993), 0.65);
    acpiseries.Add(TDate(30, 9, 1993), 0.41);
    acpiseries.Add(TDate(31, 12, 1993), 0.32);
    acpiseries.Add(TDate(31, 3, 1994), 0.27);
    acpiseries.Add(TDate(30, 6, 1994), 0.55);
    acpiseries.Add(TDate(30, 9, 1994), 0.68);
    acpiseries.Add(TDate(31, 12, 1994), 0.72);
    acpiseries.Add(TDate(31, 3, 1995), 1.25);
    acpiseries.Add(TDate(30, 6, 1995), 1.51);
    acpiseries.Add(TDate(30, 9, 1995), 1.26);
    acpiseries.Add(TDate(31, 12, 1995), 0.99);
    acpiseries.Add(TDate(31, 3, 1996), 0.6);
    acpiseries.Add(TDate(30, 6, 1996), 0.55);
    acpiseries.Add(TDate(30, 9, 1996), 0.46);
    acpiseries.Add(TDate(31, 12, 1996), 0.21);
    acpiseries.Add(TDate(31, 3, 1997), 0.17);
    acpiseries.Add(TDate(30, 6, 1997), -0.04);
    acpiseries.Add(TDate(30, 9, 1997), -0.33);
    acpiseries.Add(TDate(31, 12, 1997), -0.08);
    acpiseries.Add(TDate(31, 3, 1998), 0.25);
    acpiseries.Add(TDate(30, 6, 1998), 0.42);
    acpiseries.Add(TDate(30, 9, 1998), 0.42);
    acpiseries.Add(TDate(31, 12, 1998), 0.37);
    acpiseries.Add(TDate(31, 3, 1999), 0.21);
    acpiseries.Add(TDate(30, 6, 1999), 0.16);
    acpiseries.Add(TDate(30, 9, 1999), 0.66);
    acpiseries.Add(TDate(31, 12, 1999), 0.74);
    acpiseries.Add(TDate(31, 3, 2000), 0.73);
    acpiseries.Add(TDate(30, 6, 2000), 0.85);
    acpiseries.Add(TDate(30, 9, 2000), 2.28);
    acpiseries.Add(TDate(31, 12, 2000), 2.02);
    acpiseries.Add(TDate(31, 3, 2001), 0.69);
    acpiseries.Add(TDate(30, 6, 2001), 0.95);
    acpiseries.Add(TDate(30, 9, 2001), 0.57);
    acpiseries.Add(TDate(31, 12, 2001), 0.6);
    acpiseries.Add(TDate(31, 3, 2002), 0.89);
    acpiseries.Add(TDate(30, 6, 2002), 0.81);
    acpiseries.Add(TDate(30, 9, 2002), 0.7);
    acpiseries.Add(TDate(31, 12, 2002), 0.69);
    acpiseries.Add(TDate(31, 3, 2003), 1.01);
    acpiseries.Add(TDate(30, 6, 2003), 0.65);
    acpiseries.Add(TDate(30, 9, 2003), 0.28);
    acpiseries.Add(TDate(31, 12, 2003), 0.53);
    acpiseries.Add(TDate(31, 3, 2004), 0.7);
    acpiseries.Add(TDate(30, 6, 2004), 0.7);
    acpiseries.Add(TDate(30, 9, 2004), 0.45);
    acpiseries.Add(TDate(31, 12, 2004), 0.59);
    acpiseries.Add(TDate(31, 3, 2005), 0.72);
    acpiseries.Add(TDate(30, 6, 2005), 0.65);
    acpiseries.Add(TDate(30, 9, 2005), 0.78);
    acpiseries.Add(TDate(31, 12, 2005), 0.74);
    acpiseries.Add(TDate(31, 3, 2006), 0.7);
    acpiseries.Add(TDate(30, 6, 2006), 1.23);
    acpiseries.Add(TDate(30, 9, 2006), 1.25);
    acpiseries.Add(TDate(31, 12, 2006), 0.39);
    acpiseries.Add(TDate(31, 3, 2007), -0.03);
    acpiseries.Add(TDate(30, 6, 2007), 0.64);
    acpiseries.Add(TDate(30, 9, 2007), 0.96);
    acpiseries.Add(TDate(31, 12, 2007), 0.83);
    acpiseries.Add(TDate(31, 3, 2008), 1.13);
    acpiseries.Add(TDate(30, 6, 2008), 1.41);
    acpiseries.Add(TDate(30, 9, 2008), 1.33);
    acpiseries.Add(TDate(31, 12, 2008), 0.43);
    acpiseries.Add(TDate(31, 3, 2009), -0.09);
    acpiseries.Add(TDate(30, 6, 2009), 0.3);
    acpiseries.Add(TDate(30, 9, 2009), 0.72);
    acpiseries.Add(TDate(31, 12, 2009), 0.75);
    acpiseries.Add(TDate(31, 3, 2010), 0.71);
    acpiseries.Add(TDate(30, 6, 2010), 0.77);
    acpiseries.Add(TDate(30, 9, 2010), 0.67);
    acpiseries.Add(TDate(31, 12, 2010), 0.55);
    acpiseries.Add(TDate(31, 3, 2011), 0.98);
    acpiseries.Add(TDate(30, 6, 2011), 1.24);
    acpiseries.Add(TDate(30, 9, 2011), 0.76);
    acpiseries.Add(TDate(31, 12, 2011), 0.31);
    acpiseries.Add(TDate(31, 3, 2012), 0.03);
    
    int ainterestaccruesfrom = TDate(20,5,1994).Serial();
    int afirstcoupondate = TDate(20,8,1994).Serial();
    double result = CPIIndexValueACGBi(avaluedate, acpiseries, ainterestaccruesfrom, afirstcoupondate);
    if ( result != 163.66 ) {
        std::cout << "%TEST_FAILED% time=0 testname=testCPIIndexValueACGBi (testACGBi) "
                  << "message=expected 163.66, got " << result << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% testACGBi" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testCPIIndexValueACGBi (testACGBi)" << std::endl;
    testCPIIndexValueACGBi();
    std::cout << "%TEST_FINISHED% time=0 testCPIIndexValueACGBi (testACGBi)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

