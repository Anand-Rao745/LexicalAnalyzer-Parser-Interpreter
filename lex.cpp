#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <bits/stdc++.h>
#include "lex.h"
using namespace std;

map<string, Token>::iterator it;
static map<string, Token> keyWords = {
	{"print", PRINT},
	{"let", LET},
	{"if", IF},
	{"loop", LOOP},
	{"begin", BEGIN},
	{"end", END}

};

extern ostream& operator<<(ostream& out, const Lex& tok) {
	map<Token, string> printMap = {
	{PRINT, "PRINT"},
	{LET, "LET"},
	{IF, "IF"},
	{LOOP, "LOOP"},
	{BEGIN, "BEGIN"},
	{END, "END"},
	{ID, "ID"},
	{INT, "INT"},
	{STR,"STR"},
	{PLUS, "PLUS"},
	{MINUS,"MINUS"},
	{STAR, "STAR"},
	{SLASH,"SLASH"},
	{BANG, "BANG"},
	{LPAREN, "LPAREN"},
	{RPAREN, "RPAREN"},
	{SC, "SC"}
	};

	Token t = tok.GetToken();
	if (t == ID || t == INT || t == STR || t == ERR) {
		out << printMap.find(t)->second << "(" << tok.GetLexeme() << ")";
		return out;
	}
	out << printMap[t];
	return out;
}

Lex getNextToken(istream& in, int& linenum) {
	string lexeme;
	char ch;
	bool begin = true, op = false, inid = false, instring = false, inint = false, forwardslash = false, backslash = false, incomment = false;
	while (in.get(ch)) {
		if (begin) {
			if (ch == '\n') {
				linenum++;
			}
			if (isspace(ch)) {
				continue;
			}
			if (ch == '/') {
				forwardslash = true;
				begin = false;
				continue;
			}

			lexeme += ch;

			if (isalpha(ch)) {
				inid = true;
				begin = false;
				continue;
			}
			else if (ch == '"') {
				instring = true;
				begin = false;
				continue;
			}
			else if (isdigit(ch)) {
				inint = true;
				begin = false;
				continue;
			}
			else {
				op = true;
				begin = false;
			}
		}

		if (op) {
			Token t = ERR;
			if (ch == '+') {
				t = PLUS;
			}
			if (ch == '-') {
				t = MINUS;
			}
			if (ch == '*') {
				t = STAR;
			}
			if (ch == '!') {
				t = BANG;
			}
			if (ch == '(') {
				t = LPAREN;
			}
			if (ch == ')') {
				t = RPAREN;
			}
			if (ch == ';') {
				t = SC;
			}
			return Lex(t, lexeme, linenum);
		}


		if (inid) {
			if (isalpha(ch) || isdigit(ch)) {
				lexeme += ch;
			}
			else {
				in.putback(ch);
				Token t = ID;
				for (it = keyWords.begin(); it != keyWords.end(); it++) {
					if (it->first == lexeme) {
						t = it->second;
					}
				}
				return Lex(t, lexeme, linenum);
			}
		}

		if (instring) {
			if (ch == '\n') {
				lexeme += ch;
				return Lex(ERR, lexeme, linenum);
			}
			else if (ch == '\\') {
				backslash = true;
				instring = false;
				continue;
			}
			else if (ch == '"') {
				lexeme.erase(0, 1);
				return Lex(STR, lexeme, linenum);
			}
			else {
				lexeme += ch;
			}
		}

		if (inint) {
			if (isdigit(ch)) {
				lexeme += ch;
			}
			else if (isalpha(ch)) {
				lexeme += ch;
				return Lex(ERR, lexeme, linenum);
			}
			else {
				in.putback(ch);
				return Lex(INT, lexeme, linenum);
			}
		}

		if (forwardslash) {
			if (ch != '/') {
				in.putback(ch);
				return Lex(SLASH, lexeme, linenum);
			}
			else {
				incomment = true;
				forwardslash = false;
				continue;
			}
		}

		if (backslash) {
			if (ch == 'n') {
				lexeme += '\n';
			}
			else {
				lexeme += ch;
				instring = true;
				backslash = false;
				continue;
			}
		}

		if (incomment) {
			if (ch == '\n') {
				begin = true;
				incomment = false;
				linenum++;
			}
			continue;
		}
	}

	if (in.eof()) {
		return Lex(DONE, lexeme, linenum);
	}

	return Lex(DONE, lexeme, linenum);
}




