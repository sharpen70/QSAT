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


    
//    ifstream infile("res/input/IOFile.in");
//    string text, in, out;
//    
//    for(int aspr = 0; aspr < aspResult.size(); aspr++) {
//        getline(infile, text);
//        istringstream inf(text);
//        inf >> in >> out;
//        
//        io(in.c_str(), out.c_str());
        yyin = fopen("res/input/hc1gr.in", "r");
        yyparse();
        fclose(yyin);
    
        for(int i = 0; i < G_NLP.size(); i++) {
            G_NLP.at(i).output(stdout);
        }
        printf("\n");
        
        //    Vocabulary::instance().dumpVocabulary(stdout);

        //--------------------------------------------------------------------------
        //Completion  
        ClakeCompletion::instance().setDlp(G_NLP);
        vector<_formula*> completion = ClakeCompletion::instance().convert();
        vector< set<int> > input = Utils::convertToSATInput(completion);
//        //ClakeCompletion::instance().testCompletion();
//        printf("%d\n", Vocabulary::instance().apSize());
////        for(vector< set<int> >::iterator it = input.begin(); it != input.end(); it++) {
////            for(set<int>::iterator t = it->begin(); t != it->end(); t++) {
////                printf("%d ", *t);
////            }
////            printf("\n");
////        }
//        printf("voc size %d\n", Vocabulary::instance().apSize());
        SATSolver sat(input, Vocabulary::instance().apSize());
        sat.invokeSAT();
        printf("Models: %d", sat.models.size());
        sat.outputResult();
        
        DependenceGraph dpg(G_NLP);
        dpg.operateGraph();
        vector<int> k = dpg.getESRSizes();
              
        //LoopFormulas from k = 0 to N
//        vector< set<int> > input = Utils::convertToSATInput(completion);
        for(vector<int>::iterator kit = k.begin(); kit != k.end(); kit++) {
            vector<Loop> loops = dpg.getLoopWithESRuleSize(*kit);
            printf("size:%d\n", loops.size());
            for(vector<Loop>::iterator it = loops.begin(); it != loops.end(); it++) {
                vector<_formula*> lfs = dpg.computeLoopFormulas(*it);
                
                for(vector<_formula*>::iterator ilfs = lfs.begin(); ilfs != lfs.end(); ilfs++) {
                    Utils::formulaOutput(stdout, *ilfs);
                    printf("\n");
                    fflush(stdout);
                    set<int>  lits;
                    Utils::convertCNFformulaToLits(*ilfs, lits);
                    input.push_back(lits);
                }            
            }
            fflush(stdout);
            
//           // vector<_formula*> input = Utils::joinFormulas(completion, addLoopFormulas);
//
//            //CNF before the sat input
////            vector<_formula*> input;
////            for(vector<_formula*>::iterator it = _input.begin(); it != _input.end(); it++) {
////                vector<_formula*> tmp = CNFUtils::convertCNF(*it);
////                input = Utils::joinFormulas(input, tmp);
////            }
//
//            //Use SAT to compute the models
//            //vector< set<int> > satin = Utils::convertToSATInput(completion);
//
//            printf("voc size %d\n", Vocabulary::instance().apSize());
            SATSolver sats(input, Vocabulary::instance().apSize());
            sats.invokeSAT();
            printf("Models: %d", sats.models.size());
            //sats.outputResult();
//
//            //Compare with ASP solver
//            if(sats.models.size() <= 2) {
//                printf("\nThe k is %d\n\n", *kit);
//                break;
//            }
////            if(Utils::compareAnswerSet(aspResult, sats.models)) {
////                printf("\nThe k is %d\n\n", *kit);
////                break;
////            }
       }
//       
//       G_NLP.clear();   
    //}
    
   // infile.close();
    
    return 0;
}