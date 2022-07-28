#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

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

int addmy(int a, int b);
bool contains(vector<string> vec, string elem);
bool containsint(vector<int> vec, int elem);
string repeat(int times);
string remove(string t, string s);
void deleteduplicated(vector<string> &name);
string get_str_between_two_str(const string &s,
        const string &start_delim,
        const string &stop_delim);
std::string get_str_between_two_str_var1(const std::string &s,
        const std::string &start_delim,
        const std::string &stop_delim);
vector<string> changestringtovectorstring(string s);
vector<string> getbncnodeonce(vector<string> s, string network);
vector<string> allbncnode(vector<string> bnc,string network);
vector<string> notbncnode(vector<string> bncall, string network);
vector<string> describe(string network, string node);
void transbntobnc(string network, string goal, string parentnode);
void transbntobnc2(string network, string goal, string parentnode);
void createvarfile(string network, string goal);
void replaceAll(std::string& str, const std::string& from, const std::string& to);
void replaceparameter(string network,string goal);
void writequery(string network,string goal);
int countedge(string network);
int numberdirectparents(string network, string node);
vector<string> directchildren(string network, string node);
int AVB(string network);
int AVB2(string network);
int AD(string network);


#endif