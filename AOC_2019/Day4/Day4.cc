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


#define PASS_N 6

class Day4Test : public ::testing::Test{
public:
    int64_t m_rngs[2]= {};
    int64_t m_digits[PASS_N] = {};

    void getDigits(int64_t a_value, int64_t* a_digits){
        //Digit Extraction. Method 1
        for(int i = 0; i< PASS_N; i++){
            a_digits[i]= a_value % 10;
            a_value = a_value/ 10;
        }
    }

    int64_t getValue(int64_t* a_digits){
        assert(PASS_N == 6);
        int64_t value = a_digits[0]*1 +
                a_digits[1]*10 +
                a_digits[2]*100 +
                a_digits[3]*1000 +
                a_digits[4]*10000 +
                a_digits[5]*100000;

        return value;
    }

    int64_t incrementDigits(int64_t* a_digits){
        int64_t currVal = getValue(a_digits);
        currVal++;
        getDigits(currVal,a_digits);
        return currVal;
    }

    void boundLowerIncreasing(int64_t* a_digits){
        int64_t a_lowerBound[PASS_N];

        for(int i = 0; i < PASS_N; i++)
                a_lowerBound[i] = a_digits[i];

        for(int i = PASS_N-1; i > 0; i--){
            if(a_digits[i-1] < a_digits[i]) //Check increasing
                a_digits[i-1] = a_digits[i];

            if(a_digits[i-1] > a_digits[i] && a_digits[i] > a_lowerBound[i]) //round to the closed monotonic increasing while satisfying lower bounds.
                a_digits[i-1] = a_digits[i];
        }
    }

    bool forceMonotonic(int64_t* a_digits){
        for(int i = PASS_N-1; i > 0; i--){
            if( a_digits[i-1] < a_digits[i] )
                a_digits[i-1] = a_digits[i];
        }
        return true;
    }

    bool checkAdjacent(int64_t* a_digits){
        //Run through the digits and check validity.
        for(int i = PASS_N-1; i > 0; i--){
            if( a_digits[i-1] == a_digits[i] ){
                return true;
            }
        }
        return false;
    }

    bool checkUnique(int64_t* a_digits){
        //Run through the digits and check unique ness of matches 112233 t 111234 false 111122 true
        int64_t numSame = 0;
        int64_t numUnique = 0;
        int64_t currMatch = -1;
        int64_t prevMatch = -1;
        bool multiMatch = false;

        //Number of repetative matches.
        for(int i = 1; i < PASS_N; i++){
            if(a_digits[i] == a_digits[i-1]){ //Matching Digits
                    currMatch = a_digits[i];
                    if(currMatch != prevMatch){
                        numUnique++;
                        multiMatch = false;
                    }
                    if(currMatch == prevMatch){
                        if(!multiMatch){
                            numSame++;
                            numUnique--;
                        }
                        multiMatch = true;
                        numSame++;
                    }
                    prevMatch = currMatch;
            }
        }

//        cout << "Test: ";
//        printDigits(a_digits);
//        cout << "Num Same: " << numSame << endl;
//        cout << "Num Unique: " << numUnique << endl;
        if(numSame % 2 && numUnique > 0)
            return true;
        else if( numUnique > 0)
            return true;
        else
            return false;

    }

    bool checkIncreasing(int64_t* a_digits){
        //Run through the digits and check validity.
        for(int i = PASS_N-1; i > 0; i--){
            if( a_digits[i] > a_digits[i-1] ){
                return false;
            }
        }
        return true;
    }

    void printDigits(int64_t* a_digits){
        for(int i = PASS_N-1; i >-1; i--){
            cout << a_digits[i];
        }
        cout <<endl;
    }


protected:
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }

    bool readInput()
    {
        //Read InputDay4.txt
        ifstream InputFile("InputDay4.txt");
        bool success = InputFile.is_open();
        EXPECT_TRUE(success);
        if(InputFile.is_open())
        {
            string s;
            stringstream ss;
            string token;
            int i;
            while(getline(InputFile,s))
            {
                ss << s;
                i =0;
                while(getline(ss,token,'-'))
                {
                    m_rngs[i]= stoi(token);
                    i++;
                }
            }
            InputFile.close();
        }

        return success;
    }
};



TEST_F ( Day4Test, attempt1P1)
{
    int64_t validNum = 0;
    std::vector<int64_t> validMono;

    readInput();
    cout << "L: " << m_rngs[0] << " R: " << m_rngs[1] << endl;
    getDigits(m_rngs[0],m_digits);
    boundLowerIncreasing(m_digits);
    printDigits(m_digits);
    bool inRange = true;
    while(inRange){
        if(getValue(m_digits) > m_rngs[1])
            inRange = false;
        else{
            if(checkAdjacent(m_digits) &&
                    checkIncreasing(m_digits) &&
                    checkUnique(m_digits)){
                validNum++;
                validMono.push_back(getValue(m_digits));
            }
        }
//        printDigits(m_digits);
        incrementDigits(m_digits);
        forceMonotonic(m_digits);
    }
    cout << "Valid Password Count Mono: " << validNum << endl;
}

TEST_F (Day4Test, testCasesPasswordValidity)
{

//    111111 meets these criteria (double 11, never decreases).
//    223450 does not meet these criteria (decreasing pair of digits 50).
//    123789 does not meet these criteria (no double).
    int64_t tP1_1[PASS_N] = {1,1,1,1,1,1};
    int64_t tP1_2[PASS_N] = {0,5,4,3,2,2};
    int64_t tP1_3[PASS_N] = {9,8,7,3,2,1};

    EXPECT_TRUE(checkIncreasing(tP1_1));
    EXPECT_FALSE(checkIncreasing(tP1_2));
    EXPECT_FALSE(checkAdjacent(tP1_3));

//    112233 meets these criteria because the digits never decrease and all repeated digits are exactly two digits long.
//    123444 no longer meets the criteria (the repeated 44 is part of a larger group of 444).
//    111122 meets the criteria (even though 1 is repeated more than twice, it still contains a double 22).
    int64_t t1[PASS_N] = {3,3,2,2,1,1};
    int64_t t2[PASS_N] = {4,4,4,3,2,1};
    int64_t t3[PASS_N] = {2,2,1,1,1,1};
    int64_t t4[PASS_N] = {3,2,1,1,1,1};

    EXPECT_TRUE( checkAdjacent(t1));
    EXPECT_TRUE( checkIncreasing(t1));
    EXPECT_TRUE( checkUnique(t1));

    EXPECT_TRUE( checkAdjacent(t2));
    EXPECT_TRUE( checkIncreasing(t2));
    EXPECT_FALSE( checkUnique(t2));

    EXPECT_TRUE( checkAdjacent(t3));
    EXPECT_TRUE( checkIncreasing(t3));
    EXPECT_TRUE( checkUnique(t3));

    EXPECT_TRUE( checkAdjacent(t4));
    EXPECT_TRUE( checkIncreasing(t4));
    EXPECT_FALSE( checkUnique(t4));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
