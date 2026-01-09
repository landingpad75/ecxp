#include <parser/parser.hpp>

PAST Parser::ParseExpression() {
    return this->parseAssignment();
}