/****************************************************************************************[Dimacs.h]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef Glucose_Dimacs_h
#define Glucose_Dimacs_h

#include <stdio.h>

#include "utils/ParseUtils.h"
#include "core/SolverTypes.h"

namespace Glucose {

//=================================================================================================
// DIMACS Parser:

template<class B, class Solver>
static void readEquation(B& in, Solver& S, vec<Lit>& mons, bool &cst) {
    int parsed_lit, var, sz_mon;
    vec<Lit> lits;
    cst = false;
    mons.clear();
    if (!eagerMatch(in, "x")) {     // An equation must begin by 'x'
        printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
    }
    for (;;) {
        skipWhitespace(in);
        if (*in == '.') {           // We have a new monomial (non unary)
            ++in;
            sz_mon = parseInt(in);
            lits.clear();
            for (int i = 0; i < sz_mon; ++i) {      // Get the variables present in the monomial
                parsed_lit = parseInt(in);
                assert(parsed_lit != 0);
                var = abs(parsed_lit) - 1;
                for (; var >= S.nVars(); S.newVar());
                lits.push((parsed_lit > 0) ? mkLit(var) : ~mkLit(var));
            }
            S.addMonomial_(lits, mons, cst);
        }
        else if (*in == 'T') {      // We have found the constant
            cst = !cst;
            ++in;
        } else {
            parsed_lit = parseInt(in);
            if (parsed_lit == 0) {  // We have found a '0', we have read the complete equation
                break;
            }
            // We have a unary monomial
            lits.clear();
            var = abs(parsed_lit) - 1;
            for (; var >= S.nVars(); S.newVar());
            lits.push((parsed_lit > 0) ? mkLit(var) : ~mkLit(var));
            S.addMonomial_(lits, mons, cst);
        }
    }
}

template<class B, class Solver>
static void parse_DIMACS_main(B& in, Solver& S) {
    vec<Lit> mons;
    int vars    = 0;
    int equations = 0;
    int cnt     = 0;
    bool cst;
    for (;;){
        skipWhitespace(in);
        if (*in == EOF) break;
        else if (*in == 'p'){   // Read the prefix
            if (eagerMatch(in, "p cnf")){
                vars    = parseInt(in);
                equations = parseInt(in);
                // SATRACE'06 hack
                // if (clauses > 4000000)
                //     S.eliminate(true);
            }else{
                printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
            }
        } else if (*in == 'c' || *in == 'p') {  // Skip comments
            skipLine(in);
        } else{     // Read the next equation
            cnt++;
            readEquation(in, S, mons, cst);
            S.addEquation_(mons, cst);
        }
    }
    // Check values of the prefix
    if (vars != S.nVars())
        fprintf(stderr, "WARNING! DIMACS header mismatch: wrong number of variables.\n");
    if (cnt  != equations)
        fprintf(stderr, "WARNING! DIMACS header mismatch: wrong number of clauses.\n");
}

// Inserts problem into solver.
//
template<class Solver>
static void parse_DIMACS(gzFile input_stream, Solver& S) {
    StreamBuffer in(input_stream);
    parse_DIMACS_main(in, S); }

//=================================================================================================
}

#endif
