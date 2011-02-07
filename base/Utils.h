#pragma once
#include "definitions.h"

void split(const std::string& s,tStringList& tokens,const char* separators=",");
bool trim(std::string& s,const char* separators=" \"\t\r\n");
std::string trimmed(const std::string& s,const char* separators=" \"\t\r\n");
std::string lowercased(const std::string& s);
int toint(const std::string& s);
number_t tonumber(const std::string& s);
point2d_t topoint(const std::string& s);
std::string tostring(int i);
std::string tostring(number_t n);
std::string tostring(point2d_t p);
std::string constructPropertyAssignement(const std::string& name, const std::string& value);
bool parsePropertyAssignement(const std::string& s,std::string& name,std::string& value);
std::string constructPropertyList(const tStringList& pl);
tStringList parsePropertyList(const std::string& s);
std::string loadFile(const std::string& filename);
tStringList loadFileLines(const std::string& filename);
image_t loadImage(const std::string& filename);
std::string addDataPath(const std::string& filename);




