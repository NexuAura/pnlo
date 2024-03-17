// Pnlo 1.0, Pnlo.cpp
// License：Apache-2.0 license
// 2023-10-22 <22:47>
// 豆焰 <beanflame>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

// ----------------------------------------------------------------------------------

enum TokenType {
    TOKEN_IDN, // 标识符 azAZ09_
    TOKEN_STR, // 字符串
    TOKEN_NUM, // 数字
    TOKEN_ASS, // "="
    TOKEN_SEM, // ";"
    TOKEN_END  // !
};

struct Token {
    TokenType type;
    std::string value;
    Token() {}
    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};

std::vector<Token> tokenList;

bool isDigit(char str) {
    if (str >= '0' && str <= '9') { return true; }
    return false;
}
bool isAlpha(char str) {
    if ((str >= 'A' && str <= 'Z') || (str >= 'a' && str <= 'z')) { return true; }
    return false;
}
bool isLetter(char str) {
    if (isAlpha(str) || isDigit(str) || str == '_') { return true; }
    return false;
}

// ----------------------------------------------------------------------------------

class Lexer {
private:
    std::string input = "";
    int position = 0;

public:
    Lexer(const std::string& text) : input(text), position(0) {}
    char currentChar() {
        if (position >= input.length()) {
            return '\0';
        }
        return input[position];
    }
    void advance() {
        position++;
    }
    void skipWhitespace() {
        while (currentChar() != '\0' && isspace(currentChar())) {
            advance();
        }

        

        
    }

    std::string extractName() {
        std::string result = "";
        while (isLetter(currentChar())) {
            result += currentChar();
            advance();
        }
        return result;
    }

    std::string extractString() {
        // 解析双引号内的字符串
        std::string result = "";
        advance();  // 跳过第一个双引号
        while (currentChar() != '\0' && currentChar() != '\"') {
            result += currentChar();
            advance();
        }
        advance();  // 跳过最后一个双引号
        return result;
    }
    std::string extractNumber() {
        std::string result = "";
        bool hasDecimal = false;
        bool isNegative = false;
        bool hasExponent = false;
        bool hasExponentSign = false;

        if (currentChar() == '-') {
            isNegative = true;
            result += "-";
            advance();
        }
        else if (currentChar() == '+') {
            advance();
        }
        while (currentChar() != '\0' && (isDigit(currentChar()) || currentChar() == '.' || currentChar() == 'e' || currentChar() == 'E' || currentChar() == '+' || currentChar() == '-')) {
            if (currentChar() == '.') {
                if (hasDecimal) {
                    break;
                }
                hasDecimal = true;
            }
            else if (currentChar() == 'e' || currentChar() == 'E') {
                if (hasExponent) {
                    break;
                }
                hasExponent = true;
            }
            else if ((currentChar() == '+' || currentChar() == '-') && hasExponent && !hasExponentSign) {
                hasExponentSign = true;
            }
            else if (!isDigit(currentChar())) {
                break;
            }
            result += currentChar();
            advance();
        }
        // 检查结果是否是一个有效数字
        if (result.empty() || (hasDecimal && result.size() == 1) || (hasExponent && (result.size() == 1 || hasExponentSign || !isDigit(result.back())))) {
            std::cerr << "Invalid number!" << std::endl;
            exit(1);
        }
        return result;
    }

    Token getNextToken() {
        while (currentChar() != '\0') {

            if (isspace(currentChar())) {
                skipWhitespace();
                continue;
            }

            if (isLetter(currentChar())) {
                std::string value = extractName();
                return Token(TOKEN_IDN, value);
            }


            if (currentChar() == '\"') {
                std::string value = extractString();
                return Token(TOKEN_STR, value);
            }

            if (isDigit(currentChar()) || currentChar() == '.' || currentChar() == '+' || currentChar() == '-') {
                std::string value = extractNumber();
                return Token(TOKEN_NUM, value);
            }

            if (currentChar() == '/') {
                advance();
                if (currentChar() == '/') {
                    advance();
                    // 跳过双斜线注释内容
                    while (currentChar() != '\0' && currentChar() != '\n') {
                        advance();
                    }
                    continue;  // 继续解析下一个Token
                }
                else {
                    std::cerr << "Error: Invalid Character" << std::endl;
                    exit(1);
                }
            }

            switch (currentChar()) {
            case '=':
                advance();
                return Token(TOKEN_ASS, "=");
            case ';':
                advance();
                return Token(TOKEN_SEM, ";");
            default:
                std::cerr << "Error: Invalid Character" << std::endl;
                exit(1);
            }
        }

        return Token(TOKEN_END, "");
    }
};


// std::map<std::string, Token> varMap = paser(tokenList);
// varMap["这里是varName"] = > 这个就是value 的token，如果要知道它的值，只要加.data就可以输出std::string类型的了


std::map<std::string, Token> paser(std::vector<Token> tokenList) {
    std::map<std::string, Token> varMap;
    //for (int i = 0; i < tokenList.size(); i++) {
    int i = 0;
    while (true) {
        if (tokenList[i].type != TOKEN_STR && tokenList[i].type != TOKEN_IDN) break;    // 识别并储存varName
        std::string varName = tokenList[i].value;
        i++; // 下一个token
        if (tokenList[i].type != TOKEN_ASS) break;    // 识别"="
        i++; // 下一个token
        if (tokenList[i].type != TOKEN_STR && tokenList[i].type != TOKEN_IDN && tokenList[i].type != TOKEN_NUM ) break; // 识别并将varName和它的值一起存到map里
        varMap[varName] = tokenList[i];
        i++;
        if (tokenList[i].type != TOKEN_SEM) break;    // 识别";"
        i++;
        if (tokenList[i].type == TOKEN_END) break;
    }

    /*
    while (true)
    {

        if (tokenList[i].type != TOKEN_STR && tokenList[i].type != TOKEN_IDN)
            break; // 识别并储存varName
        std::string varName = tokenList[i].value;

        i++; // 下一个token
        if (tokenList[i].type != TOKEN_ASS)
            break; // 识别"="
        i++;       // 下一个token
        if (tokenList[i].type != TOKEN_STR && tokenList[i].type != TOKEN_NUM)
            break; // 识别并将varName和它的值一起存到map里
        varMap[varName] = tokenList[i];
        i++;
        if (tokenList[i].type != TOKEN_SEM) {
            break; // 识别";"
        }
        i++;
        if (tokenList[i].type == TOKEN_END)
            break;
        // i++;
    }
    */
    return varMap;
}

int main() {
std::string input = R"(
// Pnlo This is a comment, patorjk Sub-Zero

"Pnlo" = "Purple Neon Light Object";
"version" = "1.0";
Object1 = "obj-1";
Object2 = "obj-2";
"Object3" = "obj-3";
"Object4" = "obj-4";
main_obj_ = "Main Obj";
OFF = false;
ON = true;
Time = 2207;
Jra = null;

// [obj]
//    yes = true;
// ~
)";

    Lexer lexer(input);
    Token token = lexer.getNextToken();

    while (token.type != TOKEN_END) {
        tokenList.push_back(token);
        // std::cout << "Token: " << token.value << ", Type: " << token.type << std::endl;
        token = lexer.getNextToken();
    }
    
    tokenList.push_back(token);
    std::map<std::string, Token> obj = paser(tokenList);
    
    std::cout << "Pnlo:" << obj["Pnlo"].value << std::endl;
    std::cout << "version:" << obj["version"].value << std::endl;
    std::cout << "Object1:" << obj["Object1"].value << std::endl;
    std::cout << "Object2:" << obj["Object2"].value << std::endl;
    std::cout << "Object3:" << obj["Object3"].value << std::endl;
    std::cout << "Object4:" << obj["Object4"].value << std::endl;
    std::cout << "main_obj_:" << obj["main_obj_"].value << std::endl;
    std::cout << "Time:" << obj["Time"].value << std::endl;
    std::cout << "OFF:" << obj["OFF"].value << std::endl;
    std::cout << "ON:" << obj["ON"].value << std::endl;

    return 0;
}
