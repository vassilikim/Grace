#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <cstring>
#include <map>
#include <vector>


extern int yylineno;


//////////////////  AST ///////////////////////////

class AST {
public:
  virtual void printOn(std::ostream &out) const = 0;
  virtual ~AST() {}
  virtual void sem(){}
   
};

inline std::ostream& operator<< (std::ostream &out, const AST &t) {
  t.printOn(out);
  return out;
}

class Expr: public AST {
public:
//   virtual int eval() const = 0;
  virtual std::string getType() {return "default";};
};

class Stmt: public AST {
public:
//   virtual void run() const = 0;
};

class Func: public AST {

};


class ConstList: public Func {
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
private:
  std::vector<int> const_list;
};

class Type: public Func {
public:
  Type(){}
  Type(char *s, ConstList *c = nullptr): type(s), const_list(c) {}
  ~Type() { delete type; delete const_list; }
  virtual void printOn(std::ostream &out) const override {
    out << "Type(" << type;
    if (const_list->length() != 0) out << ", " << *const_list;
    out << ")";
  }
  char* getType(){return type;}
private:
  char *type;
  ConstList *const_list;
};


//////////////////  SYMBOL TABLE ///////////////////////////

class Scope {
public:
  Scope() : locals(), size(0) {}
  int getSize() const { return size; }
  std::string lookup(char *c) {
    if (locals.find(c) == locals.end()) return "null";
    return locals[c];
  }
  void insert(char *c, char* t) {
    if (locals.find(c) != locals.end()) {
      printf("\033[1;31merror:\n\t\033[0m");
      std::cerr << "Variable " ;
      printf("\033[1;35m%s\033[0m", c);
      std::cerr << " is declared multiple times under the same scope.\n";
      exit(1);
    }
    locals[c] = t;
    ++size;
    }
private:
  std::map<std::string, std::string> locals;
  int size;
};

class SymbolTable {
public:
  void openScope() {
    scopes.push_back(Scope());
  }
  void closeScope() { scopes.pop_back(); };
  std::string lookup(char *c, int line) {
    for (auto i = scopes.rbegin(); i != scopes.rend(); ++i) {
      std::string e = i->lookup(c);
      if (e != "null") return e;
    }
    printf("\033[1;31merror:\n\t\033[0m");
    std::cerr << "Unknown variable " ;
    printf("\033[1;35m%s\033[0m", c);
    printf(" -- line: ");
    printf("\033[1;36m%d\n\033[0m", line);
    exit(1);
  }
  int getSizeOfCurrentScope() const { return scopes.back().getSize(); }
  void insert(char *c, char* t) { scopes.back().insert(c, t); }
private:
  std::vector<Scope> scopes;
};

static SymbolTable st;
/////////////////////////////////////////////////////////////

// Expr 

class Id: public Expr {
public:
  Id(char *s, int l): str(s) {line = l;}
  ~Id() { delete str; }
  virtual void printOn(std::ostream &out) const override {
    out << "Id(" << str << ")";
  }
  virtual std::string getType() { return st.lookup(str, line);}
private:
  int line;
  char *str;
};

class String: public Expr {
public:
  String(char *s): str(s) {}
  ~String() { delete str; }
  virtual void printOn(std::ostream &out) const override {
    out << "String(" << str << ")";
  }
  virtual std::string getType() { return "string";}
private:
    char *str;
};

class Const: public Expr {
public:
  Const(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Const(" << num << ")";
  }
  virtual std::string getType(){ return "int";}
private:
  int num;
};

class ConstChar: public Expr {
public:
  ConstChar(char *c): constchar(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "ConstChar(" << constchar << ")";
  }
  virtual std::string getType() { return "char";}
private:
  char *constchar;
};

class Array: public Expr {
public:
  Array(Expr *l, Expr *r): lvalue(l), expr(r) {}
  ~Array() { delete lvalue; delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Array(" << *lvalue << ", " << *expr << ")";
  }
private:
    Expr *lvalue;
    Expr *expr;
};

class Plus: public Expr {
public:
  Plus(Expr *n, int l): expr(n) {line=l;}
  ~Plus() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Plus(" << *expr << ")";
  }
  virtual std::string getType() {
    if(expr->getType()=="int") return "int";
    printf("\033[1;31merror:\n\t\033[0m");
    printf("The operator \'");
    printf("\033[1;35m+\033[0m");
    printf("\' must be used with a integer.");
    printf("\' -- line: ");
    printf("\033[1;36m%d\n\033[0m", line);
    exit(1);
  }
private:
  int line;
  Expr *expr;
};

class Minus: public Expr {
public:
  Minus(Expr *n, int l): expr(n) {line=l;}
  ~Minus() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Minus(" << *expr << ")";
  }
  virtual std::string getType() {
    if(expr->getType()=="int") return "int";
    printf("\033[1;31merror:\n\t\033[0m");
    printf("The operator \'");
    printf("\033[1;35m-\033[0m");
    printf("\' must be used with a integer.");
    printf("\' -- line: ");
    printf("\033[1;36m%d\n\033[0m", line);
    exit(1);
  }
private:
  int line;
  Expr *expr;
};

class Not: public Expr {
public:
  Not(Expr *n, int l): expr(n) {line = l;}
  ~Not() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Not(" << *expr << ")";
  }
  virtual std::string getType() {
    if(expr->getType()=="bool") return "bool";
    printf("\033[1;31merror:\n\t\033[0m");
    printf("Logic operator \'");
    printf("\033[1;35mnot\033[0m");
    printf("\' must apply on conditions.");
    printf("\' -- line: ");
    printf("\033[1;36m%d\n\033[0m", line);
    exit(1);
  }
private:
  int line;
  Expr *expr;
};

class BinOp: public Expr {
public:
  BinOp(Expr *l, char *o, Expr *r, int li): left(l), op(o), right(r) {line=li;}
  ~BinOp() { delete left; delete right; delete op; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")";
  }
  virtual std::string getType() {
    if(left->getType()=="int"&&right->getType()=="int") return "int";
    printf("\033[1;31merror:\n\t\033[0m");
    printf("Arithmetic operator \'");
    printf("\033[1;35m%s\033[0m", op);
    printf("\' must apply on integers.");
    printf("\' -- line: ");
    printf("\033[1;36m%d\n\033[0m", line);
    exit(1);
  }
private:
  int line;
  Expr *left;
  char *op;
  Expr *right;
};

class CondOp: public Expr {
public:
  CondOp(Expr *l, char *o, Expr *r, int li): left(l), op(o), right(r) {line=li;}
  ~CondOp() { delete left; delete right; delete op; }
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")";
  }
  virtual std::string getType() {
    //exw grapsei etsi to if giati kati epaize me to string kai to char *
    if(op[0]=='a'||op[0]=='o'){ 
      if(left->getType()=="bool"&&right->getType()=="bool") return "bool";
      printf("\033[1;31merror:\n\t\033[0m");
      printf("Logic operator \'");
      printf("\033[1;35m%s\033[0m", op);
      printf("\' must apply on conditons.");
      printf("\' -- line: ");
      printf("\033[1;36m%d\n\033[0m", line);
      exit(1);
    }
    else{
      if(left->getType()=="int"&&right->getType()=="int") return "bool";
      if(left->getType()=="char"&&right->getType()=="char") return "bool";
      printf("\033[1;31merror:\n\t\033[0m");
      printf("Logic operator \'");
      printf("\033[1;35m%s\033[0m", op);
      printf("\' must apply on variables or constants of the same type.");
      printf("\' -- line: ");
      printf("\033[1;36m%d\n\033[0m", line);
      exit(1);
    }
  }
private:
  int line;
  Expr *left;
  char *op;
  Expr *right;
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
  virtual std::vector<std::string> getType(){
    std::vector<std::string> ret_vect;
    for(Expr *expr:expr_list) ret_vect.push_back(expr->getType());
    return ret_vect;
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
  Return(Expr *e = nullptr): expr(e) {}
  ~Return() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Return(";
    if (expr != nullptr) out << *expr;
    out << ")";
  }
private:
  Expr *expr;
};

class Assignment: public Stmt {
public:
  Assignment(Expr *e1, Expr *e2, int l): expr1(e1),  expr2(e2) {line=l;}
  ~Assignment() { delete expr1; delete expr2; }
  virtual void printOn(std::ostream &out) const override {
    out << "Assignment(" << *expr1 << ", " << *expr2 << ")";
  }
  virtual void sem(){
    if(expr1->getType()!=expr2->getType()){
      printf("\033[1;31merror:\n\t\033[0m");
      printf("Invalid assignment ");
      printf("\033[1;35m%s\033[0m", expr1->getType().c_str());
      printf(" <- ");
      printf("\033[1;35m%s\033[0m", expr2->getType().c_str());
      printf(" -- line: ");
      printf("\033[1;36m%d\n\033[0m", line);
      exit(1);
    }
  }
private:
  int line;
  Expr *expr1;
  Expr *expr2;
};

class If: public Stmt {
public:
  If(Expr *c, Stmt *s1, int l, Stmt *s2 = nullptr):
    cond(c), stmt1(s1), stmt2(s2) { line =l; }
  ~If() { delete cond; delete stmt1; delete stmt2; }
  virtual void printOn(std::ostream &out) const override {
    out << "If(" << *cond << ", " << *stmt1;
    if (stmt2 != nullptr) out << ", " << *stmt2;
    out << ")";
  }
  virtual void sem(){
    if (cond->getType()!="bool"){
      printf("\033[1;31merror:\n\t\033[0m");
      printf("If-statements must start with a condition.\n");
      printf(" -- line: ");
      printf("\033[1;36m%d\n\033[0m", line);
      exit(1);
    }
    stmt1->sem();
    if (stmt2 != nullptr) stmt2->sem();
  }
private:
  int line;
  Expr *cond;
  Stmt *stmt1;
  Stmt *stmt2;
};

class While: public Stmt {
public:
  While(Expr *e, Stmt *s, int l): expr(e), stmt(s) {line=l;}
  ~While() { delete expr; delete stmt; }
  virtual void printOn(std::ostream &out) const override {
    out << "While(" << *expr << ", " << *stmt << ")";
  }
  virtual void sem(){
    if(expr->getType()!="bool"){
      printf("\033[1;31merror:\n\t\033[0m");
      printf("While-statements must start with a condition.\n");
      printf(" -- line: ");
      printf("\033[1;36m%d\n\033[0m", line);
      exit(1);
    }
    stmt->sem();
  }
private:
  int line;
  Expr *expr;
  Stmt *stmt;
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
  virtual void sem(){
    for( Stmt *s: stmt_list) s->sem();
  }
private:
  std::vector<Stmt *> stmt_list;
};

class FuncCall: public Stmt, public Expr {
public:
  FuncCall(char *id, ExprList *e = nullptr): id(id), expr_list(e) {}
  ~FuncCall() { delete id; delete expr_list; }
  virtual void printOn(std::ostream &out) const override {
    out << "FuncCall(" << id;
    if (expr_list != nullptr) out << ", " << *expr_list;
    out << ")";
  }
private:
  char *id;
  ExprList *expr_list;
};


// Func



class FparType: public Func {
public:
  FparType(char *s, ConstList *c = nullptr): type(s), const_list(c) {}
  ~FparType() { delete type; delete const_list; }
  virtual void printOn(std::ostream &out) const override {
    out << "FparType(" << type;
    if (const_list != nullptr) out << ", " << *const_list;
    out << ")";
  }
  char* getType(){return type;}
private:
  char *type;
  ConstList *const_list;
};

class IdList: public Func {
public:
  IdList(): id_list() {}
  ~IdList() { for (char *s : id_list) delete s; }
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
  void sem(char* type){
    for(char* id: id_list) st.insert(id, type);
  }
private:
  std::vector<char *> id_list;
};

class FparDef: public Func {
public:
  FparDef(IdList *id = nullptr, FparType *f = nullptr): id_list(id), fpar_type(f) {}
  ~FparDef() { delete id_list; delete fpar_type; }
  virtual void printOn(std::ostream &out) const override {
    out << "FparDef(";
    if (id_list != nullptr) out << *id_list;
    if (fpar_type != nullptr) out << ", " << *fpar_type;
    out << ")";
  }
  virtual void sem() {id_list->sem(fpar_type->getType());}
private:
  IdList *id_list;
  FparType *fpar_type;
};

class RefFparDef: public FparDef {
public:
  RefFparDef(IdList *id = nullptr, FparType *f = nullptr): id_list(id), fpar_type(f) {}
  ~RefFparDef() { delete id_list; delete fpar_type; }
  virtual void printOn(std::ostream &out) const override {
    out << "RefFparDef(";
    if (id_list != nullptr) out << *id_list;
    if (fpar_type != nullptr) out << ", " << *fpar_type;
    out << ")";
  }
  virtual void sem() {id_list->sem(fpar_type->getType());}
private:
  IdList *id_list;
  FparType *fpar_type;
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
  virtual void sem(){
    for(FparDef *fdef_elem : fdef_list) fdef_elem->sem();
  }
private:
  std::vector<FparDef *> fdef_list;
};

class Header: public Func {
public:
  Header(char *id, FparDefList *f, char *s): id(id), fpar(f), rettype(s) {}
  ~Header() { delete id; delete fpar; delete rettype; }
  virtual void printOn(std::ostream &out) const override {
    out << "Header(" << id << ", " << *fpar << ", " << rettype << ")";
  }
  virtual void sem(){ fpar->sem(); }
private:
  char *id;
  FparDefList *fpar;
  char *rettype;
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
  virtual void sem(){
    for(Func *ldef_elem : ldef_list) ldef_elem->sem();
  }
private:
  std::vector<Func *> ldef_list;
};

class FuncDef: public Func {
public:
  FuncDef(Header *h, LocalDefList *l, Block *b): head(h), local(l), block(b) {SymbolTable();}
  ~FuncDef() { delete head; delete local; delete block; }
  virtual void printOn(std::ostream &out) const override {
    out << "FuncDef(" << *head << ", " << *local << ", " << *block << ")";
  }
  virtual void sem(){
    st.openScope();
    head->sem();
    local->sem();
    block->sem();
    st.closeScope();
  }
private:
  Header *head;
  LocalDefList *local;
  Block *block;
};


class FuncDecl: public Func {
public:
  FuncDecl(Header *h): header(h) {}
  ~FuncDecl() { delete header; }
  virtual void printOn(std::ostream &out) const override {
    out << "FuncDecl(" << *header << ")";
  }
private:
  Header *header;
};

class VarDef: public Func {
public:
  VarDef(IdList *id, Type *t): id_list(id), type(t) {}
  ~VarDef() { delete id_list; delete type; }
  virtual void printOn(std::ostream &out) const override {
    out << "VarDef(" << *id_list << ", " << *type << ")";
  }
  virtual void sem() {id_list->sem(type->getType());}
private:
  IdList *id_list;
  Type *type;
};


#endif