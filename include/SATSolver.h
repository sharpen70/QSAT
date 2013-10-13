/* 
 * File:   SATSolver.h
 * Author: sharpen
 *
 * Created on September 4, 2013, 9:04 PM
 */

#ifndef SATSOLVER_H
#define	SATSOLVER_H

#include <vector>
#include "Solver.h"

using namespace std;

class SATSolver {
public:
    SATSolver(vector< set<int> > cnf, int num_lits);
    SATSolver(const SATSolver& orig);
    ~SATSolver();
    
//    void invokeSAT();
    int invokeSAT();
    void outputResult();
    vector< set<int> > models;
    
private:
    Solver sat; 
    int num_lits_in_NLP;
    bool isExistModel();
};

#endif	/* SATSOLVER_H */

