#ifndef VCZH_TINYMOE
#define VCZH_TINYMOE

#include "Compiler/TinymoeLexicalAnalyzer.h"
#include "Compiler/TinymoeDeclarationAnalyzer.h"
#include "Compiler/TinymoeExpressionAnalyzer.h"
#include "Compiler/TinymoeStatementAnalyzer.h"
#include "Compiler/TinymoeAstCodegen.h"

#include "Ast/TinymoeAst.h"

namespace tinymoe
{
	extern string			GetCodeForStandardLibrary();
}

#endif