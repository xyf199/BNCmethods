#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <stdio.h>
#include "methods.h"

using namespace std;

int main(){
    string network = "./bn/";
    string filename;
    cout << "Enter the filename:";
    cin >> filename;
    network = network + filename + ".bif";
    string root;
    cout << "Enter the root you choose:";
    cin >> root;
    string goal = "./bnc/"+ filename + "_" + root + ".bif";
    transbntobnc2(network,goal,root);
    return 0;
}