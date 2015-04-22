#ifndef _INI_H
#define _INI_H

#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <cstring>
#include <string>
#include <strings.h>

#define MAXLINE 512

using namespace std;

class Ini {
public:
    Ini():if_loaded(false){
    };
    ~Ini(){
    };
    int LoadFile(const string &);
    int GetIntKey(const string &, const string &, int &);
    int GetIntKey(const string &, const string &, vector <int> &);
    int GetFloatKey(const string &, const string &, double &);
    int GetFloatKey(const string &, const string &, vector <double> &);
    int GetStringKey(const string &, const string &, string &);
    int GetStringKey(const string &, const string &, vector <string> &);
    int GetBoolKey(const string &, const string &, bool &);
    int GetBoolKey(const string &, const string &, vector <bool> &);
    size_t GetSectionSize(const string &);
    void DumpAll();
    void ClearAll();

private:
    map <string, multimap <string, string> > sections;   
    bool if_loaded;
};

#endif
