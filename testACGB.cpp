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
                "ACGB",                        // ticker
                0.0575,                        // coupon
                TDate(15,5,2021).Serial(),     // maturity
                TDate(15,5,2011).Serial(),     // issued
                2,                             // frequency
                1,                             // basis
                TDate(15,5,2011).Serial(),     // interest accrues from
                TDate(15,11,2011).Serial(),    // first coupon
                100                            // redemption
                );
    }
    catch(...) {
        sec = NULL;
        std::cout << "%TEST_FAILED% time=0 testname=init (testACGB) message=init failed" << std::endl;
    }
}

void testAccrued() {
    std::cout << "testACGB test accrued interest" << std::endl;
        
    double accruedExpected = 2.844;
    TDate settlementDate(13,11,2013);
    double accrued = sec->AccruedInterest(settlementDate);

    if( CompareDoubles(accrued, accruedExpected)!=0 ) {
        std::cout << "%TEST_FAILED% time=0 testname=testAccrued (testACGB) message=testing ACGB5.75 05/21 accrued interest failed, "
                << "settlement " << settlementDate.DateString().c_str()
                << ", expected: " << accruedExpected
                << ", got: " << accrued
                << std::endl;
    }
}

void test2() {
    std::cout << "testACGB test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (testACGB) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% testACGB" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;
    init();
    if( sec != NULL ) {            

        std::cout << "%TEST_STARTED% testAccrued (testACGB)" << std::endl;
        testAccrued();
        std::cout << "%TEST_FINISHED% time=0 testAccrued (testACGB)" << std::endl;

        std::cout << "%TEST_STARTED% test2 (testACGB)\n" << std::endl;
        test2();
        std::cout << "%TEST_FINISHED% time=0 test2 (testACGB)" << std::endl;
    }
    std::cout << "%SUITE_FINISHED% time=0" << std::endl;
    return (EXIT_SUCCESS);
}
