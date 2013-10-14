/* 
 * File:   main.cpp
 * Author: yzf
 *
 * Created on July 10, 2013, 2:17 PM
 */

#include <cstdlib>
#include <cstdio>
#include <assert.h>
#include "Vocabulary.h"
#include "structs.h"
#include "ClakeCompletion.h"
#include "SATSolver.h"
#include "DependenceGraph.h"
#include "CNFUtils.h"
#include "GLTranslator.h"
#include <set>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

FILE* fout;
extern FILE* yyin;
extern vector<Rule> G_NLP;
extern int yyparse();

void io(const char* iPathName, const char* oPathName) {
    yyin = fopen (iPathName, "r");
    fout = fopen (oPathName, "w+");

    if (! yyin) {
        printf("IO Error: cannot open the input file.\n" );
        assert(0);
    }
    if (! fout) {
        printf("IO Error: cannot open the output file.\n");
        assert(0);
    }
}

int main(int argc, char** argv) {
    
//    if(argc < 3) {
//        io("res/input/sample.in","res/output/sample.out");
//    }
//    else {
//        io(argv[1], argv[2]);
//    }
//    
//    yyparse();
//    fclose(yyin);
    
    yyin = fopen("res/input/sample.in", "r");
    yyparse();
    fclose(yyin);

    for(int i = 0; i < G_NLP.size(); i++) {
        G_NLP.at(i).output(stdout);
    }
    printf("\n");

    ClakeCompletion::instance().setDlp(G_NLP);
    vector<_formula*> completion = ClakeCompletion::instance().convert();
    vector< set<int> > input = Utils::convertToSATInput(completion);
    
    SATSolver sat(input, Vocabulary::instance().apSize());
    sat.invokeSAT();
    printf("Models: %d\n", sat.models.size());
    sat.outputResult();
    
    for(vector< set<int> >::iterator it = sat.models.begin(); it != sat.models.end(); it++) {
        GLTranslator glt(G_NLP);
        if(glt.isAnswerSet(*it)) printf("Is answer set\n");
        else printf("Not answer set\n");
    }
//    DependenceGraph dpg(G_NLP);
//    dpg.operateGraph();
//    dpg.printfLoop();
//    vector<int> k = dpg.getESRSizes();
//    
//    for(vector<int>::iterator kit = k.begin(); kit != k.end(); kit++) {
//        vector<Loop> loops = dpg.getLoopWithESRuleSize(*kit);
//        for(vector<Loop>::iterator it = loops.begin(); it != loops.end(); it++) {
//            vector<_formula*> lfs = dpg.computeLoopFormulas(*it);
//
//            for(vector<_formula*>::iterator ilfs = lfs.begin(); ilfs != lfs.end(); ilfs++) {
//                set<int>  lits;
//                Utils::convertCNFformulaToLits(*ilfs, lits);
//                input.push_back(lits);
//            }            
//        }
//
//        SATSolver sats(input, Vocabulary::instance().apSize());
//        sats.invokeSAT();
//        printf("Models: %d\n", sats.models.size());
//    }
    
    return 0;
}