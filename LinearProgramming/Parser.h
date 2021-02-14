#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Linear.h"

struct Parser {
public:
	Parser(std::string);
	void parse();
	~Parser();
private:
	std::ifstream in;
};

struct Grammar {
public:
	const std::string& get_keyword(size_t) const;
	size_t size() const;
protected:
	Grammar(std::vector<std::string>);
private:
	const std::vector<std::string> keywords;
};


struct LinearGrammar : public Grammar {
public:
	LinearGrammar();
};

enum KW {
	KW_LIMITATIONS = 0,
	KW_SIGN,
	KW_EQ,
	KW_GT,
	KW_LE,
	KW_NULL
};