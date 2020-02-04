#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <istream>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <algorithm>


//FOR TIMING..
//typedef std::chrono::steady_clock tmpclock;
//#include <chrono>
//tmpclock::time_point begin = tmpclock::now();
//tmpclock::time_point end = tmpclock::now();

//cout << "Time difference = " << chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << endl;
//cout << "Time difference = " << chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << endl;


using namespace::std;

class Day_Test : public ::testing::Test{
public:

protected:
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }

};



TEST_F ( Day_Test, attempt1P1)
{
}

TEST_F (Day_Test, testCasesOpCodeValidity)
{
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
