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
//        a_idx = 0;
        while(!halted){
            a_inst = readOpcode(a_prog,a_idx);
            halted = runOp(a_prog, a_inst, a_idx, a_input, a_output);
        }
    }

    bool runTillOutput(vector<int64_t>& a_prog,  Instruction& a_inst, int64_t& a_idx, int64_t& a_input, int64_t& a_output){
        bool output = false;
        while(!output){
            a_inst = readOpcode(a_prog,a_idx);
            bool halted = runOp(a_prog, a_inst, a_idx, a_input, a_output);
            if(a_inst.opcode == 4) output = true;

            if(halted == true) return false;
        }

        return true;
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

    bool addOp(     vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& /*a_output*/){
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

    bool mulOp(     vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& /*a_output*/){
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

    bool inOp(      vector<int64_t>& a_prog, Instruction& /*a_inst*/, int64_t& a_idx, int64_t& a_input, int64_t& /*a_output*/){
        a_prog[a_prog[a_idx+1]] = a_input; //Input AC Unit Code.
//        cout << "In Op - Address: " << a_prog[a_idx+1] << " , In: " << a_prog[a_prog[a_idx+1]]  << endl;
        a_idx += 2;
        return false;
    }

    bool outOp(     vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& a_output){
        if(a_inst.p1M == 0){
//            cout << "pM Add: "  << a_idx+1 << " , Out: " << a_prog[a_prog[a_idx+1]] << endl;
            a_output = a_prog[a_prog[a_idx+1]];
        }
        else if (a_inst.p1M == 1){
//            cout << "iM Add: "  <<a_idx+1 << " , Out: " << a_prog[a_idx+1] << endl;
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

    bool equalOp(   vector<int64_t>& a_prog, Instruction& a_inst, int64_t& a_idx, int64_t& /*a_input*/, int64_t& /*a_output*/){
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

    bool exitOp(    vector<int64_t>& /*a_prog*/, Instruction& /*a_inst*/, int64_t& /*a_idx*/, int64_t& /*a_input*/, int64_t& /*a_output*/){
//        cout << "Program finished." << endl;
        return true;
    }

};

class Day7Test : public ::testing::Test{
public:

protected:
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }

    IntcodeComp m_computer;

    vector<int64_t> m_bestSequence;
    vector<int64_t> m_Program;

    vector<int64_t> m_MemAmpA;
    vector<int64_t> m_MemAmpB;
    vector<int64_t> m_MemAmpC;
    vector<int64_t> m_MemAmpD;
    vector<int64_t> m_MemAmpE;

    int64_t m_APointer;
    int64_t m_BPointer;
    int64_t m_CPointer;
    int64_t m_DPointer;
    int64_t m_EPointer;

    int64_t m_Input;
    int64_t m_Output;
    IntcodeComp::Instruction m_opCode;

    void resetAllMem(){
        m_MemAmpA = m_Program;
        m_MemAmpB = m_Program;
        m_MemAmpC = m_Program;
        m_MemAmpD = m_Program;
        m_MemAmpE = m_Program;
    }

    void resetAllPointers(int64_t loc){
        m_APointer = loc;
        m_BPointer = loc;
        m_CPointer = loc;
        m_DPointer = loc;
        m_EPointer = loc;
    }

    void loadSeq(vector<int64_t>& a_sequence){
        resetAllMem();
        resetAllPointers(0);
        m_computer.inOp(m_MemAmpA,m_opCode,m_APointer,a_sequence[0],m_Output);
        m_computer.inOp(m_MemAmpB,m_opCode,m_BPointer,a_sequence[1],m_Output);
        m_computer.inOp(m_MemAmpC,m_opCode,m_CPointer,a_sequence[2],m_Output);
        m_computer.inOp(m_MemAmpD,m_opCode,m_DPointer,a_sequence[3],m_Output);
        m_computer.inOp(m_MemAmpE,m_opCode,m_EPointer,a_sequence[4],m_Output);
    }

    int64_t runSequence(int64_t a_prevOutput){
        //AmpA
        m_Input = a_prevOutput;
        m_computer.runProgram(m_MemAmpA, m_opCode, m_APointer, m_Input, m_Output);
        //AmpB
        m_Input = m_Output;
        m_computer.runProgram(m_MemAmpB, m_opCode, m_BPointer, m_Input, m_Output);
        //AmpC
        m_Input = m_Output;
        m_computer.runProgram(m_MemAmpC, m_opCode, m_CPointer, m_Input, m_Output);
        //AmpD
        m_Input = m_Output;
        m_computer.runProgram(m_MemAmpD, m_opCode, m_DPointer, m_Input, m_Output);
        //AmpE
        m_Input = m_Output;
        m_computer.runProgram(m_MemAmpE, m_opCode, m_EPointer, m_Input, m_Output);
        return m_Output;
    }

    int64_t runFeedbackLoop(int64_t a_prevOutput){
        bool halted = false;
        m_Output = a_prevOutput;
        while(!halted){
            //AmpA
            m_Input = m_Output;
            m_computer.runTillOutput(m_MemAmpA, m_opCode, m_APointer, m_Input, m_Output);
            //AmpB
            m_Input = m_Output;
            m_computer.runTillOutput(m_MemAmpB, m_opCode, m_BPointer, m_Input, m_Output);
            //AmpC
            m_Input = m_Output;
            m_computer.runTillOutput(m_MemAmpC, m_opCode, m_CPointer, m_Input, m_Output);
            //AmpD
            m_Input = m_Output;
            m_computer.runTillOutput(m_MemAmpD, m_opCode, m_DPointer, m_Input, m_Output);
            //AmpE
            m_Input = m_Output;
            m_computer.runTillOutput(m_MemAmpE, m_opCode, m_EPointer, m_Input, m_Output);
            if(m_opCode.opcode == 99) halted = true;
        }
        return m_Output;
    }
    int64_t runFeedBackSequence(){
        int64_t prevOutput = 0;
        prevOutput = runFeedbackLoop(prevOutput);
        return prevOutput;
    }

    int64_t findFeedbackThrust(){
        int64_t max_thrust = 0;
        int64_t thrust;
        vector<int64_t> seq;
        int64_t phases[] = {5,6,7,8,9};
        std::sort (phases,phases+5);

        do {
            seq = {phases[0],phases[1],phases[2],phases[3],phases[4]};
            loadSeq(seq);
            thrust = runFeedBackSequence();
            if(thrust > max_thrust){
                m_bestSequence = seq;
                max_thrust = thrust;
            }

        } while ( std::next_permutation(phases,phases+5) );

        return max_thrust;
    }

    int64_t findThrust(){
        int64_t max_thrust = 0;
        int64_t thrust;
        vector<int64_t> seq;
        int64_t phases[] = {0,1,2,3,4};
        std::sort (phases,phases+5);
        do {
            seq = {phases[0],phases[1],phases[2],phases[3],phases[4]};
            loadSeq(seq);
            thrust = runSequence(0);
            if(thrust > max_thrust){
                m_bestSequence = seq;
                max_thrust = thrust;
            }

        } while ( std::next_permutation(phases,phases+5) );

        return max_thrust;
    }

};



TEST_F ( Day7Test, attempt1P1)
{
    m_computer.resetMemory(m_Program,"InputDay7.txt");
    cout << "Part1 Max thrust: " << findThrust() << endl;

    m_computer.resetMemory(m_Program,"InputDay7.txt");
    cout << "Part2 Max thrust: " << findFeedbackThrust() << endl;
}

TEST_F (Day7Test, testCasesOpCodeValidity)
{
    //43210 seq:4,3,2,1,0
    vector<int64_t> t1{3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0};
    vector<int64_t> t2{3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,
                       1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0};
    m_Program = t1;
    int64_t testThrust = findThrust();
    cout << "T1 Max thrust: " << testThrust << endl;
    EXPECT_EQ(testThrust ,43210);

    m_Program = t2;
    testThrust = findThrust();
    cout << "T2 Max thrust: " << testThrust << endl;
    EXPECT_EQ(testThrust ,65210);


    //Part 2 tests
    vector<int64_t> t3{3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
                       -5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
                       53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10};
    m_Program = t3;
    testThrust = findFeedbackThrust();
    EXPECT_EQ(testThrust ,18216);
    cout << "T3 Max thrust: " << testThrust << endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
