#include "Utils.h"
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "UtilsAirplay.h"
using namespace std;


void 
split(const std::string& s,tStringList& tokens,const char* separators/*=","*/){
    std::string::size_type prevsep_pos = 0u;
    std::string::size_type sep_pos = s.find_first_of(separators);
    while(sep_pos != s.npos){
        tokens.push_back(s.substr(prevsep_pos,sep_pos-prevsep_pos));
        prevsep_pos = sep_pos+1;
        sep_pos = s.find_first_of(separators,prevsep_pos);
    }
    if(prevsep_pos<s.size()){
        tokens.push_back(s.substr(prevsep_pos));
    }
}

bool 
trim(std::string& s,const char* separators/*=" \"\t\r\n"*/){
    //Eliminamos los espacios en blanco que hay al principio y al final del string.
    string::size_type spcbeg = s.find_first_not_of(separators);
    if(spcbeg==string::npos){
        s =""; //MALF para quehaga su trabajo aunque no tenga sentido
        return false;
    }
    s.erase(0,spcbeg);
    string::size_type spcend = s.find_last_not_of(separators);
    s.erase(spcend+1,string::npos);
    return true;
}

std::string 
trimmed(const std::string& s,const char* separators/*=" \"\t\r\n"*/){
    std::string r(s);
    trim(r);
    return r;
}

std::string 
lowercased(const std::string& s){
    std::string result;
    std::transform(s.begin(), s.end(), std::back_inserter(result), std::tolower);
    return result;        

}

int 
toint(const std::string& s){
    return atoi(s.c_str());
}

number_t 
tonumber(const std::string& s){
    return atof(s.c_str());
}

point2d_t 
topoint(const std::string& s){
    tStringList v;
    split(s,v);
    assert(v.size()==2);
    return point2d_t(tonumber(v[0]),tonumber(v[1]));
}

std::string 
tostring(int i){
    static char buff[200];
    sprintf(buff,"%d",i);
    return buff;
}

std::string 
tostring(number_t n){
    static char buff[200];
    sprintf(buff,"%f",double(n));
    return buff;
}

std::string 
tostring(point2d_t p){
    return tostring(p.x) + "," + tostring(p.y);
}

std::string 
constructPropertyAssignement(const std::string& name, const std::string& value){
    return name + "=" + value;
}

bool 
parsePropertyAssignement(const std::string& s,std::string& name,std::string& value){
    tStringList tokens;
    split(s,tokens,"=");
    if(tokens.size()!=2) return false;
    name = trimmed(tokens[0]);
    value = trimmed(tokens[1]);
    return true;
}

std::string 
constructPropertyList(const tStringList& pl){
    std::string result;
    if(pl.empty()) return result;
    result += pl[0];
    for(size_t i = 1;i<pl.size();++i) result += std::string(";") + pl[i];
    return result;
}

tStringList 
parsePropertyList(const std::string& s){
    tStringList tokens;
    split(s,tokens,";");
    return tokens;
}

std::string 
loadFile(const std::string& filename){
//     QFile file(filename.c_str());
//     file.open(QIODevice::ReadOnly);
//     if(file.error()!=QFile::NoError) return "";
//     QTextStream str(&file);
//     return qstring2string(str.readAll());
    return std::string();
}

tStringList 
loadFileLines(const std::string& filename){
    tStringList lines;
    split(loadFile(filename),lines,"\n");
    return lines;
}

image_t 
loadImage(const std::string& filename){
    return image_t();//QImage(addDataPath(filename).c_str());
}

std::string 
addDataPath(const std::string& filename){
    return std::string(DATA_PATH)+"/"+filename;
}