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

bool contains(vector<string> vec, string elem)
{
    bool result = false;
    if( find(vec.begin(), vec.end(), elem) != vec.end() )
    {
        result = true;
    }
    return result;
}

string repeat(int times){
    string s;
     while(times !=0){
         if(times !=1){
             s = s + " 0,";
         }
         else{
             s = s + " 1";
         }
         times = times -1;
     }
     return s;
}

string remove(string t, string s){
  string::size_type i = t.find(s);

if (i != std::string::npos)
   t.erase(i, s.length());
   
   return t;
}

string get_str_between_two_str(const string &s,
        const string &start_delim,
        const string &stop_delim)
{
    unsigned first_delim_pos = s.find(start_delim);
    unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
    unsigned last_delim_pos = s.find(stop_delim);
 
    return s.substr(end_pos_of_first_delim,
            last_delim_pos - end_pos_of_first_delim);
}

std::string get_str_between_two_str_var1(const std::string &s,
        const std::string &start_delim,
        const std::string &stop_delim)
{
    unsigned first_delim_pos = s.find(start_delim);
    unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
    unsigned last_delim_pos = s.find_first_of(stop_delim, end_pos_of_first_delim);
    
    return s.substr(end_pos_of_first_delim,
            last_delim_pos - end_pos_of_first_delim);
}


vector<string> getbncnodeonce(vector<string> s, string network){
      ifstream filein(network);
      vector<string> bncnode;
      bncnode = s;
      string strTemp,mid;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
             for(int i=0; i<bncnode.size(); i++){ 
      if(strTemp.find(bncnode[i]) != string::npos){
        if(strTemp.find("|") != string::npos){
            mid = get_str_between_two_str(strTemp,"( "," |");
            if(contains(bncnode,mid) == false){
            bncnode.insert(bncnode.begin()+1,mid);
            } 
      }
    }
  }
        }
    }
    return bncnode;
}

vector<string> allbncnode(vector<string> bnc,string network){
    vector<string> bncall;
    while (bncall.size() != bnc.size()){
         bncall = bnc;
         bnc = getbncnodeonce(bnc, network);
    }
    return bncall;
}

vector<string> notbncnode(vector<string> bncall, string network){
      ifstream filein(network);
      vector<string> notbncnode;
      string strTemp,mid;
    while(getline(filein,strTemp)){
        if(strTemp.find("variable") != string::npos){
            mid = get_str_between_two_str(strTemp,"variable ", " {");
            if(contains(bncall,mid) == false){
                notbncnode.push_back(mid);
            }
        }
        if(strTemp.find("probability") != string::npos){
            break;
        }
    }
    return notbncnode;
}

map<string,string> directparents(string network)
{
    string strNew = "  table 0, 1; ";
    string strTemp;
    ifstream filein(network); //File to read from 
    string child,parentofchild;
    vector<string> record;
    map<string,string> directparents;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            if(strTemp.find("|") != string::npos){
            child = get_str_between_two_str(strTemp,"( "," |");
            parentofchild = get_str_between_two_str(strTemp,"| "," )");
            directparents[child] = parentofchild;
            }
            else{
            child = get_str_between_two_str(strTemp,"( "," )");
            directparents[child] = "root";
            }
        }
    }
      return directparents;
}

map<string,vector<string> > directparents2(string network)
{
    string strNew = "  table 0, 1; ";
    string strTemp;
    ifstream filein(network); //File to read from 
    string child,parentofchild;
    vector<string> record;
    map<string,vector<string> > directparents;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            if(strTemp.find("|") != string::npos){
            child = get_str_between_two_str(strTemp,"( "," |");
            parentofchild = get_str_between_two_str(strTemp,"| "," )");
            parentofchild = parentofchild+", "; //parentofchild = "asia, cancer, x, y, z, "
            record.clear();
            while(parentofchild != ""){
            string oneparent;
            oneparent = get_str_between_two_str_var1(parentofchild,parentofchild.substr(0,0),",");//oneparent = "asia"
            record.push_back(oneparent);
            string toremove = oneparent+", ";
            parentofchild = remove(parentofchild,toremove);
            }
            directparents[child] = record;
            }
            else{
            child = get_str_between_two_str(strTemp,"( "," )");
            record.clear();
            record.push_back("root");
            directparents[child] = record ;
            }
        }
    }
      return directparents;
}


string searchallparents(string network, string child){
    map<string, vector<string> > direct= directparents2(network);
    string res;
    string current = child;
    vector<string> all;
    vector<string> box1,box2;
    bool flagfor1 = true;
    if(direct[child][0] == "root"){
         return "";
    }
    else{
    for(int i =0;i< direct[child].size();i++){
        box1.push_back(direct[child][i]);
        res = res + direct[child][i] + ",";
    }
    while(true){
        if(flagfor1 == true){
            for(int i=0; i<box1.size();i++){
                for(int j=0; j<direct[box1[i]].size();j++){
                    if(direct[box1[i]][0] != "root"){
                    res = res + direct[box1[i]][j]+ ",";}
                }
            }
            for(int i=0; i<box1.size();i++){
                if(direct[box1[i]][0] != "root"){
                for(int j=0; j<direct[box1[i]].size();j++){
                  box2.push_back(direct[box1[i]][j]);
                }
                }   
            }
            box1.clear();
            flagfor1 =false;
        }
        else{
           for(int i=0; i<box2.size();i++){
                for(int j=0; j<direct[box2[i]].size();j++){
                    if(direct[box2[i]][0] != "root"){
                    res = res + direct[box2[i]][j]+ ",";}
                }
            }
            for(int i=0; i<box2.size();i++){
                if(direct[box2[i]][0] != "root"){
                for(int j=0; j<direct[box2[i]].size();j++){
                  box1.push_back(direct[box2[i]][j]);
                }
                }   
            }
            box2.clear();
            flagfor1 = true;
        }
        if(box1.empty() && box2.empty()){
            break;
        }
    }
         return res;
    }
}

map<string,int > describetonumber(string network, string node){//yes to 0, no to 1
      ifstream filein(network);
      string strTemp,current,description, toremove;
      map<string, int> corr;
      bool flag = true;
      int i = 0;
      while(getline(filein,strTemp)){
         if(flag == false){
            current = get_str_between_two_str(strTemp,"{ "," }");
            current = current + ", ";//current = yes, no, 
            while(current !=""){
                description = get_str_between_two_str_var1(current,current.substr(0,0),",");
                corr[description] = i;
                i = i +1;
                toremove = description + ", ";
                current = remove(current, toremove);
            }
            break;
         }
         if(strTemp.find(node) != string::npos){
            flag = false;
         }
      }
      return corr;
}



vector<string> describe(string network, string node){
      ifstream filein(network);
      string strTemp,current,description, toremove;
      vector<string> corr;
      bool flag = true;
      while(getline(filein,strTemp)){
         if(flag == false){
            current = get_str_between_two_str(strTemp,"{ "," }");
            current = current + ", ";//current = yes, no, 
            while(current !=""){
                description = get_str_between_two_str_var1(current,current.substr(0,0),",");
                corr.push_back(description);
                toremove = description + ", ";
                current = remove(current, toremove);
            }
            break;
         }
         if(strTemp.find(node) != string::npos){
            flag = false;
         }
      }
      return corr;
}

map<string, string> analysequery(string question){//finalresult is: aisa:yes,cancer:no 
    string information= get_str_between_two_str(question,"(",")");//asia = 0, cancer = 0, 
    information = information + ", ";
    string entity,nodeofentity,valueofentity,toremove;
    map<string,string> mapofquery;
    while(information != ""){
        entity = get_str_between_two_str_var1(information,information.substr(0,0),",");
        nodeofentity = get_str_between_two_str(entity,entity.substr(0,0)," =");
        valueofentity = get_str_between_two_str(entity,"= ",entity.substr(entity.size(),0));
        mapofquery[nodeofentity] = valueofentity;
        toremove = entity + ", ";
        information = remove(information,toremove);
    }
    return mapofquery;
}

string getcorrectprobability(string cpt,int a){
    string correctprobability,toremove;
    cpt = cpt + ", ";
    while(a != -1){
        correctprobability = get_str_between_two_str_var1(cpt,cpt.substr(0,0),",");
        toremove = correctprobability+", ";
        cpt = remove(cpt,toremove);
        a = a - 1;
    }
    return correctprobability;
}

void deleteduplicated(vector<string> &name)
{
	sort(name.begin(), name.end());
	name.erase(unique(name.begin(), name.end()), name.end());
}



vector<string> calculateifcomplete(string network, string question){ //assume query is complete, P(cancer = 0, Pollution = 0, a = 0)
    map<string,string> mapofquery = analysequery(question);//cancer:yes, pollution:no, smoker:yes.
    map<string,int> mapdestonumber;
    vector<string> result;
    int number;
    ifstream filein(network);
    string strTemp,node,description,cpt,correctprobability,evidence,beforecpt,singleparent,toremove;
    bool flag = true;
    while(getline(filein,strTemp)){
        if(strTemp.find("}") != string::npos){
            flag = true;
        }
        if(flag == false){
            if(strTemp.find("table") != string::npos){
                mapdestonumber = describetonumber(network,node);
                number = mapdestonumber[description];
                cpt = get_str_between_two_str(strTemp,"table",";");
                correctprobability = getcorrectprobability(cpt,number);
                result.push_back(correctprobability);
            }
            else{
                if(strTemp.find(beforecpt) != string::npos){
                mapdestonumber = describetonumber(network,node);
                number = mapdestonumber[description];
                cpt = get_str_between_two_str(strTemp,") ",";");
                correctprobability = getcorrectprobability(cpt,number);
                result.push_back(correctprobability);
                }
            }
        }
        if(strTemp.find("probability") != string::npos){
            if(strTemp.find("|") == string::npos){
                 node = get_str_between_two_str(strTemp,"( "," )");
                 if(mapofquery.count(node) >0){
                     description = mapofquery[node];
                     flag = false;
                 }
            }
            if(strTemp.find("|") != string::npos){
                  node = get_str_between_two_str(strTemp,"( "," |");
                  if(mapofquery.count(node) >0){
                     description = mapofquery[node];
                     flag = false;
                 
                  evidence = get_str_between_two_str(strTemp,"| "," )");//cancer, pollution, a, b, c
                  evidence = evidence + ", ";
                  beforecpt = "(";
                  while(evidence != ""){
                      singleparent = get_str_between_two_str_var1(evidence,evidence.substr(0,0),",");
                      beforecpt = beforecpt + mapofquery[singleparent]+", ";
                      toremove = singleparent + ", ";
                      evidence = remove(evidence,toremove);
                  }
                  beforecpt.pop_back();
                  beforecpt.pop_back();
                  beforecpt = beforecpt + ")";
            }
        }
        }
    }
    return result;
}



vector<string> KeySet(map<string, string> test)
{
    vector<string> keys;
    for(map<string, string>::iterator it = test.begin(); it != test.end(); ++it){
        keys.push_back(it->first);
    }
    return keys;
}

vector<string> KeySet2(map<string, vector<string> > test)
{
    vector<string> keys;
    for(map<string, vector<string> >::iterator it = test.begin(); it != test.end(); ++it){
        keys.push_back(it->first);
    }
    return keys;
}

void _BuildAllState(int _uCnt, int _uValue, int _uIdx, vector<int> _vComb, vector< vector<int> > &_vvComb)
{
	vector<int> vComb = _vComb;

	for (int i = 0; i < _uValue; i++)
	{
		vComb[_uIdx] = i;

		if (_uIdx+1 < _uCnt)
		{
			_BuildAllState(_uCnt, _uValue, _uIdx+1, vComb, _vvComb);
		}
		else
		{
			_vvComb.push_back(vComb);
		}
	}
}

vector< vector<int> > BuildAllState(int m, int n)//buildallstate(3,2), 3 balls, every 2 colors
{
	vector< vector<int> > vvComb;
	vector<int> vInt(m , 0);
	_BuildAllState(m, n, 0, vInt, vvComb);

	return vvComb;
}

void _BuildAllState2(int _uCnt, vector<int> _uValue, int _uIdx, vector<int> _vComb, vector< vector<int> > &_vvComb, int time)
{
	vector<int> vComb = _vComb;
    int con = time + 1;

	for (int i = 0; i < _uValue[time]; i++)
	{
		vComb[_uIdx] = i;

		if (_uIdx+1 < _uCnt)
		{
			_BuildAllState2(_uCnt, _uValue, _uIdx+1, vComb, _vvComb, con);
		}
		else
		{
			_vvComb.push_back(vComb);
		}
	}
}

vector< vector<int> > BuildAllState2(int m, vector<int> n)
{
	vector< vector<int> > vvComb;
	vector<int> vInt(m , 0);
	_BuildAllState2(m, n, 0, vInt, vvComb, 0);

	return vvComb;
}


double gettheresultinvectormul(vector<double> s){
    double result = 1;
    for(int i=0;i<s.size();i++){
        result = result * s[0];
    }
    return result;
}

double gettheresultinvectoradd(vector<double> s){
    double result = 0;
    for(int i=0;i<s.size();i++){
        result = result + s[0];
    }
    return result;
}

string searchallcptvalue(string network, string node, string tocheck){//appears only once
    ifstream filein(network);
    string strTemp;
    bool flag = false;
    string result;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos && strTemp.find(node) != string::npos){
            flag = true;
        }
        if(flag == true && strTemp.find(tocheck) != string::npos){
            result = get_str_between_two_str(strTemp,") ",";");
            break;
        }
    }
    return result;
}

string searchallcptvalueforroot(string network, string node, string tocheck){//appears only once
    ifstream filein(network);
    string strTemp;
    string result;
    while(getline(filein,strTemp)){
        if(strTemp.find(tocheck) != string::npos){
            result = get_str_between_two_str(strTemp,"table ",";");
            break;
        }
    }
    return result;
}

string checkcoefficient(string entity){
    double result;
    string resultstring;
    string qian = get_str_between_two_str(entity,entity.substr(0,0),"*");
    string hou = get_str_between_two_str_var1(entity,"/",")");
    string hou1 = get_str_between_two_str(hou,"(","-");
    string hou2 = get_str_between_two_str(hou,"-",hou.substr(hou.length(),hou.length()));
    double hou1double = atof(hou1.c_str());
    double hou2double = atof(hou2.c_str());
    double partresult2 = hou1double - hou2double;
    double qiandouble = atof(qian.c_str());
    result = qiandouble/partresult2; 
    stringstream ss;
    ss << result ;
    ss >> resultstring;
    return resultstring;
}

vector< vector<string> > calculate(string network, string question){  
     vector < vector<string> > finalresult;
     vector <string> partresult;
     map<string,string> mapofquery = analysequery(question);
     vector<string> vectorappearnode = KeySet(mapofquery);
     string completenode,missingquery,queryinwork;
     string query = get_str_between_two_str(question,"(",")");
     for(int i=0;i<vectorappearnode.size();i++){
        completenode = completenode + searchallparents(network,vectorappearnode[i]);
     }
     vector<string> shouldappearnode;
     string node,toremove;
     while(completenode != ""){
         node = get_str_between_two_str_var1(completenode,completenode.substr(0,0),",");
         shouldappearnode.push_back(node);
         toremove = node+",";
         completenode = remove(completenode,toremove);
     }
     deleteduplicated(shouldappearnode);
     vector<string> notappearnode;
     for(int i=0; i<shouldappearnode.size(); i++){
            if(contains(vectorappearnode,shouldappearnode[i]) == false){
                    notappearnode.push_back(shouldappearnode[i]);
            }
     }
     if(notappearnode.empty() == true){
         finalresult.push_back(calculateifcomplete(network,question));
         return finalresult;
     }
     vector< vector<string> > alldescription;
     vector<string> singlenodedescription;
     for(int i=0; i<notappearnode.size();i++){
         singlenodedescription = describe(network,notappearnode[i]);
         for(int j=0;j<singlenodedescription.size(); j++){
             singlenodedescription[j] = notappearnode[i]+" = "+singlenodedescription[j];
         }
          alldescription.push_back(singlenodedescription);
     }
     vector<int> numberofalldescription;
     for(int i = 0; i< alldescription.size();i++){
         numberofalldescription.push_back(alldescription[i].size());
     }
     vector < vector<int> > m = BuildAllState2(alldescription.size(),numberofalldescription);
     for(int i=0; i < m.size(); i++){
         missingquery = "";
         queryinwork = query;
         for(int j =0 ; j<alldescription.size(); j ++){
             missingquery = missingquery + alldescription[j][m[i][j]]+", ";//cancer = yes, asia = no, a = 0,  
         }
         missingquery.pop_back();
         missingquery.pop_back();
         queryinwork = queryinwork + ", ";
         queryinwork = "P(" + queryinwork + missingquery + ")";
         partresult = calculateifcomplete(network,queryinwork);
         finalresult.push_back(partresult);
     }
     return finalresult;
}


map<string, vector<string> > searchdirectchildren(string network){
    ifstream filein(network);
    string strTemp;
    map<string, vector<string> > directchildren;
    string evidence;
    string singleevidence;
    string currentnode;
    string toremove;
    while(getline(filein,strTemp)){
        if(strTemp.find("|") != string::npos){
            currentnode = get_str_between_two_str(strTemp,"( "," |");
            evidence = get_str_between_two_str(strTemp,"| "," )");
            evidence = evidence +", ";
            while (evidence != ""){
                singleevidence = get_str_between_two_str_var1(evidence,evidence.substr(0,0),",");
                directchildren[singleevidence].push_back(currentnode);
                toremove = singleevidence +", ";
                evidence = remove(evidence,toremove);
            }
        }
    }
    return directchildren;
}

vector<string> searchallchildren(string network, string parent){
    map<string, vector<string> > directchildren = searchdirectchildren(network);
    vector<string> result;
    vector<string> first = directchildren[parent];
    vector<string> box1,box2;
    for(int i=0; i<first.size();i++){
        box1.push_back(first[i]);
        result.push_back(first[i]);
    }
    bool flagbox1 = true;
    bool flagbox2 = false;
    while(box1.empty() != false || box2.empty() != false){
        if(flagbox1 == true){
            box2.clear();
            for(int i=0;i<box1.size();i++){
                for(int j=0;j<directchildren[box1[i]].size();j++){
                    box2.push_back(directchildren[box1[i]][j]);
                    result.push_back(directchildren[box1[i]][j]);
                }
            }
            flagbox1 = false;
            flagbox2 = true;
        }
        else{
            box1.clear();
            for(int i=0;i<box2.size();i++){
                for(int j=0;j<directchildren[box2[i]].size();j++){
                    box1.push_back(directchildren[box2[i]][j]);
                    result.push_back(directchildren[box2[i]][j]);
                }
            }
            flagbox1 = true;
            flagbox2 = false;
        }
    }
    return result;
}
 
map<string, vector<string> > checkpara(string network, string tocheck){
    ifstream filein(network);
    string strTemp;
    vector<string> nodewiththispar;
    vector<string> evidenceanddescription;
    map<string, vector<string> > nodeanddescription;
    string currentnode;
    string evidence,singleevidence;
    string description,singledescription;
    string removeevidence, removedescription;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            if(strTemp.find("|") != string::npos){
            currentnode = get_str_between_two_str(strTemp,"( "," |");
            evidence = get_str_between_two_str(strTemp,"| "," )");
            }
            else{
                currentnode = get_str_between_two_str(strTemp,"( "," )");
            }
        }
        if(strTemp.find(tocheck) != string::npos && strTemp.find("parameter") == string::npos){
            if(strTemp.find("table") != string::npos){
                vector<string> s;
                s.push_back("root");
                nodeanddescription[currentnode] = s;
                return nodeanddescription;
            }
            nodewiththispar.push_back(currentnode);
            description = get_str_between_two_str(strTemp,"(",")");
            evidence = evidence +", ";
            description = description +", ";
            while(evidence !="" && description != ""){
                singleevidence = get_str_between_two_str_var1(evidence,evidence.substr(0,0),",");
                singledescription = get_str_between_two_str_var1(description,description.substr(0,0),",");
                evidenceanddescription.push_back(singleevidence + " = " + singledescription);
                removeevidence = singleevidence +", ";
                evidence = remove(evidence,removeevidence);
                removedescription = singledescription +", ";
                description = remove(description,removedescription);
            }
            nodeanddescription[currentnode]  = evidenceanddescription;
        }
    }
    return nodeanddescription;
}


int locatedatwhichindexinevidence(string evidence, string root){
    int result = 0;
    evidence = evidence +", ";
    string currentnode = get_str_between_two_str_var1(evidence,evidence.substr(0,0),",");
    string toremove;
    while(currentnode != root){
        toremove = currentnode +", ";
        evidence = remove(evidence,toremove);
        currentnode = get_str_between_two_str_var1(evidence,evidence.substr(0,0),",");
        result = result + 1;
    }
    return result;
}

string getdesaccordingtolocation(string alldes, int location){
    string result, toremove;
    alldes = alldes +", ";
    result = get_str_between_two_str_var1(alldes,alldes.substr(0,0),",");
    while(location != 0){
        toremove = result +", ";
        alldes = remove(alldes,toremove);
        result = get_str_between_two_str_var1(alldes,alldes.substr(0,0),",");
        location = location -1;
    }
    return result;
}

double calculateifnopar(string network, string question){
    vector< vector<string> > result;
    result = calculate(network, question);
    double finalresult1 = 1;
    double finalresult2 = 0;
    double single;
    for(int i=0; i<result.size();i++){
        for(int j=0; j<result[i].size();j++){
            single = atof(result[i][j].c_str());
            finalresult1 = finalresult1 * single;
        }
        finalresult2 = finalresult2 + finalresult1;
        finalresult1 = 1;
    }
    return finalresult2;
}

double calculateifnoparundercondition(string network, string question){//P(a = yes | b = yes, c = yes)
    string hypothesis = get_str_between_two_str(question,"("," |");
    string evidence = get_str_between_two_str(question,"| ",")");
    string hypoevid = "P("+ hypothesis + ", " + evidence + ")";
    double result1 = calculateifnopar(network, hypoevid);
    double result2 = calculateifnopar(network, evidence);
    double finalresult = result1 / result2;
    return finalresult;
}

vector<string> returncorrect(vector<string> already, vector<string> now){
    vector<string> result;
    for(int i = 0; i<already.size();i++){
        for(int j = 0 ; j<now.size();j++){
            result.push_back(already[i] + ", " + now[j]);
        }
    }
    return result;
}

string searchroot(string network){
    ifstream filein(network);
    string strTemp;
    string result;
    bool flag = true;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            if(strTemp.find("|") == string::npos){
                result = get_str_between_two_str(strTemp,"( "," )");
                break;
            }
        }
    }
    return result;
}

vector<string> searchlastchildren(string network){
    ifstream filein(network);
    string strTemp;
    string node;
    string evidence;
    string singleevidence;
    vector<string> vectorforevidence;
    vector<string> result;
    while(getline(filein,strTemp)){
        if(strTemp.find("variable") != string::npos){
            node = get_str_between_two_str(strTemp,"variable "," {");
            result.push_back(node);
        }
        if(strTemp.find("probability") != string::npos){
            if(strTemp.find("|") != string::npos){
                evidence = get_str_between_two_str(strTemp,"| "," )");
                evidence = evidence + ", ";
                if(evidence != ""){
                    singleevidence = get_str_between_two_str_var1(evidence,evidence.substr(0,0),",");
                    for(vector<string>::iterator iter=result.begin();iter!=result.end();iter++){     
                          if(*iter== singleevidence){
                          result.erase(iter);
                          break;
                        }
                    }
                    singleevidence = singleevidence + ", ";
                    evidence = remove(evidence,singleevidence);
                }
            } 
        }
}
     return result;
}


string moreover(string network,string known,string missingfeature,vector<string> featurenotappear,double threshold,string assumption){
    vector<string> irrelevantfeature;
    vector<string> irrelevantdes;
    string relevantfeature;
    string chooseone;
    string choosefeaturename;
    vector<string> desforonefeature;
    string evidence;
    string conditionalquery;
    double result;
    int slength;
    string output;
    vector<string> stillonerelevant;
    string evidence1;
    bool jump = false;
    for(int i = 0; i< featurenotappear.size();i++){
        jump = false;
        chooseone = get_str_between_two_str_var1(missingfeature,missingfeature.substr(0,0),",");
        choosefeaturename = get_str_between_two_str(chooseone,chooseone.substr(0,0)," =");
        chooseone = chooseone + ", ";
        missingfeature = remove(missingfeature,chooseone);
        desforonefeature = describe(network,choosefeaturename);
        slength = relevantfeature.length();
        for(int j=0;j<desforonefeature.size();j++){
            evidence = relevantfeature + choosefeaturename + " = " + desforonefeature[j] + ", " + missingfeature + known + ", ";
            if(irrelevantfeature.size() == 0){
                evidence.pop_back();
                evidence.pop_back();
                conditionalquery = "P(" + assumption + " | " + evidence + ")";
                result = calculateifnoparundercondition(network, conditionalquery);
                if(result >= threshold){
                    relevantfeature = relevantfeature + chooseone;
                    break;
                }
            }
            else{
                for(int a = 0; a < irrelevantdes.size();a++){
                    evidence1 = evidence;
                    evidence1 = evidence1 + irrelevantdes[a];
                    conditionalquery = "P(" + assumption + " | " + evidence1 + ")";
                    result = calculateifnoparundercondition(network, conditionalquery);
                    if(result >= threshold){
                        relevantfeature = relevantfeature + chooseone;
                        jump = true;
                        break;
                    }
                }
                if(jump == true){
                    break;
                }
            }
        }
            if(slength == relevantfeature.length()){
                irrelevantfeature.push_back(featurenotappear[i]);
                if(irrelevantdes.size() == 0){
                    for(int k = 0; k< desforonefeature.size();k++){
                        irrelevantdes.push_back(choosefeaturename + " = " + desforonefeature[k]);
                    }
                }
                else{
                    for(int k = 0; k< desforonefeature.size();k++){
                        stillonerelevant.push_back(choosefeaturename + " = " + desforonefeature[k]);
                        irrelevantdes = returncorrect(irrelevantdes,stillonerelevant);
                        stillonerelevant.empty();
                    }
                }
            }
    }
    if(featurenotappear.size() == irrelevantfeature.size()){
        output =  "Your assumption is already wrong under the given features, regardless of";
        for(int i = 0; i< irrelevantfeature.size();i++){
            output = output + " " + irrelevantfeature[i];
        }
        return output;
    }
    else{
        if(irrelevantfeature.size() == 0){
            relevantfeature.pop_back();
            relevantfeature.pop_back();
            output = "Your assumption is wrong when " + relevantfeature;
            return output;
        }
        else{ 
            output = "Your assumption is wrong when " + relevantfeature + ", but don't need to consider";
            for(int i = 0; i< irrelevantfeature.size();i++){
            output = output + " "+irrelevantfeature[i];
            }
            return output;          
        }
    }
    return "help";
}

string moreover2(string network,string known,string missingfeature,vector<string> featurenotappear,double threshold,string assumption){
    vector<string> irrelevantfeature;
    vector<string> irrelevantdes;
    string relevantfeature;
    string chooseone;
    string choosefeaturename;
    vector<string> desforonefeature;
    string evidence;
    string conditionalquery;
    double result;
    int slength;
    string output;
    vector<string> stillonerelevant;
    string evidence1;
    bool jump = false;
    for(int i = 0; i< featurenotappear.size();i++){
        jump = false;
        chooseone = get_str_between_two_str_var1(missingfeature,missingfeature.substr(0,0),",");
        choosefeaturename = get_str_between_two_str(chooseone,chooseone.substr(0,0)," =");
        chooseone = chooseone + ", ";
        missingfeature = remove(missingfeature,chooseone);
        desforonefeature = describe(network,choosefeaturename);
        slength = relevantfeature.length();
        for(int j=0;j<desforonefeature.size();j++){
            evidence = relevantfeature + choosefeaturename + " = " + desforonefeature[j] + ", " + missingfeature + known + ", ";
            if(irrelevantfeature.size() == 0){
                evidence.pop_back();
                evidence.pop_back();
                conditionalquery = "P(" + assumption + " | " + evidence + ")";
                result = calculateifnoparundercondition(network, conditionalquery);
                if(result < threshold){
                    relevantfeature = relevantfeature + chooseone;
                    break;
                }
            }
            else{
                for(int a = 0; a < irrelevantdes.size();a++){
                    evidence1 = evidence;
                    evidence1 = evidence1 + irrelevantdes[a];
                    conditionalquery = "P(" + assumption + " | " + evidence1 + ")";
                    result = calculateifnoparundercondition(network, conditionalquery);
                    if(result < threshold){
                        relevantfeature = relevantfeature + chooseone;
                        jump = true;
                        break;
                    }
                }
                if(jump == true){
                    break;
                }
            }
        }
            if(slength == relevantfeature.length()){
                irrelevantfeature.push_back(featurenotappear[i]);
                if(irrelevantdes.size() == 0){
                    for(int k = 0; k< desforonefeature.size();k++){
                        irrelevantdes.push_back(choosefeaturename + " = " + desforonefeature[k]);
                    }
                }
                else{
                    for(int k = 0; k< desforonefeature.size();k++){
                        stillonerelevant.push_back(choosefeaturename + " = " + desforonefeature[k]);
                        irrelevantdes = returncorrect(irrelevantdes,stillonerelevant);
                        stillonerelevant.empty();
                    }
                }
            }
    }
    if(featurenotappear.size() == irrelevantfeature.size()){
        output =  "Your assumption is already wrong under the given features, regardless of";
        for(int i = 0; i< irrelevantfeature.size();i++){
            output = output + " " + irrelevantfeature[i];
        }
        return output;
    }
    else{
        if(irrelevantfeature.size() == 0){
            relevantfeature.pop_back();
            relevantfeature.pop_back();
            output = "Your assumption is wrong when " + relevantfeature;
            return output;
        }
        else{ 
            output = "Your assumption is wrong when " + relevantfeature + ", but don't need to consider";
            for(int i = 0; i< irrelevantfeature.size();i++){
            output = output + " "+irrelevantfeature[i];
            }
            return output;          
        }
    }
    return "help";
}

vector<string> allfeatures(string network){
    ifstream filein(network);
    string strTemp;
    string singlefeature;
    vector<string> result;
    bool flag = true;
    while(getline(filein,strTemp)){
        if(strTemp.find("feature") != string::npos){
            singlefeature = get_str_between_two_str(strTemp,"feature ","{");
            result.push_back(singlefeature);
        }
    }
    return result;
}

string decideoperator(string network, string root, string desinassumption){
    vector<string> op;
    op.push_back("<");
    op.push_back(">=");
    vector<string> des = describe(network,root);
    if(desinassumption == des[0]){//assumption: goodfruit = no
        return op[0];
    }
    else{//assumption: goodfruit = yes
        return op[1];
    }
    return "help";
}

string checkexplainability2(string network, string assumption, string known, double threshold){//hier known: sweetness = yes, appearance = no
    string store = known;
    vector<string> features;
    vector<string> featureappear;
    string singlefeature;
    string singlefeatureandvalue;
    string root = get_str_between_two_str(assumption,assumption.substr(0,0)," =");
    string desinassumption = get_str_between_two_str(assumption,"= ",assumption.substr(assumption.length(),assumption.length()));
    string op = decideoperator(network,root,desinassumption);
    features = allfeatures(network);
    known = known + ", ";
    double finalresult;
    string conditionalquery;
    while(known != ""){
        singlefeatureandvalue = get_str_between_two_str_var1(known,known.substr(0,0),",");
        singlefeature = get_str_between_two_str(singlefeatureandvalue,singlefeatureandvalue.substr(0,0)," =");
        featureappear.push_back(singlefeature);
        singlefeatureandvalue = singlefeatureandvalue+", ";
        known = remove(known,singlefeatureandvalue);
    }
    if(features.size() == featureappear.size()){
        conditionalquery = "P(" + assumption + " | " + store + ")";
        finalresult = calculateifnoparundercondition(network, conditionalquery);
        if(finalresult < threshold){
            return "Your assumption is correct under the given features";
        }
        else{
            return "Your assumption is wrong under the given features according to the conditional query";
        }
    }
    vector<string> featurenotappear;
    for(int i=0; i<features.size();i++){
        if(contains(featureappear, features[i]) == false){
            featurenotappear.push_back(features[i]);
        }
    }
    vector< vector<string> > alldescription;
    vector<string> singlenodedescription;
     for(int i=0; i<featurenotappear.size();i++){
         singlenodedescription = describe(network,featurenotappear[i]);
         for(int j=0;j<singlenodedescription.size(); j++){
             singlenodedescription[j] = featurenotappear[i]+" = "+singlenodedescription[j];
         }
          alldescription.push_back(singlenodedescription);
     }
    vector<int> numberofalldescription;
     for(int i = 0; i< alldescription.size();i++){
         numberofalldescription.push_back(alldescription[i].size());
     }
    vector < vector<int> > m = BuildAllState2(alldescription.size(),numberofalldescription);
    string missingfeature;
    string queryinwork;
     for(int i=0;i<m.size();i++){
         missingfeature = "";
         for(int j =0 ; j<alldescription.size(); j ++){
             missingfeature = missingfeature + alldescription[j][m[i][j]]+", ";//cancer = yes, asia = no, a = 0,  
         }
         missingfeature.pop_back();
         missingfeature.pop_back();
         queryinwork = "P(" + assumption + " | "+ store + ", " + missingfeature + ")";//P(asia = yes | a = 0, b = 0)
         finalresult = calculateifnoparundercondition(network,queryinwork);
         if(finalresult >= threshold){
             missingfeature = missingfeature+", ";
             return moreover2(network,store,missingfeature,featurenotappear,threshold,assumption);
         }
     }
    return "Your assumption is correct under the incomplete given features";
}


string checkexplainability(string network, string assumption, string known, double threshold){//hier known: sweetness = yes, appearance = no
    string store = known;
    vector<string> features;
    vector<string> featureappear;
    string singlefeature;
    string singlefeatureandvalue;
    string root = get_str_between_two_str(assumption,assumption.substr(0,0)," =");
    string desinassumption = get_str_between_two_str(assumption,"= ",assumption.substr(assumption.length(),assumption.length()));
    string op = decideoperator(network,root,desinassumption);
    if(op == "<"){
        return checkexplainability2(network, assumption, known, threshold);
    }
    features = allfeatures(network);
    known = known + ", ";
    double finalresult;
    string conditionalquery;
    while(known != ""){
        singlefeatureandvalue = get_str_between_two_str_var1(known,known.substr(0,0),",");
        singlefeature = get_str_between_two_str(singlefeatureandvalue,singlefeatureandvalue.substr(0,0)," =");
        featureappear.push_back(singlefeature);
        singlefeatureandvalue = singlefeatureandvalue+", ";
        known = remove(known,singlefeatureandvalue);
    }
    if(features.size() == featureappear.size()){
        conditionalquery = "P(" + assumption + " | " + store + ")";
        finalresult = calculateifnoparundercondition(network, conditionalquery);
        if(finalresult >= threshold){
            return "Your assumption is correct under the given features";
        }
        else{
            return "Your assumption is wrong under the given features according to the conditional query";
        }
    }
    vector<string> featurenotappear;
    for(int i=0; i<features.size();i++){
        if(contains(featureappear, features[i]) == false){
            featurenotappear.push_back(features[i]);
        }
    }
    vector< vector<string> > alldescription;
    vector<string> singlenodedescription;
     for(int i=0; i<featurenotappear.size();i++){
         singlenodedescription = describe(network,featurenotappear[i]);
         for(int j=0;j<singlenodedescription.size(); j++){
             singlenodedescription[j] = featurenotappear[i]+" = "+singlenodedescription[j];
         }
          alldescription.push_back(singlenodedescription);
     }
    vector<int> numberofalldescription;
     for(int i = 0; i< alldescription.size();i++){
         numberofalldescription.push_back(alldescription[i].size());
     }
    vector < vector<int> > m = BuildAllState2(alldescription.size(),numberofalldescription);
    string missingfeature;
    string queryinwork;
     for(int i=0;i<m.size();i++){
         missingfeature = "";
         for(int j =0 ; j<alldescription.size(); j ++){
             missingfeature = missingfeature + alldescription[j][m[i][j]]+", ";//cancer = yes, asia = no, a = 0,  
         }
         missingfeature.pop_back();
         missingfeature.pop_back();
         queryinwork = "P(" + assumption + " | "+ store + ", " + missingfeature + ")";//P(asia = yes | a = 0, b = 0)
         finalresult = calculateifnoparundercondition(network,queryinwork);
         if(finalresult < threshold){
             missingfeature = missingfeature+", ";
             return moreover(network,store,missingfeature,featurenotappear,threshold,assumption);
         }
     }
    return "Your assumption is correct under the incomplete given features";
}






