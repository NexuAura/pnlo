// Pnlo - 实现
#include "Pnlo.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

namespace pnlo {
namespace {

bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

bool isAlpha(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isLetter(char c) {
	return isAlpha(c) || isDigit(c) || c == '_';
}

[[noreturn]] void fail(const char* message) {
	std::cerr << message << std::endl;
	std::exit(1);
}

} // namespace

Lexer::Lexer(std::string text) : input_(std::move(text)) {}

bool Lexer::atEnd() const {
	return position_ >= input_.size();
}

char Lexer::currentChar() const {
	return atEnd() ? '\0' : input_[position_];
}

void Lexer::advance() {
	++position_;
}

void Lexer::skipWhitespace() {
	while (!atEnd() && std::isspace(static_cast<unsigned char>(currentChar()))) {
		advance();
	}
}

void Lexer::skipBlockComment() {
	// 进入时已经越过 "/*"
	while (!atEnd()) {
		if (currentChar() == '*') {
			advance();
			if (currentChar() == '/') {
				advance();
				return;
			}
			continue;
		}
		advance();
	}
	fail("Error: Unterminated Comment");
}

std::string Lexer::extractName() {
	std::string result;
	while (!atEnd() && isLetter(currentChar())) {
		result += currentChar();
		advance();
	}
	return result;
}

std::string Lexer::extractString() {
	std::string result;
	advance(); // 跳过起始双引号
	while (!atEnd() && currentChar() != '"') {
		result += currentChar();
		advance();
	}
	advance(); // 跳过结束双引号
	return result;
}

std::string Lexer::extractNumber() {
	std::string result;
	bool hasDecimal = false;
	bool hasExponent = false;
	bool hasExponentSign = false;

	if (currentChar() == '-') {
		result += '-';
		advance();
	}
	else if (currentChar() == '+') {
		advance();
	}

	while (!atEnd()) {
		const char c = currentChar();
		if (!isDigit(c) && c != '.' && c != 'e' && c != 'E' && c != '+' && c != '-') {
			break;
		}
		if (c == '.') {
			if (hasDecimal) {
				break;
			}
			hasDecimal = true;
		}
		else if (c == 'e' || c == 'E') {
			if (hasExponent) {
				break;
			}
			hasExponent = true;
		}
		else if ((c == '+' || c == '-') && hasExponent && !hasExponentSign) {
			hasExponentSign = true;
		}
		else if (!isDigit(c)) {
			break;
		}
		result += c;
		advance();
	}

	// 与原有校验保持一致：结果必须是合法数字
	if (result.empty() || (hasDecimal && result.size() == 1) ||
		(hasExponent && (result.size() == 1 || hasExponentSign || !isDigit(result.back())))) {
		fail("Invalid number!");
	}
	return result;
}

Token Lexer::nextToken() {
	while (!atEnd()) {
		const char c = currentChar();

		if (std::isspace(static_cast<unsigned char>(c))) {
			skipWhitespace();
			continue;
		}
		// 标识符判定包含数字且先于数字分支，保证 2207.2 仍被切成 2207 与 .2
		if (isLetter(c)) {
			return Token{TokenType::Identifier, extractName()};
		}
		if (c == '"') {
			return Token{TokenType::String, extractString()};
		}
		if (isDigit(c) || c == '.' || c == '+' || c == '-') {
			return Token{TokenType::Number, extractNumber()};
		}
		if (c == '/') {
			advance();
			if (currentChar() == '/') {
				advance();
				// 跳过 // 之后的行内注释
				while (!atEnd() && currentChar() != '\n') {
					advance();
				}
				continue;
			}
			if (currentChar() == '*') {
				advance();
				skipBlockComment();
				continue;
			}
			fail("Error: Invalid Character");
		}
		if (c == '=') {
			advance();
			return Token{TokenType::Assign, "="};
		}
		if (c == ';') {
			advance();
			return Token{TokenType::Semicolon, ";"};
		}
		if (c == ',') {
			advance();
			return Token{TokenType::Comma, ","};
		}
		if (c == '[') {
			advance();
			return Token{TokenType::LeftBracket, "["};
		}
		if (c == ']') {
			advance();
			return Token{TokenType::RightBracket, "]"};
		}
		if (c == '>') {
			advance();
			return Token{TokenType::ObjectStart, ">"};
		}
		if (c == '~') {
			advance();
			return Token{TokenType::ObjectEnd, "~"};
		}

		fail("Error: Invalid Character");
	}

	return Token{TokenType::End, ""};
}

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;
	while (true) {
		Token token = nextToken();
		const bool isEnd = token.type == TokenType::End;
		tokens.push_back(std::move(token));
		if (isEnd) {
			break;
		}
	}
	return tokens;
}

namespace {

bool isName(const Token& token) {
	return token.type == TokenType::Identifier || token.type == TokenType::String;
}

bool isValue(const Token& token) {
	return isName(token) || token.type == TokenType::Number;
}

// 跳到当前语句的分号之后，用于跳过暂不支持的值（如数组）
std::size_t skipStatement(const std::vector<Token>& tokens, std::size_t index) {
	while (index < tokens.size()) {
		const TokenType type = tokens[index].type;
		++index;
		if (type == TokenType::Semicolon || type == TokenType::End) {
			break;
		}
	}
	return index;
}

// 跳到 "obj > ... ~" 对象的结束符之后，支持嵌套
std::size_t skipObject(const std::vector<Token>& tokens, std::size_t index) {
	int depth = 0;
	while (index < tokens.size()) {
		const TokenType type = tokens[index].type;
		++index;
		if (type == TokenType::ObjectStart) {
			++depth;
		}
		else if (type == TokenType::ObjectEnd) {
			if (--depth == 0) {
				break;
			}
		}
		else if (type == TokenType::End) {
			break;
		}
	}
	return index;
}

void setVariable(std::vector<Variable>& variables, const std::string& name, const Token& value) {
	for (Variable& variable : variables) {
		if (variable.name == name) {
			variable.value = value;
			return;
		}
	}
	variables.push_back(Variable{name, value});
}

} // namespace

std::vector<Variable> parse(const std::vector<Token>& tokens) {
	std::vector<Variable> variables;

	std::size_t index = 0;
	while (index < tokens.size() && tokens[index].type != TokenType::End) {
		// 对象块 "obj > ... ~" 整体跳过
		if (tokens[index].type == TokenType::ObjectStart ||
			(isName(tokens[index]) && index + 1 < tokens.size() &&
			 tokens[index + 1].type == TokenType::ObjectStart)) {
			const bool nameFirst = tokens[index].type != TokenType::ObjectStart;
			index = skipObject(tokens, nameFirst ? index + 1 : index);
			continue;
		}
		// 只有 "名称 = 简单值;" 会被识别，其余语句整条跳过
		if (isName(tokens[index]) && index + 3 < tokens.size() &&
			tokens[index + 1].type == TokenType::Assign &&
			isValue(tokens[index + 2]) &&
			tokens[index + 3].type == TokenType::Semicolon) {
			setVariable(variables, tokens[index].value, tokens[index + 2]);
			index += 4;
			continue;
		}
		index = skipStatement(tokens, index);
	}

	return variables;
}

} // namespace pnlo

namespace {

// 未指定参数时读取的示例配置
const char kDefaultConfigPath[] = "examples/text.pnlo";

bool readFile(const std::string& path, std::string& content) {
	std::ifstream file(path, std::ios::binary);
	if (!file) {
		return false;
	}
	std::ostringstream buffer;
	buffer << file.rdbuf();
	content = buffer.str();
	return true;
}

} // namespace

int main(int argc, char** argv) {
	const std::string path = argc > 1 ? argv[1] : kDefaultConfigPath;

	std::string source;
	if (!readFile(path, source)) {
		std::cerr << "Error: Cannot open config file: " << path << std::endl;
		return 1;
	}

	pnlo::Lexer lexer(std::move(source));
	const std::vector<pnlo::Token> tokens = lexer.tokenize();
	const std::vector<pnlo::Variable> variables = pnlo::parse(tokens);

	for (const pnlo::Variable& variable : variables) {
		std::cout << variable.name << ":" << variable.value.value << std::endl;
	}

	return 0;
}