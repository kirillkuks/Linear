#include "Parser.h"

Parser::Parser(std::string filename) : in{ filename } {}

Parser::~Parser() {
	in.close();
}

Linear Parser::parse() {
	std::string line;
	KW last_token = KW::KW_NULL;
	LinearGrammar grammar;

	Limitations limitations;
	std::vector<double> objective_function(N);
	std::vector<bool> vars_sign(N);

	while (std::getline(in, line)) {
		std::stringstream ss;
		std::vector<double> vec(N + 1);
		std::string sign;

		ss << line;

		if (line == grammar.get_keyword(KW::KW_LIMITATIONS)) {
			last_token = KW::KW_LIMITATIONS;
		}
		else if (line == grammar.get_keyword(KW::KW_SIGN)) {
			last_token = KW::KW_SIGN;
		}
		else {
			switch (last_token) {
			case KW::KW_NULL:
				for (auto& elem : objective_function) {
					ss >> elem;
				}
				break;
			case KW::KW_LIMITATIONS:
				for (size_t i = 0; i < N; ++i) {
					ss >> vec[i];
				}
				ss >> sign;
				ss >> vec[N];

				limitations.add_limitations({ vec, define_sign(sign) });
				break;
			case KW::KW_SIGN:
				for (size_t i = 0; i < N; ++i) {
					size_t index;
					ss >> index;
					vars_sign[index - 1] = true;
				}
				break;
			}
		}
	}
	return Linear(objective_function, limitations, vars_sign);
}

LT Parser::define_sign(std::string& limitation_type) {
	LinearGrammar grammar;
	if (limitation_type == grammar.get_keyword(KW::KW_EQ)) {
		return LT::LT_EQ;
	}
	else if (limitation_type == grammar.get_keyword(KW::KW_GT)) {
		return LT::LT_GT;
	}
	else if (limitation_type == grammar.get_keyword(KW::KW_LE)) {
		return LT::LT_LE;
	}
	return LT::LT_ERROR;
}

Grammar::Grammar(std::vector<std::string> keywords) : keywords{ keywords } {}

const std::string& Grammar::get_keyword(size_t index) const {
	return keywords[index];
}

size_t Grammar::size() const {
	return keywords.size();
}

LinearGrammar::LinearGrammar() : Grammar({ "Limitations", "Sign", "=", ">=", "<=" }) {}

Limitations::Limitations() {}

void Limitations::add_limitations(std::pair<std::vector<double>, LT>&& limitation) {
	limitations.push_back(limitation);
}