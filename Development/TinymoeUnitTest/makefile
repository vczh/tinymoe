CPP = g++ -std=c++11

BIN = ./Bin/
TIN = ../Source/
AST = ../Source/Ast/
COM = ../Source/Compiler/

TIN_OBJS = $(BIN)Tinymoe.o

AST_OBJS = $(BIN)TinymoeAst.o $(BIN)TinymoeAst_CollectSideEffectExpressions.o $(BIN)TinymoeAst_CollectUsedVariables.o $(BIN)TinymoeAst_ExpandBlock.o $(BIN)TinymoeAst_GetRootLeftValue.o $(BIN)TinymoeAst_Print.o $(BIN)TinymoeAst_RemoveUnnecessaryVariables.o $(BIN)TinymoeAst_RoughlyOptimize.o $(BIN)TinymoeAst_SetParent.o

COM_OBJS = $(BIN)TinymoeAstCodegen.o $(BIN)TinymoeAstCodegen_Declaration.o $(BIN)TinymoeAstCodegen_Expression.o $(BIN)TinymoeAstCodegen_Statement.o $(BIN)TinymoeDeclarationAnalyzer.o $(BIN)TinymoeExpressionAnalyzer.o $(BIN)TinymoeLexicalAnalyzer.o $(BIN)TinymoeStatementAnalyzer.o

UNITTEST_OBJS = $(BIN)CSharpCodegen.o $(BIN)UnitTest.o $(BIN)Main.o

TESTCASE_OBJS = $(BIN)TestAstCodegen.o
#$(BIN)TestDeclarationAnalyzer.o $(BIN)TestExpressionAnalyzer.o $(BIN)TestLexicalAnalyzer.o $(BIN)TestStatementAnalyzer.o

all:	
	mkdir -p $(BIN)
	$(CPP)	-o $(BIN)CSharpCodegen.o				-c CSharpCodegen.cpp
	$(CPP)	-o $(BIN)TestAstCodegen.o				-c TestAstCodegen.cpp
	#$(CPP)	-o $(BIN)TestDeclarationAnalyzer.o			-c TestDeclarationAnalyzer.cpp
	#$(CPP)	-o $(BIN)TestExpressionAnalyzer.o			-c TestExpressionAnalyzer.cpp
	#$(CPP)	-o $(BIN)TestLexicalAnalyzer.o				-c TestLexicalAnalyzer.cpp
	#$(CPP)	-o $(BIN)TestStatementAnalyzer.o			-c TestStatementAnalyzer.cpp
	$(CPP)	-o $(BIN)UnitTest.o					-c UnitTest.cpp
	$(CPP)	-o $(BIN)Main.o						-c Main.cpp
	$(CPP)	-o $(BIN)Tinymoe.o					-c $(TIN)Tinymoe.cpp
	$(CPP)	-o $(BIN)TinymoeAst.o					-c $(AST)TinymoeAst.cpp
	$(CPP)	-o $(BIN)TinymoeAst_CollectSideEffectExpressions.o	-c $(AST)TinymoeAst_CollectSideEffectExpressions.cpp
	$(CPP)	-o $(BIN)TinymoeAst_CollectUsedVariables.o		-c $(AST)TinymoeAst_CollectUsedVariables.cpp
	$(CPP)	-o $(BIN)TinymoeAst_ExpandBlock.o			-c $(AST)TinymoeAst_ExpandBlock.cpp
	$(CPP)	-o $(BIN)TinymoeAst_GetRootLeftValue.o			-c $(AST)TinymoeAst_GetRootLeftValue.cpp
	$(CPP)	-o $(BIN)TinymoeAst_Print.o				-c $(AST)TinymoeAst_Print.cpp
	$(CPP)	-o $(BIN)TinymoeAst_RemoveUnnecessaryVariables.o	-c $(AST)TinymoeAst_RemoveUnnecessaryVariables.cpp
	$(CPP)	-o $(BIN)TinymoeAst_RoughlyOptimize.o			-c $(AST)TinymoeAst_RoughlyOptimize.cpp
	$(CPP)	-o $(BIN)TinymoeAst_SetParent.o				-c $(AST)TinymoeAst_SetParent.cpp
	$(CPP)	-o $(BIN)TinymoeAstCodegen.o				-c $(COM)TinymoeAstCodegen.cpp
	$(CPP)	-o $(BIN)TinymoeAstCodegen_Declaration.o		-c $(COM)TinymoeAstCodegen_Declaration.cpp
	$(CPP)	-o $(BIN)TinymoeAstCodegen_Expression.o			-c $(COM)TinymoeAstCodegen_Expression.cpp
	$(CPP)	-o $(BIN)TinymoeAstCodegen_Statement.o			-c $(COM)TinymoeAstCodegen_Statement.cpp
	$(CPP)	-o $(BIN)TinymoeDeclarationAnalyzer.o			-c $(COM)TinymoeDeclarationAnalyzer.cpp
	$(CPP)	-o $(BIN)TinymoeExpressionAnalyzer.o			-c $(COM)TinymoeExpressionAnalyzer.cpp
	$(CPP)	-o $(BIN)TinymoeLexicalAnalyzer.o			-c $(COM)TinymoeLexicalAnalyzer.cpp
	$(CPP)	-o $(BIN)TinymoeStatementAnalyzer.o			-c $(COM)TinymoeStatementAnalyzer.cpp
	$(CPP)	-o $(BIN)UnitTest $(TIN_OBJS) $(AST_OBJS) $(COM_OBJS) $(UNITTEST_OBJS) $(TESTCASE_OBJS)

clean:
	rm $(BIN)*
