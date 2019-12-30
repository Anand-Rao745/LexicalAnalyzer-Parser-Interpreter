/*
 * parse.h
 */

#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>
using namespace std;

#include "lex.h"
#include "parsetreeSKEL.h"

extern ParseTree *Prog(istream& in, int& line);
extern ParseTree *Slist(istream& in, int& line);
extern ParseTree *Stmt(istream& in, int& line);
extern ParseTree *IfStmt(istream& in, int& line);
extern ParseTree *LetStmt(istream& in, int& line);
extern ParseTree *PrintStmt(istream& in, int& line);
extern ParseTree *LoopStmt(istream& in, int& line);
extern ParseTree *Expr(istream& in, int& line);
extern ParseTree *Prod(istream& in, int& line);
extern ParseTree *Rev(istream& in, int& line);
extern ParseTree *Primary(istream& in, int& line);

#endif /* PARSE_H_ */
