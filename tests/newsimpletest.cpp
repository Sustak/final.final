/*
 * File:   simpleBondTest.cpp
 * Author: Marek Sestak, marek.sestak@gmail.com
 *
 * Created on 16.12.2011, 18:17:36
 */

#include <stdlib.h>
#include <iostream>

#include "final.h"
using namespace final;

/*
 * Simple C++ Test Suite
 */

void test1() {

    // std::cout << "simpleBondTest test 1" << std::endl;

    TSecurity *sec;

    try {
        sec = new TBond(
            TDate(15,1,2012),           // maturity date
            TDate(15,1,2009),           // issue date
            TDate(15,1,2009),           // interest accrues from
            TDate(15,1,2010),           // first coupon date
            ACT_ACT,                    // daycount basis
            0.04,                       // coupon rate
            1,                          // coupon frequency
            100.0                       // redemption amount
            );

        double ytm = sec->ConvYield( TDate(15,1,2009), 100.0 );
        if( fabs( ytm - 0.040 ) > 0.000000001 ) {
                std::cout << "%TEST_FAILED% time=0 testname=test1 (simpleBondTest) message=simple yield calculation, "
                          << "expected: 0.04 "
                          << " got: " << ytm
                          << std::endl;
                return;
        }
    }
    catch( TExFINAL &ex ) {
        std::cout << "%TEST_FAILED% time=0 testname=test1 (simpleBondTest) message="
                  << ex.Message.c_str()
                  << std::endl;
        return;
    }
    catch( ... ) {
        std::cout << "%TEST_FAILED% time=0 testname=test1 (simpleBondTest) message="
                  << "unknown error"
                  << std::endl;
        return;
    }
    
    std::cout << "%TEST_PASSED%";
}

void testKBN1125_0518() {

    TSecurity *sec;

    try {
        sec = GetSecurity(
            "KBN",                      // ticker
            0.01125,                    // coupon rate
            TDate(23,5,2018).Serial(),  // maturity date
            TDate(23,5,2013).Serial(),  // issue date
            2,                          // coupon frequency
            ISMA_30_360,                // daycount basis
            TDate(23,5,2013).Serial(),  // interest accrues from
            TDate(23,11,2013).Serial(), // first coupon date
            100.0                       // redemption amount
            );

        double accrued = sec->AccruedInterest( TDate(14,11,2013) );
        double accruedExpected = 0.534375;
        
        if( CompareDoubles( accrued, accruedExpected ) != 0 ) {
                std::cout << "%TEST_FAILED% time=0 testname=testKBN1125_0518 (simpleBondTest) message=accrued interest calculation failed, "
                          << "expected: " << accruedExpected
                          << " got: " << accrued
                          << std::endl;
                return;
        }
    }
    catch( TExFINAL &ex ) {
        std::cout << "%TEST_FAILED% time=0 testname=testKBN1125_0518 (simpleBondTest) message="
                  << ex.Message.c_str()
                  << std::endl;
        return;
    }
    catch( ... ) {
        std::cout << "%TEST_FAILED% time=0 testname=testKBN1125_0518 (simpleBondTest) message="
                  << "unknown error"
                  << std::endl;
        return;
    }
    
    std::cout << "%TEST_PASSED%";
    
}

void testACGB() {

    // std::cout << "simpleBondTest test 1" << std::endl;

    TSecurity *sec;

    try {
        sec = new TACGB(
            TDate(15,2,2017),           // maturity date
            TDate(10,6,2004),           // issue date
            TDate(15,2,2004),           // interest accrues from
            TDate(15,8,2004),           // first coupon date
            ACT_ACT,                    // daycount basis
            0.06,                       // coupon rate
            2,                          // coupon frequency
            100.0                       // redemption amount
            );

        double ytm = sec->ConvYield( TDate(22,12,2011), 100.0 );
        if( CompareDoubles( Round(ytm,8), 0.05997932 ) != 0 ) {
                std::cout << "%TEST_FAILED% time=0 testname=testACGB (simpleBondTest) message=ACGB simple yield calculation, "
                          << "expected: 0.05997932 "
                          << " got: " << ytm
                          << std::endl;
        }
        
        double px = sec->ConvPrice( TDate(22,12,2011), 0.03227 );
        if( CompareDoubles( px, 113.055 ) != 0 ) {
                std::cout << "%TEST_FAILED% time=0 testname=testACGB (simpleBondTest) message=ACGB simple price calculation, "
                          << "expected: 113.0552 "
                          << " got: " << px
                          << std::endl;
        }
        
        double accrued = sec->AccruedInterest( TDate(22,12,2011));
        if( CompareDoubles( accrued, 2.103 ) != 0 ) {
                std::cout << "%TEST_FAILED% time=0 testname=testACGB (simpleBondTest) message=ACGB simple accrued interest calculation, "
                          << "expected: 2.103 "
                          << " got: " << accrued
                          << std::endl;
        }
        
        double risk = sec->Risk( TDate(22,12,2011), 113.095 );
        if( CompareDoubles( Round(risk,3), 5.067 ) != 0 ) {
                std::cout << "%TEST_FAILED% time=0 testname=testACGB (simpleBondTest) message=ACGB simple risk calculation, "
                          << "expected: 5.067 "
                          << " got: " << risk
                          << std::endl;
        }
        
    }
    catch( TExFINAL &ex ) {
        std::cout << "%TEST_FAILED% time=0 testname=testACGB (simpleBondTest) message="
                  << ex.Message.c_str()
                  << std::endl;
        return;
    }
    catch( ... ) {
        std::cout << "%TEST_FAILED% time=0 testname=testACGB (simpleBondTest) message="
                  << "unknown error"
                  << std::endl;
        return;
    }
    
    std::cout << "%TEST_PASSED%";
}

void testACTB() {
    TSecurity *sec;
    try {
        sec = GetSecurity("ACTB",0,TDate(9,3,2012).Serial(),TDate(20,10,2011).Serial());
        
        double yld = Round( sec->ConvYield( TDate(23,12,2011),99.06 ), 7 );
        if( CompareDoubles(yld,0.0449813)!=0 ) {
                std::cout << "%TEST_FAILED% time=0 testname=testACTB (simpleBondTest) message=ACTB simple yield calculation, "
                          << "expected: 0.049813 "
                          << " got: " << yld
                          << std::endl;            
        }
    }
    catch( TExFINAL &ex ) {
        std::cout << "%TEST_FAILED% time=0 testname=testACTB (simpleBondTest) message="
                  << ex.Message.c_str()
                  << std::endl;
        return;
    }
    catch( ... ) {
        std::cout << "%TEST_FAILED% time=0 testname=testACTB (simpleBondTest) message="
                  << "unknown error"
                  << std::endl;
        return;
    }
    
    std::cout << "%TEST_PASSED%";
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% simpleBondTest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (simpleBondTest)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (simpleBondTest)" << std::endl;

    std::cout << "%TEST_STARTED% testACGB (simpleBondTest)\n" << std::endl;
    testACGB();
    std::cout << "%TEST_FINISHED% time=0 testACGB (simpleBondTest)" << std::endl;

    std::cout << "%TEST_STARTED% testACTB (simpleBondTest)\n" << std::endl;
    testACTB();
    std::cout << "%TEST_FINISHED% time=0 testACTB (simpleBondTest)" << std::endl;

    std::cout << "%TEST_STARTED% testKBN1125_0518 (simpleBondTest)\n" << std::endl;
    testKBN1125_0518();
    std::cout << "%TEST_FINISHED% time=0 testKBN1125_0518 (simpleBondTest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}
