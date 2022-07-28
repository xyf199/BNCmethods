#ifndef TESTMONOTONIC_H
#define TESTMONOTONIC_H

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
double gettheresultinvectormul(vector<double> s);
double gettheresultinvectoradd(vector<double> s);
string searchallcptvalue(string network, string node, string tocheck);
string searchallcptvalueforroot(string network, string node, string tocheck);
string checkcoefficient(string entity);
vector< vector<string> > calculate(string network, string question);
map<string, vector<string> > searchdirectchildren(string network);
vector<string> searchallchildren(string network, string parent);
string tosmtforone(vector<string> singleformula, int order);
string tosmtforall(vector<string> allformula, vector<string> op, int order);
void writewithsmt(string smtfile,vector<string> parameters,string prepare,string smallgreat);
string textinresult();
int command(const char *s);
string checkmonotonicity3(string network, string question, string tocheck, vector<string> parameters);
string checkmonotonicity4(string network, string question, string tocheck, vector<string> parameters,vector< vector<string> > probability);
string checkmonotonicity3ex(string network, string question, string tocheck, vector<string> parameters);
string smtforp(string tocheck, int n);
string getfirstderi(vector<string> opforoneparameter, string tocheck);
string checkmonotonicity6(string network, string question, string tocheck, vector<string> parameters,vector< vector<string> > probability);
map<string, vector<string> > checkpara(string network, string tocheck);
vector<string> checkpara2(string network, string tocheck);
string checkmonotonicitysimplify(string network, string question, string tocheck, vector<string> parameters);
string checkmonotonicitysimplify4(string network, string question, string tocheck, vector<string> parameters, vector< vector<string> > probability);
string checkmonotonicitysimplify5(string network, string question, string tocheck, vector<string> parameters);
string checkmonotonicitysimplify6(string network, string question, string tocheck, vector<string> parameters, vector< vector<string> > probability);
int locatedatwhichindexinevidence(string evidence, string root);
string getdesaccordingtolocation(string alldes, int location);
string searchparameterandreturnit(string network, string tocheck);
string searchparameterandmakedecision(string network,string tocheck,string root,string des,string desforrelevantnode,string relevantnode, string question);
string undercondition(string network, string question, string tocheck, vector<string> parameters,vector< vector<string> > probability);
string checkmonotonicitysimplifyundercondition(string network, string question, string tocheck, vector<string> parameters,vector< vector<string> > probability);
vector<string> getparameter(string network);
void replaceAll(std::string& str, const std::string& from, const std::string& to);
vector<string> replaceparameter(string network,string goal);
vector<string> replaceparameter2(string network,string goal);

#endif