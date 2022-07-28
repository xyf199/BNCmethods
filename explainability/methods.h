#ifndef EXPLAINABILITY_H
#define EXPLAINABILITY_H

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

bool contains(vector<string> vec, string elem);
string repeat(int times);
string remove(string t, string s);
string get_str_between_two_str(const string &s,
        const string &start_delim,
        const string &stop_delim);
std::string get_str_between_two_str_var1(const std::string &s,
        const std::string &start_delim,
        const std::string &stop_delim);
vector<string> getbncnodeonce(vector<string> s, string network);
vector<string> allbncnode(vector<string> bnc,string network);
vector<string> notbncnode(vector<string> bncall, string network);
map<string,string> directparents(string network);
map<string,vector<string> > directparents2(string network);
string searchallparents(string network, string child);
map<string,int > describetonumber(string network, string node);
vector<string> describe(string network, string node);
map<string, string> analysequery(string question);
string getcorrectprobability(string cpt,int a);
void deleteduplicated(vector<string> &name);
vector<string> calculateifcomplete(string network, string question);
vector<string> KeySet(map<string, string> test);
vector<string> KeySet2(map<string, vector<string> > test);
void _BuildAllState(int _uCnt, int _uValue, int _uIdx, vector<int> _vComb, vector< vector<int> > &_vvComb);
vector< vector<int> > BuildAllState(int m, int n);
void _BuildAllState2(int _uCnt, vector<int> _uValue, int _uIdx, vector<int> _vComb, vector< vector<int> > &_vvComb, int time);
vector< vector<int> > BuildAllState2(int m, vector<int> n);
vector<string> searchlastchildren(string network);
string searchroot(string network);
vector< vector<string> > calculate(string network, string question);
double calculateifnopar(string network, string question);
double gettheresultinvectormul(vector<double> s);
double gettheresultinvectoradd(vector<double> s);
string searchallcptvalue(string network, string node, string tocheck);
string searchallcptvalueforroot(string network, string node, string tocheck);
string checkcoefficient(string entity);
vector< vector<string> > calculate(string network, string question);
map<string, vector<string> > searchdirectchildren(string network);
vector<string> searchallchildren(string network, string parent);
map<string, vector<string> > checkpara(string network, string tocheck);
int locatedatwhichindexinevidence(string evidence, string root);
string getdesaccordingtolocation(string alldes, int location);
double calculateifnopar(string network, string question);
double calculateifnoparundercondition(string network, string question);
vector<string> returncorrect(vector<string> already, vector<string> now);
string searchroot(string network);
vector<string> searchlastchildren(string network);
string moreover(string network,string known,string missingfeature,vector<string> featurenotappear,double threshold,string assumption);
string moreover2(string network,string known,string missingfeature,vector<string> featurenotappear,double threshold,string assumption);
vector<string> allfeatures(string network);
string decideoperator(string network, string root, string desinassumption);
string checkexplainability2(string network, string assumption, string known, double threshold);
string checkexplainability(string network, string assumption, string known, double threshold);

#endif