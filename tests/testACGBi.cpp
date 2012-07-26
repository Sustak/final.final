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

TDataSeries acpiseries;
TACGBi* acgb;

void init()
{
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

    acgb = (TACGBi*) GetSecurity(
            "ACGBi",                    // ticker
            0.04,                       // coupon
            TDate(20,8,2015).Serial(),  // maturity
            TDate(20,5,1994).Serial(),  // issued
            4,                          // frequency
            1,                          // basis
            TDate(20,5,1994).Serial(),  // interest accrues from
            TDate(20,8,1994).Serial(),  // first coupon
            100                         // redemption
            );

    acgb->SetCPI( &acpiseries );
}

void testACGB4Accrued( const TDate& settle, double expected )
{
    /*
    double accrInt = acgb->AccruedInterest(settle);
    if( settle.Serial() >= (acgb->NextCouponDate(settle).Serial()-7) ) {
        accrInt = accrInt - (acgb->Coupon()/acgb->Frequency());
    }
    
    double inflIdx = CPIIndexRatio(ACGBi,settle,acpiseries,100.0,
            acgb->InterestAccrualDate().Serial(),
            acgb->FirstCouponDate().Serial() );
    
    double result = Round(accrInt*inflIdx,3);
    */
    
    double result = acgb->AccruedInterest( settle );
    
    if( CompareDoubles( result, expected ) !=0 ) {
        std::cout << "%TEST_FAILED% time=0 testname=testCPIIndexValueACGBi (testACGBi) "
                  << "message=accrued interest expected " << expected << ", got " << result
                  << " (settlement " << settle.Day() << "." << settle.Month() << "." << settle.Year()
                  << ")" << std::endl;
    }
}


void testACGB4Yield( const TDate& asettle, double apx, double aExpectedYield) {

    double result = acgb->ConvYield(asettle, apx);

    if( CompareDoubles( result, aExpectedYield ) !=0 ) {
        std::cout << "%TEST_FAILED% time=0 testname=testCPIIndexValueACGBi (testACGBi) "
                  << "message=yield expected " << aExpectedYield << ", got " << result
                  << " (settlement " << asettle.Day() << "." << asettle.Month() << "." << asettle.Year()
                  << ", price " << apx << ")" << std::endl;
    }
    
    double pxBack = acgb->ConvPrice(asettle,aExpectedYield);
    
    if( CompareDoubles( pxBack, apx ) !=0 ) {
        std::cout << "%TEST_FAILED% time=0 testname=testCPIIndexValueACGBi (testACGBi) "
                  << "message=price expected " << apx << ", got " << pxBack
                  << " (settlement " << asettle.Day() << "." << asettle.Month() << "." << asettle.Year()
                  << ", yield " << aExpectedYield << ")" << std::endl;
    }
}

void testCPIIndexValueACGBi() {
    
    // this test replicates figures shown in excel sheet 
    //   'acgb inflation linkers calcs.xls' found in the docs folder...
    
    TDate avaluedate = TDate(19,7,2012);
    int ainterestaccruesfrom = TDate(20,5,1994).Serial();
    int afirstcoupondate = TDate(20,8,1994).Serial();
    double result = CPIIndexValueACGBi(avaluedate, acpiseries, ainterestaccruesfrom, afirstcoupondate);
    if ( result != 163.68 ) {
        std::cout << "%TEST_FAILED% time=0 testname=testCPIIndexValueACGBi (testACGBi) "
                  << "message=expected 163.68, got " << result << std::endl;
    }
    
    testACGB4Yield( TDate(19,7,2012), 184.312, 0.0012 );
    testACGB4Yield( TDate(19,7,2012), 184.259, 0.0013 );
    testACGB4Yield( TDate(19,7,2012), 184.205, 0.0014 );
    testACGB4Yield( TDate(19,7,2012), 184.151, 0.0015 );
    testACGB4Yield( TDate(19,7,2012), 184.098, 0.0016 );
    testACGB4Yield( TDate(19,7,2012), 184.044, 0.0017 );
    testACGB4Yield( TDate(19,7,2012), 183.991, 0.0018 );
    testACGB4Yield( TDate(19,7,2012), 183.937, 0.0019 );
    testACGB4Yield( TDate(19,7,2012), 183.883, 0.002 );
    testACGB4Yield( TDate(19,7,2012), 183.83, 0.0021 );
    testACGB4Yield( TDate(19,7,2012), 183.776, 0.0022 );
    testACGB4Yield( TDate(19,7,2012), 183.723, 0.0023 );
    testACGB4Yield( TDate(19,7,2012), 183.669, 0.0024 );
    testACGB4Yield( TDate(19,7,2012), 183.616, 0.0025 );
    testACGB4Yield( TDate(19,7,2012), 183.562, 0.0026 );
    testACGB4Yield( TDate(19,7,2012), 183.509, 0.0027 );
    testACGB4Yield( TDate(19,7,2012), 183.456, 0.0028 );
    testACGB4Yield( TDate(19,7,2012), 183.402, 0.0029 );
    testACGB4Yield( TDate(19,7,2012), 183.349, 0.003 );
    testACGB4Yield( TDate(19,7,2012), 183.295, 0.0031 );
    testACGB4Yield( TDate(19,7,2012), 183.242, 0.0032 );
    testACGB4Yield( TDate(19,7,2012), 183.189, 0.0033 );
    testACGB4Yield( TDate(19,7,2012), 183.135, 0.0034 );
    testACGB4Yield( TDate(19,7,2012), 183.082, 0.0035 );
    testACGB4Yield( TDate(19,7,2012), 183.029, 0.0036 );
    testACGB4Yield( TDate(19,7,2012), 182.976, 0.0037 );
    testACGB4Yield( TDate(19,7,2012), 182.922, 0.0038 );
    testACGB4Yield( TDate(19,7,2012), 182.869, 0.0039 );
    testACGB4Yield( TDate(19,7,2012), 182.816, 0.004 );
    testACGB4Yield( TDate(19,7,2012), 182.763, 0.0041 );
    testACGB4Yield( TDate(19,7,2012), 182.709, 0.0042 );
    testACGB4Yield( TDate(19,7,2012), 182.656, 0.0043 );

    
    // lets test accrued interest (a product of nominal accrued interest and
    // current interpolated inflation index
    testACGB4Accrued( TDate(1,1,2011), 0.719 );
    testACGB4Accrued( TDate(1,1,2011), 0.719 );
    testACGB4Accrued( TDate(2,1,2011), 0.736 );
    testACGB4Accrued( TDate(3,1,2011), 0.753 );
    testACGB4Accrued( TDate(4,1,2011), 0.77 );
    testACGB4Accrued( TDate(5,1,2011), 0.787 );
    testACGB4Accrued( TDate(6,1,2011), 0.805 );
    testACGB4Accrued( TDate(7,1,2011), 0.822 );
    testACGB4Accrued( TDate(8,1,2011), 0.839 );
    testACGB4Accrued( TDate(9,1,2011), 0.856 );
    testACGB4Accrued( TDate(10,1,2011), 0.873 );
    testACGB4Accrued( TDate(11,1,2011), 0.89 );
    testACGB4Accrued( TDate(12,1,2011), 0.907 );
    testACGB4Accrued( TDate(13,1,2011), 0.924 );
    testACGB4Accrued( TDate(14,1,2011), 0.942 );
    testACGB4Accrued( TDate(15,1,2011), 0.959 );
    testACGB4Accrued( TDate(16,1,2011), 0.976 );
    testACGB4Accrued( TDate(17,1,2011), 0.993 );
    testACGB4Accrued( TDate(18,1,2011), 1.01 );
    testACGB4Accrued( TDate(19,1,2011), 1.027 );
    testACGB4Accrued( TDate(20,1,2011), 1.044 );
    testACGB4Accrued( TDate(21,1,2011), 1.061 );
    testACGB4Accrued( TDate(22,1,2011), 1.078 );
    testACGB4Accrued( TDate(23,1,2011), 1.096 );
    testACGB4Accrued( TDate(24,1,2011), 1.113 );
    testACGB4Accrued( TDate(25,1,2011), 1.13 );
    testACGB4Accrued( TDate(26,1,2011), 1.147 );
    testACGB4Accrued( TDate(27,1,2011), 1.164 );
    testACGB4Accrued( TDate(28,1,2011), 1.181 );
    testACGB4Accrued( TDate(29,1,2011), 1.198 );
    testACGB4Accrued( TDate(30,1,2011), 1.215 );
    testACGB4Accrued( TDate(31,1,2011), 1.233 );
    testACGB4Accrued( TDate(1,2,2011), 1.25 );
    testACGB4Accrued( TDate(2,2,2011), 1.267 );
    testACGB4Accrued( TDate(3,2,2011), 1.284 );
    testACGB4Accrued( TDate(4,2,2011), 1.301 );
    testACGB4Accrued( TDate(5,2,2011), 1.318 );
    testACGB4Accrued( TDate(6,2,2011), 1.335 );
    testACGB4Accrued( TDate(7,2,2011), 1.352 );
    testACGB4Accrued( TDate(8,2,2011), 1.369 );
    testACGB4Accrued( TDate(9,2,2011), 1.387 );
    testACGB4Accrued( TDate(10,2,2011), 1.404 );
    testACGB4Accrued( TDate(11,2,2011), 1.421 );
    testACGB4Accrued( TDate(12,2,2011), 1.438 );
    testACGB4Accrued( TDate(13,2,2011), -0.12 );
    testACGB4Accrued( TDate(14,2,2011), -0.103 );
    testACGB4Accrued( TDate(15,2,2011), -0.086 );
    testACGB4Accrued( TDate(16,2,2011), -0.068 );
    testACGB4Accrued( TDate(17,2,2011), -0.051 );
    testACGB4Accrued( TDate(18,2,2011), -0.034 );
    testACGB4Accrued( TDate(19,2,2011), -0.017 );
    testACGB4Accrued( TDate(20,2,2011), 0 );
    testACGB4Accrued( TDate(21,2,2011), 0.018 );
    testACGB4Accrued( TDate(22,2,2011), 0.036 );
    testACGB4Accrued( TDate(23,2,2011), 0.053 );
    testACGB4Accrued( TDate(24,2,2011), 0.071 );
    testACGB4Accrued( TDate(25,2,2011), 0.089 );
    testACGB4Accrued( TDate(26,2,2011), 0.107 );
    testACGB4Accrued( TDate(27,2,2011), 0.125 );
    testACGB4Accrued( TDate(28,2,2011), 0.142 );
    testACGB4Accrued( TDate(1,3,2011), 0.16 );
    testACGB4Accrued( TDate(2,3,2011), 0.178 );
    testACGB4Accrued( TDate(3,3,2011), 0.196 );
    testACGB4Accrued( TDate(4,3,2011), 0.214 );
    testACGB4Accrued( TDate(5,3,2011), 0.231 );
    testACGB4Accrued( TDate(6,3,2011), 0.249 );
    testACGB4Accrued( TDate(7,3,2011), 0.267 );
    testACGB4Accrued( TDate(8,3,2011), 0.285 );
    testACGB4Accrued( TDate(9,3,2011), 0.302 );
    testACGB4Accrued( TDate(10,3,2011), 0.32 );
    testACGB4Accrued( TDate(11,3,2011), 0.338 );
    testACGB4Accrued( TDate(12,3,2011), 0.356 );
    testACGB4Accrued( TDate(13,3,2011), 0.374 );
    testACGB4Accrued( TDate(14,3,2011), 0.391 );
    testACGB4Accrued( TDate(15,3,2011), 0.409 );
    testACGB4Accrued( TDate(16,3,2011), 0.427 );
    testACGB4Accrued( TDate(17,3,2011), 0.445 );
    testACGB4Accrued( TDate(18,3,2011), 0.463 );
    testACGB4Accrued( TDate(19,3,2011), 0.48 );
    testACGB4Accrued( TDate(20,3,2011), 0.498 );
    testACGB4Accrued( TDate(21,3,2011), 0.516 );
    testACGB4Accrued( TDate(22,3,2011), 0.534 );
    testACGB4Accrued( TDate(23,3,2011), 0.552 );
    testACGB4Accrued( TDate(24,3,2011), 0.569 );
    testACGB4Accrued( TDate(25,3,2011), 0.587 );
    testACGB4Accrued( TDate(26,3,2011), 0.605 );
    testACGB4Accrued( TDate(27,3,2011), 0.623 );
    testACGB4Accrued( TDate(28,3,2011), 0.641 );
    testACGB4Accrued( TDate(29,3,2011), 0.658 );
    testACGB4Accrued( TDate(30,3,2011), 0.676 );
    testACGB4Accrued( TDate(31,3,2011), 0.694 );
    testACGB4Accrued( TDate(1,4,2011), 0.712 );
    testACGB4Accrued( TDate(2,4,2011), 0.73 );
    testACGB4Accrued( TDate(3,4,2011), 0.747 );
    testACGB4Accrued( TDate(4,4,2011), 0.765 );
    testACGB4Accrued( TDate(5,4,2011), 0.783 );
    testACGB4Accrued( TDate(6,4,2011), 0.801 );
    testACGB4Accrued( TDate(7,4,2011), 0.818 );
    testACGB4Accrued( TDate(8,4,2011), 0.836 );
    testACGB4Accrued( TDate(9,4,2011), 0.854 );
    testACGB4Accrued( TDate(10,4,2011), 0.872 );
    testACGB4Accrued( TDate(11,4,2011), 0.89 );
    testACGB4Accrued( TDate(12,4,2011), 0.907 );
    testACGB4Accrued( TDate(13,4,2011), 0.925 );
    testACGB4Accrued( TDate(14,4,2011), 0.943 );
    testACGB4Accrued( TDate(15,4,2011), 0.961 );
    testACGB4Accrued( TDate(16,4,2011), 0.979 );
    testACGB4Accrued( TDate(17,4,2011), 0.996 );
    testACGB4Accrued( TDate(18,4,2011), 1.014 );
    testACGB4Accrued( TDate(19,4,2011), 1.032 );
    testACGB4Accrued( TDate(20,4,2011), 1.05 );
    testACGB4Accrued( TDate(21,4,2011), 1.068 );
    testACGB4Accrued( TDate(22,4,2011), 1.085 );
    testACGB4Accrued( TDate(23,4,2011), 1.103 );
    testACGB4Accrued( TDate(24,4,2011), 1.121 );
    testACGB4Accrued( TDate(25,4,2011), 1.139 );
    testACGB4Accrued( TDate(26,4,2011), 1.157 );
    testACGB4Accrued( TDate(27,4,2011), 1.174 );
    testACGB4Accrued( TDate(28,4,2011), 1.192 );
    testACGB4Accrued( TDate(29,4,2011), 1.21 );
    testACGB4Accrued( TDate(30,4,2011), 1.228 );
    testACGB4Accrued( TDate(1,5,2011), 1.246 );
    testACGB4Accrued( TDate(2,5,2011), 1.263 );
    testACGB4Accrued( TDate(3,5,2011), 1.281 );
    testACGB4Accrued( TDate(4,5,2011), 1.299 );
    testACGB4Accrued( TDate(5,5,2011), 1.317 );
    testACGB4Accrued( TDate(6,5,2011), 1.334 );
    testACGB4Accrued( TDate(7,5,2011), 1.352 );
    testACGB4Accrued( TDate(8,5,2011), 1.37 );
    testACGB4Accrued( TDate(9,5,2011), 1.388 );
    testACGB4Accrued( TDate(10,5,2011), 1.406 );
    testACGB4Accrued( TDate(11,5,2011), 1.423 );
    testACGB4Accrued( TDate(12,5,2011), 1.441 );
    testACGB4Accrued( TDate(13,5,2011), -0.125 );
    testACGB4Accrued( TDate(14,5,2011), -0.107 );
    testACGB4Accrued( TDate(15,5,2011), -0.089 );
    testACGB4Accrued( TDate(16,5,2011), -0.071 );
    testACGB4Accrued( TDate(17,5,2011), -0.053 );
    testACGB4Accrued( TDate(18,5,2011), -0.036 );
    testACGB4Accrued( TDate(19,5,2011), -0.018 );
    testACGB4Accrued( TDate(20,5,2011), 0 );
    testACGB4Accrued( TDate(21,5,2011), 0.017 );
    testACGB4Accrued( TDate(22,5,2011), 0.035 );
    testACGB4Accrued( TDate(23,5,2011), 0.052 );
    testACGB4Accrued( TDate(24,5,2011), 0.07 );
    testACGB4Accrued( TDate(25,5,2011), 0.087 );
    testACGB4Accrued( TDate(26,5,2011), 0.104 );
    testACGB4Accrued( TDate(27,5,2011), 0.122 );
    testACGB4Accrued( TDate(28,5,2011), 0.139 );
    testACGB4Accrued( TDate(29,5,2011), 0.156 );
    testACGB4Accrued( TDate(30,5,2011), 0.174 );
    testACGB4Accrued( TDate(31,5,2011), 0.191 );
    testACGB4Accrued( TDate(1,6,2011), 0.209 );
    testACGB4Accrued( TDate(2,6,2011), 0.226 );
    testACGB4Accrued( TDate(3,6,2011), 0.243 );
    testACGB4Accrued( TDate(4,6,2011), 0.261 );
    testACGB4Accrued( TDate(5,6,2011), 0.278 );
    testACGB4Accrued( TDate(6,6,2011), 0.295 );
    testACGB4Accrued( TDate(7,6,2011), 0.313 );
    testACGB4Accrued( TDate(8,6,2011), 0.33 );
    testACGB4Accrued( TDate(9,6,2011), 0.348 );
    testACGB4Accrued( TDate(10,6,2011), 0.365 );
    testACGB4Accrued( TDate(11,6,2011), 0.382 );
    testACGB4Accrued( TDate(12,6,2011), 0.4 );
    testACGB4Accrued( TDate(13,6,2011), 0.417 );
    testACGB4Accrued( TDate(14,6,2011), 0.435 );
    testACGB4Accrued( TDate(15,6,2011), 0.452 );
    testACGB4Accrued( TDate(16,6,2011), 0.469 );
    testACGB4Accrued( TDate(17,6,2011), 0.487 );
    testACGB4Accrued( TDate(18,6,2011), 0.504 );
    testACGB4Accrued( TDate(19,6,2011), 0.521 );
    testACGB4Accrued( TDate(20,6,2011), 0.539 );
    testACGB4Accrued( TDate(21,6,2011), 0.556 );
    testACGB4Accrued( TDate(22,6,2011), 0.574 );
    testACGB4Accrued( TDate(23,6,2011), 0.591 );
    testACGB4Accrued( TDate(24,6,2011), 0.608 );
    testACGB4Accrued( TDate(25,6,2011), 0.626 );
    testACGB4Accrued( TDate(26,6,2011), 0.643 );
    testACGB4Accrued( TDate(27,6,2011), 0.66 );
    testACGB4Accrued( TDate(28,6,2011), 0.678 );
    testACGB4Accrued( TDate(29,6,2011), 0.695 );
    testACGB4Accrued( TDate(30,6,2011), 0.713 );
    testACGB4Accrued( TDate(1,7,2011), 0.73 );
    testACGB4Accrued( TDate(2,7,2011), 0.747 );
    testACGB4Accrued( TDate(3,7,2011), 0.765 );
    testACGB4Accrued( TDate(4,7,2011), 0.782 );
    testACGB4Accrued( TDate(5,7,2011), 0.8 );
    testACGB4Accrued( TDate(6,7,2011), 0.817 );
    testACGB4Accrued( TDate(7,7,2011), 0.834 );
    testACGB4Accrued( TDate(8,7,2011), 0.852 );
    testACGB4Accrued( TDate(9,7,2011), 0.869 );
    testACGB4Accrued( TDate(10,7,2011), 0.886 );
    testACGB4Accrued( TDate(11,7,2011), 0.904 );
    testACGB4Accrued( TDate(12,7,2011), 0.921 );
    testACGB4Accrued( TDate(13,7,2011), 0.939 );
    testACGB4Accrued( TDate(14,7,2011), 0.956 );
    testACGB4Accrued( TDate(15,7,2011), 0.973 );
    testACGB4Accrued( TDate(16,7,2011), 0.991 );
    testACGB4Accrued( TDate(17,7,2011), 1.008 );
    testACGB4Accrued( TDate(18,7,2011), 1.026 );
    testACGB4Accrued( TDate(19,7,2011), 1.043 );
    testACGB4Accrued( TDate(20,7,2011), 1.06 );
    testACGB4Accrued( TDate(21,7,2011), 1.078 );
    testACGB4Accrued( TDate(22,7,2011), 1.095 );
    testACGB4Accrued( TDate(23,7,2011), 1.112 );
    testACGB4Accrued( TDate(24,7,2011), 1.13 );
    testACGB4Accrued( TDate(25,7,2011), 1.147 );
    testACGB4Accrued( TDate(26,7,2011), 1.165 );
    testACGB4Accrued( TDate(27,7,2011), 1.182 );
    testACGB4Accrued( TDate(28,7,2011), 1.199 );
    testACGB4Accrued( TDate(29,7,2011), 1.217 );
    testACGB4Accrued( TDate(30,7,2011), 1.234 );
    testACGB4Accrued( TDate(31,7,2011), 1.251 );
    testACGB4Accrued( TDate(1,8,2011), 1.269 );
    testACGB4Accrued( TDate(2,8,2011), 1.286 );
    testACGB4Accrued( TDate(3,8,2011), 1.304 );
    testACGB4Accrued( TDate(4,8,2011), 1.321 );
    testACGB4Accrued( TDate(5,8,2011), 1.338 );
    testACGB4Accrued( TDate(6,8,2011), 1.356 );
    testACGB4Accrued( TDate(7,8,2011), 1.373 );
    testACGB4Accrued( TDate(8,8,2011), 1.391 );
    testACGB4Accrued( TDate(9,8,2011), 1.408 );
    testACGB4Accrued( TDate(10,8,2011), 1.425 );
    testACGB4Accrued( TDate(11,8,2011), 1.443 );
    testACGB4Accrued( TDate(12,8,2011), 1.46 );
    testACGB4Accrued( TDate(13,8,2011), -0.122 );
    testACGB4Accrued( TDate(14,8,2011), -0.104 );
    testACGB4Accrued( TDate(15,8,2011), -0.087 );
    testACGB4Accrued( TDate(16,8,2011), -0.07 );
    testACGB4Accrued( TDate(17,8,2011), -0.052 );
    testACGB4Accrued( TDate(18,8,2011), -0.035 );
    testACGB4Accrued( TDate(19,8,2011), -0.017 );
    testACGB4Accrued( TDate(20,8,2011), 0 );
    testACGB4Accrued( TDate(21,8,2011), 0.018 );
    testACGB4Accrued( TDate(22,8,2011), 0.035 );
    testACGB4Accrued( TDate(23,8,2011), 0.053 );
    testACGB4Accrued( TDate(24,8,2011), 0.07 );
    testACGB4Accrued( TDate(25,8,2011), 0.088 );
    testACGB4Accrued( TDate(26,8,2011), 0.106 );
    testACGB4Accrued( TDate(27,8,2011), 0.123 );
    testACGB4Accrued( TDate(28,8,2011), 0.141 );
    testACGB4Accrued( TDate(29,8,2011), 0.158 );
    testACGB4Accrued( TDate(30,8,2011), 0.176 );
    testACGB4Accrued( TDate(31,8,2011), 0.194 );
    testACGB4Accrued( TDate(1,9,2011), 0.211 );
    testACGB4Accrued( TDate(2,9,2011), 0.229 );
    testACGB4Accrued( TDate(3,9,2011), 0.246 );
    testACGB4Accrued( TDate(4,9,2011), 0.264 );
    testACGB4Accrued( TDate(5,9,2011), 0.282 );
    testACGB4Accrued( TDate(6,9,2011), 0.299 );
    testACGB4Accrued( TDate(7,9,2011), 0.317 );
    testACGB4Accrued( TDate(8,9,2011), 0.334 );
    testACGB4Accrued( TDate(9,9,2011), 0.352 );
    testACGB4Accrued( TDate(10,9,2011), 0.37 );
    testACGB4Accrued( TDate(11,9,2011), 0.387 );
    testACGB4Accrued( TDate(12,9,2011), 0.405 );
    testACGB4Accrued( TDate(13,9,2011), 0.422 );
    testACGB4Accrued( TDate(14,9,2011), 0.44 );
    testACGB4Accrued( TDate(15,9,2011), 0.458 );
    testACGB4Accrued( TDate(16,9,2011), 0.475 );
    testACGB4Accrued( TDate(17,9,2011), 0.493 );
    testACGB4Accrued( TDate(18,9,2011), 0.51 );
    testACGB4Accrued( TDate(19,9,2011), 0.528 );
    testACGB4Accrued( TDate(20,9,2011), 0.545 );
    testACGB4Accrued( TDate(21,9,2011), 0.563 );
    testACGB4Accrued( TDate(22,9,2011), 0.581 );
    testACGB4Accrued( TDate(23,9,2011), 0.598 );
    testACGB4Accrued( TDate(24,9,2011), 0.616 );
    testACGB4Accrued( TDate(25,9,2011), 0.633 );
    testACGB4Accrued( TDate(26,9,2011), 0.651 );
    testACGB4Accrued( TDate(27,9,2011), 0.669 );
    testACGB4Accrued( TDate(28,9,2011), 0.686 );
    testACGB4Accrued( TDate(29,9,2011), 0.704 );
    testACGB4Accrued( TDate(30,9,2011), 0.721 );
    testACGB4Accrued( TDate(1,10,2011), 0.739 );
    testACGB4Accrued( TDate(2,10,2011), 0.757 );
    testACGB4Accrued( TDate(3,10,2011), 0.774 );
    testACGB4Accrued( TDate(4,10,2011), 0.792 );
    testACGB4Accrued( TDate(5,10,2011), 0.809 );
    testACGB4Accrued( TDate(6,10,2011), 0.827 );
    testACGB4Accrued( TDate(7,10,2011), 0.845 );
    testACGB4Accrued( TDate(8,10,2011), 0.862 );
    testACGB4Accrued( TDate(9,10,2011), 0.88 );
    testACGB4Accrued( TDate(10,10,2011), 0.897 );
    testACGB4Accrued( TDate(11,10,2011), 0.915 );
    testACGB4Accrued( TDate(12,10,2011), 0.933 );
    testACGB4Accrued( TDate(13,10,2011), 0.95 );
    testACGB4Accrued( TDate(14,10,2011), 0.968 );
    testACGB4Accrued( TDate(15,10,2011), 0.985 );
    testACGB4Accrued( TDate(16,10,2011), 1.003 );
    testACGB4Accrued( TDate(17,10,2011), 1.021 );
    testACGB4Accrued( TDate(18,10,2011), 1.038 );
    testACGB4Accrued( TDate(19,10,2011), 1.056 );
    testACGB4Accrued( TDate(20,10,2011), 1.073 );
    testACGB4Accrued( TDate(21,10,2011), 1.091 );
    testACGB4Accrued( TDate(22,10,2011), 1.109 );
    testACGB4Accrued( TDate(23,10,2011), 1.126 );
    testACGB4Accrued( TDate(24,10,2011), 1.144 );
    testACGB4Accrued( TDate(25,10,2011), 1.161 );
    testACGB4Accrued( TDate(26,10,2011), 1.179 );
    testACGB4Accrued( TDate(27,10,2011), 1.197 );
    testACGB4Accrued( TDate(28,10,2011), 1.214 );
    testACGB4Accrued( TDate(29,10,2011), 1.232 );
    testACGB4Accrued( TDate(30,10,2011), 1.249 );
    testACGB4Accrued( TDate(31,10,2011), 1.267 );
    testACGB4Accrued( TDate(1,11,2011), 1.285 );
    testACGB4Accrued( TDate(2,11,2011), 1.302 );
    testACGB4Accrued( TDate(3,11,2011), 1.32 );
    testACGB4Accrued( TDate(4,11,2011), 1.337 );
    testACGB4Accrued( TDate(5,11,2011), 1.355 );
    testACGB4Accrued( TDate(6,11,2011), 1.373 );
    testACGB4Accrued( TDate(7,11,2011), 1.39 );
    testACGB4Accrued( TDate(8,11,2011), 1.408 );
    testACGB4Accrued( TDate(9,11,2011), 1.425 );
    testACGB4Accrued( TDate(10,11,2011), 1.443 );
    testACGB4Accrued( TDate(11,11,2011), 1.461 );
    testACGB4Accrued( TDate(12,11,2011), 1.478 );
    testACGB4Accrued( TDate(13,11,2011), -0.123 );
    testACGB4Accrued( TDate(14,11,2011), -0.106 );
    testACGB4Accrued( TDate(15,11,2011), -0.088 );
    testACGB4Accrued( TDate(16,11,2011), -0.07 );
    testACGB4Accrued( TDate(17,11,2011), -0.053 );
    testACGB4Accrued( TDate(18,11,2011), -0.035 );
    testACGB4Accrued( TDate(19,11,2011), -0.018 );
    testACGB4Accrued( TDate(20,11,2011), 0 );
    testACGB4Accrued( TDate(21,11,2011), 0.018 );
    testACGB4Accrued( TDate(22,11,2011), 0.035 );
    testACGB4Accrued( TDate(23,11,2011), 0.053 );
    testACGB4Accrued( TDate(24,11,2011), 0.071 );
    testACGB4Accrued( TDate(25,11,2011), 0.089 );
    testACGB4Accrued( TDate(26,11,2011), 0.106 );
    testACGB4Accrued( TDate(27,11,2011), 0.124 );
    testACGB4Accrued( TDate(28,11,2011), 0.142 );
    testACGB4Accrued( TDate(29,11,2011), 0.16 );
    testACGB4Accrued( TDate(30,11,2011), 0.177 );
    testACGB4Accrued( TDate(1,12,2011), 0.195 );
    testACGB4Accrued( TDate(2,12,2011), 0.213 );
    testACGB4Accrued( TDate(3,12,2011), 0.23 );
    testACGB4Accrued( TDate(4,12,2011), 0.248 );
    testACGB4Accrued( TDate(5,12,2011), 0.266 );
    testACGB4Accrued( TDate(6,12,2011), 0.284 );
    testACGB4Accrued( TDate(7,12,2011), 0.301 );
    testACGB4Accrued( TDate(8,12,2011), 0.319 );
    testACGB4Accrued( TDate(9,12,2011), 0.337 );
    testACGB4Accrued( TDate(10,12,2011), 0.355 );
    testACGB4Accrued( TDate(11,12,2011), 0.372 );
    testACGB4Accrued( TDate(12,12,2011), 0.39 );
    testACGB4Accrued( TDate(13,12,2011), 0.408 );
    testACGB4Accrued( TDate(14,12,2011), 0.426 );
    testACGB4Accrued( TDate(15,12,2011), 0.443 );
    testACGB4Accrued( TDate(16,12,2011), 0.461 );
    testACGB4Accrued( TDate(17,12,2011), 0.479 );
    testACGB4Accrued( TDate(18,12,2011), 0.496 );
    testACGB4Accrued( TDate(19,12,2011), 0.514 );
    testACGB4Accrued( TDate(20,12,2011), 0.532 );
    testACGB4Accrued( TDate(21,12,2011), 0.55 );
    testACGB4Accrued( TDate(22,12,2011), 0.567 );
    testACGB4Accrued( TDate(23,12,2011), 0.585 );
    testACGB4Accrued( TDate(24,12,2011), 0.603 );
    testACGB4Accrued( TDate(25,12,2011), 0.621 );
    testACGB4Accrued( TDate(26,12,2011), 0.638 );
    testACGB4Accrued( TDate(27,12,2011), 0.656 );
    testACGB4Accrued( TDate(28,12,2011), 0.674 );
    testACGB4Accrued( TDate(29,12,2011), 0.691 );
    testACGB4Accrued( TDate(30,12,2011), 0.709 );
    testACGB4Accrued( TDate(31,12,2011), 0.727 );
    testACGB4Accrued( TDate(1,1,2012), 0.745 );
    testACGB4Accrued( TDate(2,1,2012), 0.762 );
    testACGB4Accrued( TDate(3,1,2012), 0.78 );
    testACGB4Accrued( TDate(4,1,2012), 0.798 );
    testACGB4Accrued( TDate(5,1,2012), 0.816 );
    testACGB4Accrued( TDate(6,1,2012), 0.833 );
    testACGB4Accrued( TDate(7,1,2012), 0.851 );
    testACGB4Accrued( TDate(8,1,2012), 0.869 );
    testACGB4Accrued( TDate(9,1,2012), 0.887 );
    testACGB4Accrued( TDate(10,1,2012), 0.904 );
    testACGB4Accrued( TDate(11,1,2012), 0.922 );
    testACGB4Accrued( TDate(12,1,2012), 0.94 );
    testACGB4Accrued( TDate(13,1,2012), 0.957 );
    testACGB4Accrued( TDate(14,1,2012), 0.975 );
    testACGB4Accrued( TDate(15,1,2012), 0.993 );
    testACGB4Accrued( TDate(16,1,2012), 1.011 );
    testACGB4Accrued( TDate(17,1,2012), 1.028 );
    testACGB4Accrued( TDate(18,1,2012), 1.046 );
    testACGB4Accrued( TDate(19,1,2012), 1.064 );
    testACGB4Accrued( TDate(20,1,2012), 1.082 );
    testACGB4Accrued( TDate(21,1,2012), 1.099 );
    testACGB4Accrued( TDate(22,1,2012), 1.117 );
    testACGB4Accrued( TDate(23,1,2012), 1.135 );
    testACGB4Accrued( TDate(24,1,2012), 1.152 );
    testACGB4Accrued( TDate(25,1,2012), 1.17 );
    testACGB4Accrued( TDate(26,1,2012), 1.188 );
    testACGB4Accrued( TDate(27,1,2012), 1.206 );
    testACGB4Accrued( TDate(28,1,2012), 1.223 );
    testACGB4Accrued( TDate(29,1,2012), 1.241 );
    testACGB4Accrued( TDate(30,1,2012), 1.259 );
    testACGB4Accrued( TDate(31,1,2012), 1.277 );
    testACGB4Accrued( TDate(1,2,2012), 1.294 );
    testACGB4Accrued( TDate(2,2,2012), 1.312 );
    testACGB4Accrued( TDate(3,2,2012), 1.33 );
    testACGB4Accrued( TDate(4,2,2012), 1.348 );
    testACGB4Accrued( TDate(5,2,2012), 1.365 );
    testACGB4Accrued( TDate(6,2,2012), 1.383 );
    testACGB4Accrued( TDate(7,2,2012), 1.401 );
    testACGB4Accrued( TDate(8,2,2012), 1.418 );
    testACGB4Accrued( TDate(9,2,2012), 1.436 );
    testACGB4Accrued( TDate(10,2,2012), 1.454 );
    testACGB4Accrued( TDate(11,2,2012), 1.472 );
    testACGB4Accrued( TDate(12,2,2012), 1.489 );
    testACGB4Accrued( TDate(13,2,2012), -0.124 );
    testACGB4Accrued( TDate(14,2,2012), -0.106 );
    testACGB4Accrued( TDate(15,2,2012), -0.089 );
    testACGB4Accrued( TDate(16,2,2012), -0.071 );
    testACGB4Accrued( TDate(17,2,2012), -0.053 );
    testACGB4Accrued( TDate(18,2,2012), -0.035 );
    testACGB4Accrued( TDate(19,2,2012), -0.018 );
    testACGB4Accrued( TDate(20,2,2012), 0 );
    testACGB4Accrued( TDate(21,2,2012), 0.018 );
    testACGB4Accrued( TDate(22,2,2012), 0.036 );
    testACGB4Accrued( TDate(23,2,2012), 0.055 );
    testACGB4Accrued( TDate(24,2,2012), 0.073 );
    testACGB4Accrued( TDate(25,2,2012), 0.091 );
    testACGB4Accrued( TDate(26,2,2012), 0.109 );
    testACGB4Accrued( TDate(27,2,2012), 0.127 );
    testACGB4Accrued( TDate(28,2,2012), 0.145 );
    testACGB4Accrued( TDate(29,2,2012), 0.164 );
    testACGB4Accrued( TDate(1,3,2012), 0.182 );
    testACGB4Accrued( TDate(2,3,2012), 0.2 );
    testACGB4Accrued( TDate(3,3,2012), 0.218 );
    testACGB4Accrued( TDate(4,3,2012), 0.236 );
    testACGB4Accrued( TDate(5,3,2012), 0.255 );
    testACGB4Accrued( TDate(6,3,2012), 0.273 );
    testACGB4Accrued( TDate(7,3,2012), 0.291 );
    testACGB4Accrued( TDate(8,3,2012), 0.309 );
    testACGB4Accrued( TDate(9,3,2012), 0.327 );
    testACGB4Accrued( TDate(10,3,2012), 0.345 );
    testACGB4Accrued( TDate(11,3,2012), 0.364 );
    testACGB4Accrued( TDate(12,3,2012), 0.382 );
    testACGB4Accrued( TDate(13,3,2012), 0.4 );
    testACGB4Accrued( TDate(14,3,2012), 0.418 );
    testACGB4Accrued( TDate(15,3,2012), 0.436 );
    testACGB4Accrued( TDate(16,3,2012), 0.455 );
    testACGB4Accrued( TDate(17,3,2012), 0.473 );
    testACGB4Accrued( TDate(18,3,2012), 0.491 );
    testACGB4Accrued( TDate(19,3,2012), 0.509 );
    testACGB4Accrued( TDate(20,3,2012), 0.527 );
    testACGB4Accrued( TDate(21,3,2012), 0.545 );
    testACGB4Accrued( TDate(22,3,2012), 0.564 );
    testACGB4Accrued( TDate(23,3,2012), 0.582 );
    testACGB4Accrued( TDate(24,3,2012), 0.6 );
    testACGB4Accrued( TDate(25,3,2012), 0.618 );
    testACGB4Accrued( TDate(26,3,2012), 0.636 );
    testACGB4Accrued( TDate(27,3,2012), 0.655 );
    testACGB4Accrued( TDate(28,3,2012), 0.673 );
    testACGB4Accrued( TDate(29,3,2012), 0.691 );
    testACGB4Accrued( TDate(30,3,2012), 0.709 );
    testACGB4Accrued( TDate(31,3,2012), 0.727 );
    testACGB4Accrued( TDate(1,4,2012), 0.745 );
    testACGB4Accrued( TDate(2,4,2012), 0.764 );
    testACGB4Accrued( TDate(3,4,2012), 0.782 );
    testACGB4Accrued( TDate(4,4,2012), 0.8 );
    testACGB4Accrued( TDate(5,4,2012), 0.818 );
    testACGB4Accrued( TDate(6,4,2012), 0.836 );
    testACGB4Accrued( TDate(7,4,2012), 0.855 );
    testACGB4Accrued( TDate(8,4,2012), 0.873 );
    testACGB4Accrued( TDate(9,4,2012), 0.891 );
    testACGB4Accrued( TDate(10,4,2012), 0.909 );
    testACGB4Accrued( TDate(11,4,2012), 0.927 );
    testACGB4Accrued( TDate(12,4,2012), 0.945 );
    testACGB4Accrued( TDate(13,4,2012), 0.964 );
    testACGB4Accrued( TDate(14,4,2012), 0.982 );
    testACGB4Accrued( TDate(15,4,2012), 1 );
    testACGB4Accrued( TDate(16,4,2012), 1.018 );
    testACGB4Accrued( TDate(17,4,2012), 1.036 );
    testACGB4Accrued( TDate(18,4,2012), 1.055 );
    testACGB4Accrued( TDate(19,4,2012), 1.073 );
    testACGB4Accrued( TDate(20,4,2012), 1.091 );
    testACGB4Accrued( TDate(21,4,2012), 1.109 );
    testACGB4Accrued( TDate(22,4,2012), 1.127 );
    testACGB4Accrued( TDate(23,4,2012), 1.145 );
    testACGB4Accrued( TDate(24,4,2012), 1.164 );
    testACGB4Accrued( TDate(25,4,2012), 1.182 );
    testACGB4Accrued( TDate(26,4,2012), 1.2 );
    testACGB4Accrued( TDate(27,4,2012), 1.218 );
    testACGB4Accrued( TDate(28,4,2012), 1.236 );
    testACGB4Accrued( TDate(29,4,2012), 1.254 );
    testACGB4Accrued( TDate(30,4,2012), 1.273 );
    testACGB4Accrued( TDate(1,5,2012), 1.291 );
    testACGB4Accrued( TDate(2,5,2012), 1.309 );
    testACGB4Accrued( TDate(3,5,2012), 1.327 );
    testACGB4Accrued( TDate(4,5,2012), 1.345 );
    testACGB4Accrued( TDate(5,5,2012), 1.364 );
    testACGB4Accrued( TDate(6,5,2012), 1.382 );
    testACGB4Accrued( TDate(7,5,2012), 1.4 );
    testACGB4Accrued( TDate(8,5,2012), 1.418 );
    testACGB4Accrued( TDate(9,5,2012), 1.436 );
    testACGB4Accrued( TDate(10,5,2012), 1.454 );
    testACGB4Accrued( TDate(11,5,2012), 1.473 );
    testACGB4Accrued( TDate(12,5,2012), 1.491 );
    testACGB4Accrued( TDate(13,5,2012), -0.127 );
    testACGB4Accrued( TDate(14,5,2012), -0.109 );
    testACGB4Accrued( TDate(15,5,2012), -0.091 );
    testACGB4Accrued( TDate(16,5,2012), -0.073 );
    testACGB4Accrued( TDate(17,5,2012), -0.055 );
    testACGB4Accrued( TDate(18,5,2012), -0.036 );
    testACGB4Accrued( TDate(19,5,2012), -0.018 );
    testACGB4Accrued( TDate(20,5,2012), 0 );
    testACGB4Accrued( TDate(21,5,2012), 0.018 );
    testACGB4Accrued( TDate(22,5,2012), 0.036 );
    testACGB4Accrued( TDate(23,5,2012), 0.053 );
    testACGB4Accrued( TDate(24,5,2012), 0.071 );
    testACGB4Accrued( TDate(25,5,2012), 0.089 );
    testACGB4Accrued( TDate(26,5,2012), 0.107 );
    testACGB4Accrued( TDate(27,5,2012), 0.125 );
    testACGB4Accrued( TDate(28,5,2012), 0.142 );
    testACGB4Accrued( TDate(29,5,2012), 0.16 );
    testACGB4Accrued( TDate(30,5,2012), 0.178 );
    testACGB4Accrued( TDate(31,5,2012), 0.196 );
    testACGB4Accrued( TDate(1,6,2012), 0.213 );
    testACGB4Accrued( TDate(2,6,2012), 0.231 );
    testACGB4Accrued( TDate(3,6,2012), 0.249 );
    testACGB4Accrued( TDate(4,6,2012), 0.267 );
    testACGB4Accrued( TDate(5,6,2012), 0.285 );
    testACGB4Accrued( TDate(6,6,2012), 0.302 );
    testACGB4Accrued( TDate(7,6,2012), 0.32 );
    testACGB4Accrued( TDate(8,6,2012), 0.338 );
    testACGB4Accrued( TDate(9,6,2012), 0.356 );
    testACGB4Accrued( TDate(10,6,2012), 0.374 );
    testACGB4Accrued( TDate(11,6,2012), 0.391 );
    testACGB4Accrued( TDate(12,6,2012), 0.409 );
    testACGB4Accrued( TDate(13,6,2012), 0.427 );
    testACGB4Accrued( TDate(14,6,2012), 0.445 );
    testACGB4Accrued( TDate(15,6,2012), 0.463 );
    testACGB4Accrued( TDate(16,6,2012), 0.48 );
    testACGB4Accrued( TDate(17,6,2012), 0.498 );
    testACGB4Accrued( TDate(18,6,2012), 0.516 );
    testACGB4Accrued( TDate(19,6,2012), 0.534 );
    testACGB4Accrued( TDate(20,6,2012), 0.552 );
    testACGB4Accrued( TDate(21,6,2012), 0.569 );
    testACGB4Accrued( TDate(22,6,2012), 0.587 );
    testACGB4Accrued( TDate(23,6,2012), 0.605 );
    testACGB4Accrued( TDate(24,6,2012), 0.623 );
    testACGB4Accrued( TDate(25,6,2012), 0.64 );
    testACGB4Accrued( TDate(26,6,2012), 0.658 );
    testACGB4Accrued( TDate(27,6,2012), 0.676 );
    testACGB4Accrued( TDate(28,6,2012), 0.694 );
    testACGB4Accrued( TDate(29,6,2012), 0.712 );
    testACGB4Accrued( TDate(30,6,2012), 0.729 );
    testACGB4Accrued( TDate(1,7,2012), 0.747 );
    testACGB4Accrued( TDate(2,7,2012), 0.765 );
    testACGB4Accrued( TDate(3,7,2012), 0.783 );
    testACGB4Accrued( TDate(4,7,2012), 0.801 );
    testACGB4Accrued( TDate(5,7,2012), 0.818 );
    testACGB4Accrued( TDate(6,7,2012), 0.836 );
    testACGB4Accrued( TDate(7,7,2012), 0.854 );
    testACGB4Accrued( TDate(8,7,2012), 0.872 );
    testACGB4Accrued( TDate(9,7,2012), 0.89 );
    testACGB4Accrued( TDate(10,7,2012), 0.907 );
    testACGB4Accrued( TDate(11,7,2012), 0.925 );
    testACGB4Accrued( TDate(12,7,2012), 0.943 );
    testACGB4Accrued( TDate(13,7,2012), 0.961 );
    testACGB4Accrued( TDate(14,7,2012), 0.979 );
    testACGB4Accrued( TDate(15,7,2012), 0.996 );
    testACGB4Accrued( TDate(16,7,2012), 1.014 );
    testACGB4Accrued( TDate(17,7,2012), 1.032 );
    testACGB4Accrued( TDate(18,7,2012), 1.05 );
    testACGB4Accrued( TDate(19,7,2012), 1.067 );
    testACGB4Accrued( TDate(20,7,2012), 1.085 );
    testACGB4Accrued( TDate(21,7,2012), 1.103 );
    testACGB4Accrued( TDate(22,7,2012), 1.121 );
    testACGB4Accrued( TDate(23,7,2012), 1.139 );
    testACGB4Accrued( TDate(24,7,2012), 1.156 );
    testACGB4Accrued( TDate(25,7,2012), 1.174 );    
    
}

int main(int argc, char** argv) {
    init();
    
    std::cout << "%SUITE_STARTING% testACGBi" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testCPIIndexValueACGBi (testACGBi)" << std::endl;
    testCPIIndexValueACGBi();
    std::cout << "%TEST_FINISHED% time=0 testCPIIndexValueACGBi (testACGBi)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

