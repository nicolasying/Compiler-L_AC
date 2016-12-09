
Compilateur-L_AC: Interpreter Compiler Runtime
	$(info '-----Message-from-Nicolas-----')
	@printf "Hello. Now let's begin compiling the release version of the project.\n\n"
	@printf "The project consists of three parts: \nan interpreter, a compiler and a runtime inwhich you can run compiled files.\n\n"
	@printf " .lac is the file extension for code written in Language Alain Chillès.\n"
	@printf " .lacc is the compiled version for the code.\n"
	@printf "This project uses Standard Hugolonicolasien for interchangeability for compiled files.\n\n"
	@printf "To compile a file, e.g. factorielle.lac, run \n"
	@printf "$$: Release/compiler ./factorielle.lac\n"
	@printf "To run a compiled code in the runtime, e.g. factorielle.lacc, run\n"
	@printf "$$: Release/runtime ./factorielle.lacc\n"
	@printf "To launch a shell-like interpreter, run\n"
	@printf "$$: Release/interpreter\n\n"
	@printf "Some patience for the compilation, and enjoy.\n\n"
	@printf "You can always add a -DDEBUG flag in the gcc command to build a verbose version of the programs.\n"
	@printf "Get the verbose version by running\n$$: make verbose\n"

Interpreter: mode_calculatrice.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c
	@gcc -Wall -o Release/interpreter mode_calculatrice.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c

Compiler: compiler.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c
	@gcc -Wall -o Release/compiler compiler.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c

Runtime: runtime.c processeur.c common_component.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c
	@gcc -Wall -o Release/runtime runtime.c processeur.c common_component.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c

verbose: 
	@gcc -DDEBUG -Wall -g -o Debug/interpreter mode_calculatrice.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c
	@gcc -DDEBUG -Wall -g -o Debug/compiler compiler.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c
	@gcc -DDEBUG -Wall -g -o Debug/runtime runtime.c processeur.c common_component.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c
	@printf "Verbose version compiled in Debug folder.\n"

clean: 
	rm Release/runtime Release/compiler Release/interpreter