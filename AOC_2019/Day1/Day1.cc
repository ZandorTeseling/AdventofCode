#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <math.h>

using namespace::std;
class Day1Test : public ::testing::Test{
protected:
    virtual void SetUp(){
        //Read InputDay1.txt
        ifstream InputFile("InputDay1.txt");
        m_FileSuccess = InputFile.is_open();
        if(InputFile.is_open())
        {
            string s;
            while(getline(InputFile, s) &&  !InputFile.eof())
            {
                int mass;
                if(s[0] == '\0')
                        continue;
                mass = stoi(s);
                m_CargoMasses.push_back(mass);
            }
            InputFile.close();
        }
    }
    virtual void TearDown(){
    }
    bool m_FileSuccess;
    vector<int> m_CargoMasses;

};

TEST_F ( Day1Test, attempt1)
{
    EXPECT_TRUE(m_FileSuccess);
    double Part1Fuel = 0;
    double Part2Fuel = 0;

    int mass;
    double massFuel;
    double fuelFuel;
    double modulSelfFuel;

     for(uint i = 0; i < m_CargoMasses.size(); i++)
    {
        modulSelfFuel = 0;

        mass = m_CargoMasses[i];
        massFuel = floor(mass/3.0)-2;
        Part1Fuel += massFuel;
        //Part 2 Additional self fuel per module
        fuelFuel = massFuel;
        while((floor(fuelFuel/3.0)-2) >= 0)
        {
            fuelFuel = floor(fuelFuel/3.0)-2;
            Part2Fuel += fuelFuel;
            modulSelfFuel += fuelFuel;
        }
        cout << "Mass " << i << ": " <<  mass << "\tCargo Fuel: " << massFuel << "\tFuel Fuel: " << modulSelfFuel << endl;
    }
    cout << "Tot Fuel: " << std::setprecision (10) << Part1Fuel+Part2Fuel<< "\tP1: " << Part1Fuel << " \tP2:" << Part2Fuel <<endl;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
