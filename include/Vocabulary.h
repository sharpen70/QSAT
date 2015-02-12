#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "structs.h"
#include <cstdio>
#include <vector>
#include <string>

using namespace std;
/*
 * 字符表类，保存各种信息
 */
class Vocabulary {
private:
    int atom_id;
    map<string, int> atom_map;
    map<int, string> map_atom;
    vector<char*> atom_list;
    vector<int> sel_atoms;
    int auxiliary;
private:
    Vocabulary();
    Vocabulary(const Vocabulary&);
    Vocabulary& operator =(const Vocabulary&);
    ~Vocabulary();
    
public:
    static Vocabulary& instance();
    void dumpVocabulary(FILE* _out);
    int addMapAtom(char*);
    int addAtom(char*);
    int addSelAtom(char*);
    int addMapSelAtom(char*);
    vector<int>& getSelAtoms();
    int apSize();
    char* getAtom(int id);
    const char* getMapAtom(int id);
    int queryAtom(char*);
    int queryMapAtom(char*);
    int newAux(int);
};

#endif
