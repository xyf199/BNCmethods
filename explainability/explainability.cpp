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
    string assumption;
    cout << "Enter your assumption: ";
    cin >> assumption;
    string known;
    cout << "Enter the known feature(s): ";
    cin >> known;
    double threshold;
    cout << "Enter the threshold:";
    cin >> threshold;
    string s = checkexplainability("fruit.bif",assumption,known,threshold);
    cout << s << endl;
    return 0;
}