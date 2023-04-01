#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <map>
#include <vector>
#include <string>

class AST {
public:
  virtual void printOn(std::ostream &out) const = 0;
};

inline std::ostream& operator<< (std::ostream &out, const AST &t) {
  t.printOn(out);
  return out;
}

class Expr: public AST {
public:
//   virtual int eval() const = 0;
};

class Stmt: public AST {
public:
//   virtual void run() const = 0;
};

class Func: public AST {

};


// Expr 

class Id: public Expr {
public:
  Id(std::string s): str(s) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Id(" << str << ")";
  }
private:
    std::string str;
};

class String: public Expr {
public:
  String(std::string s): str(s) {}
  virtual void printOn(std::ostream &out) const override {
    out << "String(" << str << ")";
  }
private:
    std::string str;
};

class Const: public Expr {
public:
  Const(int n): num(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Const(" << num << ")";
  }
private:
  int num;
};

class Array: public Expr {
public:
  Array(Expr *l, Expr *r): lvalue(l), expr(r) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Array(" << *lvalue << ", " << *expr << ")";
  }
private:
    Expr *lvalue;
    Expr *expr;
};

class Plus: public Expr {
public:
  Plus(Expr *n): expr(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Plus(" << *expr << ")";
  }
private:
  Expr *expr;
};

class Minus: public Expr {
public:
  Minus(Expr *n): expr(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Minus(" << *expr << ")";
  }
private:
  Expr *expr;
};

class Not: public Expr {
public:
  Not(Expr *n): expr(n) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Not(" << *expr << ")";
  }
private:
  Expr *expr;
};

class BinOp: public Expr {
public:
  BinOp(Expr *l, std::string o, Expr *r): left(l), op(o), right(r) {}
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")";
  }
private:
  Expr *left;
  std::string op;
  Expr *right;
};

class CondOp: public Expr {
public:
  CondOp(Expr *l, std::string o, Expr *r): left(l), op(o), right(r) {}
  virtual void printOn(std::ostream &out) const override {
    out << op << "(" << *left << ", " << *right << ")";
  }
private:
  Expr *left;
  std::string op;
  Expr *right;
};

class ExprList: public Stmt {
public:
  ExprList(): expr_list() {}
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
  Assignment(Expr *e1, Expr *e2): expr1(e1),  expr2(e2) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Assignment(" << *expr1 << ", " << *expr2 << ")";
  }
private:
  Expr *expr1;
  Expr *expr2;
};

class If: public Stmt {
public:
  If(Expr *c, Stmt *s1, Stmt *s2 = nullptr):
    cond(c), stmt1(s1), stmt2(s2) {}
  virtual void printOn(std::ostream &out) const override {
    out << "If(" << *cond << ", " << *stmt1;
    if (stmt2 != nullptr) out << ", " << *stmt2;
    out << ")";
  }
private:
  Expr *cond;
  Stmt *stmt1;
  Stmt *stmt2;
};

class While: public Stmt {
public:
  While(Expr *e, Stmt *s): expr(e), stmt(s) {}
  virtual void printOn(std::ostream &out) const override {
    out << "While(" << *expr << ", " << *stmt << ")";
  }
private:
  Expr *expr;
  Stmt *stmt;
};

class Block: public Stmt {
public:
  Block(): stmt_list() {}
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
private:
  std::vector<Stmt *> stmt_list;
};

class FuncCall: public Stmt, public Expr {
public:
  FuncCall(std::string id, ExprList *e = nullptr): id(id), expr_list(e) {}
  virtual void printOn(std::ostream &out) const override {
    out << "FuncCall(" << id;
    if (expr_list != nullptr) out << ", " << *expr_list;
    out << ")";
  }
private:
  std::string id;
  ExprList *expr_list;
};


// Func

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

class FparType: public Func {
public:
  FparType(std::string s, ConstList *c = nullptr): type(s), const_list(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "FparType(" << type;
    if (const_list != nullptr) out << ", " << *const_list;
    out << ")";
  }
private:
  std::string type;
  ConstList *const_list;
};

class IdList: public Func {
public:
  IdList(): id_list() {}
  void append(std::string s) { id_list.push_back(s); }
  void putinfront(std::string s) { id_list.insert(id_list.begin(), s); }
  virtual void printOn(std::ostream &out) const override {
    out << "IdList(";
    bool first = true;
    for (std::string s: id_list) {
      if (!first) out << ", ";
      first = false;
      out << s;
    }
    out << ")";
  }
private:
  std::vector<std::string> id_list;
};

class FparDef: public Func {
public:
  FparDef(IdList *id = nullptr, FparType *f = nullptr): id_list(id), fpar_type(f) {}
  virtual void printOn(std::ostream &out) const override {
    out << "FparDef(";
    if (id_list != nullptr) out << *id_list;
    if (fpar_type != nullptr) out << ", " << *fpar_type;
    out << ")";
  }
private:
  IdList *id_list;
  FparType *fpar_type;
};


class FparDefList: public Func {
public:
  FparDefList(): fdef_list() {}
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
private:
  std::vector<FparDef *> fdef_list;
};

class Header: public Func {
public:
  Header(std::string id, FparDefList *f, std::string s): id(id), fpar(f), rettype(s) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Header(" << id << ", " << *fpar << ", " << rettype << ")";
  }
private:
  std::string id;
  FparDefList *fpar;
  std::string rettype;
};

class LocalDefList: public Func {
public:
  LocalDefList(): ldef_list() {}
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
private:
  std::vector<Func *> ldef_list;
};

class FuncDef: public Func {
public:
  FuncDef(Header *h, LocalDefList *l, Block *b): head(h), local(l), block(b) {}
  virtual void printOn(std::ostream &out) const override {
    out << "FuncDef(" << *head << ", " << *local << ", " << *block << ")";
  }
private:
  Header *head;
  LocalDefList *local;
  Block *block;
};

class Type: public Func {
public:
  Type(std::string s, ConstList *c = nullptr): type(s), const_list(c) {}
  virtual void printOn(std::ostream &out) const override {
    out << "Type(" << type;
    if (const_list->length() != 0) out << ", " << *const_list;
    out << ")";
  }
private:
  std::string type;
  ConstList *const_list;
};



class FuncDecl: public Func {
public:
  FuncDecl(Header *h): header(h) {}
  virtual void printOn(std::ostream &out) const override {
    out << "FuncDecl(" << *header << ")";
  }
private:
  Header *header;
};

class VarDef: public Func {
public:
  VarDef(IdList *id, Type *t): id_list(id), type(t) {}
  virtual void printOn(std::ostream &out) const override {
    out << "VarDef(" << *id_list << ", " << *type << ")";
  }
private:
  IdList *id_list;
  Type *type;
};


#endif