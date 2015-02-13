#include "Vocabulary.h"
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <assert.h>

Vocabulary& Vocabulary::instance() {
  static Vocabulary vocabulary;
  return vocabulary;
}

Vocabulary::Vocabulary() : atom_id(1),auxiliary(1) {
  atom_list.clear();
}

Vocabulary::~Vocabulary() {
  atom_list.clear();
}


int Vocabulary::addAtom(char* atom) {
  int id = queryAtom(atom);
  
  if(id > 0) {
    return id;
  }
  else {
    atom_list.push_back(atom);
    auxiliary = atom_list.size() + 1;
    return atom_id++;
  }
}

int Vocabulary::addSelAtom(char* atom) {
  int id = queryAtom(atom);
  
  if(id > 0) {
    return id;
  }
  else {
    atom_list.push_back(atom);
    id = atom_id++;
    auxiliary = atom_list.size() + 1;
    sel_atoms.push_back(id);
    
    return id;
  }
}

int Vocabulary::addMapAtom(char* atom) {
  int id = queryMapAtom(atom);
  
  if(id > 0) return id;
  else {
    string s(atom);
    id = this->atom_map.size() + 1;
    this->atom_map.insert(pair<string, int>(s, id));
//    this->map_atom.insert(pair<int, string>(id, s));
    auxiliary = id + 1;
    
    return id;
  }
}

int Vocabulary::addMapSelAtom(char* atom) {
  int id = queryMapAtom(atom);
  
  if(id > 0) {
    return id;
  }
  else {
    string s(atom);
    id = this->atom_map.size() + 1;
    this->atom_map.insert(pair<string, int>(s, id));
//    this->map_atom.insert(pair<int, string>(id, s));
    auxiliary = id + 1;
    sel_atoms.push_back(id);
    
    return id;
  }
}

vector<int>& Vocabulary::getSelAtoms() {
  return sel_atoms;
}

int Vocabulary::queryAtom(char* atom) {
  for(int i = 0; i < atom_list.size(); i++) {
    if(strcmp(atom, atom_list.at(i)) == 0) {
      return i + 1;
    }
  }
  
  return -1;
}

int Vocabulary::queryMapAtom(char* atom) {
  string s(atom);
  map<string, int>::iterator it = this->atom_map.find(s);
  if(it == this->atom_map.end()) return -1;
  else return it->second;
}

void Vocabulary::dumpVocabulary(FILE* _out) {
  for(int i = 0; i < atom_list.size(); i++) {
    fprintf(_out, "%s\n", atom_list.at(i));
  }
}

char* Vocabulary::getAtom(int id) {
  return atom_list.at(id - 1);
}

const char* Vocabulary::getMapAtom(int id) {
  return map_atom[id].c_str();
}

int Vocabulary::apSize() {
  return atom_map.size();
}

int Vocabulary::newAux(int num) {
  int t = auxiliary;
  auxiliary += num;
  return t;
}