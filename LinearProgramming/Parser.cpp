#include "Parser.h"

Parser::Parser(std::string filename) : in{ filename } {}

Parser::~Parser() {
	in.close();
}

void Parser::parse() {
	std::string line;
	KW last_token = KW::KW_NULL;
	std::stringstream ss;
	LinearGrammar grammar;

	std::vector<int> objective_function(5);
	while (std::getline(in, line)) {
		ss << line;
		std::cout << line << std::endl;

		if (line == grammar.get_keyword(KW::KW_LIMITATIONS)) {
			last_token = KW::KW_LIMITATIONS;
		}
		if (line == grammar.get_keyword(KW::KW_SIGN)) {
			last_token = KW::KW_SIGN;
		}

		switch (last_token) {
		case KW::KW_NULL:
			for (auto& elem : objective_function) {
				ss >> elem;
			}
			break;
		case KW::KW_LIMITATIONS:
			break;
		case KW::KW_SIGN:
			break;
		}

	}
}

Grammar::Grammar(std::vector<std::string> keywords) : keywords{ keywords } {}

const std::string& Grammar::get_keyword(size_t index) const {
	return keywords[index];
}

size_t Grammar::size() const {
	return keywords.size();
}

LinearGrammar::LinearGrammar() : Grammar({ "Limitations", "Sign", "=", ">", "<" }) {}