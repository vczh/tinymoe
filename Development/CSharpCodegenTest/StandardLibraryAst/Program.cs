using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TinymoeDotNet;
using TinymoeProgramNamespace;

namespace StandardLibraryAst
{
    class Program
    {
        static void Main(string[] args)
        {
            var program = new TinymoeProgram();
            var continuation = new TinymoeFunction((TinymoeObject[] arguments) =>
            {
            });
            var trap = new TinymoeProgram.standard_library__continuation_trap();
            trap.SetField("continuation", continuation);
            var state = new TinymoeProgram.standard_library__continuation_state();
            state.SetField("trap", trap);
            program.hello_world__main(state, continuation);
        }
    }
}
