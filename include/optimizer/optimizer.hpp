#pragma once

#include <extra/ast.hpp>
#include <memory>
#include <unordered_map>
#include <string>

class Optimizer {
public:
    Optimizer() = default;
    
    std::shared_ptr<AST::ExprAST> optimize(const std::shared_ptr<AST::ExprAST>& ast);
    
private:
    std::unordered_map<std::string, std::shared_ptr<AST::ExprAST>> constants_;
    
    std::shared_ptr<AST::ExprAST> optimizeNode(const std::shared_ptr<AST::ExprAST>& node);
    std::shared_ptr<AST::ExprAST> optimizeBinaryOp(const std::shared_ptr<AST::BinaryOperationExpr>& node);
    std::shared_ptr<AST::ExprAST> optimizeIfStatement(const std::shared_ptr<AST::IfStatement>& node);
    std::shared_ptr<AST::ExprAST> optimizeWhileLoop(const std::shared_ptr<AST::WhileLoop>& node);
    std::shared_ptr<AST::ExprAST> optimizeFunctionDeclaration(const std::shared_ptr<AST::FunctionDeclaration>& node);
    std::shared_ptr<AST::ExprAST> optimizeProgramRoot(const std::shared_ptr<AST::ProgramRoot>& node);
    std::shared_ptr<AST::ExprAST> optimizeVariableDeclaration(const std::shared_ptr<AST::VariableDeclarationExpr>& node);
    std::shared_ptr<AST::ExprAST> optimizeIdentifier(const std::shared_ptr<AST::IdentifierExpr>& node);
    std::shared_ptr<AST::ExprAST> optimizeAssignment(const std::shared_ptr<AST::AssignmentExpr>& node);
    std::shared_ptr<AST::ExprAST> optimizeFunctionCall(const std::shared_ptr<AST::FunctionCallExpr>& node);
    std::shared_ptr<AST::ExprAST> optimizeArrayLiteral(const std::shared_ptr<AST::ArrayLiteral>& node);
    std::shared_ptr<AST::ExprAST> optimizeMemberAccess(const std::shared_ptr<AST::MemberAccessExpr>& node);
    std::shared_ptr<AST::ExprAST> optimizeEqualityCheck(const std::shared_ptr<AST::EqualityCheckExpr>& node);
    
    bool isConstant(const std::shared_ptr<AST::ExprAST>& node) const;
    bool isNumberLiteral(const std::shared_ptr<AST::ExprAST>& node) const;
    double getNumberValue(const std::shared_ptr<AST::ExprAST>& node) const;
    
    std::vector<std::shared_ptr<AST::ExprAST>> eliminateDeadCode(
        const std::vector<std::shared_ptr<AST::ExprAST>>& body);
    
    std::shared_ptr<AST::ExprAST> foldBinaryOp(
        char op, 
        const std::shared_ptr<AST::ExprAST>& left,
        const std::shared_ptr<AST::ExprAST>& right);
    
    std::shared_ptr<AST::ExprAST> simplifyAlgebraic(
        char op,
        const std::shared_ptr<AST::ExprAST>& left,
        const std::shared_ptr<AST::ExprAST>& right);
};