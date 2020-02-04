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

class Day8Test : public ::testing::Test{
public:

protected:
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }

};



TEST_F ( Day8Test, attempt1P1)
{
    string s;
    int64_t i =0;
    int64_t page = 0;
    int64_t pageDim = 25*6;
    vector<int> pixels;
    int64_t minZeros = 25*6;
    int64_t minPage = 0;
    int64_t zeros = 0;

    ifstream InputFile("InputDay8.txt");
     getline(InputFile,s);

    while(s[i]){
        pixels.push_back(s[i] - '0');
        if(pixels[i] == 0) zeros++;
        i++;
        //If new page update max counts
        if((i % pageDim) == 0 && s[i])
        {
//            cout << "Page " << page << ": " << zeros << endl;
            if(zeros < minZeros){
                minZeros = zeros;
                minPage = page;
            }
            page++;
            zeros = 0;
        }
    }
    cout << "Min zeros: " << minZeros << " Page: " << minPage << endl;

    int ones = 0;
    int twos = 0;
    for(vector<int>::iterator it = pixels.begin()+pageDim*minPage; it < pixels.begin()+(pageDim*minPage+pageDim); it++){
        if(*it == 1) ones++;
        if(*it == 2) twos++;
    }
    cout << "Num ones: " << ones << " Num twos: " << twos << endl;
    cout << "Part1 Res: " << ones*twos << endl;

    //Part 2
    //Step over pages while transperant.
    vector<int> pic;
    for(int i = 0; i< pageDim; i++) pic.push_back(pixels[i]); //Load first page into image.

    for(int i = 0; i < pageDim; i++){ //Pixel indexing
        if((i % 25) == 0) cout << endl;
        for(int j = 0; j <= page; j++){
//            cout << j*pageDim + i << endl;
            if(pic[i] == 2 && pixels[j*pageDim+i] != 2){
               pic[i] = pixels[j*pageDim+i];
               break;
            }

        }
        if( pic[i] == 0) cout << ' ' ;
        if( pic[i] == 1) cout << '0' ;
        if( pic[i] == 2) cout << ' ' ;

    }
    cout << endl;
}

TEST_F (Day8Test, testCasesOpCodeValidity)
{
    vector<int> t1{0,2,2,2,1,1,2,2,2,2,1,2,0,0,0,0};
    vector<int> image;

    int w = 2;
    int h =2;
    int page = t1.size()/(w*h);

    for(int i =0; i < w*h; i++){ //Pixel indexing
        if((i % w) ==0 ) cout << endl;
        image.push_back(2);
        for(int j = 0; j < page; j++){
//            cout << j << endl;
            if(t1[w*h*j + i] != 2){
                image[i] = t1[w*h*j + i];
                break;
            }
        }
//        cout << image[i];
        if( image[i] == 0) cout << ' ' ;
        if( image[i] == 1) cout << '0' ;
    }
    cout << endl;

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
