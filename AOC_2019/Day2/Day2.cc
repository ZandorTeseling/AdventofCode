#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <istream>
#include <stdio.h>
#include <vector>
#include <string>
#include <math.h>

using namespace::std;

class Day2Test : public ::testing::Test{
protected:
    virtual void SetUp(){
        resetMemory(m_Program);
    }
    virtual void TearDown(){
    }

    void runProgram(vector<int64_t>& a_Program)
    {
        for(uint i =0; i < a_Program.size(); i +=4)
        {
            runOp(i,a_Program);
        }
    }

    bool runOp(int a_address, vector<int64_t>& a_Program){
        //Check addresses lie in program.
        int ProSize = a_Program.size();
        if( a_address > ProSize
                || a_Program[a_address+1] > ProSize
                ||a_Program[a_address+2] > ProSize
                || a_Program[a_address+3] > ProSize) return exitEarly(a_Program);

        if(a_Program[a_address] == 1) return addOp(a_Program[a_address+1],a_Program[a_address+2],a_Program[a_address+3],a_Program);
        else if(a_Program[a_address] == 2) return mulOp(a_Program[a_address+1],a_Program[a_address+2],a_Program[a_address+3],a_Program);
        else if(a_Program[a_address] == 99) return exitOp(a_Program);

        return false;
    }

    bool addOp(int pos1, int pos2, int pos3, vector<int64_t>& a_program){
        a_program[pos3] = a_program[pos1] + a_program[pos2];
        return false;
    }

    bool mulOp(int pos1, int pos2, int pos3, vector<int64_t>& a_program){
        a_program[pos3] = a_program[pos1] * a_program[pos2];
        return false;
    }
    bool exitEarly(vector<int64_t>& a_program)
    {
        cout << "Program finished: " << a_program[0] << endl;
        return exitOp(a_program);
    }

    bool exitOp( vector<int64_t>& a_program){
        cout << "Program finished: " << a_program[0] << endl;
        return true;
    }

    bool resetMemory(vector<int64_t>& a_program)
    {
        a_program.clear();
        //Read InputDay2.txt
        ifstream InputFile("InputDay2.txt");
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
                    a_program.push_back(stoi(token));
                }
            }
            InputFile.close();
        }
        return success;
    }

    vector<int64_t> m_Program;

};

/*--- Day 2: 1202 Program Alarm ---
Instructions:
    1: [Add] two numbers and store in third position     [Prog Pos1 Pos2 StorageLoc]
    2: [Multiply] two numbers and store in third position     [Prog Pos1 Pos2 StorageLoc]
    99:Exit program

    Opcodes are in 4 position chuncks.
*/

//Part1

TEST_F ( Day2Test, attempt1P1)
{
    //Part1
    //Replace pos[1] with 12 ans pos[2] with 2
    m_Program[1]=12;
    m_Program[2]=2;
    runProgram(m_Program);
}


TEST_F ( Day2Test, attempt1P2)
{
    //Noun : parameter 1
    //Verb : parameter 2
    //Look for 19690720.
    bool Found = false;
    int Noun;
    int Verb;

    for(int i =0; i<= 99 && !Found; i++){
        for( int j = 0; j <= 99 && !Found; j++){
            resetMemory(m_Program);
            m_Program[1] = i;
            m_Program[2] = j;
            runProgram(m_Program);
            if(m_Program[0] == 19690720)
            {
                cout << "Noun: " << i << " \tVerb: " << j << endl;
                Noun = i;
                Verb = j;
                Found = true;
            }
        }
    }


    //Whats it 100*noun + verb;
    cout << "Part 2: " << 100*Noun+ Verb << endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
