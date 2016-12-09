Compilateur-L_AC: Release/runtime Release/compiler Release/interpreter

Release/interpreter: mode_calculatrice.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c
	gcc -Wall -o Release/interpreter mode_calculatrice.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c

Release/compiler: compiler.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c
	gcc -Wall -o Release/compiler compiler.c processeur.c common_component.c analyse_lexical.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c

Release/runtime: runtime.c processeur.c common_component.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c
	gcc -Wall -o Release/runtime runtime.c processeur.c common_component.c Syntax_calculate/BNF_C.c Syntax_calculate/calculate.c

clean: 
	rm Release/runtime Release/compiler Release/interpreter