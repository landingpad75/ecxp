#include <executor/executor.hpp>
using NodeType = AST::NodeType; 
using string = std::string;

std::unique_ptr<Values::Runtime> Interpreter::evaluateIdentifier(const AST::IdentifierExpr& node, Environment& env){
	auto* hi = env.get(node.name);

	if(hi == nullptr){
		std::cout << "Couldn't find '" << node.name << "'\n";
		exit(0);
	}
	return hi->clone();
}