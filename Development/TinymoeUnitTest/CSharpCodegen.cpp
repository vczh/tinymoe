#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;
using namespace tinymoe::ast;

class CSharpNameResolver
{
private:
	map<AstDeclaration*, AstDeclaration*>		declScopes;
	map<AstDeclaration*, string>				resolvedNames;
	map<pair<AstDeclaration*, string>, int>		scopedAppearCount;
public:
	void Scope(AstDeclaration* decl, AstDeclaration* scope)
	{
		declScopes.insert(make_pair(decl, scope));
	}

	string Resolve(AstDeclaration* decl)
	{
		auto it = resolvedNames.find(decl);
		if (it == resolvedNames.end())
		{
			stringstream ss;
			for (auto c : decl->composedName)
			{
				if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9' || c == '_')
				{
					ss << c;
				}
				else
				{
					ss << '_';
				}
			}
			string name = ss.str();

			auto scope = declScopes.find(decl)->second;
			auto scopeName = make_pair(scope, name);
			auto itappear = scopedAppearCount.find(scopeName);
			if (itappear == scopedAppearCount.end())
			{
				scopedAppearCount.insert(make_pair(scopeName, 1));
			}
			else
			{
				int counter = ++itappear->second;
				char buffer[20] = { 0 };
				_itoa_s(counter, buffer, 10);
				name += string("_x") + buffer;
			}
			resolvedNames.insert(make_pair(decl, name));
			return name;
		}
		else
		{
			return it->second;
		}
	}
};

class CSharpTypeCodegen :public AstTypeVisitor
{
public:
	CSharpNameResolver&		resolver;
	string					result;

	CSharpTypeCodegen(CSharpNameResolver& _resolver)
		:resolver(_resolver)
	{
	}

	void Visit(AstPredefinedType* node)override
	{
		switch (node->typeName)
		{
		case AstPredefinedTypeName::Object:
			result = "TinymoeObject";
			break;
		case AstPredefinedTypeName::Symbol:
			result = "TinymoeSymbol";
			break;
		case AstPredefinedTypeName::Array:
			result = "TinymoeArray";
			break;
		case AstPredefinedTypeName::Boolean:
			result = "TinymoeBoolean";
			break;
		case AstPredefinedTypeName::Integer:
			result = "TinymoeInteger";
			break;
		case AstPredefinedTypeName::Float:
			result = "TinymoeFloat";
			break;
		case AstPredefinedTypeName::String:
			result = "TinymoeString";
			break;
		case AstPredefinedTypeName::Function:
			result = "TinymoeFunction";
			break;
		}
	}

	void Visit(AstReferenceType* node)override
	{
		result = resolver.Resolve(node->typeDeclaration.lock().get());
	}

	static string ToString(AstType::Ptr type, CSharpNameResolver& _resolver)
	{
		CSharpTypeCodegen codegen(_resolver);
		type->Accept(&codegen);
		return codegen.result;
	}
};

string FunctionToName(CSharpNameResolver& resolver, AstFunctionDeclaration* decl)
{
	return resolver.Resolve(decl);
}

string FunctionToTypedName(CSharpNameResolver& resolver, AstFunctionDeclaration* decl)
{
	auto methodName = FunctionToName(resolver, decl);
	return (decl->ownerType ? CSharpTypeCodegen::ToString(decl->ownerType, resolver) + "__" : "") + methodName;
}

string FunctionToValue(CSharpNameResolver& resolver, AstFunctionDeclaration* decl)
{
	stringstream ss;
	ss << "new TinymoeFunction(__args__ => " << FunctionToTypedName(resolver, decl) << "(";
	for (auto it = decl->arguments.begin(); it != decl->arguments.end(); it++)
	{
		ss << "__args__[" << it - decl->arguments.begin() << "]";
		if (it + 1 == decl->arguments.end())
		{
			ss << "))";
		}
		else
		{
			ss << ", ";
		}
	}
	return ss.str();
}

class CSharpDeclarationCodegen :public AstDeclarationVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream&				o;
	string					prefix;

	CSharpDeclarationCodegen(CSharpNameResolver& _resolver, ostream& _o, string _prefix)
		:resolver(_resolver)
		, o(_o)
		, prefix(_prefix)
	{
	}

	void Visit(AstSymbolDeclaration* node)override
	{
		resolver.Scope(node, nullptr);
		o << prefix << "public readonly TinymoeObject " << resolver.Resolve(node) << " = new TinymoeString(\"" << resolver.Resolve(node) << "\");" << endl << endl;
	}

	void Visit(AstTypeDeclaration* node)override
	{
		resolver.Scope(node, nullptr);
		o << prefix << "public class " << resolver.Resolve(node);
		if (node->baseType.expired())
		{
			o << " : TinymoeObject" << endl;
		}
		else
		{
			o << " : " << CSharpTypeCodegen::ToString(node->baseType.lock(), resolver) << endl;
		}
		o << prefix << "{" << endl;
		o << prefix << "\tpublic " << resolver.Resolve(node) << "()" << endl;
		o << prefix << "\t{" << endl;
		for (auto field : node->fields)
		{
			resolver.Scope(field.get(), node);
			o << prefix << "\t\tSetField(\"" << resolver.Resolve(field.get()) << "\", null);" << endl;
		}
		o << prefix << "\t}" << endl;
		o << prefix << "}" << endl << endl;
	}

	void Visit(AstFunctionDeclaration* node)override
	{
		resolver.Scope(node, nullptr);
		o << prefix << "public void " << FunctionToTypedName(resolver, node) << "(";
		for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
		{
			resolver.Scope(it->get(), node);
			o << "TinymoeObject " << resolver.Resolve(it->get());
			if (it + 1 == node->arguments.end())
			{
				o << ")" << endl;
			}
			else
			{
				o << ", ";
			}
		}
		o << prefix << "{" << endl;
		o << prefix << "}" << endl << endl;
	}
};

class CSharpExtensionDeclarationCodegen :public AstDeclarationVisitor
{
public:
	CSharpNameResolver&		resolver;
	ostream&				o;
	string					prefix;

	CSharpExtensionDeclarationCodegen(CSharpNameResolver& _resolver, ostream& _o, string _prefix)
		:resolver(_resolver)
		, o(_o)
		, prefix(_prefix)
	{
	}

	void Visit(AstSymbolDeclaration* node)override
	{
	}

	void Visit(AstTypeDeclaration* node)override
	{
	}

	void Visit(AstFunctionDeclaration* node)override
	{
		if (node->ownerType)
		{
			auto typeName = CSharpTypeCodegen::ToString(node->ownerType, resolver);
			auto methodName = FunctionToName(resolver, node);
			auto targetName = FunctionToValue(resolver, node);
			o << prefix << "TinymoeObject.SetExtension(" << endl;
			o << prefix << "\ttypeof(" << typeName << ")," << endl;
			o << prefix << "\t\"" << methodName << "\"," << endl;
			o << prefix << "\t" << targetName << endl;
			o << prefix << "\t); " << endl;
		}
	}
};

void GenerateCSharpCode(AstAssembly::Ptr assembly, ostream& o)
{
	CSharpNameResolver resolver;
	o << "using System;" << endl;
	o << "using System.Collections.Generic;" << endl;
	o << "using TinymoeDotNet;" << endl;
	o << "" << endl;
	o << "namespace TinymoeProgramNamespace" << endl;
	o << "{" << endl;
	o << "\tpublic class TinymoeProgram : TinymoeOperations" << endl;
	o << "\t{" << endl;
	{
		CSharpDeclarationCodegen codegen(resolver, o, "\t\t");
		for (auto decl : assembly->declarations)
		{
			decl->Accept(&codegen);
		}
	}
	o << "\t\tpublic TinymoeProgram()" << endl;
	o << "\t\t{" << endl;
	{
		CSharpExtensionDeclarationCodegen codegen(resolver, o, "\t\t\t");
		for (auto decl : assembly->declarations)
		{
			decl->Accept(&codegen);
		}
	}
	o << "\t\t}" << endl;
	o << "\t}" << endl;
	o << "}" << endl;
}