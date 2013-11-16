#include <stdlib.h>
#include <iostream>

#include "../final/final.h"
#include "../utils/datetime.h"

using namespace final;
using namespace utils;

TSecurity *sec;

void init() {
    std::cout << "init" << std::endl;
    try {
        sec = GetSecurity( 
                "KBN",                       // ticker
                0.01125,                     // coupon
                TDate(23,5,2018).Serial(),   // maturity
                TDate(23,5,2013).Serial(),   // issued
                2,                           // frequency
                ISMA_30_360,                 // basis
                TDate(23,5,2013).Serial(),   // interest accrues from
                TDate(23,11,2013).Serial(),  // first coupon
                100                          // redemption
                );
    }
    catch(...) {
        sec = NULL;
        std::cout << "%TEST_FAILED% time=0 testname=init (testACGB) message=init failed" << std::endl;
    }
}

void testAccrued() {
    std::cout << "testKBN11125_0518 test accrued interest" << std::endl;
            
    double accruedExpected = 0.534375;
    TDate settlementDate(14,11,2013);
    double accrued = sec->AccruedInterest(settlementDate);

    if( CompareDoubles(accrued, accruedExpected)!=0 ) {
        std::cout << "%TEST_FAILED% time=0 testname=testAccrued (testKBN11125_0518) message=testing "
                << sec->SecurityDescriptor().c_str() << " accrued interest failed, "
                << "settlement " << settlementDate.DateString().c_str()
                << ", expected: " << accruedExpected
                << ", got: " << accrued
                << std::endl;
    }
    
    // a complete example from Nov-14-2013
    double px = 98.17099;
    double face = 50000000.0;
    double totalExpected = 49352682.50;
    double total = (px+accrued)*face/100.0;
    if( CompareDoubles(total, totalExpected)!=0 ) {
        std::cout << "%TEST_FAILED% time=0 testname=testAccrued (testKBN11125_0518) message=testing "
                << sec->SecurityDescriptor().c_str() << " total market value, "
                << "settlement " << settlementDate.DateString().c_str()
                << "price " << px
                << ", expected: " << accruedExpected
                << ", got: " << accrued
                << std::endl;
    }
    
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% testKBN11125_0518" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;
    init();
    if( sec != NULL ) {            

        std::cout << "%TEST_STARTED% testAccrued (testKBN11125_0518)" << std::endl;
        testAccrued();
        std::cout << "%TEST_FINISHED% time=0 testAccrued (testKBN11125_0518)" << std::endl;

    }
    std::cout << "%SUITE_FINISHED% time=0" << std::endl;
    return (EXIT_SUCCESS);
}


