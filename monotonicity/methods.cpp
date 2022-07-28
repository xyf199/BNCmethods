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



vector<string> describe(string network, string node){//how many descriptions for a node
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

vector< vector<int> > BuildAllState(int m, int n)//buildallstate(3,2) 3 balls, every 2 color
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

vector< vector<int> > BuildAllState2(int m, vector<int> n)//buildallstate(3,2) 指的是3个球，每个2种颜色
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

string searchallcptvalue(string network, string node, string tocheck){
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

string searchallcptvalueforroot(string network, string node, string tocheck){
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



string tosmtforone(vector<string> singleformula, int order){//order from zero
    if(order == singleformula.size()-2){
        return "(* "+singleformula[order]+" "+singleformula[order+1]+")";
    }
    else{
        return "(* "+singleformula[order]+" "+tosmtforone(singleformula,order+1)+")";
    }
    
}

//0 - aa + bb - cc        0-aa+bb
//- + -                   - +   (- 0 aa)
//(- (+ (- 0 aa) bb) cc)

string tosmtforall(vector<string> allformula, vector<string> op, int order){//order from zero
    string result = "0";
    while(order < allformula.size()-1){
        result = "("+op[order]+" "+result+" "+allformula[order+1]+")";
        order= order +1;
    }
    return result;
}

void writewithsmt(string smtfile,vector<string> parameters,string prepare,string smallgreat){
    ofstream myfile;
    myfile.open(smtfile,ios::app);
    myfile << "(set-logic QF_NRA)\n";
    for(int i=0;i<parameters.size();i++){
        myfile << "(declare-fun " + parameters[i] +" () Real)\n";
    }
    for(int i=0;i<parameters.size();i++){
        myfile << "(assert (<= 0 "+parameters[i]+"))\n";
        myfile << "(assert (<= "+parameters[i]+" 1))\n";
    }
    myfile << "(assert ("+smallgreat+" 0 "+prepare+"))\n";
    myfile << "(check-sat)\n";
    myfile << "(exit)\n";
    myfile.close();
}

string textinresult(){
    string strTemp;
    ifstream filein("result.txt");
    string result;
    while(getline(filein,strTemp)){
        result = strTemp;
    }
    return result;
}

int command(const char *s)
{
    char str[20000];
    strcpy(str, s);
    return system(strcat(str, " >> result.txt "));
}

void fileEmpty(const string fileName)
{
    fstream file(fileName, ios::out);
}

 
string checkmonotonicity3(string network, string question, string tocheck, vector<string> parameters){
    vector< vector<string> > probability = calculate(network,question);
    vector<string> smtformula;
    smtformula.push_back("0");
    bool contain = false;
    string temp;
    string coefficient;
    vector<string> op;
    vector<string> tempvalue;
    vector<string> tempvalue2;
    double sum = 1;
    double one; 
    for(int i=0; i<probability.size();i++){
        for(int j=0; j<probability[i].size();j++){
            if(probability[i][j].find(tocheck) != string::npos){
                contain = true;
                if(probability[i][j].find("-") != string::npos){
                    op.push_back("-");
                }
                else{
                    op.push_back("+");
                }
                if(probability[i][j].find("/") != string::npos){
                    coefficient = checkcoefficient(probability[i][j]);
                    tempvalue.push_back(coefficient);
                }
            }
            else{
                if(probability[i][j].find("p") != string::npos){//other parameters expect tocheck
                    if(probability[i][j].find("-") != string::npos){
                        if(probability[i][j].find("/") !=string::npos){
                            coefficient = checkcoefficient(probability[i][j]);
                            temp = probability[i][j];
                            temp = get_str_between_two_str(temp,"p","/");
                            temp = "p" + temp;
                            temp.pop_back();
                            tempvalue.push_back("(* "+coefficient+" (- 1 "+temp+" )"+")");
                        }
                        else{
                            temp = probability[i][j];
                            temp = get_str_between_two_str(temp,"-",temp.substr(temp.length(),temp.length()));
                            tempvalue.push_back("(- 1 "+temp+" )");
                        }
                    }
                    else{tempvalue.push_back(probability[i][j]);}//p1,p2
                }
                else{tempvalue.push_back(probability[i][j]);}//constant
            }
        }
        if(contain == true){//here make some improvement
            for(int i=0;i<tempvalue.size();i++){
                if(tempvalue[i].find("p") != string::npos){
                    tempvalue2.push_back(tempvalue[i]);
                }
                else{
                    one = atof(tempvalue[i].c_str());
                    sum = sum * one;
                }
            }
            tempvalue2.push_back(to_string(sum));
            if(tempvalue2.size() == 0){smtformula.push_back("1");}
            else{
            if(tempvalue2.size() == 1){smtformula.push_back(tempvalue2[0]);}
            else{smtformula.push_back(tosmtforone(tempvalue2,0));}
            }
        }
        contain = false;
        tempvalue.clear();
        tempvalue2.clear();
        sum = 1;
    }
    if(smtformula.size()==1){
        return "The parameter is irrelevant.";
    }
    string prepare = tosmtforall(smtformula,op,0);
    writewithsmt("monotonicity.smt2",parameters,prepare,">");
    string terminal = "dreal monotonicity.smt2";
    const char* terminal1 = terminal.c_str();
    command(terminal1);
    string result = textinresult();
    if(result == "unsat"){
        fileEmpty("monotonicity.smt2");
        fileEmpty("result.txt");
        return "monotonic increasing";
    }
    fileEmpty("monotonicity.smt2");
    fileEmpty("result.txt");
    writewithsmt("monotonicity.smt2",parameters,prepare,"<");
    command(terminal1);
    result = textinresult();
    if(result == "unsat"){
        fileEmpty("monotonicity.smt2");
        fileEmpty("result.txt");
        return "monotonic decreasing";
    }
    fileEmpty("monotonicity.smt2");
    fileEmpty("result.txt");
    return "unknown";
}

string checkmonotonicity4(string network, string question, string tocheck, vector<string> parameters,vector< vector<string> > probability){
    vector<string> smtformula;
    smtformula.push_back("0");
    bool contain = false;
    string temp;
    string coefficient;
    vector<string> op;
    vector<string> tempvalue;
    vector<string> tempvalue2;
    double sum = 1;
    double one; 
    for(int i=0; i<probability.size();i++){
        for(int j=0; j<probability[i].size();j++){
            if(probability[i][j].find(tocheck) != string::npos){
                contain = true;
                if(probability[i][j].find("-") != string::npos){
                    op.push_back("-");
                }
                else{
                    op.push_back("+");
                }
                if(probability[i][j].find("/") != string::npos){
                    coefficient = checkcoefficient(probability[i][j]);
                    if(atof(coefficient.c_str()) != 1.0){
                    tempvalue.push_back(coefficient);
                    }
                }
            }
            else{
                if(probability[i][j].find("p") != string::npos){
                    if(probability[i][j].find("-") != string::npos){
                        if(probability[i][j].find("/") !=string::npos){
                            coefficient = checkcoefficient(probability[i][j]);
                            temp = probability[i][j];
                            temp = get_str_between_two_str(temp,"p","/");
                            temp = "p" + temp;
                            temp.pop_back();
                            if(atof(coefficient.c_str()) == 1.0){
                                tempvalue.push_back("(- 1 "+temp+" )");
                            }
                            else{
                                tempvalue.push_back(coefficient);
                                tempvalue.push_back("(- 1 "+temp+" )");
                            }
                        }
                        else{
                            temp = probability[i][j];
                            temp = get_str_between_two_str(temp,"-",temp.substr(temp.length(),temp.length()));
                            tempvalue.push_back("(- 1 "+temp+" )");
                        }
                    }
                    else{tempvalue.push_back(probability[i][j]);}//p1,p2
                }
                else{tempvalue.push_back(probability[i][j]);}//constant
            }
        }
        if(contain == true){//here make some improvement
            for(int i=0;i<tempvalue.size();i++){
                if(tempvalue[i].find("p") != string::npos){
                    tempvalue2.push_back(tempvalue[i]);
                }
                else{
                    one = atof(tempvalue[i].c_str());
                    sum = sum * one;
                }
            }
            tempvalue2.push_back(to_string(sum));
            if(tempvalue2.size() == 0){smtformula.push_back("1");}
            else{
            if(tempvalue2.size() == 1){smtformula.push_back(tempvalue2[0]);}
            else{smtformula.push_back(tosmtforone(tempvalue2,0));}
            }
        }
        contain = false;
        tempvalue.clear();
        tempvalue2.clear();
        sum = 1;
    }
    if(smtformula.size()==1){
        return "The parameter is irrelevant.";
    }
    string prepare = tosmtforall(smtformula,op,0);
    writewithsmt("monotonicity.smt2",parameters,prepare,">");
    string terminal = "dreal monotonicity.smt2 --precision 0.1";
    const char* terminal1 = terminal.c_str();
    command(terminal1);
    string result = textinresult();
    if(result == "unsat"){
        fileEmpty("monotonicity.smt2");
        fileEmpty("result.txt");
        return "monotonic increasing";
    }
    fileEmpty("monotonicity.smt2");
    fileEmpty("result.txt");
    writewithsmt("monotonicity.smt2",parameters,prepare,"<");
    command(terminal1);
    result = textinresult();
    if(result == "unsat"){
        fileEmpty("monotonicity.smt2");
        fileEmpty("result.txt");
        return "monotonic decreasing";
    }
    fileEmpty("monotonicity.smt2");
    fileEmpty("result.txt");
    return "unknown";
}


string checkmonotonicity3ex(string network, string question, string tocheck, vector<string> parameters){
    vector< vector<string> > probability = calculate(network,question);
    vector<string> smtformula;
    smtformula.push_back("0");
    bool contain = false;
    string temp;
    string coefficient;
    vector<string> op;
    vector<string> tempvalue;
    vector<string> tempvalue2;
    double sum = 1;
    double one;
    for(int i=0; i<probability.size();i++){
        for(int j=0; j<probability[i].size();j++){
            if(probability[i][j].find(tocheck) != string::npos){
                contain = true;
                if(probability[i][j].find("-") != string::npos){
                    op.push_back("-");
                }
                else{
                    op.push_back("+");
                }
                if(probability[i][j].find("/") != string::npos){
                    coefficient = checkcoefficient(probability[i][j]);
                    tempvalue.push_back(coefficient);
                }
            }
            else{
                if(probability[i][j].find("p") != string::npos){//other parameters expect tocheck
                    if(probability[i][j].find("-") != string::npos){
                        if(probability[i][j].find("/") !=string::npos){
                            coefficient = checkcoefficient(probability[i][j]);
                            temp = probability[i][j];
                            temp = get_str_between_two_str(temp,"-","/");
                            temp.pop_back();
                            tempvalue.push_back("(* "+coefficient+" (- 1 "+temp+" ))");
                        }
                        else{
                            temp = probability[i][j];
                            temp = get_str_between_two_str(temp,"-",temp.substr(temp.length(),temp.length()));
                            tempvalue.push_back("(- 1 "+temp+" )");
                        }
                    }
                    else{tempvalue.push_back(probability[i][j]);}//p1,p2
                }
                else{tempvalue.push_back(probability[i][j]);}//constant
            }
        }
        if(contain == true){//here make some improvement
            for(int i=0;i<tempvalue.size();i++){
                if(tempvalue[i].find("p") != string::npos){
                    tempvalue2.push_back(tempvalue[i]);
                }
                else{
                    one = atof(tempvalue[i].c_str());
                    sum = sum * one;
                }
            }
            tempvalue2.push_back(to_string(sum));
            if(tempvalue2.size() == 0){smtformula.push_back("1");}
            else{
            if(tempvalue2.size() == 1){smtformula.push_back(tempvalue2[0]);}
            else{smtformula.push_back(tosmtforone(tempvalue2,0));}
            }
        }
        contain = false;
        tempvalue.clear();
        tempvalue2.clear();
        sum = 1;
    }
    if(smtformula.size()==1){
        return "The parameter is irrelevant.";
    }
    string prepare = tosmtforall(smtformula,op,0);
    writewithsmt("monotonicity.smt2",parameters,prepare,">");
    
    return "unknown";
}

string smtforp(string tocheck, int n){
    string s;
    if(n == 0){
        if(tocheck.find("-") != string::npos){
            s = "-1";
            return s;
        } 
        else{
            s = "1";
            return s;
        }
    }
    if(n == 1){
        s = tocheck;
        return s;
    }
    for(int i = 0;i<n-1;i++){
        s = s + "(* " + tocheck + " ";
    }
    s = s + tocheck +")";
    for(int i = 0; i<n-2;i++){
        s = s +")";
    }
    return s;
}

string getfirstderi(vector<string> opforoneparameter, string tocheck){
    int numberforplus = 0;
    int numberforminus = 0;
    string numberplus;
    string numberminus;
    string numberplusderi;
    string numberminusderi;
    string m;
    string a,b,c,d;
    string ab, cd;
    string finalresult;
    int plusforder;
    int minusforder;
    for(int i = 0; i< opforoneparameter.size();i++){
        if(opforoneparameter[i] == "+"){
            numberforplus = numberforplus + 1;
        }
        else{
            numberforminus = numberforminus + 1;
        }
    }
    if(numberforminus == 0){
        numberplus = to_string(numberforplus);
        numberforplus = numberforplus - 1;
        return "(* " + numberplus + " " + smtforp(tocheck,numberforplus) + ")";
    }
    if(numberforplus == 0){
        numberminus = to_string(numberforminus);
        numberforminus = numberforminus - 1;
        m = "(- 1 "+ tocheck +")";
        return "(- 0 (* " + numberminus + " " + smtforp(m,numberforminus) + "))";
    } 
    else{
        plusforder = numberforplus - 1;
        minusforder = numberforminus - 1;
        numberminus = to_string(numberforminus);
        numberplus = to_string(numberforplus);
        numberminusderi = to_string(minusforder);
        numberplusderi = to_string(plusforder);
        a = "(* " + numberplus + " " + smtforp(tocheck,plusforder) +")";
        m = "(- 1 "+ tocheck +")";
        b = smtforp(m,numberforminus);
        c = smtforp(tocheck,numberforplus);
        d = "(* " + numberminus + " " + smtforp(m,minusforder) +")";
        ab = "(* " + a + " " + b + ")";
        cd = "(* " + c + " " + d + ")";
        finalresult = "(- " + ab + " " + cd + ")";
        return finalresult;
        //continue, finalversion should be ab - cd 
    }
    return "help";
}

string checkmonotonicity6(string network, string question, string tocheck, vector<string> parameters,vector< vector<string> > probability){
    vector<string> smtformula;
    smtformula.push_back("0");
    bool contain = false;
    string temp;
    string coefficient;
    vector<string> op;
    vector<string> opforoneparameter;
    vector<string> tempvalue;
    vector<string> tempvalue2;
    string firstderi;
    double sum = 1;
    double one; 
    int morethanone = 0;
    for(int i=0; i<probability.size();i++){
        morethanone = 0;
        opforoneparameter.empty();
        for(int j=0; j<probability[i].size();j++){
            if(probability[i][j].find(tocheck) != string::npos){
                contain = true;
                morethanone = morethanone + 1;
                if(morethanone < 2){
                if(probability[i][j].find("-") != string::npos){
                    opforoneparameter.push_back("-");
                    op.push_back("-");
                  }
                else{
                    opforoneparameter.push_back("+");
                    op.push_back("+");
                  }
                }
                else{
                    op.pop_back();
                    op.push_back("+");
                   if(probability[i][j].find("-") != string::npos){
                        opforoneparameter.push_back("-");
                    }
                   else{
                        opforoneparameter.push_back("+");         
                    }
                }
                if(probability[i][j].find("/") != string::npos){
                    coefficient = checkcoefficient(probability[i][j]);
                    if(atof(coefficient.c_str()) != 1.0){
                    tempvalue.push_back(coefficient);
                    }
                }
            }
            else{
                if(probability[i][j].find("p") != string::npos){//other parameters expect tocheck
                    if(probability[i][j].find("-") != string::npos){
                        if(probability[i][j].find("/") !=string::npos){
                            coefficient = checkcoefficient(probability[i][j]);
                            temp = probability[i][j];
                            temp = get_str_between_two_str(temp,"p","/");
                            temp = "p" + temp;
                            temp.pop_back();
                            if(atof(coefficient.c_str()) == 1.0){
                                tempvalue.push_back("(- 1 "+temp+" )");
                            }
                            else{
                                tempvalue.push_back(coefficient);
                                tempvalue.push_back("(- 1 "+temp+" )");
                            }
                        }
                        else{
                            temp = probability[i][j];
                            temp = get_str_between_two_str(temp,"-",temp.substr(temp.length(),temp.length()));
                            tempvalue.push_back("(- 1 "+temp+" )");
                        }
                    }
                    else{tempvalue.push_back(probability[i][j]);}//p1,p2
                }
                else{tempvalue.push_back(probability[i][j]);}//constant
            }
            if(morethanone > 1){
                firstderi = getfirstderi(opforoneparameter, tocheck);
                tempvalue.push_back(firstderi);
            }
        }
        if(contain == true){//here make some improvement
            for(int i=0;i<tempvalue.size();i++){
                if(tempvalue[i].find("p") != string::npos){
                    tempvalue2.push_back(tempvalue[i]);
                }
                else{
                    one = atof(tempvalue[i].c_str());
                    sum = sum * one;
                }
            }
            tempvalue2.push_back(to_string(sum));
            if(tempvalue2.size() == 0){smtformula.push_back("1");}
            else{
            if(tempvalue2.size() == 1){smtformula.push_back(tempvalue2[0]);}
            else{smtformula.push_back(tosmtforone(tempvalue2,0));}
            }
        }
        contain = false;
        tempvalue.clear();
        tempvalue2.clear();
        sum = 1;
    }
    if(smtformula.size()==1){
        return "The parameter is irrelevant.";
    }
    string prepare = tosmtforall(smtformula,op,0);
    writewithsmt("monotonicity.smt2",parameters,prepare,">");
    string terminal = "dreal monotonicity.smt2 --precision 0.1";
    const char* terminal1 = terminal.c_str();
    command(terminal1);
    string result = textinresult();
    if(result == "unsat"){
        fileEmpty("monotonicity.smt2");
        fileEmpty("result.txt");
        return "monotonic increasing";
    }
    fileEmpty("monotonicity.smt2");
    fileEmpty("result.txt");
    writewithsmt("monotonicity.smt2",parameters,prepare,"<");
    command(terminal1);
    result = textinresult();
    if(result == "unsat"){
        fileEmpty("monotonicity.smt2");
        fileEmpty("result.txt");
        return "monotonic decreasing";
    }
    fileEmpty("monotonicity.smt2");
    fileEmpty("result.txt");
    return "unknown";
}


map<string, vector<string> > checkpara(string network, string tocheck){
    //一个参数不能多次出现在一个node的cpt里,问一下
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
//if nodewiththispar.size() > 1, appears more than once
    return nodeanddescription;
}

vector<string> checkpara2(string network, string tocheck){
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
            nodewiththispar.push_back(currentnode);
        }
    }
    return nodewiththispar;
}

string checkmonotonicitysimplify(string network, string question, string tocheck, vector<string> parameters){
    map<string, vector<string> > relevantnodeanddescription = checkpara(network,tocheck);
    vector<string> allkey = KeySet2(relevantnodeanddescription);
    string relevantnode = allkey[0];
    vector<string> relevantdescription = relevantnodeanddescription[relevantnode];
    string singlenode;
    vector<string> directchildren;
    vector<string> allchildren;
    map<string, string> mapofquery = analysequery(question);
    vector<string> allappearnodeinquery = KeySet(mapofquery);
    bool flagcase1parent = true;
    bool flagcase0children = true;
    bool flagcase0parent = true;
    if(relevantnodeanddescription[relevantnode][0] == "root"){
        bool rootcase0children = true;
        bool rootcase1children = true;
        if(contains(allappearnodeinquery,relevantnode) == true){
            string allcptvalue;
            allcptvalue = searchallcptvalueforroot(network,relevantnode,tocheck);
            allcptvalue = allcptvalue + ", ";
            map<string,int> mymap = describetonumber(network,relevantnode);
            string descriptioninquery = get_str_between_two_str_var1(question,relevantnode+" = ",",");
            int time = mymap[descriptioninquery];
            string currentvalue,toremove;
            currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
            while(time != 0){
                toremove = currentvalue + ", ";
                allcptvalue = remove(allcptvalue,toremove);
                currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
                time = time -1;
            }
            if(currentvalue.find("-") != string::npos){
                return "monotonic decreasing";
            }
            else{
                return "monotonic increasing";
            }
        }
        else{
            return checkmonotonicity3(network,question,tocheck,parameters);
        }
    }
    if(contains(allappearnodeinquery,relevantnode) == false || allkey.empty() == true){
        if(allkey.empty() == true){
            return "Parameter "+tocheck+" doesn't appear in the BNC.";
        }
        vector<string> s;
        s.push_back(relevantnode);
        directchildren = getbncnodeonce(s,network);
        allchildren = allbncnode(directchildren,network);
        allchildren.erase(allchildren.begin());
        for(int i=0;i<allchildren.size();i++){
            if(question.find(allchildren[i]) != string::npos){
                flagcase0children = false;
                break;
            }
        }
        if(flagcase0children == true){
            return "The parameter is irrelevant.";
        }
        else{
            for(int i=0; i<relevantdescription.size();i++){
             singlenode = get_str_between_two_str(relevantdescription[i],relevantdescription[i].substr(0,0)," =");
             if(question.find(singlenode) != string::npos && question.find(relevantdescription[i]) == string::npos){
                     flagcase0parent = false;
                     break;
                    }
            }
        if(flagcase0parent == false){
            return "The parameter is irrelevant.";
        }
        else{
            return checkmonotonicity3(network,question,tocheck,parameters);
            }
        }
    }
    else{
        for(int i=0; i<relevantdescription.size();i++){
            singlenode = get_str_between_two_str(relevantdescription[i],relevantdescription[i].substr(0,0)," =");
            if(question.find(singlenode) != string::npos && question.find(relevantdescription[i]) == string::npos){
                    flagcase1parent = false;
                    break;
             }
        }
        if(flagcase1parent == false){
            return "The parameter is irrelevant.";
        }
        else{
            string allcptvalue;
            allcptvalue = searchallcptvalue(network,relevantnode,tocheck);
            allcptvalue = allcptvalue + ", ";
            map<string,int> mymap = describetonumber(network,relevantnode);
            string descriptioninquery = get_str_between_two_str_var1(question,relevantnode+" = ",",");
            int time = mymap[descriptioninquery];
            string currentvalue,toremove;
            currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
            while(time != 0){
                toremove = currentvalue + ", ";
                allcptvalue = remove(allcptvalue,toremove);
                currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
                time = time -1;
            }
            if(currentvalue.find("-") != string::npos){
                return "monotonic decreasing";
            }
            else{
                return "monotonic increasing";
            }
        }
    }
    return "help";
}

string checkmonotonicitysimplify4(string network, string question, string tocheck, vector<string> parameters, vector< vector<string> > probability){
    map<string, vector<string> > relevantnodeanddescription = checkpara(network,tocheck);
    vector<string> allkey = KeySet2(relevantnodeanddescription);
    string relevantnode = allkey[0];
    vector<string> relevantdescription = relevantnodeanddescription[relevantnode];
    string singlenode;
    vector<string> directchildren;
    vector<string> allchildren;
    map<string, string> mapofquery = analysequery(question);
    vector<string> allappearnodeinquery = KeySet(mapofquery);
    bool flagcase1parent = true;
    bool flagcase0children = true;
    bool flagcase0parent = true;
    if(relevantnodeanddescription[relevantnode][0] == "root"){
        bool rootcase0children = true;
        bool rootcase1children = true;
        if(contains(allappearnodeinquery,relevantnode) == true){
            string allcptvalue;
            allcptvalue = searchallcptvalueforroot(network,relevantnode,tocheck);
            allcptvalue = allcptvalue + ", ";
            map<string,int> mymap = describetonumber(network,relevantnode);
            question.pop_back();
            question = question + ", )";            
            string descriptioninquery = get_str_between_two_str_var1(question,relevantnode+" = ",",");
            int time = mymap[descriptioninquery];
            string currentvalue,toremove;
            currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
            while(time != 0){
                toremove = currentvalue + ", ";
                allcptvalue = remove(allcptvalue,toremove);
                currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
                time = time -1;
            }
            if(currentvalue.find("-") != string::npos){
                return "monotonic decreasing";
            }
            else{
                return "monotonic increasing";
            }
        }
        else{
            return checkmonotonicity4(network,question,tocheck,parameters,probability);
        }
    }
    if(contains(allappearnodeinquery,relevantnode) == false || allkey.empty() == true){//relevant node doesn't appear
        if(allkey.empty() == true){//no this parameter
            return "Parameter "+tocheck+" doesn't appear in the BNC.";
        }
        vector<string> s;
        s.push_back(relevantnode);
        directchildren = getbncnodeonce(s,network);
        allchildren = allbncnode(directchildren,network);
        allchildren.erase(allchildren.begin());
        for(int i=0;i<allchildren.size();i++){
            if(question.find(allchildren[i]) != string::npos){
                flagcase0children = false;
                break;
            }
        }
        if(flagcase0children == true){
            return "The parameter is irrelevant.";
        }
        else{
            for(int i=0; i<relevantdescription.size();i++){
             singlenode = get_str_between_two_str(relevantdescription[i],relevantdescription[i].substr(0,0)," =");
             if(question.find(singlenode) != string::npos && question.find(relevantdescription[i]) == string::npos){
                     flagcase0parent = false;
                     break;
                    }
            }
        if(flagcase0parent == false){
            return "The parameter is irrelevant.";
        }
        else{
            return checkmonotonicity4(network,question,tocheck,parameters,probability);
            }
        }
    }
    else{
        for(int i=0; i<relevantdescription.size();i++){
            singlenode = get_str_between_two_str(relevantdescription[i],relevantdescription[i].substr(0,0)," =");
            if(question.find(singlenode) != string::npos && question.find(relevantdescription[i]) == string::npos){
                    flagcase1parent = false;
                    break;
             }
        }
        if(flagcase1parent == false){
            return "The parameter is irrelevant.";
        }
        else{
            string allcptvalue;
            allcptvalue = searchallcptvalue(network,relevantnode,tocheck);
            allcptvalue = allcptvalue + ", ";
            map<string,int> mymap = describetonumber(network,relevantnode);
            question.pop_back();
            question = question + ", )";
            string descriptioninquery = get_str_between_two_str_var1(question,relevantnode+" = ",",");
            int time = mymap[descriptioninquery];
            string currentvalue,toremove;
            currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
            while(time != 0){
                toremove = currentvalue + ", ";
                allcptvalue = remove(allcptvalue,toremove);
                currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
                time = time -1;
            }           
            if(currentvalue.find("-") != string::npos){
                return "monotonic decreasing";
            }
            else{
                return "monotonic increasing";
            }
        }
    }
    return "help";
}

string checkmonotonicitysimplify5(string network, string question, string tocheck, vector<string> parameters){
    map<string, vector<string> > relevantnodeanddescription = checkpara(network,tocheck);
    vector<string> allkey = KeySet2(relevantnodeanddescription);
    string relevantnode = allkey[0];
    vector<string> relevantdescription = relevantnodeanddescription[relevantnode];
    string singlenode;
    vector<string> directchildren;
    vector<string> allchildren;
    map<string, string> mapofquery = analysequery(question);
    vector<string> allappearnodeinquery = KeySet(mapofquery);
    bool flagcase1parent = true;
    bool flagcase0children = true;
    bool flagcase0parent = true;
    if(relevantnodeanddescription[relevantnode][0] == "root"){
        bool rootcase0children = true;
        bool rootcase1children = true;
        if(contains(allappearnodeinquery,relevantnode) == true){
            string allcptvalue;
            allcptvalue = searchallcptvalueforroot(network,relevantnode,tocheck);
            allcptvalue = allcptvalue + ", ";
            map<string,int> mymap = describetonumber(network,relevantnode);
            question.pop_back();
            question = question + ", )";            
            string descriptioninquery = get_str_between_two_str_var1(question,relevantnode+" = ",",");
            int time = mymap[descriptioninquery];
            string currentvalue,toremove;
            currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
            while(time != 0){
                toremove = currentvalue + ", ";
                allcptvalue = remove(allcptvalue,toremove);
                currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
                time = time -1;
            }
            if(currentvalue.find("-") != string::npos){
                return "monotonic decreasing";
            }
            else{
                return "monotonic increasing";
            }
        }
        else{
            //return //checkmonotonicity4(network,question,tocheck,parameters,probability);
            return "should use basic algo";
        }
    }
    if(contains(allappearnodeinquery,relevantnode) == false || allkey.empty() == true){
        if(allkey.empty() == true){
            return "Parameter "+tocheck+" doesn't appear in the BNC.";
        }
        vector<string> s;
        s.push_back(relevantnode);
        directchildren = getbncnodeonce(s,network);
        allchildren = allbncnode(directchildren,network);
        allchildren.erase(allchildren.begin());
        for(int i=0;i<allchildren.size();i++){
            if(question.find(allchildren[i]) != string::npos){
                flagcase0children = false;
                break;
            }
        }
        if(flagcase0children == true){
            return "The parameter is irrelevant.";
        }
        else{
            for(int i=0; i<relevantdescription.size();i++){
             singlenode = get_str_between_two_str(relevantdescription[i],relevantdescription[i].substr(0,0)," =");
             if(question.find(singlenode) != string::npos && question.find(relevantdescription[i]) == string::npos){
                     flagcase0parent = false;
                     break;
                    }
            }
        if(flagcase0parent == false){
            return "The parameter is irrelevant.";
        }
        else{
            return "use basic algo";
            }
        }
    }
    else{
        for(int i=0; i<relevantdescription.size();i++){
            singlenode = get_str_between_two_str(relevantdescription[i],relevantdescription[i].substr(0,0)," =");
            if(question.find(singlenode) != string::npos && question.find(relevantdescription[i]) == string::npos){
                    flagcase1parent = false;
                    break;
             }
        }
        if(flagcase1parent == false){
            return "The parameter is irrelevant.";
        }
        else{
            string allcptvalue;
            allcptvalue = searchallcptvalue(network,relevantnode,tocheck);
            allcptvalue = allcptvalue + ", ";
            map<string,int> mymap = describetonumber(network,relevantnode);
            question.pop_back();
            question = question + ", )";
            string descriptioninquery = get_str_between_two_str_var1(question,relevantnode+" = ",",");
            int time = mymap[descriptioninquery];
            string currentvalue,toremove;
            currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
            while(time != 0){
                toremove = currentvalue + ", ";
                allcptvalue = remove(allcptvalue,toremove);
                currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
                time = time -1;
            }           
            if(currentvalue.find("-") != string::npos){
                return "monotonic decreasing";
            }
            else{
                return "monotonic increasing";
            }
        }
    }
    return "help";
}

string checkmonotonicitysimplify6(string network, string question, string tocheck, vector<string> parameters, vector< vector<string> > probability){
    vector<string> morethanone = checkpara2(network,tocheck);
    if(morethanone.size() > 1){
        return checkmonotonicity6(network,question,tocheck,parameters,probability);
    }
    map<string, vector<string> > relevantnodeanddescription = checkpara(network,tocheck);
    vector<string> allkey = KeySet2(relevantnodeanddescription);
    string relevantnode = allkey[0];
    vector<string> relevantdescription = relevantnodeanddescription[relevantnode];
    string singlenode;
    vector<string> directchildren;
    vector<string> allchildren;
    map<string, string> mapofquery = analysequery(question);
    vector<string> allappearnodeinquery = KeySet(mapofquery);
    bool flagcase1parent = true;
    bool flagcase0children = true;
    bool flagcase0parent = true;
    if(relevantnodeanddescription[relevantnode][0] == "root"){
        bool rootcase0children = true;
        bool rootcase1children = true;
        if(contains(allappearnodeinquery,relevantnode) == true){
            string allcptvalue;
            allcptvalue = searchallcptvalueforroot(network,relevantnode,tocheck);
            allcptvalue = allcptvalue + ", ";
            map<string,int> mymap = describetonumber(network,relevantnode);
            question.pop_back();
            question = question + ", )";            
            string descriptioninquery = get_str_between_two_str_var1(question,relevantnode+" = ",",");
            int time = mymap[descriptioninquery];
            string currentvalue,toremove;
            currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
            while(time != 0){
                toremove = currentvalue + ", ";
                allcptvalue = remove(allcptvalue,toremove);
                currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
                time = time -1;
            }
            if(currentvalue.find("-") != string::npos){
                return "monotonic decreasing";
            }
            else{
                return "monotonic increasing";
            }
        }
        else{
            return checkmonotonicity4(network,question,tocheck,parameters,probability);
        }
    }
    if(contains(allappearnodeinquery,relevantnode) == false || allkey.empty() == true){
        if(allkey.empty() == true){
            return "Parameter "+tocheck+" doesn't appear in the BNC.";
        }
        vector<string> s;
        s.push_back(relevantnode);
        directchildren = getbncnodeonce(s,network);
        allchildren = allbncnode(directchildren,network);
        allchildren.erase(allchildren.begin());
        for(int i=0;i<allchildren.size();i++){
            if(question.find(allchildren[i]) != string::npos){
                flagcase0children = false;
                break;
            }
        }
        if(flagcase0children == true){
            return "The parameter is irrelevant.";
        }
        else{
            for(int i=0; i<relevantdescription.size();i++){
             singlenode = get_str_between_two_str(relevantdescription[i],relevantdescription[i].substr(0,0)," =");
             if(question.find(singlenode) != string::npos && question.find(relevantdescription[i]) == string::npos){
                     flagcase0parent = false;
                     break;
                    }
            }
        if(flagcase0parent == false){
            return "The parameter is irrelevant.";
        }
        else{
            return checkmonotonicity4(network,question,tocheck,parameters,probability);
            }
        }
    }
    else{
        for(int i=0; i<relevantdescription.size();i++){
            singlenode = get_str_between_two_str(relevantdescription[i],relevantdescription[i].substr(0,0)," =");
            if(question.find(singlenode) != string::npos && question.find(relevantdescription[i]) == string::npos){
                    flagcase1parent = false;
                    break;
             }
        }
        if(flagcase1parent == false){
            return "The parameter is irrelevant.";
        }
        else{
            string allcptvalue;
            allcptvalue = searchallcptvalue(network,relevantnode,tocheck);
            allcptvalue = allcptvalue + ", ";
            map<string,int> mymap = describetonumber(network,relevantnode);
            question.pop_back();
            question = question + ", )";
            string descriptioninquery = get_str_between_two_str_var1(question,relevantnode+" = ",",");
            int time = mymap[descriptioninquery];
            string currentvalue,toremove;
            currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
            while(time != 0){
                toremove = currentvalue + ", ";
                allcptvalue = remove(allcptvalue,toremove);
                currentvalue = get_str_between_two_str_var1(allcptvalue,allcptvalue.substr(0,0),", ");
                time = time -1;
            }           
            if(currentvalue.find("-") != string::npos){
                return "monotonic decreasing";
            }
            else{
                return "monotonic increasing";
            }
        }
    }
    return "help";
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

string searchparameterandreturnit(string network, string tocheck){
    ifstream filein(network);
    string strTemp;
    string currentnode;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            currentnode = get_str_between_two_str(strTemp,"( "," |");
        }
        if(strTemp.find(tocheck) != string::npos){
            break;
        }
    }
    return currentnode;
}

string searchparameterandmakedecision(string network,string tocheck,string root,string des,string desforrelevantnode,string relevantnode, string question){
    ifstream filein(network);
    string strTemp;
    string currentnode;
    string currentevidence,alldesofparents,singledes;
    bool flag= true;
    int location;
    map<string, int> mymap;
    int number;
    string toremove,correctentity;
    string allcptentites;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            if(strTemp.find("|") != string::npos){
            currentnode = get_str_between_two_str(strTemp,"( "," |");
            currentevidence = get_str_between_two_str(strTemp,"| ",")");
            }
        }
        if(strTemp.find(tocheck)!= string::npos){
            if(currentevidence.find(root) == string::npos || question.find(currentnode) == string::npos){
                flag = false;
                break;
            }
            else{
                location = locatedatwhichindexinevidence(currentevidence,root);
                alldesofparents = get_str_between_two_str(currentevidence,"(",")");
                singledes = getdesaccordingtolocation(alldesofparents,location);
                if(singledes == des){
                    flag = false;
                    break;
                }
                else{
                    mymap = describetonumber(network,relevantnode);
                    number = mymap[desforrelevantnode];
                    allcptentites = get_str_between_two_str(strTemp,") ",";");
                    allcptentites = allcptentites +", ";
                    correctentity = get_str_between_two_str_var1(allcptentites,allcptentites.substr(0,0),",");
                    while(number != 0){
                        toremove = correctentity +", ";
                        allcptentites = remove(allcptentites,toremove);
                        correctentity = get_str_between_two_str_var1(allcptentites,allcptentites.substr(0,0),",");
                        number = number -1;
                    }
                }
            }
        }
    }
    if(flag == false){
        return "cannot make a decision";
    }
    else{
        if(correctentity.find("-") != string::npos){
            return "monotonic increasing";
        }
        else{
            return "monotonic decreasing";
        }
    }
    return "help";
}


string undercondition(string network, string question, string tocheck, vector<string> parameters,vector< vector<string> > probability){
    //P(asia = yes | tub = yes)
        string questionabove,questionbelow;
        string rootpart = get_str_between_two_str(question,"("," |");
        string rootname = get_str_between_two_str(rootpart,rootpart.substr(0,0)," =");
        string rootdes = get_str_between_two_str(rootpart,"= ",rootpart.substr(rootpart.length(),rootpart.length()));
        vector<string> allrotdes = describe(network,rootname);
        if(rootdes == allrotdes[0]){
            rootpart = rootname + " = " + allrotdes[1];
        }
        else{
            rootpart = rootname + " = " + allrotdes[0];
        }
        string evidencepart = get_str_between_two_str(question,"| ",")");
        questionabove = "P("+rootpart+", "+evidencepart+")";
        questionbelow = "P("+evidencepart+")";
        string aboveresult = checkmonotonicitysimplify4(network,questionabove,tocheck,parameters,probability);
        if(aboveresult == "unknown"){return "unknown";}
        string belowresult = checkmonotonicitysimplify4(network,questionbelow,tocheck,parameters,probability);
        if(aboveresult == "The parameter is irrelevant." && belowresult == "monotonic decreasing"){return "monotonic decreasing";}
        if(aboveresult == "The parameter is irrelevant." && belowresult == "monotonic increasing"){return "monotonic increasing";}
        if(belowresult == "unknown"){return "unknown";}
        if(aboveresult == "monotonic increasing" && belowresult == "monotonic increasing"){return "unknown";}
        if(aboveresult == "monotonic decreasing" && belowresult == "monotonic decreasing"){return "unknown";}
        if(aboveresult == "monotonic increasing" && belowresult == "monotonic decreasing"){return "monotonic decreasing";}
        if(aboveresult == "monotonic decreasing" && belowresult == "monotonic increasing"){return "monotonic increasing";}
        if(aboveresult == "monotonic decreasing" && belowresult == "The parameter is irrelevant."){return "monotonic increasing";}
        if(aboveresult == "monotonic increasing" && belowresult == "The parameter is irrelevant."){return "monotonic decreasing";}
        if(aboveresult == "The parameter is irrelevant." && belowresult == "The parameter is irrelevant."){return "The parameter is irrelevant.";}
        return "help";
}

string checkmonotonicitysimplifyundercondition(string network, string question, string tocheck, vector<string> parameters,vector< vector<string> > probability){
    //P(asia = yes | tub = yes)
        string questionabove,questionbelow;
        string rootpart = get_str_between_two_str(question,"("," |");
        string rootname = get_str_between_two_str(rootpart,rootpart.substr(0,0)," =");
        string rootdes = get_str_between_two_str(rootpart,"= ",rootpart.substr(rootpart.length(),rootpart.length()));
        vector<string> allrotdes = describe(network,rootname);
        string evidencepart = get_str_between_two_str(question,"| ",")");
        questionabove = "P("+rootpart+", "+evidencepart+")";
        questionbelow = "P("+evidencepart+")";
        string aboveresult = checkmonotonicitysimplify4(network,questionabove,tocheck,parameters,probability);
        if(aboveresult == "unknown"){return "unknown";}
        string belowresult = checkmonotonicitysimplify4(network,questionbelow,tocheck,parameters,probability);
        if(belowresult == "unknown"){return "unknown";}
        if(aboveresult == "monotonic increasing" && belowresult == "monotonic increasing"){
            if(allrotdes.size() == 2){
            return undercondition(network,question,tocheck,parameters,probability);
            }
            else{return "unknown";}
        }
        if(aboveresult == "monotonic decreasing" && belowresult == "monotonic decreasing"){
            if(allrotdes.size() == 2){
            return undercondition(network,question,tocheck,parameters,probability);
            }
            else{return "unknown";}           
        }
        if(aboveresult == "monotonic increasing" && belowresult == "monotonic decreasing"){return "monotonic increasing";}
        if(aboveresult == "monotonic decreasing" && belowresult == "monotonic increasing"){return "monotonic decreasing";}
        if(aboveresult == "The parameter is irrelevant." && belowresult == "monotonic decreasing"){return "monotonic increasing";}
        if(aboveresult == "The parameter is irrelevant." && belowresult == "monotonic increasing"){return "monotonic decreasing";}
        if(aboveresult == "monotonic decreasing" && belowresult == "The parameter is irrelevant."){return "monotonic decreasing";}
        if(aboveresult == "monotonic increasing" && belowresult == "The parameter is irrelevant."){return "monotonic increasing";}
        if(aboveresult == "The parameter is irrelevant." && belowresult == "The parameter is irrelevant."){return "The parameter is irrelevant.";}
        return "help";
}


vector<string> getparameter(string network){
    ifstream filein(network);
    string strTemp;
    string parameter;
    vector<string> result;
    while(getline(filein,strTemp)){
        if(strTemp.find("parameter") != string::npos){
            parameter = get_str_between_two_str(strTemp,"parameter ","{");
            result.push_back(parameter);
        }
}
    return result;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty()){
        return;
    }
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

vector<string> replaceparameter(string network,string goal){
    ifstream filein(network);
    ofstream fileout(goal);
    string strTemp;
    bool flag = false;
    string cptvalue;
    string first;
    string number;
    vector<string> s;
    string currentparameter;
    string replaceparameter;
    vector<string> result;
    for(int i=0;i<10;i++){
        number = to_string(i);
        s.push_back("0"+number);
    }
    for(int i=10;i<100;i++){
        number = to_string(i);
        s.push_back(number);
    }
    int i = 0;
    int j = 0;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            flag = true;
        }
        if(strTemp.find("parameter") != string::npos){
            flag = false;
            if(j <= i){
                strTemp = "parameter " + s[j] + "{";
                result.push_back(s[j]);
                j = j + 1;
            }
            else{
                break;
            }
        }
        if(flag == true && strTemp.find("probability") == string::npos && strTemp.find("}") == string::npos && strTemp.find(",") != string::npos){
            if(strTemp.find("table") == string::npos){
            cptvalue = get_str_between_two_str(strTemp,") ",";");
            if(cptvalue.find("p") != string::npos){
                first = get_str_between_two_str_var1(cptvalue,cptvalue.substr(0,0),",");
                if(first.find("*") != string::npos){
                    currentparameter = get_str_between_two_str(first,"p","/");
                    currentparameter.pop_back();
                    currentparameter = "p"+currentparameter;
                    replaceparameter = "p"+s[i];
                    replaceAll(strTemp,currentparameter,replaceparameter);
                    i = i + 1;
                    cout << i << endl;
                }
                else{
                    currentparameter = first;
                    replaceparameter = "p"+s[i];
                    replaceAll(strTemp,currentparameter,replaceparameter);
                    i = i + 1;
                }
            }
            }
            else{
                cptvalue = get_str_between_two_str(strTemp,"table ",";");
                if(cptvalue.find("p") != string::npos){
                first = get_str_between_two_str_var1(cptvalue,cptvalue.substr(0,0),",");
                if(first.find("*") != string::npos){
                    currentparameter = get_str_between_two_str(first,"p","/");
                    currentparameter.pop_back();
                    currentparameter = "p"+currentparameter;
                    replaceparameter = "p"+s[i];
                    replaceAll(strTemp,currentparameter,replaceparameter);
                    i = i + 1;
                }
                else{
                    currentparameter = first;
                    replaceparameter = "p"+s[i];
                    replaceAll(strTemp,currentparameter,replaceparameter);
                    i = i + 1;
                }
            }               
            }
        }
        strTemp += "\n";
        fileout << strTemp;
    }
    return result;
}

vector<string> replaceparameter2(string network,string goal){
    ifstream filein(network);
    ofstream fileout(goal);
    string strTemp;
    bool flag = false;
    string cptvalue;
    string first;
    string number;
    vector<string> s;
    string currentparameter;
    string replaceparameter;
    vector<string> result;
    for(int i=0;i<10;i++){
        number = to_string(i);
        s.push_back("00"+number);
    }
    for(int i=10;i<100;i++){
        number = to_string(i);
        s.push_back("0"+number);
    }
    for(int i=100;i<500;i++){
        number = to_string(i);
        s.push_back(number);
    }
    int i = 0;
    int j = 0;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            flag = true;
        }
        if(strTemp.find("parameter") != string::npos){
            flag = false;
            if(j <= i){
                strTemp = "parameter " + s[j] + "{";
                result.push_back(s[j]);
                j = j + 1;
            }
            else{
                break;
            }
        }
        if(flag == true && strTemp.find("probability") == string::npos && strTemp.find("}") == string::npos && strTemp.find(",") != string::npos){
            if(strTemp.find("table") == string::npos){
            cptvalue = get_str_between_two_str(strTemp,") ",";");
            if(cptvalue.find("p") != string::npos){
                first = get_str_between_two_str_var1(cptvalue,cptvalue.substr(0,0),",");
                if(first.find("*") != string::npos){
                    currentparameter = get_str_between_two_str(first,"p","/");
                    currentparameter.pop_back();
                    currentparameter = "p"+currentparameter;
                    replaceparameter = "p"+s[i];
                    replaceAll(strTemp,currentparameter,replaceparameter);
                    i = i + 1;
                    cout << i << endl;
                }
                else{
                    currentparameter = first;
                    replaceparameter = "p"+s[i];
                    replaceAll(strTemp,currentparameter,replaceparameter);
                    i = i + 1;
                }
            }
            }
            else{
                cptvalue = get_str_between_two_str(strTemp,"table ",";");
                if(cptvalue.find("p") != string::npos){
                first = get_str_between_two_str_var1(cptvalue,cptvalue.substr(0,0),",");
                if(first.find("*") != string::npos){
                    currentparameter = get_str_between_two_str(first,"p","/");
                    currentparameter.pop_back();
                    currentparameter = "p"+currentparameter;
                    replaceparameter = "p"+s[i];
                    replaceAll(strTemp,currentparameter,replaceparameter);
                    i = i + 1;
                }
                else{
                    currentparameter = first;
                    replaceparameter = "p"+s[i];
                    replaceAll(strTemp,currentparameter,replaceparameter);
                    i = i + 1;
                }
            }               
            }
        }
        strTemp += "\n";
        fileout << strTemp;
    }
    return result;
}
