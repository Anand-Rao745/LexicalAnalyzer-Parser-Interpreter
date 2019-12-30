#ifndef VAL_H
#define VAL_H

#include <string>
#include <bits/stdc++.h>
using namespace std;

class Val {
  int        i;
  string    s;
  enum ValType { ISINT, ISSTR, ISERR } vt;

public:
  Val() : i(0), vt(ISERR) {}
  Val(int i) : i(i), vt(ISINT) {}
  Val(string s) : i(0), s(s), vt(ISSTR) {}
  Val(ValType vt, string errmsg) : i(0), s(errmsg), vt(ISERR) {}

  ValType getVt() const { return vt; }

  bool isErr() const { return vt == ISERR; }
  bool isInt() const { return vt == ISINT; }
  bool isStr() const { return vt == ISSTR; }

  int ValInt() const {
    if (isInt()) return i;
    throw "This Val is not an Int";
  }
  string ValString() const {
    if (isStr()) return s;
    throw "This Val is not a Str";
  }

  friend ostream& operator<<(ostream& out, const Val& v) {
    if (v.isInt()) {
      out << v.ValInt();
    }
    else if (v.isStr()) {
      out << v.ValString();
    }
    return out;

  }

  string GetErrMsg() const {
    if (isErr()) return s;
    throw "This Val is not an Error";
  }



  Val operator+(const Val& op) const {
    if (isInt() && op.isInt())
      return ValInt() + op.ValInt();
    if (isStr() && op.isStr())
      return ValString() + op.ValString();
    return Val(ISERR, "Type mismatch on operands of +");
  }

  Val operator-(const Val& op) const {
    if (isInt() && op.isInt()) {
      return ValInt() - op.ValInt();
    }
    if (isStr() && op.isStr()) {
      string original = ValString();
      string sub = op.ValString();
      auto it = original.find(sub);
      original.replace(it, sub.length(), "");
      return original;
    }
    return Val(ISERR, "Type mismatch on operands of -");
  }

  Val operator*(const Val& op) const {
    if (isInt() && op.isInt()) {
      return ValInt() * op.ValInt();
    }
    if ((isInt() && op.isStr())) {
      if (ValInt() < 0) {
        return Val(ISERR, "Cannot multiply string by negative int");
      }
      string mult;
      for (int i = 0; i < ValInt(); i++) {
        mult += op.ValString();
      }
      return mult;
    }
    if ((isStr() && op.isInt())) {
      if (op.ValInt() < 0) {
        return Val(ISERR, "Cannot multiply string by negative int");
      }
      string mult;
      for (int i = 0; i < op.ValInt(); i++) {
        mult += ValString();
      }
      return mult;
    }
    return Val(ISERR, "Type mismatch on operands of *");

  }

  Val operator/(const Val& op) const {
    if (op.ValInt() == 0) {
      return Val(ISERR, "Divide by zero error");
    }
    else if (isInt() && op.isInt()) {
      return ValInt() / op.ValInt();
    }
    return Val(ISERR, "Type mismatch on operands of /");

  }

  Val operator!() const {
    if (isInt()) {
      int number = ValInt();
      int reverse = 0;
      bool isnegative = false;
      if (number < 0) {
        number *= -1;
        isnegative = true;
      }
      while (number > 0) {
        reverse = reverse * 10 + number % 10;
        number = number / 10;
      }
      if (isnegative) {
        reverse *= -1;
      }
      return reverse;
    }
    if (isStr()) {
      string temp = ValString();
      reverse(temp.begin(), temp.end());
      return temp;
    }
    return Val(ISERR, "Type mismatch on operands of !");
  }

};

#endif
