#include "llvm/ADT/STLExtras.h"
#include <algorithm>
#include <ccypte>
#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <vector>

/*
 * Lexer
 */
enum Token {
		tok_eof = -1;
		tok_def = -2;
		tok_extern = -3;
		tok_identifier = -4;
		tok_number = -5;
};

static std::string IdentifierStr;
static double NumVal;

static int gettok(){
		static int LastChar = ' ';
		while (isspace(LastChar))
				LastChar = getchar();

		if (isalpha(LastChar)) {
				IdentifierStr = LastChar;
				while (isalnum((LastChar = getchar())))
						IdentifierStr += LastChar;

				if (IdentifierStr == "def")
						return tok_def;
				if (IdentifierStr == "extern")
						return tok_extern;
				return tok_identifier;
		}

		//小数用途のドット
		if (isdigit(LastChar) || LastChar == '.'){
				std::string NumStr;
				do {
						NumStr += LastChar;
						LastChar = getchar();
				}while(isdigit(LastChar) || LastChar == '.');
				NumVal = strtod(NumStr.c_str(), nullptr);
				return tok_number;
		}


		if (LastChar == '#') {
				do
						LastChar = getchar();
				while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
				if (LastChar != EOF)
						return gettok();
		}

		if(LastChar == EOF)
				return tok_eof;
		int ThisChar = LastChar;
		LastChar = getchar();
		return ThisChar;
}

namespace {
		class ExprAST {
				public:
						virtual ~ExprAST() = default;
		};
		class NumberExprAST:public ExprAST {
				double Val;
				public:
				NumberExprAST(double Val):Val(Val){}
		};
		class VariableExprAST:public ExprAST{
				std::string Name;
				public:
				VariableExprAST(const std::string &Name): Name(Name){}
		};
		class BinaryExprAST:public ExprAST {
				char Op;
				std::unique_ptr<ExprAST> LHS,RHS;
				
				public:
				BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
								std::unique_ptr<ExprAST> RHS):Op(Op),LHS(std::move(LHS)),RHS(std::move(RHS)){}
		};
