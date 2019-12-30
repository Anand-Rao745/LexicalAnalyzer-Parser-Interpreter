/*
 * lex.h
 *
 * CS280
 * Fall 2019
 */

#ifndef LEX_H_
#define LEX_H_

#include <string>
#include <iostream>
using std::string;
using std::istream;
using std::ostream;

enum Token {
		// keywords
	PRINT, LET, IF, LOOP, BEGIN, END,

		// an identifier
	ID,

		// an integer and string constant
	INT, STR,

		// the operators, parens, semicolon
	PLUS, MINUS, STAR, SLASH, BANG, LPAREN, RPAREN, SC,
		// any error returns this token
	ERR,

		// when completed (EOF), return this token
	DONE
};

class Lex {
	Token	tok;
	string	lexeme;
	int		lnum;

public:
	Lex() {
		tok = ERR;
		lnum = -1;
	}
	Lex(Token tok, string lexeme, int line) {
		this->tok = tok;
		this->lexeme = lexeme;
		this->lnum = line;
	}

	bool operator==(const Token tok) const { return this->tok == tok; }
	bool operator!=(const Token tok) const { return this->tok != tok; }

	Token	GetToken() const { return tok; }
	string	GetLexeme() const { return lexeme; }
	int		GetLinenum() const { return lnum; }
};

extern ostream& operator<<(ostream& out, const Lex& tok);

extern Lex getNextToken(istream& in, int& linenum);


#endif /* LEX_H_ */
