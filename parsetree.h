/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
#include "valSKEL.cpp"
using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };

// a "forward declaration" for a class to hold values
class Value;

class ParseTree {
public:
  int   linenum;
  ParseTree* left;
  ParseTree* right;

public:
  ParseTree(int linenum, ParseTree* l = 0, ParseTree * r = 0)
    : linenum(linenum), left(l), right(r) {}

  virtual ~ParseTree() {
    delete left;
    delete right;
  }

  int GetLineNumber() const { return linenum; }

  int Traverse(int input, int (ParseTree::* func)(void)) {
    if (left) input += left->Traverse(input, func);
    if (right) input += right->Traverse(input, func);
    return Visit(input, func);
  }

  virtual int Visit(int input, int (ParseTree::* func)(void)) {
    return input;
  }

  int MaxDepth() const {
    int leftcount = 0;
    int rightcount = 0;
    if (left) {
      leftcount = left->MaxDepth();
    }
    if (right) {
      rightcount += right->MaxDepth();
    }
    if (leftcount > rightcount) {
      return leftcount + 1;
    }
    return rightcount + 1;
  }

  virtual bool IsIdent() const { return false; }
  virtual bool IsVar() const { return false; }
  virtual string GetId() const { return ""; }
  virtual int IsBang() const { return 0; }
  virtual bool IsLet() const { return false; }

  int BangCount() const {
    int bangcount = 0;
    if (left) {
      bangcount += left->BangCount();
    }
    if (right) {
      bangcount += right->BangCount();
    }
    bangcount += IsBang();
    return bangcount;
  }

  virtual void CheckLetBeforeUse(map<string, bool>& var) {
    int count = 0;
    if (left) {
      count += left->CheckLetBeforeUse(var);
    }
    if (right) {
      count += right->CheckLetBeforeUse(var);
    }
    if (isLet()) {
      var[this->GetId()] = true;
    }
    if (IsIdent() && var.find(GetId()) == var.end()) {
      cout << "UNDECLARED VARIABLE " << GetId() << endl;
      count++;
    }
    return count;
  }

  //virtual Value Eval();
  virtual Val Eval(map<string, Val>& symbols) { return Val(); };

  string runtime_err(int linenum, string message) {
    string error = "RUNTIME ERROR at ";
    error += to_string(linenum);
    error += ": ";
    error += message;
    throw error;
  }

};

class StmtList : public ParseTree {

public:
  StmtList(ParseTree* l, ParseTree* r) : ParseTree(0, l, r) {}

  Val Eval(map<string, Val>& symbols) {
    left->Eval(symbols);
    if (right)
      right->Eval(symbols);
    return Val();
  }
};

class Let : public ParseTree {
  string id;

public:
  Let(Lex& t, ParseTree* e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {}
  Val Eval(map<string, Val>& symbols) {
    auto L = left->Eval(symbols);
    auto it = symbols.find(id);
    if (it != symbols.end()) {
      it->second = L;
    }
    else {
      symbols[id] = L;
    }
    return L;
  }

  string GetId() const { return id; }
  bool IsLet() const { return true; }
};

class Print : public ParseTree {
public:
  Print(int line, ParseTree* e) : ParseTree(line, e) {}
  Val Eval(map<string, Val>& symbols) {
    auto L = left->Eval(symbols);
    cout << L;
    return Val();
  }
};

class Loop : public ParseTree {

public:
  Loop(int line, ParseTree* e, ParseTree* s) : ParseTree(line, e, s) {}
  Val Eval(map<string, Val>& symbols) {
    while (true) {
      auto L = left->Eval(symbols);
      if (L.isInt()) {
        if (L.ValInt() == 0) {
          break;
        }
        else {
          auto R = right->Eval(symbols);
        }
      }
      else {
        runtime_err(linenum, "Expression is not an integer");

      }
    }
    return Val();
  }
};

class If : public ParseTree {

public:
  If(int line, ParseTree* e, ParseTree* s) : ParseTree(line, e, s) {}
  Val Eval(map<string, Val>& symbols) {
    auto L = left->Eval(symbols);
    if (L.isInt()) {
      if (L.ValInt() != 0) {
        auto R = right->Eval(symbols);
      }
    }
    else {
      runtime_err(linenum, "Expression is not an integer");
    }
    return Val();
  }
};


class PlusExpr : public ParseTree {
public:
  PlusExpr(int line, ParseTree* l, ParseTree* r) : ParseTree(line, l, r) {}
  Val Eval(map<string, Val>& symbols) {
    auto L = left->Eval(symbols);
    if (L.isErr())
      runtime_err(linenum, L.GetErrMsg());
    auto R = right->Eval(symbols);
    if (R.isErr())
      runtime_err(linenum, R.GetErrMsg());

    auto answer = L + R;
    if (answer.isErr())
      runtime_err(linenum, answer.GetErrMsg());

    return answer;
  }
};

class MinusExpr : public ParseTree {
public:
  MinusExpr(int line, ParseTree* l, ParseTree* r) : ParseTree(line, l, r) {}
  Val Eval(map<string, Val>& symbols) {
    auto L = left->Eval(symbols);
    if (L.isErr())
      runtime_err(linenum, L.GetErrMsg());
    auto R = right->Eval(symbols);
    if (R.isErr())
      runtime_err(linenum, R.GetErrMsg());

    auto answer = L - R;
    if (answer.isErr())
      runtime_err(linenum, answer.GetErrMsg());
    return answer;
  }
};

class TimesExpr : public ParseTree {
public:
  TimesExpr(int line, ParseTree* l, ParseTree* r) : ParseTree(line, l, r) {}
  Val Eval(map<string, Val>& symbols) {
    auto L = left->Eval(symbols);
    if (L.isErr())
      runtime_err(linenum, L.GetErrMsg());
    auto R = right->Eval(symbols);
    if (R.isErr())
      runtime_err(linenum, R.GetErrMsg());

    auto answer = L * R;
    if (answer.isErr())
      runtime_err(linenum, answer.GetErrMsg());

    return answer;
  }
};

class DivideExpr : public ParseTree {
public:
  DivideExpr(int line, ParseTree* l, ParseTree* r) : ParseTree(line, l, r) {}
  Val Eval(map<string, Val>& symbols) {
    auto L = left->Eval(symbols);
    if (L.isErr())
      runtime_err(linenum, L.GetErrMsg());
    auto R = right->Eval(symbols);
    if (R.isErr())
      runtime_err(linenum, R.GetErrMsg());

    auto answer = L / R;
    if (answer.isErr())
      runtime_err(linenum, answer.GetErrMsg());

    return answer;
  }
};

class BangExpr : public ParseTree {
public:
  BangExpr(int line, ParseTree* l) : ParseTree(line, l) {}
  virtual int IsBang() const { return 1; }
  Val Eval(map<string, Val>& symbols) {
    auto L = left->Eval(symbols);
    return !L;
  }
};

class IConst : public ParseTree {
  int val;

public:
  IConst(Lex& t) : ParseTree(t.GetLinenum()) {
    val = stoi(t.GetLexeme());
  }
  Val Eval(map<string, Val>& symbols) {
    return Val(val);
  }
};

class SConst : public ParseTree {
  string val;

public:
  SConst(Lex& t) : ParseTree(t.GetLinenum()) {
    val = t.GetLexeme();
  }

  Val Eval(map<string, Val>& symbols) {
    return Val(val);
  }
};

class Ident : public ParseTree {
  string id;

public:
  Ident(Lex& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}
  Val Eval(map<string, Val>& symbols) {
    return symbols[id];
  }

  bool IsIdent() const { return true; }
  string GetId() const { return id; }
};

#endif /* PARSETREE_H_ */
