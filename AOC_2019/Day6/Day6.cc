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

/*
Before you use your map data to plot a course, you need to make sure it wasn't corrupted during the download.
To verify maps, the Universal Orbit Map facility uses orbit count checksums - the total number of direct orbits (like the one shown above) and indirect orbits.
Whenever A orbits B and B orbits C, then A indirectly orbits C. This chain can be any number of objects long: if A orbits B, B orbits C, and C orbits D, then A indirectly orbits D.

For example, suppose you have the following map:

COM)B
B)C
C)D
D)E
E)F
B)G
G)H
D)I
E)J
J)K
K)L

Visually, the above map of orbits looks like this:

        G - H       J - K - L
       /           /
COM - B - C - D - E - F
               \
                I

In this visual representation, when two objects are connected by a line, the one on the right directly orbits the one on the left.

Here, we can count the total number of orbits as follows:

    D directly orbits C and indirectly orbits B and COM, a total of 3 orbits.
    L directly orbits K and indirectly orbits J, E, D, C, B, and COM, a total of 7 orbits.
    COM orbits nothing.

The total number of direct and indirect orbits in this example is 42.

*/

using namespace::std;

class Day6Test : public ::testing::Test{
public:

    struct node{
        public:
            string name;
            node* parent = nullptr; //Used to navigate the tree!!!
            vector<node*> children;
            int orbits= 0;
    };

protected:
    virtual void SetUp(){
    }
    virtual void TearDown(){
    }

    int64_t m_orbits;
    int64_t m_transfers;
    vector<string> m_rawOrbits;
    map<string, node> m_nodes;

    bool readInput(vector<string>& a_rawOrbits)    {
        //Read InputDay6.txt
        ifstream InputFile("InputDay6.txt");
        bool success = InputFile.is_open();
        EXPECT_TRUE(success);
        if(InputFile.is_open())
        {
            string s;
            while(getline(InputFile, s))
            {
                if(s[0] == '\0')
                    continue;
                a_rawOrbits.push_back(s);
            }
            InputFile.close();
        }
        return success;
    }

    void processRawOrbits(vector<string>& a_rawOrbits, map<string,node>& a_nodes){
        for(string elem: a_rawOrbits){
            stringstream ss;
            ss << elem;
            string token;

            getline(ss,token,')');
            string parent = token;
            getline(ss,token,')');
            string child = token;

            //Initialise these keys in the map.
            a_nodes[child].name = child;
            a_nodes[parent].name = parent;

            a_nodes[child].parent = &a_nodes[parent];
            a_nodes[parent].children.push_back(&a_nodes[child]);
        }
    }

    void pathToRoot( map<string,node>& a_nodes, vector<string>& a_path, const string a_node){
        node* nodep = &a_nodes[a_node];
        while(nodep != nullptr){
            a_path.push_back(nodep->name);
            nodep = nodep->parent;
        }
    }

    int64_t transfersToNode( map<string,node>& a_nodes, const string a_startnode, const string a_endnode){
        node* nodep = a_nodes[a_startnode].parent;
        int64_t transfer = 0;
        while(nodep != nullptr){
            if(a_endnode.compare(nodep->name) == 0)
                break;
            nodep = nodep->parent;
//            cout <<nodep->name << endl;
            transfer++;
        }
        return transfer;
    }


    string findIntersection( vector<string>& a_pathYou, vector<string>& a_pathSan){
        //Search from lowest in paths up to root for common parent.
        for(string elemYou:a_pathYou){
            for(string elemSan:a_pathSan){
                if(elemYou.compare(elemSan) ==0){
                    cout<< "Common Found: " << elemSan << endl;
                    return elemSan;
                }
            }
        }
        return "";
    }

};



TEST_F ( Day6Test, attempt1P1)
{
    readInput(m_rawOrbits);
    processRawOrbits(m_rawOrbits,m_nodes);

    m_orbits = 0;
    for(pair<string,node> elem: m_nodes){
        node* parent = elem.second.parent;
        while(parent != nullptr){
            parent = parent->parent; //Go up tree.
            m_orbits++;
        }
    }
    cout << "Part1 Orbit Num: " << m_orbits << endl;

    vector<string> pathSan;
    vector<string> pathYou;
    pathToRoot(m_nodes, pathSan, "SAN");
    pathToRoot(m_nodes, pathYou, "YOU");

    string closestNode = findIntersection(pathYou,pathSan);
    m_transfers = 0;
    m_transfers = transfersToNode( m_nodes, "SAN", closestNode);
    m_transfers += transfersToNode( m_nodes, "YOU", closestNode);

    cout << "Part2 Transfer Num: " << m_transfers << endl;
}

TEST_F (Day6Test, testCasesPart1)
{
    vector<string> raw_orbits;
    map<string, node> nodes;

    raw_orbits.push_back( "COM)B" );
    raw_orbits.push_back( "B)C" );
    raw_orbits.push_back( "C)D" );
    raw_orbits.push_back( "D)E" );
    raw_orbits.push_back( "E)F" );
    raw_orbits.push_back( "B)G" );
    raw_orbits.push_back( "G)H" );
    raw_orbits.push_back( "D)I" );
    raw_orbits.push_back( "E)J" );
    raw_orbits.push_back( "J)K" );
    raw_orbits.push_back( "K)L" );

    //Build up map with nodes.
    processRawOrbits(raw_orbits,nodes);

    //Recursively Search map.
    int64_t orbitNum = 0;
    for(pair<string,node> elem: nodes){
        //For each node go to his parent.
        node* parent = elem.second.parent;

        while(parent != nullptr)
        {
            orbitNum++;
            parent = parent->parent;
        }
    }

    cout << "Orbit Num: " << orbitNum << endl;
    EXPECT_EQ(42,orbitNum);

}

TEST_F (Day6Test, testCasesPart2)
{
    vector<string> raw_orbits;
    map<string, node> nodes;

    raw_orbits.push_back( "COM)B" );
    raw_orbits.push_back( "B)C" );
    raw_orbits.push_back( "C)D" );
    raw_orbits.push_back( "D)E" );
    raw_orbits.push_back( "E)F" );
    raw_orbits.push_back( "B)G" );
    raw_orbits.push_back( "G)H" );
    raw_orbits.push_back( "D)I" );
    raw_orbits.push_back( "E)J" );
    raw_orbits.push_back( "J)K" );
    raw_orbits.push_back( "K)L" );
    //Part 2 Test
    raw_orbits.push_back( "K)YOU" );
    raw_orbits.push_back( "I)SAN" );

    //Build up map with nodes.
    processRawOrbits(raw_orbits,nodes);

    vector<string> pathSan;
    vector<string> pathYou;
    pathToRoot(nodes, pathSan, "SAN");
    pathToRoot(nodes, pathYou, "YOU");

    string commonNode = findIntersection(pathYou,pathSan);
    int64_t transfer = transfersToNode(  nodes, "SAN", commonNode);
    transfer += transfersToNode(  nodes, "YOU", commonNode);

    cout << "Transfer Num: " << transfer << endl;
    EXPECT_EQ(4,transfer);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
