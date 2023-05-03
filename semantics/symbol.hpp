#ifndef __SYMBOL_HPP__
#define __SYMBOL_HPP__

#include <vector>
#include <map>
#include <iostream>
#include <cstring>


enum Datatype { TYPE_int, TYPE_bool, TYPE_char, TYPE_nothing };
enum IdType { TYPE_var, TYPE_array, TYPE_function };


inline std::ostream& operator<<(std::ostream &out, Datatype t) {
  switch (t) {
    case TYPE_int: out << "int"; break;
    case TYPE_bool: out << "bool"; break;
    case TYPE_char: out << "char"; break;
    case TYPE_nothing: out << "nothing"; break;
  }
  return out;
}


static void showSemanticError(int errorCode, int line, char* op, Datatype expected=TYPE_nothing, Datatype given=TYPE_nothing) {
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
    printf("Type mismatch in assignment: ");
    std::cout<<"\033[1;35m"<<expected<<"\033[0m <- \033[1;35m"<<given<<"\033[0m";
  } else if (errorCode == 10) {
    std::cerr << "Variable " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " is declared multiple times under the same scope.";
  } else if (errorCode == 11) {
    std::cerr << "Unknown variable " ;
    printf("\033[1;35m%s\033[0m", op);
  } else if (errorCode == 12) {
    std::cerr << "Function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " is defined multiple times.";
  } else if (errorCode == 13) {
    std::cerr << "Non void function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " called as a statement.";
  } else if (errorCode == 14) {
    std::cerr << "Function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " called with wrong number of arguments.";
  } else if (errorCode == 15) {
    std::cerr << "Parameter " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " does not match the type of the argument.";
    std::cout<<" Expected \033[1;35m"<<expected<<"\033[0m instead of \033[1;35m"
             <<given<<"\033[0m";
  } else if (errorCode == 16) {
    std::cerr << "Function \033[1;35m"<<op<<"\033[0m with a return type \033[1;35m"<<expected
              <<"\033[0m returns \033[1;35m"<< given<<"\033[0m";
    std::cerr << ".";
  } else if (errorCode == 17) {
    std::cerr << "Function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " must return \033[1;35m"<<expected<<"\033[0m.";
  } else if (errorCode == 18) {
    std::cerr << "Undefined function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << ".";
  }
  printf(" -- line: ");
  printf("\033[1;36m%d\n\033[0m", line);
  printf("\n- Compilation \033[1;31mFAILED\033[0m.\n");           
  exit(1);
}


class SymbolEntry {
public:
    SymbolEntry() {}
    virtual ~SymbolEntry() {}
    Datatype getDatatype() {
        return datatype;
    }
    IdType getType() {
        return type;
    };
    virtual std::vector<Datatype> getParameterTypes() = 0;
    virtual void setDefined() = 0;
    virtual bool checkDefined() = 0;

protected:
    Datatype datatype;
    IdType type;
};

class FunctionParameter : public SymbolEntry {
public:
    FunctionParameter(char *n, Datatype t, bool isArray, std::vector<int> d = {}) : name(n), isArray(isArray), dimensions(d) { datatype = t; }
    virtual std::vector<Datatype> getParameterTypes() override {
        return {};
    };
    virtual void setDefined() override {}
    virtual bool checkDefined() override {
        return true;
    }
private:
    char *name;
    bool isArray;
    std::vector<int> dimensions;
};

class VarEntry : public SymbolEntry
{
public:
    VarEntry() {}
    VarEntry(Datatype t) { 
        datatype = t;
        type = TYPE_var;
    }
    virtual std::vector<Datatype> getParameterTypes() override {
        return {};
    };
    virtual void setDefined() override {}
    virtual bool checkDefined() override {
        return true;
    }
};

class ArrayEntry : public SymbolEntry {
public:
    ArrayEntry(Datatype t, int n, std::vector<int> d) : num_dimensions(n), dimensions(d) { 
        datatype = t;
        type = TYPE_array;
    }
    virtual std::vector<Datatype> getParameterTypes() override {
        return {};
    };
    virtual void setDefined() override {}
    virtual bool checkDefined() override {
        return true;
    }

private:
    int num_dimensions;
    std::vector<int> dimensions;
};

class FunctionEntry : public SymbolEntry {
public:
    FunctionEntry(Datatype t, int n, std::vector<FunctionParameter> p, bool isDefined) : num_parameters(n), parameters(p), isDefined(isDefined) { 
        datatype = t; 
        type = TYPE_function;
    }
    virtual std::vector<Datatype> getParameterTypes() override {
        std::vector<Datatype> v = {};
        for (int i = 0; i < num_parameters; ++i)
        {
            v.push_back(parameters[i].getDatatype());
        }
        return v;
    }
    virtual void setDefined() override {
        isDefined = true;
    }
    virtual bool checkDefined() override {
        return isDefined;
    }
private:
    int num_parameters;
    std::vector<FunctionParameter> parameters;
    bool isDefined;
};

class Scope {
public:
    Scope() : locals(), functions() {}
    SymbolEntry * lookup(char *c, std::vector<IdType> type) {
        if (type[0] == TYPE_function) {
            if (functions.find(c) == functions.end()) return nullptr;
            return functions[c];
        }
        if (locals.find(c) == locals.end()) return nullptr;
        return locals[c];
    }
    void insertVar(char *c, Datatype t, int line) {
        if (locals.find(c) != locals.end()) {
            showSemanticError(10, line, c);
        }
        locals[c] = new VarEntry(t);
    }
    void insertArray(char *c, Datatype t, std::vector<int> n, int line) {
        if (locals.find(c) != locals.end()) {
            showSemanticError(10, line, c);
        }
        locals[c] = new ArrayEntry(t, n.size(), n);
    }
    void insertFunction(char *c, Datatype t, std::vector<FunctionParameter> n, int line, bool isDefined) {
        functions[c] = new FunctionEntry(t, n.size(), n, isDefined);
    }
    void addNameAndType(char *n, Datatype t) {
        name = n;
        type = t;
    }
    Datatype getScopeDatatype() {
        return type; 
    }
    char* getScopeName() {
        return const_cast<char*>(name); 
    }
    void setReturned() {
        isReturned = true;
    }
    bool checkReturned() {
        return isReturned;
    }
private:
    std::map<std::string, SymbolEntry *> locals;
    std::map<std::string, FunctionEntry *> functions;
    char *name;
    Datatype type;
    bool isReturned = false;
};

class SymbolTable {
public:
    void openScope() {
        scopes.push_back(Scope());
    }
    void closeScope(int line) { 
        if (scopes.back().checkReturned() == false && scopes.back().getScopeDatatype() != TYPE_nothing) {
            showSemanticError(17, line, scopes.back().getScopeName(), scopes.back().getScopeDatatype());
        }
        scopes.pop_back(); 
    };
    SymbolEntry * lookup(char *c, int line, std::vector<IdType> type) {
        for (auto i = scopes.rbegin(); i != scopes.rend(); ++i) {
            SymbolEntry * e = i->lookup(c, type);
            if (e != nullptr) {
                return e;
            }
        }
        int code = 11;
        if (type[0] == TYPE_function)
            code = 18;
        showSemanticError(code, line, c);
        return new VarEntry();
        }
    void insertVar(char *c, Datatype t, int line) {
        scopes.back().insertVar(c, t, line); 
    }
    void insertArray(char *c, Datatype t, std::vector<int> n, int line) { 
        scopes.back().insertArray(c, t, n, line); 
    }
    void insertFunction(char *c, Datatype t, std::vector<FunctionParameter> n, int line, bool isDefined) { 
        for (auto i = scopes.rbegin(); i != scopes.rend(); ++i) {
            SymbolEntry * e = i->lookup(c, {TYPE_function});
            if (e != nullptr && e->checkDefined() == false && isDefined == true) {
                e->setDefined();
            } else if (e != nullptr && isDefined == false) {
                showSemanticError(12, line, c);
            }
        }
        scopes.back().insertFunction(c, t, n, line, isDefined); 
    }
    void insertFunctionToPreviousScope(char *c, Datatype t, std::vector<FunctionParameter> n, int line) {
        for (auto i = scopes.rbegin(); i != scopes.rend(); ++i) {
            SymbolEntry * e = i->lookup(c, {TYPE_function});
            if (e != nullptr && e->checkDefined() == true) {
                showSemanticError(12, line, c);
            }
        }
        if (scopes.size() > 1) {
            scopes[scopes.size() - 2].insertFunction(c, t, n, line, true);
        } else {
            scopes.back().insertFunction(c, t, n, line, true); 
            scopes.back().insertFunction(const_cast<char *>("writeString"), TYPE_nothing, {FunctionParameter(const_cast<char *>("n"), TYPE_char, false)}, line, true); 
            scopes.back().insertFunction(const_cast<char *>("writeInteger"), TYPE_nothing, {FunctionParameter(const_cast<char *>("n"), TYPE_int, false)}, line, true); 
            scopes.back().insertFunction(const_cast<char *>("readInteger"), TYPE_int, {}, line, true); 
        }
    }
    void addScopeNameAndType(char* c, Datatype t) {
        scopes.back().addNameAndType(c, t); 
    }
    Datatype getCurrentScopeDatatype() {
        return scopes.back().getScopeDatatype(); 
    }
    char* getCurrentScopeName() {
        return scopes.back().getScopeName(); 
    }
    void setCurrentScopeReturned() {
        scopes.back().setReturned();
    }
    void setCurrentFunctionDefined() {
        if (scopes.size() > 1) {
            scopes[scopes.size() - 2].lookup(getCurrentScopeName(), {TYPE_function})->setDefined();
        }
    }
private:
    std::vector<Scope> scopes;
};

extern SymbolTable st;


#endif