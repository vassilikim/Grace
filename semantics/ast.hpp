#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>

#include "symbol.hpp"

extern int yylineno;

inline std::ostream& operator<<(std::ostream &out, Datatype t) {
  switch (t) {
    case TYPE_int: out << "int"; break;
    case TYPE_bool: out << "bool"; break;
    case TYPE_char: out << "char"; break;
    case TYPE_nothing: out << "nothing"; break;
    case TYPE_string: out << "string"; break;
  }
  return out;
}

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
  void type_check(Datatype t, int code, int line, char* op = "") {
    sem();
    if (type != t) {
      showSemanticError(code, line, op);
    }
  }
  virtual char* getName() {}
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
  virtual char *getName() override {
    return str;
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
    expr->type_check(TYPE_int, 2, line);
  }
  virtual char *getName() override {
    return lvalue->getName();
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
    expr->type_check(TYPE_int, 3, line, "+");
    type = TYPE_int;
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
    expr->type_check(TYPE_int, 3, line, "-");
    type = TYPE_int;
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
    expr->type_check(TYPE_bool, 4, line, "not");
    type = TYPE_bool;
  }
private:
  Expr *expr;
  int line;
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
      left->type_check(TYPE_int, 6, line, op);
      right->type_check(TYPE_int, 6, line, op);
    }
    type = TYPE_bool;
  }
private:
  Expr *left;
  char *op;
  Expr *right;
  int line;
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
  virtual void sem() override {
    for (Expr *e : expr_list) {
      e->sem();
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
  Return(Expr *e = nullptr): expr(e) {}
  ~Return() { delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "Return(";
    if (expr != nullptr) out << *expr;
    out << ")";
  }
  virtual void sem() override {
    expr->sem();
  }
private:
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
    SymbolEntry * e = st.lookup(expr1->getName(), line);
    expr2->type_check(e->getType(), 9, line);
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
private:
  Expr *cond;
  Stmt *stmt1;
  Stmt *stmt2;
  int line;
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
private:
  Expr *expr;
  Stmt *stmt;
  int line;
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
private:
  std::vector<Stmt *> stmt_list;
};

class FuncCall: public Expr, public Stmt {
public:
  FuncCall(char *id, ExprList *e = nullptr): id(id), expr_list(e) {}
  ~FuncCall() { delete id; delete expr_list; }
  virtual void printOn(std::ostream &out) const override {
    out << "FuncCall(" << id;
    if (expr_list != nullptr) out << ", " << *expr_list;
    out << ")";
  }
  virtual void sem() override {
    if (expr_list != nullptr) expr_list->sem();
  }
private:
  char *id;
  ExprList *expr_list;
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
    if (id_list != nullptr && fpar_type != nullptr) {
      std::vector<FunctionParameter> s;
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
      return s;
    }
  }
private:
  IdList *id_list;
  FparType *fpar_type;
  int line;
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
  virtual void sem() override {
    st.addScopeNameAndType(id, rettype);
    st.insertFunctionToPreviousScope(id, rettype, fpar->getFunctionParameters(), line);
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
private:
  std::vector<Func *> ldef_list;
};

class FuncDef: public Func {
public:
  FuncDef(Header *h, LocalDefList *l, Block *b): head(h), local(l), block(b) {}
  ~FuncDef() { delete head; delete local; delete block; }
  virtual void printOn(std::ostream &out) const override {
    out << "FuncDef(" << *head << ", " << *local << ", " << *block << ")";
  }
  virtual void sem() override {
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
  virtual void sem() override {
    header->sem();
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
private:
  IdList *id_list;
  Type *type;
  int line;
};


#endif