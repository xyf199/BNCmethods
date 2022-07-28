#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <regex>
#include <map>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include "methods.h"

using namespace std;

int main(){
    vector<string> s;
    string number;
    string d;
    string network = "./pbnc/";
    string filename;
    string question;
    cout << "Enter the filename:";
    cin >> filename;
    network = network + filename + ".bif";
    cout << "Enter the query:";
    cin >> question;
    question = "P(" + question + ")";
    s = getparameter(network);
    vector< vector<string> > probability = calculate(network,question);
    for(int i=0;i<s.size();i++){
    d = checkmonotonicitysimplify4(network,question,s[i],s,probability);
    cout << s[i]+":"+d << endl;
    }
    return 0;
}

/* An example
int main(){
    vector<string> s;
    string number;
    string d;
    string network = "./pbnc/hepar2_100_hepatotoxic.bif";
    string question = "P(nausea = present)";
    vector< vector<string> > probability = calculate(network,question);
    s = getparameter(network);
    for(int i=0;i<s.size();i++){
    d = checkmonotonicitysimplify4(network,question,s[i],s,probability);
    cout << s[i]+":"+d << endl;
    }
    return 0;
}*/