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
  } else if (errorCode == 19) {
    std::cerr << "Function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " is declared in two scopes.";
  } else if (errorCode == 20) {
    std::cerr << "Function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " is declared but not defined.";
  } else if (errorCode == 21) {
    std::cerr << "Function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " is already defined with expected datatype \033[1;35m"<<expected<<"\033[0m.";
  } else if (errorCode == 22) {
    std::cerr << "Function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " is already defined with different parameters";
  } else if (errorCode == 23) {
    std::cerr << "Function " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " must have type void and no parameters.";
  } else if (errorCode == 24) {
    std::cerr << "Array parameter " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " must be a reference.";
  } else if (errorCode == 25) {
    std::cerr << "Array " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " has wrong number of dimensions.";
  } else if (errorCode == 26) {
    std::cerr << "String " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " cannot be assigned a value.";
  } else if (errorCode == 27) {
    std::cerr << "String " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " cannot be assigned as a value.";
  } else if (errorCode == 28) {
    std::cerr << "Variable " ;
    printf("\033[1;35m%s\033[0m", op);
    std::cerr << " is not an array.";
  }
  printf(" -- line: ");
  printf("\033[1;36m%d\n\033[0m", line);
  printf("- Compilation \033[1;31mFAILED\033[0m.\n");           
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
    int getNumOfDimensions() {
        return numdim;
    }
    std::vector<int> getDimensions() {
        return dim;
    }
    virtual std::vector<Datatype> getParameterDatatypes() = 0;
    virtual std::vector<char *> getParameterNames() = 0;
    virtual std::vector<bool> getParameterRefs() = 0;
    virtual std::vector<IdType> getParameterTypes() = 0;
    virtual std::vector<int> getParameterNumOfDimensions() = 0;
    virtual std::vector<std::vector<int>> getParameterDimensions() = 0;
    virtual void setDefined() = 0;
    virtual bool checkDefined() = 0;

protected:
    Datatype datatype;
    IdType type;
    int numdim;
    std::vector<int> dim;
};

class FunctionParameter : public SymbolEntry {
public:
    FunctionParameter(char *n, Datatype t, bool isArray, bool isRef, std::vector<int> d = {}) : name(n), isArray(isArray), isRef(isRef), dimensions(d) { 
        datatype = t; 
        if (isArray == true) {
            type = TYPE_array;
        } else {
            type = TYPE_var;
        }
        numdim = d.size();
        dim = d;
    }
    virtual std::vector<Datatype> getParameterDatatypes() override {
        return {};
    };
    virtual std::vector<char *> getParameterNames() override {
        return {};
    };
    virtual std::vector<bool> getParameterRefs() override {
        return {};
    };
    virtual std::vector<IdType> getParameterTypes() override {
        return {};
    };
    virtual std::vector<int> getParameterNumOfDimensions() override {
        return {};
    };
    virtual std::vector<std::vector<int>> getParameterDimensions() override {
        return {};
    };
    virtual void setDefined() override {}
    virtual bool checkDefined() override {
        return true;
    }
    char * getName() {
        return name;
    }
    bool getRef() {
        return isRef;
    }
private:
    char *name;
    bool isArray;
    bool isRef;
    std::vector<int> dimensions;
};

class VarEntry : public SymbolEntry
{
public:
    VarEntry() {}
    VarEntry(Datatype t) { 
        datatype = t;
        type = TYPE_var;
        numdim = 0;
        dim = {};
    }
    virtual std::vector<Datatype> getParameterDatatypes() override {
        return {};
    };
    virtual std::vector<char *> getParameterNames() override {
        return {};
    };
    virtual std::vector<bool> getParameterRefs() override {
        return {};
    };
    virtual std::vector<IdType> getParameterTypes() override {
        return {};
    };
    virtual std::vector<int> getParameterNumOfDimensions() override {
        return {};
    };
    virtual std::vector<std::vector<int>> getParameterDimensions() override {
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
        numdim = n;
        dim = d;
    }
    virtual std::vector<Datatype> getParameterDatatypes() override {
        return {};
    };
    virtual std::vector<char *> getParameterNames() override {
        return {};
    };
    virtual std::vector<bool> getParameterRefs() override {
        return {};
    };
    virtual std::vector<IdType> getParameterTypes() override {
        return {};
    };
    virtual std::vector<int> getParameterNumOfDimensions() override {
        return {};
    };
    virtual std::vector<std::vector<int>> getParameterDimensions() override {
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
        numdim = 0;
        dim = {};
    }
    virtual std::vector<Datatype> getParameterDatatypes() override {
        std::vector<Datatype> v = {};
        for (int i = 0; i < num_parameters; ++i)
        {
            v.push_back(parameters[i].getDatatype());
        }
        return v;
    }
    virtual std::vector<char *> getParameterNames() override {
        std::vector<char *> v = {};
        for (int i = 0; i < num_parameters; ++i)
        {
            v.push_back(parameters[i].getName());
        }
        return v;
    };
    virtual std::vector<bool> getParameterRefs() override {
        std::vector<bool> v = {};
        for (int i = 0; i < num_parameters; ++i)
        {
            v.push_back(parameters[i].getRef());
        }
        return v;
    };
    virtual std::vector<IdType> getParameterTypes() override {
        std::vector<IdType> v = {};
        for (int i = 0; i < num_parameters; ++i)
        {
            v.push_back(parameters[i].getType());
        }
        return v;
    };
    virtual std::vector<int> getParameterNumOfDimensions() override {
        std::vector<int> v = {};
        for (int i = 0; i < num_parameters; ++i)
        {
            v.push_back(parameters[i].getNumOfDimensions());
        }
        return v;
    };
    virtual std::vector<std::vector<int>> getParameterDimensions() override {
        std::vector<std::vector<int>> v = {};
        for (int i = 0; i < num_parameters; ++i)
        {
            v.push_back(parameters[i].getDimensions());
        }
        return v;
    };
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
    void allFunctionsDefined(int line) {
        for (const auto& f : functions) {
            if (!f.second->checkDefined()) {
                return showSemanticError(20, line, strdup(f.first.c_str()));
            }
        }
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
        scopes.back().allFunctionsDefined(line);
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
        for (int i = 0; i < int(n.size()); i++) {
            if (n[i].getType() == TYPE_array && n[i].getRef() == false) {
                showSemanticError(24, line, n[i].getName());
            }
        }

        for (auto i = scopes.rbegin(); i != scopes.rend(); ++i) {
            SymbolEntry * e = i->lookup(c, {TYPE_function});
            if (e != nullptr && e->getDatatype() != t) {
                showSemanticError(21, line, c, e->getDatatype());
            } else if (e != nullptr && n.size() != e->getParameterDatatypes().size()) {
                showSemanticError(22, line, c);
            } else if (e != nullptr) {
                for (int i = 0; i < int(n.size()); i++) {
                    if (e->getParameterDatatypes()[i] != n[i].getDatatype() || e->getParameterTypes()[i] != n[i].getType() || std::string(e->getParameterNames()[i]) != std::string(n[i].getName()) || e->getParameterRefs()[i] != n[i].getRef() || e->getParameterNumOfDimensions()[i] != n[i].getNumOfDimensions()) {
                        showSemanticError(22, line, c);
                    } else {
                        std::vector<int> dimParam1 = e->getParameterDimensions()[i];
                        std::vector<int> dimParam2 = n[i].getDimensions();
                        for (int j = 0; j < int(dimParam1.size()); j++) {
                            if (dimParam1[j] != dimParam2[j]) {
                                showSemanticError(22, line, c);
                            }
                        }
                    }
                }
            }
            if (e != nullptr && e->checkDefined() == false && isDefined == true) {
                if (i->getScopeName() == getCurrentScopeName()) {
                    e->setDefined();
                    return;
                } else {
                    showSemanticError(19, line, c);
                }
            } else if (e != nullptr && isDefined == false) {
                showSemanticError(12, line, c);
            }
        }
        scopes.back().insertFunction(c, t, n, line, isDefined); 
    }
    void insertFunctionToPreviousScope(char *c, Datatype t, std::vector<FunctionParameter> n, int line) {
        if (scopes.size() == 1 && (t != TYPE_nothing || n.size() > 0)) {
            showSemanticError(23, line, c);
        }

        for (int i = 0; i < int(n.size()); i++) {
            if (n[i].getType() == TYPE_array && n[i].getRef() == false) {
                showSemanticError(24, line, n[i].getName());
            }
        }

        for (auto i = scopes.rbegin(); i != scopes.rend(); ++i) {
            SymbolEntry * e = i->lookup(c, {TYPE_function});
            if (e != nullptr && e->getDatatype() != t) {
                showSemanticError(21, line, c, e->getDatatype());
            } else if (e != nullptr && n.size() != e->getParameterDatatypes().size()) {
                showSemanticError(22, line, c);
            } else if (e != nullptr) {
                for (int i = 0; i < int(n.size()); i++) {
                    if (e->getParameterDatatypes()[i] != n[i].getDatatype() || e->getParameterTypes()[i] != n[i].getType() || std::string(e->getParameterNames()[i]) != std::string(n[i].getName()) || e->getParameterRefs()[i] != n[i].getRef() || e->getParameterNumOfDimensions()[i] != n[i].getNumOfDimensions()) {
                        showSemanticError(22, line, c);
                    } else {
                        std::vector<int> dimParam1 = e->getParameterDimensions()[i];
                        std::vector<int> dimParam2 = n[i].getDimensions();
                        for (int j = 0; j < int(dimParam1.size()); j++) {
                            if (dimParam1[j] != dimParam2[j]) {
                                showSemanticError(22, line, c);
                            }
                        }
                    }
                }
            }
            if (e != nullptr && e->checkDefined() == false) {
                if (scopes.size() > 1 && scopes[scopes.size()-2].getScopeName() != i->getScopeName()) {
                    showSemanticError(19, line, c);
                } else {
                    e->setDefined();
                    return;
                }
            } else if (e != nullptr && e->checkDefined() == true) {
                showSemanticError(12, line, c);
            }
        }
        if (scopes.size() > 1) {
            scopes[scopes.size() - 2].insertFunction(c, t, n, line, true);
        } else {
            scopes.back().insertFunction(c, t, n, line, true); 
            scopes.back().insertFunction(const_cast<char *>("writeChar"), TYPE_nothing, {FunctionParameter(const_cast<char *>("c"), TYPE_char, false, false)}, line, true); 
            scopes.back().insertFunction(const_cast<char *>("writeInteger"), TYPE_nothing, {FunctionParameter(const_cast<char *>("n"), TYPE_int, false, false)}, line, true);
            scopes.back().insertFunction(const_cast<char *>("writeString"), TYPE_nothing, {FunctionParameter(const_cast<char *>("s"), TYPE_char, true, true, {0})}, line, true); 
            scopes.back().insertFunction(const_cast<char *>("readInteger"), TYPE_int, {}, line, true); 
            scopes.back().insertFunction(const_cast<char *>("readChar"), TYPE_char, {}, line, true);
            scopes.back().insertFunction(const_cast<char *>("readString"), TYPE_nothing, {FunctionParameter(const_cast<char *>("n"), TYPE_char, false, false), FunctionParameter(const_cast<char *>("s"), TYPE_char, true, true, {0})}, line, true);
            scopes.back().insertFunction(const_cast<char *>("ascii"), TYPE_int, {FunctionParameter(const_cast<char *>("c"), TYPE_char, false, false)}, line, true);
            scopes.back().insertFunction(const_cast<char *>("chr"), TYPE_char, {FunctionParameter(const_cast<char *>("n"), TYPE_int, false, false)}, line, true);
            scopes.back().insertFunction(const_cast<char *>("strlen"), TYPE_int, {FunctionParameter(const_cast<char *>("s"), TYPE_char, true, true, {0})}, line, true);
            scopes.back().insertFunction(const_cast<char *>("strcmp"), TYPE_int, {FunctionParameter(const_cast<char *>("s1"), TYPE_char, true, true, {0}), FunctionParameter(const_cast<char *>("s2"), TYPE_char, true, true, {0})}, line, true);
            scopes.back().insertFunction(const_cast<char *>("strcpy"), TYPE_nothing, {FunctionParameter(const_cast<char *>("trg"), TYPE_char, true, true, {0}), FunctionParameter(const_cast<char *>("n"), TYPE_char, true, true, {0})}, line, true);
            scopes.back().insertFunction(const_cast<char *>("strcat"), TYPE_nothing, {FunctionParameter(const_cast<char *>("trg"), TYPE_char, true, true, {0}), FunctionParameter(const_cast<char *>("n"), TYPE_char, true, true, {0})}, line, true);  
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