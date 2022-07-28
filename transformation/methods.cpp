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

int addmy(int a, int b){
    int c = a + b;
    return c;
}

bool contains(vector<string> vec, string elem)
{
    bool result = false;
    if( find(vec.begin(), vec.end(), elem) != vec.end() )
    {
        result = true;
    }
    return result;
}

bool containsint(vector<int> vec, int elem)
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


void deleteduplicated(vector<string> &name)
{
	sort(name.begin(), name.end());
	name.erase(unique(name.begin(), name.end()), name.end());
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

vector<string> changestringtovectorstring(string s){ //get asia, cancer, smoke, a or yes, no, a, b
   string node,toremove;
   vector<string> result;
   s = s+", ";
   while(s != ""){
   node = get_str_between_two_str_var1(s,s.substr(0,0),",");
   result.push_back(node);
   toremove = node +", ";
   s = remove(s,toremove);
   }
   return result;
}

vector<string> getbncnodeonce(vector<string> s, string network){
      ifstream filein(network);
      vector<string> bncnode;
      bncnode = s;
      string strTemp,mid;
      string evidence;
      string currentnode;
      vector<string> forevidence;
      bool flag = false;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            if(strTemp.find("|") != string::npos){
                evidence = get_str_between_two_str(strTemp,"| "," )");
                currentnode = get_str_between_two_str(strTemp,"( "," |");
                forevidence = changestringtovectorstring(evidence);
                for(int i=0;i<bncnode.size();i++){
                    for(int j=0;j<forevidence.size();j++){
                        if(bncnode[i] == forevidence[j]){
                            if(contains(bncnode,currentnode) == false){
                                bncnode.push_back(currentnode);
                                flag = true;
                                break;
                            }
                        }
                    }
                    if(flag == true){
                        break;
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




void transbntobnc(string network, string goal, string parentnode)
{
    ifstream filein(network); //File to read from
    ofstream fileout(goal); //Temporary file
    map<string,int> mymap;
    string strTemp, node, number;
    string evidence;
    int discrete;
    string mid;
    bool flag1 =true;
    bool flag2 = true;
    vector<string> bncnode,bncall,notbnc;
    for (int i = 0; i<1; i++){
    bncnode.push_back(parentnode);
    }
    bncnode = getbncnodeonce(bncnode,network);
    bncall = allbncnode(bncnode,network);
    notbnc = notbncnode(bncall,network);
    for(int i = 0;i<bncall.size();i++){
        cout <<bncall[i] <<endl;
    }
    bool finalresult = true;
    bool flagforcpt = true;
    bool flagforsinglecpt = true;
    string cpt;
    string newcpt;
    string tableinformation;
    vector<string> forevidence;
    vector<string> describeforsingle;
    vector<string> describeforallnotbncinevidence;
    vector<int> positionofnotbncinevidence;
    vector<string> forcpt;
    vector<string> fortableinforamtion;
    while(getline(filein,strTemp)){
        if(strTemp.find("}") != string::npos){
            flagforcpt = true;
        }
        if(flagforcpt == false){
            cpt = get_str_between_two_str(strTemp,"(",")");
            tableinformation = get_str_between_two_str(strTemp,") ",";");
            fortableinforamtion = changestringtovectorstring(tableinformation);
            forcpt = changestringtovectorstring(cpt);

            for(int i=0; i<positionofnotbncinevidence.size();i++){
                if(forcpt[positionofnotbncinevidence[i]] != describeforallnotbncinevidence[i]){
                    finalresult = false;
                    break;
                }
                finalresult = true;
            }
            if(finalresult == true){
                strTemp = "  (";
                for(int i=0;i<forcpt.size();i++){
                    if(containsint(positionofnotbncinevidence,i) == false){
                        strTemp =strTemp + forcpt[i];
                    }
                }
                strTemp = strTemp + ") "+tableinformation;
                strTemp =strTemp + ";";
            }
        }
        if(strTemp.find("variable") != string::npos){
            node = get_str_between_two_str(strTemp,"variable "," {");
            if(contains(notbnc,node) == true){
                finalresult = false;
            }
            else{
                finalresult = true;
            }
        }
        if(strTemp.find("probability") != string::npos){
            finalresult = true;
            if(strTemp.find("|") == string::npos){
                node = get_str_between_two_str(strTemp,"( "," )");
                if(contains(notbnc,node) == true){
                    finalresult = false;
                }
                else{
                    finalresult = true;
                }
            }
            if(strTemp.find("|") != string::npos){
                node = get_str_between_two_str(strTemp,"( "," |");
                if(contains(notbnc,node) == true){
                    finalresult = false;
                }
                else{
                    evidence = get_str_between_two_str(strTemp,"| "," )");
                    forevidence = changestringtovectorstring(evidence);
                    strTemp = "probability ( "+node+" | ";
                    for(int i=0;i<forevidence.size();i++){
                        if(contains(notbnc,forevidence[i]) == true){
                            positionofnotbncinevidence.push_back(i);
                            describeforsingle = describe(network,forevidence[i]);
                            describeforallnotbncinevidence.push_back(describeforsingle[describeforsingle.size()-1]); 
                            flagforcpt = false;
                        }
                        else{strTemp = strTemp + forevidence[i] +", ";}
                    }
                    strTemp.pop_back();
                    strTemp.pop_back();
                    strTemp = strTemp + " )" + " {";
                }
            }
        }
    if(finalresult == true){
     strTemp += "\n";
     fileout << strTemp;
    }
}
}

void transbntobnc2(string network, string goal, string parentnode)
{
    ifstream filein(network); //File to read from
    ofstream fileout(goal); //Temporary file
    map<string,int> mymap;
    string strTemp, node, number;
    int discrete;
    string mid;
    bool flag1 =true;
    bool flag2 = true;
    vector<string> bncnode,bncall,notbnc;
    string currentnode;
    string evidence;
    vector<string> boxforevidence;
    vector<string> boxforbncnode;
    vector<int> positionofnotbncnode;
    vector<string> descriptionofnotbncnode; 
    vector<string> alldescription;
    string singledescription;
    string newevidence;
    string cpt,entity;
    string newcpt;
    vector<string> forcpt;
    for (int i = 0; i<1; i++){
    bncnode.push_back(parentnode);
    }
    bncnode = getbncnodeonce(bncnode,network);
    bncall = allbncnode(bncnode,network);
    notbnc = notbncnode(bncall,network);
    bool finalresult = true;
    bool flagforcpt = true;
    bool flagforsinglecpt = true;
    bool checkcpt = true;
    string variable;
    while(getline(filein,strTemp)){
        if(strTemp.find("parameter") != string::npos){
            finalresult = true;
        }
        if(strTemp.find("}") != string::npos){
            flagforcpt = true;
        }
        if(strTemp.find("variable") != string::npos){
           variable = get_str_between_two_str(strTemp,"variable "," {");
           if(contains(bncall,variable) == true){
               finalresult = true;
           }
           else{
               finalresult = false;
           }
        }
       if(strTemp.find("probability") != string::npos){
           if(strTemp.find("|") == string::npos){
               currentnode = get_str_between_two_str(strTemp,"( "," )");
               if(currentnode == parentnode){
                   finalresult = true;
               }
               else{finalresult = false;}
           }
           if(strTemp.find("|") != string::npos){
               flagforcpt = true;
               boxforevidence.clear();
               boxforbncnode.clear();
               positionofnotbncnode.clear();
               descriptionofnotbncnode.clear();
               alldescription.clear();
               currentnode = get_str_between_two_str(strTemp,"( "," |");
               evidence = get_str_between_two_str(strTemp,"| "," )");
               if(contains(bncall,currentnode) == false){
                   finalresult = false;
               }
               else{
                   finalresult = true;
                   boxforevidence = changestringtovectorstring(evidence);
                   for(int i=0;i<boxforevidence.size();i++){
                       if(contains(bncall,boxforevidence[i]) == true){
                           boxforbncnode.push_back(boxforevidence[i]);
                       }
                       else{
                           positionofnotbncnode.push_back(i);
                           alldescription = describe(network,boxforevidence[i]);
                           singledescription = alldescription[0];
                           descriptionofnotbncnode.push_back(singledescription);
                       }
                   }
                   if(boxforbncnode.size() == boxforevidence.size()){
                       flagforcpt = true;
                       newevidence = evidence;
                   }
                   else{
                       flagforcpt = false;
                       for(int i=0;i<boxforbncnode.size();i++){
                       newevidence = newevidence + boxforbncnode[i] +", ";
                       }
                       newevidence.pop_back();
                       newevidence.pop_back();
                   }
                   //probability ( choledocholithotomy | gallstones ){
                   strTemp = "probability ( "+currentnode+" | "+ newevidence+ " ){";
                   newevidence = "";
               }
           }
       }
       if(finalresult == true && flagforcpt == false && strTemp.find("probability") == string::npos && strTemp.find("}") == string::npos){
               checkcpt = true;
               cpt = get_str_between_two_str(strTemp,"(",")");
               forcpt = changestringtovectorstring(cpt);
               entity = get_str_between_two_str(strTemp,")",";");
               for(int i=0;i<positionofnotbncnode.size();i++){
                   if(forcpt[positionofnotbncnode[i]] != descriptionofnotbncnode[i]){
                       checkcpt = false;
                       break;
                   }
               }
               if(checkcpt == true){
                   newcpt = "";
                   for(int i=0;i<forcpt.size();i++){
                       if(containsint(positionofnotbncnode,i) == false){
                           newcpt = newcpt + forcpt[i] + ", ";
                       }
                   }
                   newcpt.pop_back();
                   newcpt.pop_back();
                   strTemp = "("+newcpt+")"+entity+";";
               }
               if(checkcpt == false){
                   strTemp = "notice";
               }
       }
       if(finalresult == true){
           if(strTemp != "notice"){
           strTemp += "\n";
           fileout << strTemp;
           }
        }
    }
}

void createvarfile(string network, string goal){
    ifstream filein(network);
    ofstream fileout(goal);
    string strTemp;
    string currentnode;
    string number;
    string write;
    bool flag = true;
    while(getline(filein,strTemp)){
        if(flag == false){
            number = get_str_between_two_str(strTemp,"[ "," ]");
            write = currentnode+"-"+number;
            fileout << write+"\n";
            flag = true;
        }
        if(strTemp.find("variable") != string::npos){
            currentnode = get_str_between_two_str(strTemp,"variable "," {");
            flag = false;
        } 
        if(strTemp.find("probability") != string::npos){
            break;
        }
    }
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

void replaceparameter(string network,string goal){
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
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            flag = true;
        }
        if(strTemp.find("parameter") != string::npos){
            flag = false;
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
}

void writequery(string network,string goal){
    ifstream filein(network);
    ofstream fileout(goal);
    string strTemp;
    string des;
    string currentvariable;
    string all;
    string first;
    bool flag = false;
    int i=20;
    while(getline(filein,strTemp)){
        if(i > 0){
        flag = false;
        if(strTemp.find("variable") != string::npos){
            currentvariable = get_str_between_two_str(strTemp,"variable "," {");
        }
        if(strTemp.find("discrete") != string::npos){
            flag = true;
            all = get_str_between_two_str(strTemp,"{ "," }");
            first = get_str_between_two_str_var1(all,all.substr(0,0),",");
            des = currentvariable + " = " + first;
            i = i - 1;
        }
        if(flag == true){
            des += "\n";
            fileout << des;
        }
        }
    }
}

int countedge(string network){
    ifstream filein(network);
    int result = 0;
    int temp = 0;
    string strTemp;
    string parents;
    string single;
    while(getline(filein,strTemp)){
        temp = 0;
        if(strTemp.find("probability") != string::npos && strTemp.find("|") != string::npos){
            parents = get_str_between_two_str(strTemp,"| "," )");
            parents = parents + ", ";
            cout << parents << endl;
            while(parents != ""){
                single = get_str_between_two_str_var1(parents,parents.substr(0,0),",");
                temp = temp+1;
                single = single + ", ";
                parents = remove(parents,single);
            }
        }
        result = result + temp;
    }
    return result;
}

int numberdirectparents(string network, string node){
    ifstream filein(network);
    string strTemp;
    string parents;
    string single;
    string currentnode;
    int result=0;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos){
            currentnode = get_str_between_two_str(strTemp,"( "," |");
            if(currentnode == node){
            if(strTemp.find("|") != string::npos){
                parents = get_str_between_two_str(strTemp,"| "," )");
                parents = parents + ", ";
                while(parents != ""){
                    single = get_str_between_two_str_var1(parents,parents.substr(0,0),",");
                    result = result + 1;
                    single = single + ", ";
                    parents = remove(parents,single);
                }
               }
            }
        }
    }
    return result;
}

vector<string> directchildren(string network, string node){
    ifstream filein(network);
    string strTemp;
    string parents;
    string single;
    string currentnode;
    vector<string> evidence;
    vector<string> result;
    while(getline(filein,strTemp)){
        if(strTemp.find("probability") != string::npos && strTemp.find("|") != string::npos){
            parents = get_str_between_two_str(strTemp,"| "," )");
            evidence = changestringtovectorstring(parents);
            if(contains(evidence,node) == true){
                currentnode = get_str_between_two_str(strTemp,"( "," |");
                result.push_back(currentnode);
            }
            evidence.clear();
        }
    }
    return result;
}

int AVB(string network){
    ifstream filein(network);
    string strTemp;
    vector<string> allnode;
    string currentnode;
    int numberallparents = 0;
    int parents = 0;
    int numberallchildren = 0;
    int other = 0;
    vector<string> allchildren;
    while(getline(filein,strTemp)){
        if(strTemp.find("variable") != string::npos){
            currentnode = get_str_between_two_str(strTemp,"variable "," {");
            allnode.push_back(currentnode);
        }
        if(strTemp.find("probability") != string::npos){
            break;
        }
    }
    for(int i=0;i<allnode.size();i++){
        allchildren.clear();
        parents = numberdirectparents(network,allnode[i]);
        numberallparents = numberallparents + numberdirectparents(network,allnode[i]);
        allchildren = directchildren(network,allnode[i]);
        numberallchildren = numberallchildren + allchildren.size();
        for(int j=0;j<allchildren.size();j++){
            other = other + numberdirectparents(network,allchildren[j]);
        }
    }
    int numberofnode = allnode.size();
    int s = numberallparents + numberallchildren + other;
    double result = s;
    return numberallparents;
}


int AVB2(string network){
    ifstream filein(network);
    string strTemp;
    vector<string> allnode;
    string currentnode;
    int numberallchildren = 0;
    int numberallparents = 0;
    int other = 0;
    int a = 0;
    int single = 0;
    vector<string> allchildren;
    while(getline(filein,strTemp)){
        if(strTemp.find("variable") != string::npos){
            currentnode = get_str_between_two_str(strTemp,"variable "," {");
            allnode.push_back(currentnode);
        }
        if(strTemp.find("probability") != string::npos){
            break;
        }
    }
    for(int i=0;i<allnode.size();i++){
        single = numberdirectparents(network,allnode[i]);
        numberallparents = single + numberallparents;
    }
    for(int i=0;i<allnode.size();i++){
        allchildren.clear();
        allchildren = directchildren(network,allnode[i]);
        numberallchildren += allchildren.size();
        for(int j=0;j<allchildren.size();j++){
            a = numberdirectparents(network,allchildren[j]);
            other = other + a -1;
        }
    }
    return numberallchildren + other + numberallparents;
}

int AD(string network){
    ifstream filein(network);
    string strTemp;
    vector<string> allnode;
    string currentnode;
    int numberallchildren = 0;
    int numberallparents = 0;
    int other = 0;
    int a = 0;
    int single = 0;
    vector<string> allchildren;
    while(getline(filein,strTemp)){
        if(strTemp.find("variable") != string::npos){
            currentnode = get_str_between_two_str(strTemp,"variable "," {");
            allnode.push_back(currentnode);
        }
        if(strTemp.find("probability") != string::npos){
            break;
        }
    }
    for(int i=0;i<allnode.size();i++){
        single = numberdirectparents(network,allnode[i]);
        numberallparents = single + numberallparents;
    }
    for(int i=0;i<allnode.size();i++){
        allchildren.clear();
        allchildren = directchildren(network,allnode[i]);
        numberallchildren += allchildren.size();
    }
    return numberallchildren +  numberallparents;
}



