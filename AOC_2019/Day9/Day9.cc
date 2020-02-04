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


/*                           --- Day 2: 1202 Program Alarm ---
Instructions:
    1: [Add] two numbers and store in third position          [Prog Pos1 Pos2 StorageLoc]
    2: [Multiply] two numbers and store in third position     [Prog Pos1 Pos2 StorageLoc]

                             --- Day 5: Sunny with a Chance of Asteroids ---
Instructions :
Part1:
    3: [Input] take a input and saves it to pos1              [Prog Pos1 ]
    4: [Output] provides output from pos1                     [Prog Pos1 ]
Part2:
    5: [Jump-if-True] if the first parameter is non-zero, it sets the instruction pointer to the value from the second parameter. Otherwise, it does nothing.
    6: [Jump-if-False] if the first parameter is zero, it sets the instruction pointer to the value from the second parameter. Otherwise, it does nothing.
    7: [Less Than] if the first parameter is less than the second parameter, it stores 1 in the position given by the third parameter. Otherwise, it stores 0.
    8: [Equals] if the first parameter is equal to the second parameter, it stores 1 in the position given by the third parameter. Otherwise, it stores 0.

                             --- Day 9: Sensor Boost ---
Part1:
    9: [

    99:Exit program

    Opcodes are in different size position chuncks.

Parameter Modes:
    0: [Position Mode] parameter is interpereted at the position 50 in memory
    1: [Immediate Mode] parameter is interpereted as a value
    2: [Relative Mode] parameter is interpereted value at position (parameter + relative base) in memory

    ABCDE
     1002

    DE - two-digit opcode,      02 == opcode 2
     C - mode of 1st parameter,  0 == position mode
     B - mode of 2nd parameter,  1 == immediate mode
     A - mode of 3rd parameter,  0 == position mode, omitted due to being a leading zero
*/

using namespace::std;

class IntcodeComp
{
public:
    struct Instruction{
        int64_t opcode;
        int64_t p1M;
        int64_t p2M;
        int64_t p3M;
    };

    bool resetMemory(const string fileName)    {
        m_Program.clear();
        //Read InputDay5.txt
        ifstream InputFile(fileName);
        bool success = InputFile.is_open();
        EXPECT_TRUE(success);
        if(InputFile.is_open())
        {
            string s;
            stringstream ss;
            string token;

            while(getline(InputFile, s) &&  !InputFile.eof())
            {
                ss << s;
                int i = 0;
                while(getline(ss,token,','))
                {
                    i++;
                    if(token[0] == '\0')
                        continue;
                    m_Program.push_back(stoi(token));
                }
            }
            InputFile.close();
        }
        extendMemory();
        return success;
    }

    Instruction readOpcode(){
        int64_t val = m_Program[m_programPointer];
        int64_t origOpCode[5];
        for(int i = 0; i< 5; i++){
            origOpCode[i]= val % 10;
            val = val/ 10;
        }
        m_opCode.opcode = origOpCode[0] + origOpCode[1]*10;
        m_opCode.p1M = origOpCode[2];
        m_opCode.p2M = origOpCode[3];
        m_opCode.p3M = origOpCode[4];

        return m_opCode;
    }

    void setInput(int64_t a_input){
        m_Input = a_input;
    }

    void setMemory(vector<int64_t>& a_program){
        m_Program.clear();
        m_Program = a_program;
        extendMemory();
    }

    void extendMemory(){
        //Add extra zeros to end of program.
        for(int i = 0; i < 100; i++) m_Program.push_back(0);
    }

    void resetState(){
        m_programPointer = 0;
        m_relativeBase = 0;
        m_Input = 0;
    }

    void runProgram(){
        bool halted = false;
//        m_programPointer = 0;
        while(!halted){
            readOpcode();
            halted = runOp();
        }
    }

    bool runTillOutput(){
        bool output = false;
        while(!output){
            m_opCode = readOpcode();
            bool halted = runOp();
            if(m_opCode.opcode == 4) output = true;

            if(halted == true) return false;
        }

        return true;
    }

    bool runOp(){
        if(m_opCode.opcode == 1)       return addOp();
        else if(m_opCode.opcode == 2)  return mulOp();
        else if(m_opCode.opcode == 3)  return inOp();
        else if(m_opCode.opcode == 4)  return outOp();
        else if(m_opCode.opcode == 5)  return jumpTrueOp();
        else if(m_opCode.opcode == 6)  return jumpFalseOp();
        else if(m_opCode.opcode == 7)  return lessThanOp();
        else if(m_opCode.opcode == 8)  return equalOp();
        else if(m_opCode.opcode == 9)  return shiftRelativeBase();
        else if(m_opCode.opcode == 99) return exitOp();

        return false;
    }

    bool addOp(){
        int64_t p1;
        int64_t p2;
        int64_t p3;

        if(m_opCode.p1M == 0) p1 = m_Program[m_Program[m_programPointer+1]]; //Position Mode
        else if(m_opCode.p1M == 1) p1 =  m_Program[m_programPointer+1]; //Immediate Mode
        else if(m_opCode.p1M == 2) p1 =  m_Program[m_Program[m_programPointer+1] + m_relativeBase]; //Relative Mode

        if(m_opCode.p2M == 0) p2 = m_Program[m_Program[m_programPointer+2]];
        else if(m_opCode.p2M == 1) p2 =  m_Program[m_programPointer+2];
        else if(m_opCode.p2M == 2) p2 =  m_Program[m_Program[m_programPointer+2] + m_relativeBase]; //Relative Mode

        p3 = p1 + p2;

        if(m_opCode.p3M == 0) m_Program[m_Program[m_programPointer+3]] = p3; //Position Mode
        else if(m_opCode.p3M == 1) m_Program[m_programPointer+3] = p3; //Immediate Mode
        else if(m_opCode.p3M == 2) m_Program[m_Program[m_programPointer+3] + m_relativeBase] = p3; //Relative Mode

        m_programPointer += 4;
        return false;
    }

    bool mulOp(){
        int64_t p1;
        int64_t p2;
        int64_t p3;
        if(m_opCode.p1M == 0) p1 = m_Program[m_Program[m_programPointer+1]]; //Position Mode
        else if(m_opCode.p1M == 1) p1 =  m_Program[m_programPointer+1]; //Immediate Mode
        else if(m_opCode.p1M == 2) p1 =  m_Program[m_Program[m_programPointer+1] + m_relativeBase]; //Relative Mode

        if(m_opCode.p2M == 0) p2 = m_Program[m_Program[m_programPointer+2]];
        else if(m_opCode.p2M == 1) p2 =  m_Program[m_programPointer+2];
        else if(m_opCode.p2M == 2) p2 =  m_Program[m_Program[m_programPointer+2] + m_relativeBase]; //Relative Mode


        p3 = p1 * p2;

        if(m_opCode.p3M == 0) m_Program[m_Program[m_programPointer+3]] = p3; //Position Mode
        else if(m_opCode.p3M == 1) m_Program[m_programPointer+3] = p3; //Immediate Mode
        else if(m_opCode.p3M == 2) m_Program[m_Program[m_programPointer+3] + m_relativeBase] = p3; //Relative Mode

        m_programPointer += 4;
        return false;
    }

    bool inOp(){
        if(m_opCode.p1M == 0) m_Program[m_Program[m_programPointer+1]] = m_Input; //Input AC Unit Code.
        else if(m_opCode.p1M == 2) m_Program[m_Program[m_programPointer+1] + m_relativeBase] = m_Input; //Relative Mode
//        cout << "In Op - Address: " << m_Program[m_programPointer+1] << " , In: " << m_Program[m_Program[m_programPointer+1]]  << endl;
        m_programPointer += 2;
        return false;
    }

    bool outOp(){
        if(m_opCode.p1M == 0){
            cout << "pM Add: "  << m_programPointer+1 << " , Out: " << m_Program[m_Program[m_programPointer+1]] << endl;
            m_Output = m_Program[m_Program[m_programPointer+1]];
        }
        else if (m_opCode.p1M == 1){
            cout << "iM Add: "  <<m_programPointer+1 << " , Out: " << m_Program[m_programPointer+1] << endl;
            m_Output = m_Program[m_programPointer+1];
        }
        else if (m_opCode.p1M == 2){
            cout << "rM Add: "  <<m_programPointer+1 << " , Out: " << m_Program[m_Program[m_programPointer+1]+m_relativeBase] << endl;
            m_Output = m_Program[m_Program[m_programPointer+1]+m_relativeBase];
        }

        m_programPointer += 2;
        return false;
    }

    bool jumpTrueOp(){
        int64_t p1;
        int64_t p2;

        if(m_opCode.p1M == 0) p1 = m_Program[m_Program[m_programPointer+1]]; //Position Mode
        else if(m_opCode.p1M == 1) p1 =  m_Program[m_programPointer+1]; //Immediate Mode
        else if(m_opCode.p1M == 2) p1 =  m_Program[m_Program[m_programPointer+1] + m_relativeBase]; //Relative Mode

        if(m_opCode.p2M == 0) p2 = m_Program[m_Program[m_programPointer+2]];
        else if(m_opCode.p2M == 1) p2 =  m_Program[m_programPointer+2];
        else if(m_opCode.p2M == 2) p2 =  m_Program[m_Program[m_programPointer+2] + m_relativeBase]; //Relative Mode


        if(p1 != 0) m_programPointer = p2;
        else m_programPointer += 3; //TODO check if 4 chunck param

        return false;
    }

    bool jumpFalseOp(){
        int64_t p1;
        int64_t p2;
        if(m_opCode.p1M == 0) p1 = m_Program[m_Program[m_programPointer+1]]; //Position Mode
        else if(m_opCode.p1M == 1) p1 =  m_Program[m_programPointer+1]; //Immediate Mode
        else if(m_opCode.p1M == 2) p1 =  m_Program[m_Program[m_programPointer+1] + m_relativeBase]; //Relative Mode

        if(m_opCode.p2M == 0) p2 = m_Program[m_Program[m_programPointer+2]];
        else if(m_opCode.p2M == 1) p2 =  m_Program[m_programPointer+2];
        else if(m_opCode.p2M == 2) p2 =  m_Program[m_Program[m_programPointer+2] + m_relativeBase]; //Relative Mode


        if(p1 == 0) m_programPointer = p2;
        else m_programPointer += 3; //TODO check if 4 chunck param

        return false;
    }

    bool lessThanOp(){
        int64_t p1;
        int64_t p2;
        if(m_opCode.p1M == 0) p1 = m_Program[m_Program[m_programPointer+1]]; //Position Mode
        else if(m_opCode.p1M == 1) p1 =  m_Program[m_programPointer+1]; //Immediate Mode
        else if(m_opCode.p1M == 2) p1 =  m_Program[m_Program[m_programPointer+1] + m_relativeBase]; //Relative Mode

        if(m_opCode.p2M == 0) p2 = m_Program[m_Program[m_programPointer+2]];
        else if(m_opCode.p2M == 1) p2 =  m_Program[m_programPointer+2];
        else if(m_opCode.p2M == 2) p2 =  m_Program[m_Program[m_programPointer+2] + m_relativeBase]; //Relative Mode

        if(p1 < p2){
            if(m_opCode.p3M == 0) m_Program[m_Program[m_programPointer+3]] = 1; //Position Mode
            else if(m_opCode.p3M == 1) m_Program[m_programPointer+3] = 1; //Immediate Mode
            else if(m_opCode.p3M == 2) m_Program[m_Program[m_programPointer+3] + m_relativeBase] = 1; //Relative Mode
        }
        else{
            if(m_opCode.p3M == 0) m_Program[m_Program[m_programPointer+3]] = 0; //Position Mode
            else if(m_opCode.p3M == 1) m_Program[m_programPointer+3] = 0; //Immediate Mode
            else if(m_opCode.p3M == 2) m_Program[m_Program[m_programPointer+3] + m_relativeBase] = 0; //Relative Mode
        }

        m_programPointer += 4;
        return false;
    }

    bool equalOp( ){
        int64_t p1;
        int64_t p2;
        if(m_opCode.p1M == 0) p1 = m_Program[m_Program[m_programPointer+1]]; //Position Mode
        else if(m_opCode.p1M == 1) p1 =  m_Program[m_programPointer+1]; //Immediate Mode
        else if(m_opCode.p1M == 2) p1 =  m_Program[m_Program[m_programPointer+1] + m_relativeBase]; //Relative Mode

        if(m_opCode.p2M == 0) p2 = m_Program[m_Program[m_programPointer+2]];
        else if(m_opCode.p2M == 1) p2 =  m_Program[m_programPointer+2];
        else if(m_opCode.p2M == 2) p2 =  m_Program[m_Program[m_programPointer+2] + m_relativeBase]; //Relative Mode


        if(p1 == p2){
            if(m_opCode.p3M == 0) m_Program[m_Program[m_programPointer+3]] = 1; //Position Mode
            else if(m_opCode.p3M == 1) m_Program[m_programPointer+3] = 1; //Immediate Mode
            else if(m_opCode.p3M == 2) m_Program[m_Program[m_programPointer+3] + m_relativeBase] = 1; //Relative Mode
        }
        else{
            if(m_opCode.p3M == 0) m_Program[m_Program[m_programPointer+3]] = 0; //Position Mode
            else if(m_opCode.p3M == 1) m_Program[m_programPointer+3] = 0; //Immediate Mode
            else if(m_opCode.p3M == 2) m_Program[m_Program[m_programPointer+3] + m_relativeBase] = 0; //Relative Mode
        }


        m_programPointer += 4;
        return false;
    }

    bool shiftRelativeBase(){
        int64_t p1;
        if(m_opCode.p1M == 0) p1 = m_Program[m_Program[m_programPointer+1]]; //Position Mode
        else if(m_opCode.p1M == 1) p1 =  m_Program[m_programPointer+1]; //Immediate Mode
        else if(m_opCode.p1M == 2) p1 =  m_Program[m_Program[m_programPointer+1] + m_relativeBase]; //Relative Mode

        m_relativeBase += p1;
        m_programPointer += 2;
        return false;
    }

    bool exitOp(){
//        cout << "Program finished." << endl;
        return true;
    }


private:

    vector<int64_t> m_Program;
    int64_t m_relativeBase;
    int64_t m_programPointer;
    int64_t m_Input;
    int64_t m_Output;
    IntcodeComp::Instruction m_opCode;

};


class Day_Test : public ::testing::Test{
public:

protected:
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }
    IntcodeComp m_intComp;

};



TEST_F ( Day_Test, attempt1P1)
{
    m_intComp.resetMemory("InputDay9.txt");
    m_intComp.resetState();
    cout << "Part1: " << endl;
    m_intComp.setInput(1);
    m_intComp.runProgram();
}

TEST_F ( Day_Test, attempt1P2)
{
    m_intComp.resetMemory("InputDay9.txt");
    m_intComp.resetState();
    cout << "Part2: " << endl;
    m_intComp.setInput(2);
    m_intComp.runProgram();
}

TEST_F (Day_Test, testCasesOpCodeValidity)
{
    vector<int64_t> t1{109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99};
    vector<int64_t> t2{1102,34915192,34915192,7,4,7,99,0};
    vector<int64_t> t3{104,1125899906842624,99};


    m_intComp.setMemory(t1);
    m_intComp.resetState();
    m_intComp.runProgram();

    m_intComp.setMemory(t2);
    m_intComp.resetState();
    m_intComp.runTillOutput();

    m_intComp.setMemory(t3);
    m_intComp.resetState();
    m_intComp.runTillOutput();

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
