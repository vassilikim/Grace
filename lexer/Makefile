lexer: lexer.cpp
	g++ -o lexer lexer.cpp

lexer.cpp: lexer.l
	flex -o lexer.cpp lexer.l 

clean: 
	rm lexer.cpp lexer