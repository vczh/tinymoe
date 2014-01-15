#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;
using namespace tinymoe::ast;

void GenerateCSharpCode(AstAssembly::Ptr assembly, ostream& o)
{
	o << "using System;" << endl;
	o << "using System.Collections.Generic;" << endl;
	o << "using TinymoeDotNet;" << endl;
	o << "" << endl;
	o << "namespace TinymoeProgramNamespace" << endl;
	o << "{" << endl;
	o << "	public class TinymoeProgram : TinymoeOperations" << endl;
	o << "	{" << endl;
	o << "	}" << endl;
	o << "}" << endl;
}