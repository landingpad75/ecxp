#include <optimizer/optimizer.hpp>
#include <cmath>
#include <iostream>

std::shared_ptr<AST::ExprAST> Optimizer::optimize(const std::shared_ptr<AST::ExprAST>& ast) {
    if (!ast) return ast;
    return optimizeNode(ast);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeNode(const std::shared_ptr<AST::ExprAST>& node) {
    if (!node) return node;
    
    switch (node->nodeType()) {
        case AST::NodeType::BinaryOperation:
            return optimizeBinaryOp(std::static_pointer_cast<AST::BinaryOperationExpr>(node));
        
        case AST::NodeType::IfStatement:
            return optimizeIfStatement(std::static_pointer_cast<AST::IfStatement>(node));
        
        case AST::NodeType::WhileLoop:
            return optimizeWhileLoop(std::static_pointer_cast<AST::WhileLoop>(node));
        
        case AST::NodeType::FunctionDeclaration:
            return optimizeFunctionDeclaration(std::static_pointer_cast<AST::FunctionDeclaration>(node));
        
        case AST::NodeType::ProgramRoot:
            return optimizeProgramRoot(std::static_pointer_cast<AST::ProgramRoot>(node));
        
        case AST::NodeType::VariableDeclaration:
            return optimizeVariableDeclaration(std::static_pointer_cast<AST::VariableDeclarationExpr>(node));
        
        case AST::NodeType::IdentifierExpr:
            return optimizeIdentifier(std::static_pointer_cast<AST::IdentifierExpr>(node));
        
        case AST::NodeType::AssignmentExpr:
            return optimizeAssignment(std::static_pointer_cast<AST::AssignmentExpr>(node));
        
        case AST::NodeType::FunctionCall:
            return optimizeFunctionCall(std::static_pointer_cast<AST::FunctionCallExpr>(node));
        
        case AST::NodeType::ArrayLiteral:
            return optimizeArrayLiteral(std::static_pointer_cast<AST::ArrayLiteral>(node));
        
        case AST::NodeType::MemberAccess:
            return optimizeMemberAccess(std::static_pointer_cast<AST::MemberAccessExpr>(node));
        
        case AST::NodeType::EqualityCheck:
            return optimizeEqualityCheck(std::static_pointer_cast<AST::EqualityCheckExpr>(node));
        
        default:
            return node;
    }
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeBinaryOp(
    const std::shared_ptr<AST::BinaryOperationExpr>& node) {
    
    auto left = optimizeNode(node->lhs);
    auto right = optimizeNode(node->rhs);
    
    auto folded = foldBinaryOp(node->op, left, right);
    if (folded) return folded;
    
    auto simplified = simplifyAlgebraic(node->op, left, right);
    if (simplified) return simplified;
    
    return std::make_shared<AST::BinaryOperationExpr>(node->op, left, right);
}

std::shared_ptr<AST::ExprAST> Optimizer::foldBinaryOp(
    char op,
    const std::shared_ptr<AST::ExprAST>& left,
    const std::shared_ptr<AST::ExprAST>& right) {
    
    if (!isNumberLiteral(left) || !isNumberLiteral(right)) {
        return nullptr;
    }
    
    double lval = getNumberValue(left);
    double rval = getNumberValue(right);
    double result = 0;
    
    switch (op) {
        case '+': result = lval + rval; break;
        case '-': result = lval - rval; break;
        case '*': result = lval * rval; break;
        case '/': 
            if (rval == 0) return nullptr;
            result = lval / rval; 
            break;
        default: return nullptr;
    }
    
    return std::make_shared<AST::NumberLiteral>(result);
}

std::shared_ptr<AST::ExprAST> Optimizer::simplifyAlgebraic(
    char op,
    const std::shared_ptr<AST::ExprAST>& left,
    const std::shared_ptr<AST::ExprAST>& right) {
    
    if (op == '+' && isNumberLiteral(right) && getNumberValue(right) == 0) {
        return left;
    }
    
    if (op == '+' && isNumberLiteral(left) && getNumberValue(left) == 0) {
        return right;
    }
    
    if (op == '-' && isNumberLiteral(right) && getNumberValue(right) == 0) {
        return left;
    }
    
    if (op == '*' && isNumberLiteral(right) && getNumberValue(right) == 1) {
        return left;
    }
    
    if (op == '*' && isNumberLiteral(left) && getNumberValue(left) == 1) {
        return right;
    }
    
    if (op == '*' && isNumberLiteral(right) && getNumberValue(right) == 0) {
        return std::make_shared<AST::NumberLiteral>(0);
    }
    
    if (op == '*' && isNumberLiteral(left) && getNumberValue(left) == 0) {
        return std::make_shared<AST::NumberLiteral>(0);
    }
    
    if (op == '/' && isNumberLiteral(right) && getNumberValue(right) == 1) {
        return left;
    }
    
    return nullptr;
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeIfStatement(
    const std::shared_ptr<AST::IfStatement>& node) {

    auto condition = optimizeNode(node->condition);

    std::vector<std::shared_ptr<AST::ExprAST>> optimizedConsequent;
    for (const auto& stmt : node->consequent) {
        auto optimized = optimizeNode(stmt);
        if (optimized) optimizedConsequent.push_back(optimized);
    }
    optimizedConsequent = eliminateDeadCode(optimizedConsequent);

    std::vector<std::shared_ptr<AST::ExprAST>> optimizedAlternate;
    for (const auto& stmt : node->alternate) {
        auto optimized = optimizeNode(stmt);
        if (optimized) optimizedAlternate.push_back(optimized);
    }
    optimizedAlternate = eliminateDeadCode(optimizedAlternate);

    return std::make_shared<AST::IfStatement>(
        condition, node->conditionType, optimizedConsequent, optimizedAlternate);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeWhileLoop(
    const std::shared_ptr<AST::WhileLoop>& node) {

    auto condition = optimizeNode(node->condition);

    std::vector<std::shared_ptr<AST::ExprAST>> optimizedBody;
    for (const auto& stmt : node->body) {
        auto optimized = optimizeNode(stmt);
        if (optimized) optimizedBody.push_back(optimized);
    }
    optimizedBody = eliminateDeadCode(optimizedBody);

    return std::make_shared<AST::WhileLoop>(condition, node->loopType, optimizedBody);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeFunctionDeclaration(
    const std::shared_ptr<AST::FunctionDeclaration>& node) {

    std::vector<std::shared_ptr<AST::ExprAST>> optimizedBody;
    for (const auto& stmt : node->body) {
        auto optimized = optimizeNode(stmt);
        if (optimized) optimizedBody.push_back(optimized);
    }
    optimizedBody = eliminateDeadCode(optimizedBody);

    return std::make_shared<AST::FunctionDeclaration>(
        node->parameters, node->name, optimizedBody, node->returnType);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeProgramRoot(
    const std::shared_ptr<AST::ProgramRoot>& node) {

    std::vector<std::shared_ptr<AST::ExprAST>> optimizedBody;
    for (const auto& stmt : node->body) {
        auto optimized = optimizeNode(stmt);
        if (optimized) optimizedBody.push_back(optimized);
    }
    optimizedBody = eliminateDeadCode(optimizedBody);

    return std::make_shared<AST::ProgramRoot>(optimizedBody);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeVariableDeclaration(
    const std::shared_ptr<AST::VariableDeclarationExpr>& node) {

    auto optimizedValue = optimizeNode(node->initialValue);

    if (node->isConstant && isConstant(optimizedValue)) {
        constants_[node->identifier] = optimizedValue;
    }

    return std::make_shared<AST::VariableDeclarationExpr>(
        node->identifier, node->typeHint, optimizedValue, node->isConstant);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeIdentifier(
    const std::shared_ptr<AST::IdentifierExpr>& node) {

    auto it = constants_.find(node->name);
    if (it != constants_.end()) {
        return it->second->clone();
    }

    return node;
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeAssignment(
    const std::shared_ptr<AST::AssignmentExpr>& node) {

    auto optimizedTarget = optimizeNode(node->target);
    auto optimizedValue = optimizeNode(node->value);

    if (node->target->nodeType() == AST::NodeType::IdentifierExpr) {
        auto ident = std::static_pointer_cast<AST::IdentifierExpr>(node->target);
        constants_.erase(ident->name);
    }

    return std::make_shared<AST::AssignmentExpr>(optimizedTarget, optimizedValue);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeFunctionCall(
    const std::shared_ptr<AST::FunctionCallExpr>& node) {

    auto optimizedCallee = optimizeNode(node->callee);

    std::vector<std::shared_ptr<AST::ExprAST>> optimizedArgs;
    for (const auto& arg : node->arguments) {
        optimizedArgs.push_back(optimizeNode(arg));
    }

    return std::make_shared<AST::FunctionCallExpr>(optimizedCallee, optimizedArgs);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeArrayLiteral(
    const std::shared_ptr<AST::ArrayLiteral>& node) {

    std::vector<std::shared_ptr<AST::ExprAST>> optimizedElements;
    for (const auto& elem : node->elements) {
        optimizedElements.push_back(optimizeNode(elem));
    }

    return std::make_shared<AST::ArrayLiteral>(optimizedElements);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeMemberAccess(
    const std::shared_ptr<AST::MemberAccessExpr>& node) {

    auto optimizedObject = optimizeNode(node->object);
    auto optimizedProperty = optimizeNode(node->property);

    return std::make_shared<AST::MemberAccessExpr>(
        optimizedObject, optimizedProperty, node->computed);
}

std::shared_ptr<AST::ExprAST> Optimizer::optimizeEqualityCheck(
    const std::shared_ptr<AST::EqualityCheckExpr>& node) {

    auto optimizedLeft = optimizeNode(node->left);
    auto optimizedRight = optimizeNode(node->right);

    return std::make_shared<AST::EqualityCheckExpr>(
        optimizedLeft, optimizedRight, node->operatorToken);
}

bool Optimizer::isConstant(const std::shared_ptr<AST::ExprAST>& node) const {
    if (!node) return false;

    auto type = node->nodeType();
    return type == AST::NodeType::NumberLiteral ||
           type == AST::NodeType::StringLiteral;
}

bool Optimizer::isNumberLiteral(const std::shared_ptr<AST::ExprAST>& node) const {
    return node && node->nodeType() == AST::NodeType::NumberLiteral;
}

double Optimizer::getNumberValue(const std::shared_ptr<AST::ExprAST>& node) const {
    if (!isNumberLiteral(node)) return 0.0;

    auto numNode = std::static_pointer_cast<AST::NumberLiteral>(node);
    return numNode->value;
}

std::vector<std::shared_ptr<AST::ExprAST>> Optimizer::eliminateDeadCode(
    const std::vector<std::shared_ptr<AST::ExprAST>>& body) {

    std::vector<std::shared_ptr<AST::ExprAST>> result;

    for (const auto& stmt : body) {
        result.push_back(stmt);

        if (stmt->nodeType() == AST::NodeType::ReturnStatement ||
            stmt->nodeType() == AST::NodeType::BreakStatement) {
            break;
        }
    }

    return result;
}