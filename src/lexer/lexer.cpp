#include "./lexer.hpp"
#include "../utilities/utils.hpp"
#include <iostream>
#include <map>
#include "../config.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

static std::map<char, TokenType> singles = {
    {'(', TokenType::OpenParen},
    {')', TokenType::CloseParen},
    {'{', TokenType::OpenBrace},
    {'}', TokenType::CloseBrace},
    {'[', TokenType::OpenBracket},
    {']', TokenType::CloseBracket},
    {'$', TokenType::DollarSign},
    {';', TokenType::Semicolon},
    {':', TokenType::Colon},
    {',', TokenType::Comma},
    {'.', TokenType::Dot}
};


Lexer::Lexer(){

}

Lexer::~Lexer(){

}

std::map<std::string, TokenType> Keywords()
{
    KeywordList keywords;
    keywords["int"] = TokenType::Type;
    keywords["float"] = TokenType::Type;
    keywords["bool"] = TokenType::Type;
    keywords["char*"] = TokenType::Type;
    keywords["vector"] = TokenType::Type;
    keywords["object"] = TokenType::Type;

    keywords["set"] = TokenType::Set;
    keywords["lock"] = TokenType::Lock;
    keywords["fun"] = TokenType::Fun;
    keywords["if"] = TokenType::If;
    keywords["else"] = TokenType::Else;
    keywords["for"] = TokenType::For;
    keywords["while"] = TokenType::While;
    keywords["return"] = TokenType::Return;
    keywords["break"] = TokenType::Break;
    keywords["take"] = TokenType::Take;
    keywords["from"] = TokenType::From;
    keywords["give"] = TokenType::Give;
    keywords["using"] = TokenType::Using;
    keywords["when"] = TokenType::When;
    keywords["skip"] = TokenType::Skip;
    keywords["class"] = TokenType::Class;
    keywords["uses"] = TokenType::Uses;
    return keywords;
}

Token token(std::string value, TokenType type, int start, int end)
{
    return {value, type, start, end};
}

bool skippable(char src)
{
    return src == ' ' || src == '\n' || src == '\t' || src == '\b';
}

Token tokenc(char c, TokenType type, int& pos, int& place) {
    pos++;
    return token(std::string(1, c), type, place++, place);
}

TokenArr Lexer::tokenize(std::string sourceCode)
{
    KeywordList keywords = Keywords();
    TokenArr tokens{};
    int place = 0;
    int pos = 0;
    std::string src = sourceCode;

    while (src.size() > pos)
    {
        if (singles.find(src[pos]) != singles.end()) {
            tokens.push_back(tokenc(src[pos], singles[src[pos]], pos, place));
            continue;
        }
        switch (src[pos])
        {
        case '=':
            if (src[pos] != '=')
                tokens.push_back(tokenc('=', TokenType::Equals, pos, place));
            else
            {
                pos++;
                pos++;
                tokens.push_back(token("==", TokenType::ComparativeOperator, place++, place++));
            }
            break;

        case '@':
            if (src[pos] == '=')
            {
                pos++;
                pos++;
                tokens.push_back(token("@=", TokenType::ComparativeOperator, place++, place++));
            }
            else
            {
                tokens.push_back(tokenc('@', TokenType::At, pos, place));
            }
            break;
        case '!':
            if (src[pos] == '=')
            {
                pos++;
                pos++;
                tokens.push_back(token("!=", TokenType::ComparativeOperator, place++, place++));
            }
            else
            {
                tokens.push_back(tokenc('!', TokenType::ExclamationMark, pos, place));
            }
            break;

        case '<':
            if (src[pos+1] == '=')
            {
                pos++;
                pos++;
                tokens.push_back(token("<=", TokenType::ComparativeOperator, place++, place++));
            }
            else
            {
                tokens.push_back(tokenc('<', TokenType::ComparativeOperator, pos, place));
            }
            break;

        case '>':
            if (src[pos+1] == '=')
            {
                pos++;
                pos++;
                tokens.push_back(token(">=", TokenType::ComparativeOperator, place++, place++));
            }
            else
            {
                tokens.push_back(tokenc('>', TokenType::ComparativeOperator, pos, place));
            }
            break;

        case '\'':
        case '\"':
        {
            char odk = src[pos++];
            int start = place++;
            std::string str = "";
            while (src.size() > 0 && src[pos] != odk)
            {
                str += src[pos++];
                place++;
            }
            pos++;
            tokens.push_back(token(str, TokenType::String, start, place++));
        }
        break;

        case '*':
        case '+':
        case '-':
        case '/':
            if (src[pos] == '/' && (src[pos+1] == '/' || src[pos+1] == '*'))
            {
                char commentType = src[pos+1];
                int start = place++;
                pos += 2;
                place++;

                if (commentType == '/')
                {
                    while (!src.empty() && src[pos] != '\n')
                    {
                        pos++;
                        place++;
                    }
                }

                if (commentType == '*')
                {
                    pos++;
                    while (!src.empty())
                    {
                        if (src[pos] == '*' && src.size() > 1 && src[pos+1] == '/')
                        {
                            pos += 2;
                            break;
                        }
                        place++;
                        pos++;
                    }
                }
            }
            else
            {
                std::string idk = "";
                idk += src[pos++];
                tokens.push_back(token(idk, TokenType::BinaryOperator, place++, place));
            }
            break;

        default:
            if (isalpha(src[pos]))
            {
                std::string idk = "";
                int start = place++;
                while (src.size() > 0 && (isalnum(src[pos]) || src[pos] == '_' || src[pos] == '*'))
                {
                    char text = src[pos++];
                    idk += text;
                    place++;
                }
                if (keywords.find(idk) == keywords.end())
                {
                    tokens.push_back(token(idk, TokenType::Identifier, start, place++));
                }
                else
                {
                    std::transform(idk.begin(), idk.end(), idk.begin(),
                        [](unsigned char c){ return std::tolower(c); });
                    tokens.push_back(token( idk , keywords[idk], start, place++));
                }
            }
            else if (isdigit(src[pos]))
            {
                std::string idk = "";
                int start = place++;
                while (src.size() > 0 && isdigit(src[pos]))
                {
                    idk += src[pos++];
                    place++;
                }
                tokens.push_back(token(idk, TokenType::Number, start, place++));
            }
            else if (skippable(src[pos]))
            {
                pos++;
                place++;
            }
            else
            {
                std::cout << "Unrecognized character found: " << src[pos] << std::endl;
                exit(1);
            }
            break;
        }
    }

    tokens.push_back(token("EndOfFile", TokenType::_EOF, ++place, ++place));
    return tokens;
}