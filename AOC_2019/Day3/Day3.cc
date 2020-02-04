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

using namespace::std;

class Day3Test : public ::testing::Test{
public:
    struct Wire{
        char d;
        int l;
        int wireColour;
        int totAtNodeEnd{0};
        int idx;
        int x0{0};
        int y0{0};
        int x1{0};
        int y1{0};
    };

    struct Intersect{
        int x;
        int y;
        int idxW1;
        int idxW2;
    };

protected:
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }

    bool readInput(vector<Wire> &a_wire1, vector<Wire> &a_wire2 )
    {
        //Read InputDay3.txt
        ifstream InputFile("InputDay3.txt");
        bool success = InputFile.is_open();
        EXPECT_TRUE(success);
        if(InputFile.is_open())
        {
            Wire tmpWire;
            string s;
            stringstream ss;
            string token;
            int i = 0;
            int j = 0;
            while(getline(InputFile, s))
            {
                tmpWire.wireColour = i;
                tmpWire.totAtNodeEnd = 0;
//                cout << s << endl;
                j = 0;
                ss.clear();
                ss << s;
                while(getline(ss,token,','))
                {
                    if(token[0] == '\0')
                        continue;
                    tmpWire.d = token[0];
                    tmpWire.l = stoi(token.substr(1));
                    tmpWire.totAtNodeEnd += tmpWire.l;
                    tmpWire.idx = j++;
//                    cout << "Wire: " << i << " Section: " << tmpWire.Idx << " Dir: " << tmpWire.D << " Len:" << tmpWire.L << endl;
                    if(i == 0) a_wire1.push_back(tmpWire);
                    else if(i == 1) a_wire2.push_back(tmpWire);
                }
                i++;
            }
            InputFile.close();
        }
        return success;
    }

    int transverseWire(vector<Wire> &a_wire, vector<Wire> &a_horizSet,vector<Wire> &a_vertSet)
    {
        int numHorizontal = 0;
        for(uint i = 0; i < a_wire.size(); i++ )
        {
            //Load Previous Wire Termination as start point.
            if(i > 0)
            {
                a_wire[i].x0 = a_wire[i-1].x1;
                a_wire[i].y0 = a_wire[i-1].y1;
            }

            a_wire[i].x1 = a_wire[i].x0;
            a_wire[i].y1 = a_wire[i].y0;
            switch(a_wire[i].d){
                case 'U':
                    a_wire[i].y1 = a_wire[i].y0 + a_wire[i].l;
                    break;
                case 'D':
                    a_wire[i].y1 = a_wire[i].y0 - a_wire[i].l;
                    break;
                case 'R':
                    a_wire[i].x1 = a_wire[i].x0 + a_wire[i].l;
                    break;
                case 'L':
                    a_wire[i].x1 = a_wire[i].x0 - a_wire[i].l;
                    break;
                default:
                    break;
            }

            if(a_wire[i].d == 'U' || a_wire[i].d == 'D' ){
                a_vertSet.push_back(a_wire[i]);
            }
            else{
                numHorizontal++;
                a_horizSet.push_back(a_wire[i]);
            }
//            cout << " Section: " << a_wire[i].Idx << " x0 y0:[ " << a_wire[i].x0  <<" , " << a_wire[i].y0 << " ] x1 y1:[ " << a_wire[i].x1  <<" , " << a_wire[i].y1 << " ]" << endl;
        }
        return numHorizontal;
    }

    bool isIntersecting(Wire& a_hW,Wire& a_vW, Intersect& a_intersect){
        //Check that vertical line x lies between horizontal min-max x
        if(min(a_hW.x0, a_hW.x1) <= a_vW.x0
                && a_vW.x0 <= max(a_hW.x0, a_hW.x1)){
            //Check that horizontal line y lies between vertical line min-max y
            if(min(a_vW.y0, a_vW.y1) <= a_hW.y0
                    && a_hW.y0 <= max(a_vW.y0, a_vW.y1)){

                    a_intersect.x = a_vW.x0;
                    a_intersect.y = a_hW.y0;
                    if(a_hW.wireColour == 0)
                    {
                        a_intersect.idxW1 = a_hW.idx;
                        a_intersect.idxW2 = a_vW.idx;
                    }
                    else
                    {
                        a_intersect.idxW1 = a_vW.idx;
                        a_intersect.idxW2 = a_hW.idx;
                    }
                    a_intersect.idxW2 = a_vW.idx;

                    if(a_intersect.x == 0 && a_intersect.y == 0){
                        return false;
                    }
                    else{
//                        cout << "Intersection [x y]: " << a_vW.x0 << " , " <<  a_hW.y0 << endl;
                        return true;
                    }
            }
        }
        return false;
    }

    //Return min manhattan Distance.
    int findIntersections(vector<Wire> &a_horizSet,
                           vector<Wire> &a_vertSet,
                           uint a_w1HorzNum,
                           uint a_w1VertNum,
                           vector<Intersect> &a_intersections)
    {
        bool foundInt;
        Intersect currentInt;
        int currManHat;
        int minManHat = numeric_limits<int>::max();

        //Loop wire1 Horizontal
        for(uint i = 0; i < a_w1HorzNum; i++)
        {
            //So Check wire2 Vertical
            for(uint j =a_w1VertNum; j < a_vertSet.size(); j++){
                foundInt = isIntersecting(a_horizSet[i],a_vertSet[j],currentInt);
                if(foundInt){
                    a_intersections.push_back(currentInt);
//                    cout << "Intersection [x y]: " << currentInt.x << " , " <<  currentInt.y << endl;
                    currManHat = abs(currentInt.x) + abs(currentInt.y);
                    if(currManHat < minManHat) minManHat = currManHat;
                }
            }
        }
        //Loop wire2 Horizontal
        for(uint i = a_w1HorzNum; i < a_horizSet.size(); i++)
        {
            //So Check wire1 Vertical
            for(uint j =0; j < a_w1VertNum; j++){
                foundInt = isIntersecting(a_horizSet[i],a_vertSet[j],currentInt);
                if(foundInt){
                    a_intersections.push_back(currentInt);
//                    cout << "Intersection [x y]: " << currentInt.x << " , " <<  currentInt.y << endl;
                    currManHat = abs(currentInt.x) + abs(currentInt.y);
                    if(currManHat < minManHat) minManHat = currManHat;
                }
            }
        }
        return minManHat;
    }

    int lengthFromInterceptPoint(vector<Wire> &a_wire1,
                             vector<Wire> &a_wire2, Intersect& a_intersection)
    {
        int length1 = 0;
        int length2 = 0;
        int idxW1 =a_intersection.idxW1;
        int idxW2 =a_intersection.idxW2;

        for(int i = 0; i < idxW1; i++)
        {
            length1 += a_wire1[i].l;
        }
        for(int i = 0; i < idxW2; i++)
        {
            length2 += a_wire2[i].l;
        }

        //Know x y location and which section along wire 1 and 2.
        //Also know current length of end point of a section.
        switch(a_wire1[idxW1].d){
        case 'U':
            length1 = a_wire1[idxW1].totAtNodeEnd -a_wire1[idxW1].l + abs(a_intersection.y - a_wire1[idxW1].y0);
//            length1 += abs(a_wire1[idxW1].y0 - a_intersection.y);
            break;
        case 'D':
            length1 = a_wire1[idxW1].totAtNodeEnd -a_wire1[idxW1].l + abs(a_wire1[idxW1].y0 - a_intersection.y);
//            length1 += abs(a_wire1[idxW1].y0 - a_intersection.y);
            break;
        case 'L':
            length1 = a_wire1[idxW1].totAtNodeEnd -a_wire1[idxW1].l + abs(a_wire1[idxW1].x0 - a_intersection.x);
//            length1 += abs(a_wire1[idxW1].x0 - a_intersection.x);
            break;
        case 'R':
            length1 = a_wire1[idxW1].totAtNodeEnd -a_wire1[idxW1].l + abs(a_intersection.x - a_wire1[idxW1].x0);
//            length1 += abs(a_wire1[idxW1].x0 - a_intersection.x);
            break;
        }

        switch(a_wire2[idxW2].d){
        case 'U':
            length2 = a_wire2[idxW2].totAtNodeEnd -a_wire2[idxW2].l + abs(a_intersection.y - a_wire2[idxW2].y0);
//            length2 += abs(a_wire2[idxW2].y0 - a_intersection.y);
            break;
        case 'D':
            length2 = a_wire2[idxW2].totAtNodeEnd -a_wire2[idxW2].l + abs(a_wire2[idxW2].y0 - a_intersection.y);
//            length2 += abs(a_wire2[idxW2].y0 - a_intersection.y);
            break;
        case 'L':
            length2 = a_wire2[idxW2].totAtNodeEnd -a_wire2[idxW2].l + abs(a_wire2[idxW2].x0 - a_intersection.x);
//            length2 += abs(a_wire2[idxW2].x0 - a_intersection.x);
            break;
        case 'R':
            length2 = a_wire2[idxW2].totAtNodeEnd -a_wire2[idxW2].l + abs(a_intersection.x - a_wire2[idxW2].x0);
//            length2 += abs(a_wire2[idxW2].x0 - a_intersection.x);
            break;
        }
        cout << endl
             << "L1: " << length1 << " L2: " << length2 <<  " Tot: "  << length1+length2 << endl
             << "Intersection [x y]: " << a_intersection.x << " , " <<  a_intersection.y << endl
             << "w1 [x0 y0 dir lEndNode lsec]: " << a_wire1[idxW1].x0 << " , " <<  a_wire1[idxW1].y0 << " , " << a_wire1[idxW1].d  << " , "  <<  a_wire1[idxW1].totAtNodeEnd << " , "  <<  a_wire1[idxW1].l << endl
             << "w2 [x0 y0 dir lEndNode lsec]: " << a_wire2[idxW2].x0 << " , " <<  a_wire2[idxW2].y0 << " , " << a_wire2[idxW2].d  <<  " , " <<  a_wire2[idxW2].totAtNodeEnd  << " , "  <<  a_wire2[idxW2].l << endl;
        return length1 + length2;
    }

    typedef pair<pair<int,int>,int> CompareType;
    struct CompareMappedVal
    {
        bool operator()(const CompareType& left, const CompareType& right){
            return left.second < right.second;
        }
    };

    static bool compare(pair<pair<int,int>,int> i, pair<pair<int,int>,int> j)
    {
        return i.second < j.second;
    }

    int getMinLength(map<pair<int,int>,int> map)
    {
        pair<pair<int,int>,int> min = *min_element(map.begin(),map.end(), &compare);
        return min.second;
    }

    vector<Wire> m_Wire1;
    vector<Wire> m_Wire2;

    vector<Wire> m_HorizontalSet;
    vector<Wire> m_VerticalSet;

    vector<Intersect> m_Intersections;
    map<pair<int,int>,int> signalDelays;

};


TEST_F ( Day3Test, attempt1P1)
{
    //Part1
    readInput(m_Wire1,m_Wire2);
    //Both Wires start at 0,0;
    int nHorizWire1;
    nHorizWire1 = transverseWire(m_Wire1,m_HorizontalSet,m_VerticalSet);
    transverseWire(m_Wire2,m_HorizontalSet,m_VerticalSet);
    cout << "MinManHat: " << findIntersections(m_HorizontalSet, m_VerticalSet, nHorizWire1, m_Wire1.size()-nHorizWire1, m_Intersections) << endl;


    //Part2
    pair<int,int> Location;
    int length;


    for(uint i = 0; i < m_Intersections.size(); i++)
    {
        Location.first = m_Intersections[i].x;
        Location.second = m_Intersections[i].y;
        if(signalDelays.find(Location) != signalDelays.end()){
            cout << "This intersection already populated" << endl;
        }
        else{
            //Find Lengths and load into map.
            length = lengthFromInterceptPoint(m_Wire1,m_Wire2, m_Intersections[i]);
//            cout << length << endl;
            signalDelays.insert({Location, length});
        }
    }
    cout << "Min time delay: " << getMinLength(signalDelays) << endl;
}


TEST_F ( Day3Test, attempt1P2)
{

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
