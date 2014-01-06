#ifndef VCZH_COMPILER_TINYMOEASTCODEGEN
#define VCZH_COMPILER_TINYMOEASTCODEGEN

#include "TinymoeStatementAnalyzer.h"
#include "../Ast/TinymoeAst.h"

namespace tinymoe
{
	namespace compiler
	{
		extern ast::AstAssembly::Ptr			GenerateAst(SymbolAssembly::Ptr symbolAssembly);
	}
}

#endif