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

    99:Exit program

    Opcodes are in different size position chuncks.

Parameter Modes:
    0: [Position Mode] parameter is interpereted at the position 50 in memory
    1: [Immediate Mode] paramete is interpereted as a value

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

    bool resetMemory(vector<int64_t>& a_prog, const string fileName)    {
        a_prog.clear();
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
                    a_prog.push_back(stoi(token));
                }
            }
            InputFile.close();
        }
        return success;
    }

    Instruction readOpcode(vector<int64_t>& a_prog, int64_t& a_idx){
        Instruction res;
        int64_t val = a_prog[a_idx];
        int64_t origOpCode[5];
        for(int i = 0; i< 5; i++){
            origOpCode[i]= val % 10;
            val = val/ 10;
        }
        res.opcode = origOpCode[0] + origOpCode[1]*10;
        res.p1M = origOpCode[2];
        res.p2M = origOpCode[3];
        res.p3M = origOpCode[4];

        return res;
    }

    void runProgram(vector<int64_t>& a_prog,  Instruction& a_inst, int64_t& a_idx, int64_t& a_input, int64_t& a_output){
        bool halted = false;
        a_idx = 0;
        while(!halted){
            a_inst = readOpcode(a_prog,a_idx);
            halted = runOp(a_prog, a_inst, a_idx, a_input, a_output);
        }
    }

    bool runOp(vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& a_input, int64_t& a_output){
        if(a_inst.opcode == 1)       return addOp(a_prog, a_inst, a_idx, a_input, a_output);
        else if(a_inst.opcode == 2)  return mulOp(a_prog, a_inst, a_idx, a_input, a_output);
        else if(a_inst.opcode == 3)  return inOp(a_prog, a_inst, a_idx, a_input, a_output);
        else if(a_inst.opcode == 4)  return outOp(a_prog, a_inst, a_idx, a_input, a_output);
        else if(a_inst.opcode == 5)  return jumpTrueOp(a_prog, a_inst, a_idx, a_input, a_output);
        else if(a_inst.opcode == 6)  return jumpFalseOp(a_prog, a_inst, a_idx, a_input, a_output);
        else if(a_inst.opcode == 7)  return lessThanOp(a_prog, a_inst, a_idx, a_input, a_output);
        else if(a_inst.opcode == 8)  return equalOp(a_prog, a_inst, a_idx, a_input, a_output);
        else if(a_inst.opcode == 99) return exitOp(a_prog, a_inst, a_idx, a_input, a_output);

        return false;
    }

    bool addOp(vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& /*a_output*/){
        int64_t p1;
        int64_t p2;
        if(a_inst.p1M == 0) p1 = a_prog[a_prog[a_idx+1]]; //Position Mode
        else if(a_inst.p1M == 1) p1 =  a_prog[a_idx+1]; //Immediate Mode

        if(a_inst.p2M == 0) p2 = a_prog[a_prog[a_idx+2]];
        else if(a_inst.p2M == 1) p2 =  a_prog[a_idx+2];

        a_prog[a_prog[a_idx+3]]= p1 + p2;
        a_idx += 4;
        return false;
    }

    bool mulOp(vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& /*a_output*/){
        int64_t p1;
        int64_t p2;
        if(a_inst.p1M == 0) p1 = a_prog[a_prog[a_idx+1]]; //Position Mode
        else if(a_inst.p1M == 1) p1 =  a_prog[a_idx+1]; //Immediate Mode

        if(a_inst.p2M == 0) p2 = a_prog[a_prog[a_idx+2]];
        else if(a_inst.p2M == 1) p2 =  a_prog[a_idx+2];

        a_prog[a_prog[a_idx+3]] = p1 * p2;
        a_idx += 4;
        return false;
    }

    bool inOp(vector<int64_t>& a_prog, Instruction& /*a_inst*/, int64_t& a_idx, int64_t& a_input, int64_t& /*a_output*/){
        a_prog[a_prog[a_idx+1]] = a_input; //Input AC Unit Code.
        cout << "In Op - Address: " << a_prog[a_idx+1] << " , In: " << a_prog[a_prog[a_idx+1]]  << endl;
        a_idx += 2;
        return false;
    }

    bool outOp(vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& a_output){
        if(a_inst.p1M == 0){
            cout << "pM Add: "  << a_idx+1 << " , Out: " << a_prog[a_prog[a_idx+1]] << endl;
            a_output = a_prog[a_prog[a_idx+1]];
        }
        else if (a_inst.p1M == 1){
            cout << "iM Add: "  <<a_idx+1 << " , Out: " << a_prog[a_idx+1] << endl;
            a_output = a_prog[a_idx+1];
        }
        a_idx += 2;
        return false;
    }

    bool jumpTrueOp(vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& /*a_output*/){
        int64_t p1;
        int64_t p2;
        if(a_inst.p1M == 0) p1 = a_prog[a_prog[a_idx+1]]; //Position Mode
        else if(a_inst.p1M == 1) p1 =  a_prog[a_idx+1]; //Immediate Mode

        if(a_inst.p2M == 0) p2 = a_prog[a_prog[a_idx+2]];
        else if(a_inst.p2M == 1) p2 =  a_prog[a_idx+2];

        if(p1 != 0) a_idx = p2;
        else a_idx += 3; //TODO check if 4 chunck param

        return false;
    }

    bool jumpFalseOp(vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& /*a_output*/){
        int64_t p1;
        int64_t p2;
        if(a_inst.p1M == 0) p1 = a_prog[a_prog[a_idx+1]]; //Position Mode
        else if(a_inst.p1M == 1) p1 =  a_prog[a_idx+1]; //Immediate Mode

        if(a_inst.p2M == 0) p2 = a_prog[a_prog[a_idx+2]];
        else if(a_inst.p2M == 1) p2 =  a_prog[a_idx+2];

        if(p1 == 0) a_idx = p2;
        else a_idx += 3; //TODO check if 4 chunck param

        return false;
    }

    bool lessThanOp(vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& /*a_output*/){
        int64_t p1;
        int64_t p2;
        if(a_inst.p1M == 0) p1 = a_prog[a_prog[a_idx+1]]; //Position Mode
        else if(a_inst.p1M == 1) p1 =  a_prog[a_idx+1]; //Immediate Mode

        if(a_inst.p2M == 0) p2 = a_prog[a_prog[a_idx+2]];
        else if(a_inst.p2M == 1) p2 =  a_prog[a_idx+2];

        if(p1 < p2) a_prog[a_prog[a_idx+3]] = 1;
        else a_prog[a_prog[a_idx+3]] = 0;

        a_idx += 4;
        return false;
    }

    bool equalOp(vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& /*a_output*/){
        int64_t p1;
        int64_t p2;
        if(a_inst.p1M == 0) p1 = a_prog[a_prog[a_idx+1]]; //Position Mode
        else if(a_inst.p1M == 1) p1 =  a_prog[a_idx+1]; //Immediate Mode

        if(a_inst.p2M == 0) p2 = a_prog[a_prog[a_idx+2]];
        else if(a_inst.p2M == 1) p2 =  a_prog[a_idx+2];

        if(p1 == p2) a_prog[a_prog[a_idx+3]] = 1;
        else a_prog[a_prog[a_idx+3]] = 0;

        a_idx += 4;
        return false;
    }

    bool exitOp(vector<int64_t>& /*a_prog*/, Instruction& /*a_inst*/, int64_t& /*a_idx*/, int64_t& /*a_input*/, int64_t& /*a_output*/){
        cout << "Program finished." << endl;
        return true;
    }

};

class Day5Test : public ::testing::Test{
public:

protected:
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }

    IntcodeComp m_computer;

    vector<int64_t> m_Program;
    int64_t m_InstructionPointer;
    int64_t m_Input;
    int64_t m_Output;
    IntcodeComp::Instruction m_opCode;
};

TEST_F ( Day5Test, attempt1P1)
{
    m_computer.resetMemory(m_Program,"InputDay5.txt");
    m_Input = 5;
    m_computer.runProgram(m_Program, m_opCode, m_InstructionPointer, m_Input, m_Output);
}

TEST_F (Day5Test, testCasesOpCodeValidity)
{
    vector<int64_t> t1{3,9,8,9,10,9,4,9,99,-1,8};
    vector<int64_t> t2{3,9,7,9,10,9,4,9,99,-1,8};
    vector<int64_t> t3{3,3,1108,-1,8,3,4,3,99};
    vector<int64_t> t4{3,3,1107,-1,8,3,4,3,99};

    m_Input = 8;
    m_computer.runProgram(t1, m_opCode, m_InstructionPointer, m_Input, m_Output);
    EXPECT_TRUE(m_Output);

    m_Input = 8;
    m_computer.runProgram(t2, m_opCode, m_InstructionPointer, m_Input, m_Output);
    EXPECT_FALSE(m_Output);

    m_Input = 8;
    m_computer.runProgram(t3, m_opCode, m_InstructionPointer, m_Input, m_Output);
    EXPECT_TRUE(m_Output);

    m_Input = 6;
    m_computer.runProgram(t4, m_opCode, m_InstructionPointer, m_Input, m_Output);
    EXPECT_TRUE(m_Output);

    vector<int64_t> t5{3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9};
    vector<int64_t> t6{3,3,1105,-1,9,1101,0,0,12,4,12,99,1};
    m_Input = 6;
    m_computer.runProgram(t5, m_opCode, m_InstructionPointer, m_Input, m_Output);
    EXPECT_TRUE(m_Output);

    m_Input = 0;
    m_computer.runProgram(t6, m_opCode, m_InstructionPointer, m_Input, m_Output);
    EXPECT_FALSE(m_Output);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
