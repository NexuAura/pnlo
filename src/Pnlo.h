// Pnlo 1.0
// License：Apache-2.0 license
// 2023-10-22 <22:47>
// 豆焰 <beanflame>

#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace pnlo {

enum class TokenType {
	Identifier,   // 标识符 azAZ09_
	String,       // 字符串
	Number,       // 数字
	Assign,       // "="
	Semicolon,    // ";"
	Comma,        // ","
	LeftBracket,  // "["
	RightBracket, // "]"
	ObjectStart,  // ">"
	ObjectEnd,    // "~"
	End,          // 输入结束
};

struct Token {
	TokenType type = TokenType::End;
	std::string value;
};

// 一条 "名称 = 值;" 赋值语句
struct Variable {
	std::string name;
	Token value;
};

// 词法分析：把 Pnlo 源码切成词法单元序列
class Lexer {
public:
	explicit Lexer(std::string text);

	// 读取下一个词法单元，到达末尾时返回类型为 TokenType::End 的单元
	Token nextToken();

	// 读取全部词法单元，末尾包含一个 TokenType::End
	std::vector<Token> tokenize();

private:
	bool atEnd() const;
	char currentChar() const;
	void advance();
	void skipWhitespace();
	void skipBlockComment();

	std::string extractName();
	std::string extractString();
	std::string extractNumber();

	std::string input_;
	std::size_t position_ = 0;
};

// 语法分析：解析顶层赋值语句，按出现顺序返回；暂不支持的数组值与对象块会被跳过
std::vector<Variable> parse(const std::vector<Token>& tokens);

} // namespace pnlo