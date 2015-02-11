#ifndef CLAKECOMPLETION_H
#define CLAKECOMPLETION_H

#include <vector>
#include <map>
#include "structs.h"
#include "Rule.h"
#include "Utils.h"

using namespace std;
class ClakeCompletion {
private:
    map< int, vector<Rule> > ipf_atoms_rules;
    vector<int> no_ipf_atoms;
    vector<Rule> constrants;
public:
    ClakeCompletion(const vector<Rule>& nlp);
    ~ClakeCompletion();
    vector<_formula*> convert();
    _formula* convertSyntaxTree();
    void test();
    void testCompletion();
};
#endif