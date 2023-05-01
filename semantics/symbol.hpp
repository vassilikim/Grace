#ifndef __SYMBOL_HPP__
#define __SYMBOL_HPP__

#include <vector>
#include <map>
#include <iostream>
#include <cstring>

enum Datatype { TYPE_int, TYPE_bool, TYPE_char, TYPE_string, TYPE_nothing };

class SymbolEntry {
public:
    SymbolEntry(Datatype t) : type(t) {}
    SymbolEntry() {}
private : 
    Datatype type;
};

class Scope {
public:
    Scope() : locals() {}
    SymbolEntry * lookup(char *c) {
        if (locals.find(c) == locals.end()) return nullptr;
        return &locals[c];
    }
    void insert(char *c, Datatype t) {
        if (locals.find(c) != locals.end()) {
            printf("\033[1;31merror:\n\t\033[0m");
            std::cerr << "Variable " ;
            printf("\033[1;35m%s\033[0m", c);
            std::cerr << " is declared multiple times under the same scope.\n";
            exit(1);
        }
        locals[c] = SymbolEntry(t);
    }
private:
    std::map<std::string, SymbolEntry> locals;
};

class SymbolTable {
public:
    void openScope() {
        scopes.push_back(Scope());
    }
    void closeScope() { 
        scopes.pop_back(); 
    };
    SymbolEntry * lookup(char *c, int line) {
        for (auto i = scopes.rbegin(); i != scopes.rend(); ++i) {
            SymbolEntry * e = i->lookup(c);
            if (e != nullptr) return e;
        }
        printf("\033[1;31merror:\n\t\033[0m");
        std::cerr << "Unknown variable " ;
        printf("\033[1;35m%s\033[0m", c);
        printf(" -- line: ");
        printf("\033[1;36m%d\n\033[0m", line);
        exit(1);
    }
    void insert(char *c, Datatype t) { 
        scopes.back().insert(c, t); 
    }
private:
    std::vector<Scope> scopes;
};

extern SymbolTable st;


#endif