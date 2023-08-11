#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>

#include "symbol.hpp"

extern int yylineno;


//////////////////////////////////////////////////////
///Functions that maps the variables in  the memory///
/////////////////////////////////////////////////////

static std::vector<std::string> VarMap;

static void allocateVariable(std::string c){
  VarMap.push_back(c);
}

static int findInVarMap(std::string c){
  for(int i=0; i<int(VarMap.size()); i++){
    if(VarMap[i]==c) return i;
  }
  return -1;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


class AST {
public:
  virtual void printOn(std::ostream &out) const = 0;
  virtual ~AST() {}
  virtual void sem (){}
  virtual void compile(){}
};

inline std::ostream& operator<< (std::ostream &out, const AST &t) {
  t.printOn(out);
  return out;
}

class Expr: public AST {
public:
  void type_check(Datatype t, int code, int line, char* op = const_cast<char*>("")) {
    sem();
    if (type != t) {
      showSemanticError(code, line, op, t, type);
    }
  }
  Datatype type_check_return(std::vector<Datatype> t, int code, int line, char* op = const_cast<char*>("")) {
    sem();
    bool isValid = false;
      for(Datatype tt : t) {
          if (tt == type) {
              isValid = true;
              break;
          }
      }
    if (isValid == false) {
      showSemanticError(code, line, op);
    }
    return type;
  }
  virtual char *getName() {
    return const_cast<char *>("");
  }
  virtual SymbolEntry * getSymbolEntry() {
    return new VarEntry();
  }
protected:
  Datatype type;
};

class Stmt: public AST {

};

class Func: public AST {

};

class Const: public Expr {
public:
  Const(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Const(" << num << ")";
  }
  virtual void sem() override {
    type = TYPE_int;
  }

  virtual void compile() override{
    std::cout<<"  pushl $"<<num<<std::endl;
  }

private:
  int num;
};


class ConstList: public Expr {
public:
  ConstList(): const_list() {}
  void append(int n) { const_list.push_back(n); }
  void putinfront(int n) { const_list.insert(const_list.begin(), n); }
  int length() { return const_list.size(); }
  virtual void printOn(std::ostream &out) const override {
    out << "ConstList(";
    bool first = true;
    for (int n: const_list) {
      if (!first) out << ", ";
      first = false;
      out << n;
    }
    out << ")";
  }
  std::vector<int> getConstList() {
    std::vector<int> v;
    for (int c : const_list) {
      v.push_back(c);
    }
    return v;
  }
private:
  std::vector<int> const_list;
};

class Type: public Func {
public:
  Type(){}
  Type(Datatype s, ConstList *c = nullptr): type(s), const_list(c) {}
  ~Type() { delete const_list; }
  virtual void printOn(std::ostream &out) const override {
    out << "Type(" << type;
    if (const_list->length() != 0) out << ", " << *const_list;
    out << ")";
  }
  Datatype getType() {
    return type;
  }
  std::vector<int> getConstList() {
    if (const_list != nullptr)
      return const_list->getConstList();
    return {};
  }
private:
  Datatype type;
  ConstList *const_list;
};


// Expr 

class Id: public Expr {
public:
  Id(char *s, int l) : str(s), line(l) {}
  ~Id() { delete str; }
  virtual void printOn(std::ostream &out) const override {
    out << "Id(" << str << ")";
  }
  virtual void sem() override {
    SymbolEntry * e = st.lookup(str, line, {TYPE_var, TYPE_array});
    type = e->getDatatype();
  }
  virtual char *getName() override {
    return str;
  }
  virtual SymbolEntry *getSymbolEntry() override {
    return st.lookup(str, line, {TYPE_var, TYPE_array});
  }

  virtual void compile() override{
    std::cout<<"  pushl "<< 4*findInVarMap(str) <<"(%edi)\n";
  }

private:
  char *str;
  int line;
};

class String: public Expr {
public:
  String(char *s): str(s) {}
  ~String() { delete str; }
  virtual void printOn(std::ostream &out) const override {
    out << "String(" << str << ")";
  }
  virtual void sem() override {
    type = TYPE_char;
  }
  virtual char *getName() override {
    return str;
  }
private:
    char *str;
};

class ConstChar: public Expr {
public:
  ConstChar(char *c): constchar(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "ConstChar(" << constchar << ")";
  }
  virtual void sem() override {
    type = TYPE_char;
  }
  virtual char *getName() override {
    return constchar;
  }
private:
  char *constchar;
};

class ArrayCall: public Expr {
public:
  ArrayCall(Expr *l, Expr *r, int li): lvalue(l), expr(r), line(li) {}
  ~ArrayCall() { delete lvalue; delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "ArrayCall(" << *lvalue << ", " << *expr << ")";
  }
  virtual void sem() override {
    lvalue->sem();
    type = st.lookup(lvalue->getName(), line, {TYPE_array})->getDatatype();
    expr->type_check(TYPE_int, 2, line);
  }
  virtual char *getName() override {
    return lvalue->getName();
  }
  virtual SymbolEntry *getSymbolEntry() override {
    return st.lookup(lvalue->getName(), line, {TYPE_array});
  }
private:
    Expr *lvalue;
    Expr *expr;
    int line;
};

class Plus: public Expr {
public:
  Plus(Expr *n, int l): expr(n), line(l) {}
  ~Plus() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Plus(" << *expr << ")";
  }
  virtual void sem() override {
    expr->type_check(TYPE_int, 3, line, const_cast<char*>("+"));
    type = TYPE_int;
  }

  virtual void compile() override{
    expr->compile();
  }

private:
  Expr *expr;
  int line;
};

class Minus: public Expr {
public:
  Minus(Expr *n, int l): expr(n), line(l) {}
  ~Minus() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Minus(" << *expr << ")";
  }
  virtual void sem() override {
    expr->type_check(TYPE_int, 3, line, const_cast<char*>("-"));
    type = TYPE_int;
  }

  virtual void compile() override{
    expr->compile();
    std::cout<<"  popl %eax\n"
             <<"  negl %eax\n"
             <<"  pushl %eax\n";
  }

private:
  Expr *expr;
  int line;
};

class Not: public Expr {
public:
  Not(Expr *n, int l): expr(n), line(l) {}
  ~Not() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Not(" << *expr << ")";
  }
  virtual void sem() override {
    expr->type_check(TYPE_bool, 4, line, const_cast<char*>("not"));
    type = TYPE_bool;
  }

  virtual void compile() override{
    expr->compile();
    static int NotCondcounter = 0;
    NotCondcounter++;
    numOfNotCond= NotCondcounter;
    if(NotCondcounter<=0){
      printf("\n- \033[1;31mOVERFLOW\033[0m of the NOT condion counter.\n");
      printf("\n- Compilation \033[1;31mFAILED\033[0m.\n");
    exit(1);
    }
    std::cout<<"  popl %eax\n";
    std::cout<<"  cmpl %eax $0\n";
    std::cout<<"  je NotCondTrue_"<<numOfNotCond<<"\n";
    std::cout<<"  pushl $0\n";
    std::cout<<"  jmp NotCondEnd_"<<numOfNotCond<<"\n";
    std::cout<<"\nNotCondTrue_"<<numOfNotCond<<":\n\n";
    std::cout<<"  pushl $1\n";
    std::cout<<"\nNotCondEnd_"<<numOfNotCond<<":\n\n";

  }

private:
  Expr *expr;
  int line;
  int numOfNotCond; // uesd to know how may NotConditions we have seen to name the assembly label
};

class BinOp: public Expr {
public:
  BinOp(Expr *l, char *o, Expr *r, int li): left(l), op(o), right(r), line(li) {}
  ~BinOp() { delete left; delete right; delete op; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")";
  }
  virtual void sem() override {
    left->type_check(TYPE_int, 5, line, op);
    right->type_check(TYPE_int, 5, line, op);
    type = TYPE_int;
  }

  virtual void compile() override{
    left->compile();
    right->compile();
    std::cout<<"  popl %ebx\n";
    std::cout<<"  popl %eax\n";
    switch (op[0]) {
      case '+':
        std::cout<<"  addl %ebx, %eax\n";
        std::cout<<"  pushl %eax\n";
        break;
      case '-':
        std::cout<<"  subl %ebx, %eax\n";
        std::cout<<"  pushl %eax\n";
        break;
      case '*':
        std::cout<<"  mull %ebx\n";
        std::cout<<"  pushl %eax\n";
        break;
      case 'd':
        std::cout<<"  cdq\n";
        std::cout<<"  divl %ebx\n";
        std::cout<<"  pushl %eax\n";
        break;
      case 'm':
        std::cout<<("  cdq\n");
        std::cout<<("  divl %ebx\n");
        std::cout<<("  pushl %edx\n");
        break;
    }
  }

private:
  Expr *left;
  char *op;
  Expr *right;
  int line;
};

class CondOp: public Expr {
public:
  CondOp(Expr *l, char *o, Expr *r, int li): left(l), op(o), right(r), line(li) {}
  ~CondOp() { delete left; delete right; delete op; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")";
  }
  virtual void sem() override {
    if(op[0]=='a'||op[0]=='o') {
      left->type_check(TYPE_bool, 4, line, op);
      right->type_check(TYPE_bool, 4, line, op);
    } else {
      right->type_check(left->type_check_return({TYPE_int, TYPE_char}, 6, line, op), 6, line, op);
    }
    type = TYPE_bool;
  }

  virtual void compile() override{
    static int Condcounter = 0;
    Condcounter++;
    numOfCond=Condcounter;
    if(Condcounter<=0){
      printf("\n- \033[1;31mOVERFLOW\033[0m of the NOT condion counter.\n");
      printf("\n- Compilation \033[1;31mFAILED\033[0m.\n");
    exit(1);
    }
    left->compile();
    right->compile();
    std::cout<<"  popl %ebx\n";
    std::cout<<"  popl %eax\n";
    if(op[0] == '='){
        std::cout<<"  cmpl %eax %ebx\n";
        std::cout<<"  je CondTrue_"<<numOfCond<<"\n";

    }
    else if(op[0] == '#'){
        std::cout<<"  cmpl %eax %ebx\n";
        std::cout<<"  jne CondTrue_"<<numOfCond<<"\n";
    }
    else if(op[0] == '<' && op[1] == '='){
        std::cout<<"  cmpl %eax %ebx\n";
        std::cout<<"  jle CondTrue_"<<numOfCond<<"\n";

    }
    else if(op[0] == '>' && op[1] == '='){
        std::cout<<"  cmpl %eax %ebx\n";
        std::cout<<"  jge CondTrue_"<<numOfCond<<"\n";

    }
    else if(op[0] == '<'){
        std::cout<<"  cmpl %eax %ebx\n";
        std::cout<<"  jl CondTrue_"<<numOfCond<<"\n";

    }
    else if(op[0] == '>'){
        std::cout<<"  cmpl %eax %ebx\n";
        std::cout<<"  jg CondTrue_"<<numOfCond<<"\n";

    }
    else if(op[0] == 'a'){
        std::cout<<"  andl %eax %ebx\n";
        std::cout<<"  jnz CondTrue_"<<numOfCond<<"\n";
    }
    else if(op[0] == 'o'){
        std::cout<<"  orl %eax %ebx\n";
        std::cout<<"  jnz CondTrue_"<<numOfCond<<"\n";
    }
    std::cout<<"  pushl $0\n";
    std::cout<<"  jmp CondEnd_"<<numOfCond<<"\n";
    std::cout<<"\nCondTrue_"<<numOfCond<<":\n\n";
    std::cout<<"  pushl $1\n";
    std::cout<<"\nCondEnd_"<<numOfCond<<":\n\n";
  }

private:
  Expr *left;
  char *op;
  Expr *right;
  int line;
  int numOfCond; // uesd to know how may Conditions we have seen to name the assembly label
};

class ExprList: public Stmt {
public:
  ExprList(): expr_list() {}
  ~ExprList() { for (Expr *e : expr_list) delete e;}
  void append(Expr *e) { expr_list.push_back(e); }
  void putinfront(Expr *e) { expr_list.insert(expr_list.begin(), e); }
  virtual void printOn(std::ostream &out) const override {
    out << "ExprList(";
    bool first = true;
    for (Expr *e : expr_list) {
      if (!first) out << ", ";
      first = false;
      out << *e;
    }
    out << ")";
  }
  virtual void exprListSem(std::vector<Datatype> types, int l, char* f) {
    if (expr_list.size() != types.size()) {
      showSemanticError(14, l, f);
    }
    int i = 0;
    for (Expr *e : expr_list) {
      e->sem();
      e->type_check(types[i], 15, l, e->getName());
      ++i;
    }
  }
private:
  std::vector<Expr *> expr_list;
};


// Stmt 

class BlankStmt: public Stmt {
public:
  BlankStmt() {}
  virtual void printOn(std::ostream &out) const override {
    out << "BlankStmt()";
  }
};

class Return: public Stmt {
public:
  Return(int l, Expr *e = nullptr): line(l), expr(e) {}
  ~Return() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Return(";
    if (expr != nullptr) out << *expr;
    out << ")";
  }
  virtual void sem() override {
    Datatype t = st.getCurrentScopeDatatype();
    if (expr != nullptr) {
      expr->sem();
      expr->type_check(t, 16, line, st.getCurrentScopeName());
    } else if (t != TYPE_nothing) {
      showSemanticError(17, line, st.getCurrentScopeName(), t);
    }
    st.setCurrentScopeReturned();
  }
private:
  int line;
  Expr *expr;
};

class Assignment: public Stmt {
public:
  Assignment(Expr *e1, Expr *e2, int l): expr1(e1),  expr2(e2), line(l) {}
  ~Assignment() { delete expr1; delete expr2; }
  virtual void printOn(std::ostream &out) const override {
    out << "Assignment(" << *expr1 << ", " << *expr2 << ")";
  }
  
  virtual void sem() override {
    expr1->sem();
    expr2->sem();
    SymbolEntry *e1 = expr1->getSymbolEntry();
    expr2->type_check(e1->getDatatype(), 9, line);
  }

  virtual void compile() override{
    expr2->compile();
    std::cout<<"  popl %eax\n";
    if(typeid(*expr1) == typeid(Id))
      std::cout<<"  movl %eax, "<< 4*findInVarMap(expr1->getName()) <<"(%edi)\n";

}

private:
  Expr *expr1;
  Expr *expr2;
  int line;
};

class If: public Stmt {
public:
  If(Expr *c, Stmt *s1, int l, Stmt *s2 = nullptr):
    cond(c), stmt1(s1), stmt2(s2), line(l) {}
  ~If() { delete cond; delete stmt1; delete stmt2; }
  virtual void printOn(std::ostream &out) const override {
    out << "If(" << *cond << ", " << *stmt1;
    if (stmt2 != nullptr) out << ", " << *stmt2;
    out << ")";
  }
  virtual void sem() override {
    cond->type_check(TYPE_bool, 7, line);
    stmt1->sem();
    if (stmt2 != nullptr) stmt2->sem();
  }

  virtual void compile() override{
    static int Ifcounter = 0;
    Ifcounter++;
    numOfIf = Ifcounter;
    if(Ifcounter<=0){
      printf("\n- \033[1;31mOVERFLOW\033[0m of the IfElse counter.\n");
      printf("\n- Compilation \033[1;31mFAILED\033[0m.\n");
    exit(1);
    }
    cond->compile();
    std::cout<<"  popl %eax\n";
    std::cout<<"  andl %eax %eax\n";
    std::cout<<"  jz Else_"<<numOfIf<<"\n";
    stmt1->compile();
    std::cout<<"  jmp IfEnd_"<<numOfIf<<"\n";
    std::cout<<"\nElse_"<<numOfIf<<":\n\n";
    if(stmt2!=nullptr)
      stmt2->compile();
    std::cout<<"\nIfEnd_"<<numOfIf<<":\n";
  }

private:
  Expr *cond;
  Stmt *stmt1;
  Stmt *stmt2;
  int line;
  int numOfIf; // uesd to know how may "if" we have seen to name the assembly label
};

class While: public Stmt {
public:
  While(Expr *e, Stmt *s, int l): expr(e), stmt(s), line(l) {}
  ~While() { delete expr; delete stmt; }
  virtual void printOn(std::ostream &out) const override {
    out << "While(" << *expr << ", " << *stmt << ")";
  }
  virtual void sem() override {
    expr->type_check(TYPE_bool, 8, line);
    stmt->sem();
  }

  virtual void compile() override{
    static int Whilecounter = 0;
    Whilecounter++;
    numOfWhile = Whilecounter;
    if(Whilecounter<=0){
      printf("\n- \033[1;31mOVERFLOW\033[0m of the While counter.\n");
      printf("\n- Compilation \033[1;31mFAILED\033[0m.\n");
    exit(1);
    }
    expr->compile();
    std::cout<<"  popl %eax\n";
    std::cout<<"  andl %eax %eax\n";
    std::cout<<"  jz WhileEnd_"<<numOfWhile<<"\n";
    stmt->compile();
    std::cout<<"\nWhileEnd_"<<numOfWhile<<":\n\n";

  }

private:
  Expr *expr;
  Stmt *stmt;
  int line;
  int numOfWhile; // uesd to know how may "while" we have seen to name the assembly label
};

class Block: public Stmt {
public:
  Block(): stmt_list() {}
  ~Block() {for (Stmt *s : stmt_list) delete s; }
  void append(Stmt *s) { stmt_list.push_back(s); }
  virtual void printOn(std::ostream &out) const override {
    out << "Block(";
    bool first = true;
    for (Stmt *s : stmt_list) {
      if (!first) out << ", ";
      first = false;
      out << *s;
    }
    out << ")";
  }

  virtual void sem() override {
    for (Stmt *s: stmt_list) s->sem();
  }

  virtual void compile() override {
    for (Stmt *s: stmt_list) {s->compile(); std::cout<<"\n";}
  }

private:
  std::vector<Stmt *> stmt_list;
};

class FuncCall: public Expr, public Stmt {
public:
  FuncCall(char *id, int l, ExprList *e = nullptr): id(id), line(l), expr_list(e) {}
  ~FuncCall() { delete id; delete expr_list; }
  virtual void printOn(std::ostream &out) const override {
    out << "FuncCall(" << id;
    if (expr_list != nullptr) out << ", " << *expr_list;
    out << ")";
  }
  virtual void sem() override {
    SymbolEntry *e = st.lookup(id, line, {TYPE_function});
    if (expr_list != nullptr) {
      expr_list->exprListSem(e->getParameterTypes(), line, id);
    } 
    else if (e->getParameterTypes().size() != 0) {
      showSemanticError(14, line, id);
    }
    type = e->getDatatype();
    if (isVoid == true && type != TYPE_nothing) {
      showSemanticError(13, line, id);
    }
  }
  void makeStmt() {
    isVoid = true;
  }
private:
  char *id;
  int line;
  ExprList *expr_list;
  bool isVoid = false;
};


// Func

class FparType: public Func {
public:
  FparType(Datatype s, ConstList *c = nullptr): type(s), const_list(c) {}
  ~FparType() { delete const_list; }
  virtual void printOn(std::ostream &out) const override {
    out << "FparType(" << type;
    if (const_list != nullptr) out << ", " << *const_list;
    out << ")";
  }
  Datatype getType() {
    return type;
  }
  std::vector<int> getConstList() {
    if (const_list != nullptr)
      return const_list->getConstList();
    return {};
  }
private:
  Datatype type;
  ConstList *const_list;
};

class IdList: public Func {
public:
  IdList(): id_list() {}
  void append(char *s) { id_list.push_back(s); }
  void putinfront(char *s) { id_list.insert(id_list.begin(), s); }
  virtual void printOn(std::ostream &out) const override {
    out << "IdList(";
    bool first = true;
    for (char *s: id_list) {
      if (!first) out << ", ";
      first = false;
      out << s;
    }
    out << ")";
  }
  std::vector<char *> getIdList() {
    std::vector<char *> v;
    for (char *id : id_list) {
      v.push_back(id);
    }
    return v;
  };
private:
  std::vector<char *> id_list;
};

class FparDef: public Func {
public:
  FparDef(int l = -1, IdList *id = nullptr, FparType *f = nullptr, bool ref = false): line(l), id_list(id), fpar_type(f), ref(ref) {}
  ~FparDef() { delete id_list; delete fpar_type; }
  virtual void printOn(std::ostream &out) const override {
    out << "FparDef(";
    if (id_list != nullptr) out << *id_list;
    if (fpar_type != nullptr) out << ", " << *fpar_type;
    out << ")";
  }
  virtual void sem() override {
    if (id_list != nullptr && fpar_type != nullptr) {
      Datatype t = fpar_type->getType();
      std::vector<int> v = fpar_type->getConstList();
      if (v.size() == 0) {
        for (char *c : id_list->getIdList()) {
          st.insertVar(c, t, line);
        }
      } else {
        for (char *c : id_list->getIdList()) {
          st.insertArray(c, t, v, line);
        }
      }
    }
  }
  std::vector<FunctionParameter> getFunctionParameter() {
    std::vector<FunctionParameter> s;
    if (id_list != nullptr && fpar_type != nullptr) {
      Datatype t = fpar_type->getType();
      std::vector<int> v = fpar_type->getConstList();
      if (v.size() == 0) {
        for (char *c : id_list->getIdList()) {
          s.push_back(FunctionParameter(c, t, false));
        }
      } else {
        for (char *c : id_list->getIdList()) {
          s.push_back(FunctionParameter(c, t, false, v));
        }
      }
    }
    return s;
  }
private:
  int line;
  IdList *id_list;
  FparType *fpar_type;
  bool ref;
};

class FparDefList: public Func {
public:
  FparDefList(): fdef_list() {}
  ~FparDefList() { for (FparDef *f : fdef_list) delete f; }
  void append(FparDef *f) { fdef_list.push_back(f); }
  void putinfront(FparDef *f) { fdef_list.insert(fdef_list.begin(), f); }
  virtual void printOn(std::ostream &out) const override {
    out << "FparDefList(";
    bool first = true;
    for (FparDef *f : fdef_list) {
      if (!first) out << ", ";
      first = false;
      out << *f;
    }
    out << ")";
  }
  virtual void sem() override {
    for(FparDef *fdef_elem : fdef_list) fdef_elem->sem();
  }
  std::vector<FunctionParameter> getFunctionParameters() {
    std::vector<FunctionParameter> s;
    for (FparDef *f : fdef_list) {
      for (FunctionParameter e : f->getFunctionParameter()) {
        s.push_back(e);
      }
    }
    return s;
  }
private:
  std::vector<FparDef *> fdef_list;
};

class Header: public Func {
public:
  Header(char *id, FparDefList *f, Datatype s, int l): id(id), fpar(f), rettype(s), line(l) {}
  ~Header() { delete id; delete fpar; }
  virtual void printOn(std::ostream &out) const override {
    out << "Header(" << id << ", " << *fpar << ", " << rettype << ")";
  }
  void headerSem(bool isDefined) {
    st.addScopeNameAndType(id, rettype);
    if (isDefined == true) {
      st.insertFunctionToPreviousScope(id, rettype, fpar->getFunctionParameters(), line);
    } else {
      st.insertFunction(id, rettype, fpar->getFunctionParameters(), line, false);
    }
    fpar->sem(); 
  }
private:
  char *id;
  FparDefList *fpar;
  Datatype rettype;
  int line;
};

class LocalDefList: public Func {
public:
  LocalDefList(): ldef_list() {}
  ~LocalDefList() { for (Func *f : ldef_list) delete f; }
  void append(Func *f) { ldef_list.push_back(f); }
  virtual void printOn(std::ostream &out) const override {
    out << "LocalDefList(";
    bool first = true;
    for (Func *f : ldef_list) {
      if (!first) out << ", ";
      first = false;
      out << *f;
    }
    out << ")";
  }
  virtual void sem() override {
    for(Func *ldef_elem : ldef_list) ldef_elem->sem();
  }

  virtual void compile() override{
    for(Func *ldef_elem : ldef_list) ldef_elem->compile();
  }

private:
  std::vector<Func *> ldef_list;
};

class FuncDef: public Func {
public:
  FuncDef(Header *h, LocalDefList *l, Block *b, int li): head(h), local(l), block(b), line(li) {}
  ~FuncDef() { delete head; delete local; delete block; }
  virtual void printOn(std::ostream &out) const override {
    out << "FuncDef(" << *head << ", " << *local << ", " << *block << ")";
  }
  virtual void sem() override {
    st.openScope();
    head->headerSem(true);
    local->sem();
    block->sem();
    st.setCurrentFunctionDefined();
    st.closeScope(line);
  }

  virtual void compile() override{
    local->compile();
    block->compile();
  }


private:
  Header *head;
  LocalDefList *local;
  Block *block;
  int line;
};


class FuncDecl: public Func {
public:
  FuncDecl(Header *h): header(h) {}
  ~FuncDecl() { delete header; }
  virtual void printOn(std::ostream &out) const override {
    out << "FuncDecl(" << *header << ")";
  }
  virtual void sem() override {
    header->headerSem(false);
  }
private:
  Header *header;
};

class VarDef: public Func {
public:
  VarDef(IdList *id, Type *t, int l): id_list(id), type(t), line(l) {}
  ~VarDef() { delete id_list; delete type; }
  virtual void printOn(std::ostream &out) const override {
    out << "VarDef(" << *id_list << ", " << *type << ")";
  }
  virtual void sem() override {
    Datatype t = type->getType();
    std::vector<int> v = type->getConstList();
    if (v.size() == 0) {
      for (char *c : id_list->getIdList()) {
        st.insertVar(c, t, line);
      }
    } else {
      for (char *c : id_list->getIdList()) {
        st.insertArray(c, t, v, line);
      }
    }
  }

  virtual void compile() override{
    for (char *c : id_list->getIdList()) {
      allocateVariable(c);
    }
  }


private:
  IdList *id_list;
  Type *type;
  int line;
};


inline void prologue() {
  std::cout<<"  .text\n"
           <<"  .global _start\n"
           <<"\n"
           <<"_start:\n"
           <<"  movl $var, %edi\n\n";
}

inline void epilogue() {
  std::cout<<"\n"
           <<"  movl $0, %ebx\n"
           <<"  movl $1, %eax\n"
           <<"  int $0x80\n"
           <<"\n"
           <<"  .data\n"
           <<"var:\n"
           <<"  .rept "<< VarMap.size()<<"\n"
           <<"  .long 0\n"
           <<"  .endr\n"
           <<"NL:\n"
           <<"  .asciz \"\\n\"\n";
}




#endif