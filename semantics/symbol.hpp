#ifndef __SYMBOL_HPP__
#define __SYMBOL_HPP__

#include <vector>
#include <map>
#include <iostream>
#include <cstring>

static void showSemanticError(int errorCode, int line, char* op) {
  printf("\033[1;31merror:\n\t\033[0m");
  if (errorCode == 1) {
    printf("The size of an array must be an integer.");
  } else if (errorCode == 2) {
    printf("The parameters in an array call must be integers.");
  } else if (errorCode == 3) {
    printf("The operator \'");
    printf("\033[1;35m%s\033[0m", op);
    printf("\' must be used with a integer.");
  } else if (errorCode == 4) {
    printf("Logic operator \'");
    printf("\033[1;35m%s\033[0m", op);
    printf("\' must apply on conditions.");
  } else if (errorCode == 5) {
    printf("Arithmetic operator \'");
    printf("\033[1;35m%s\033[0m", op);
    printf("\' must apply on integers.");
  } else if (errorCode == 6) {
    printf("Logic operator \'");
    printf("\033[1;35m%s\033[0m", op);
    printf("\' must apply on variables or constants of the same type.");
  } else if (errorCode == 7) {
    printf("If-statements must start with a condition.");
  } else if (errorCode == 8) {
    printf("While-statements must start with a condition.");
  } else if (errorCode == 9) {
    printf("Type mismatch in assignment.");
  } else if (errorCode == 10) {
    std::cerr << "Variable " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " is declared multiple times under the same scope.";
  } else if (errorCode == 11) {
    std::cerr << "Unknown variable " ;
    printf("\033[1;35m%s\033[0m", op);
  }
  printf(" -- line: ");
  printf("\033[1;36m%d\n\033[0m", line);
  exit(1);
}

enum Datatype { TYPE_int, TYPE_bool, TYPE_char, TYPE_string, TYPE_nothing };

class SymbolEntry {
public:
    SymbolEntry() {}
    Datatype getType() {
        return type;
    }
protected:
    Datatype type;
};

class FunctionParameter : public SymbolEntry {
public:
    FunctionParameter(char *n, Datatype t, bool isArray, std::vector<int> d = {}) : name(n), isArray(isArray), dimensions(d) { type = t; }

private:
    char *name;
    bool isArray;
    std::vector<int> dimensions;
};

class VarEntry : public SymbolEntry
{
public:
    VarEntry(Datatype t) { type = t; }
};

class ArrayEntry : public SymbolEntry {
public:
    ArrayEntry(Datatype t, int n, std::vector<int> d) : num_dimensions(n), dimensions(d) { type = t; }

private:
    int num_dimensions;
    std::vector<int> dimensions;
};

class FunctionEntry : public SymbolEntry {
public:
    FunctionEntry(Datatype t, int n, std::vector<FunctionParameter> p) : num_parameters(n), parameters(p) { type = t; }

private:
    int num_parameters;
    std::vector<FunctionParameter> parameters;
};

class Scope {
public:
    Scope() : locals() {}
    SymbolEntry * lookup(char *c) {
        if (locals.find(c) == locals.end()) return nullptr;

        return &locals[c];
    }
    void insertVar(char *c, Datatype t, int line) {
        if (locals.find(c) != locals.end()) {
            showSemanticError(10, line, c);
        }
        locals[c] = VarEntry(t);
    }
    void insertArray(char *c, Datatype t, std::vector<int> n, int line) {
        if (locals.find(c) != locals.end()) {
            showSemanticError(10, line, c);
        }
        locals[c] = ArrayEntry(t, n.size(), n);
    }
    void insertFunction(char *c, Datatype t, std::vector<FunctionParameter> n, int line) {
        if (locals.find(c) != locals.end()) {
            showSemanticError(10, line, c);
        }
        locals[c] = FunctionEntry(t, n.size(), n);
    }
    void addNameAndType(char *n, Datatype t) {
        name = n;
        type = t;
    }
private:
    std::map<std::string, SymbolEntry> locals;
    char *name;
    Datatype type;
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
        showSemanticError(11, line, c);
        return new SymbolEntry();
        }
    void insertVar(char *c, Datatype t, int line) {
        scopes.back().insertVar(c, t, line); 
    }
    void insertArray(char *c, Datatype t, std::vector<int> n, int line) { 
        scopes.back().insertArray(c, t, n, line); 
    }
    void insertFunction(char *c, Datatype t, std::vector<FunctionParameter> n, int line) { 
        scopes.back().insertFunction(c, t, n, line); 
    }
    void insertFunctionToPreviousScope(char *c, Datatype t, std::vector<FunctionParameter> n, int line) {
        if (scopes.size() > 1) {
            scopes[scopes.size() - 2].insertFunction(c, t, n, line);
        }
    }
    void addScopeNameAndType(char* c, Datatype t) {
        scopes.back().addNameAndType(c, t); 
    }
private:
    std::vector<Scope> scopes;
};

extern SymbolTable st;


#endif