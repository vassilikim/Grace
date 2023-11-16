#ifndef __AST_HPP__
#define __AST_HPP__

#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <algorithm>

#include "symbol.hpp"


extern int yylineno;

#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include <llvm/Support/raw_ostream.h>
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"
/* 
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
*/

using namespace llvm;

// Global LLVM variables related to the LLVM suite.
static LLVMContext TheContext;
static std::unique_ptr<Module> TheModule;
static std::map<std::string, AllocaInst*> NamedValues;
static IRBuilder<> builder(TheContext);
static Function* mainFunc;

static Function *writeInteger;
static Function *writeChar;
static Function *writeString;

// Useful LLVM types.
//static Type *i8;
//static Type *i32;
static Type *i64;

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAllocaInt(Function *TheFunction, const std::string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getInt32Ty(TheContext), nullptr, VarName);
}

/* static AllocaInst *CreateEntryBlockAllocaChar(Function *TheFunction, const std::string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getInt8Ty(TheContext), nullptr, VarName);
} */


class AST {
public:
  virtual void printOn(std::ostream &out) const = 0;
  virtual ~AST() {}
  virtual void sem(){}
  virtual Value *compile(){return 0;}// = 0;

  void llvm_compile_and_dump() {  

    i64 = Type::getInt64Ty(TheContext);

     // Open a new context and module.
    TheModule = std::make_unique<Module>("Grace IR", TheContext);

    std::unique_ptr<Module> module = std::make_unique<Module>("MyModule", TheContext);

    // declare void @writeInteger(i64)
    FunctionType *writeInteger_type = FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *> { i64 }, false);
    writeInteger = Function::Create(writeInteger_type, Function::ExternalLinkage, "writeInteger", TheModule.get());

     // declare void @writeChar(i8)
    FunctionType *writeChar_type = FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *> { i64 }, false);
    writeChar = Function::Create(writeChar_type, Function::ExternalLinkage, "writeChar", TheModule.get());

    // declare void @writeString(i8*)
    FunctionType *writeString_type = FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *> { PointerType::get(i64, 0) }, false);
    writeString = Function::Create(writeString_type, Function::ExternalLinkage, "writeString", TheModule.get());


    // Create a function named "main" with return type i32
    FunctionType *funcType = FunctionType::get(Type::getInt32Ty(TheContext), false);
    mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", TheModule.get());

    // Create a basic block
    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", mainFunc);

    // Create an IR builder
    builder.SetInsertPoint(BB);
    // Emit the program code.
    compile();
    // Generate code: return 0;
    Value *returnValue = ConstantInt::get(Type::getInt32Ty(TheContext), 0);
    builder.CreateRet(returnValue);

    printf("- \033[1;33mIR Generation\033[0m: \033[1;32mPASSED\033[0m\n");
    /* printf("============================\n");
    TheModule->print(outs(), nullptr);
    printf("============================\n"); */

    // Open the file for writing
    std::error_code EC;
    llvm::raw_fd_ostream outputFile("temp.ll", EC);
      if (EC) {
        printf("\033[1;31merror:\n\t\033[0m");
        printf("Error opening file for writing IR.\n");
        printf("- Compilation \033[1;31mFAILED\033[0m.\n");
        exit(1);
    }
    TheModule->print(outputFile, nullptr);
    printf("- \033[1;33mIR Save\033[0m: \033[1;32mPASSED\033[0m\n");

  }

protected:

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

  virtual Value *compile() override{
    return  ConstantInt::get(Type::getInt32Ty(TheContext), num);
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

class TypeBts: public Func {
public:
  TypeBts(){}
  TypeBts(Datatype s, ConstList *c = nullptr): type(s), const_list(c) {}
  ~TypeBts() { delete const_list; }
  virtual void printOn(std::ostream &out) const override {
    out << "TypeBts(" << type;
    if (const_list->length() != 0) out << ", " << *const_list;
    out << ")";
  }
  Datatype getTypeBts() {
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

  virtual Value *compile() override {
    return builder.CreateLoad(NamedValues[str]->getAllocatedType(), NamedValues[str], str);
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

  char *getString(){
    int i=1, j=0;
    char* new_str = str;
    do{
      if(str[i]=='\"' && str[i-1]!='\\'){new_str[j] = '\0'; break;}
      if(str[i]!='\\') new_str[j] = str[i];
      else{
        if(str[i+1]=='n') new_str[j] = '\n';
        else if(str[i+1]=='t') new_str[j] = '\t';
        else if(str[i+1]=='r') new_str[j] = '\r';
        else if(str[i+1]=='0') new_str[j] = '\0';
        else if(str[i+1]=='\\') new_str[j] = '\\';
        else if(str[i+1]=='\'') new_str[j] = '\'';
        else if(str[i+1]=='\"') new_str[j] = '\"';
        else if(str[i+1]=='x'){
          int n1, n2;
          if(str[i+2] >= '0' && str[i+2] <= '9') n1 = str[i+2] -'0';
          else n1 = str[i+2] -'a' +10;
          if(str[i+3] >= '0' && str[i+3] <= '9') n2 = str[i+3] -'0';
          else n2 = int(str[i+3]) -'a' +10;
          new_str[j] = char(n1* 16 + n2);
          i++;i++;
        }
        i++;
      }
      i++;
      j++;
    }while(str[i]!='\0');
    return new_str;
  }

  virtual Value *compile() override {
    return builder.CreateGlobalStringPtr(getString());
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

  // GIATI EKANES TO CONSTCHAR CHAR* ???????
  char getChar(){
    int len;
    for(int i=1;i<8;i++){
      if(constchar[i]=='\''&& constchar[i+1]!='\'') //case of '\''
      {
        len = i+1;
        break;
      }
    }
    if(len == 3) return constchar[1];
    if(len == 4){
      if(constchar[2]=='n') return '\n';
      if(constchar[2]=='t') return '\t';
      if(constchar[2]=='r') return '\r';
      if(constchar[2]=='0') return '\0';
      if(constchar[2]=='\\') return '\\';
      if(constchar[2]=='\'') return '\'';
      else return '\"';
    }
    int n1, n2;
    if(constchar[3] >= '0' && constchar[3] <= '9') n1 = constchar[3] -'0';
    else n1 = constchar[3] -'a' +10;
    if(constchar[4] >= '0' && constchar[4] <= '9') n2 = constchar[4] -'0';
    else n2 = int(constchar[4]) -'a' +10;
    return char(n1* 16 + n2);
  }

  virtual Value *compile() override{
    std::string str = constchar;
    return  ConstantInt::get(Type::getInt32Ty(TheContext), getChar());
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


  virtual Value *compile() override{
    if(strcmp(typeid(*lvalue).name(), "2Id")==0){
      //expr->getIndexes();
    }
    else{
      return lvalue->compile();
    }
    return nullptr;
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

 virtual Value* compile()  override {
    return expr->compile();
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

 virtual Value* compile()  override {
    return builder.CreateMul(ConstantInt::get(Type::getInt32Ty(TheContext), -1) ,expr->compile(), "multmp");
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

  virtual Value* compile()  override {
    Value *e = expr->compile();
    return builder.CreateNot(e, "ifcond");
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

  virtual Value* compile()  override {
    Value *l = left->compile();
    Value *r = right->compile();
    if(strcmp(op, "+") == 0) return builder.CreateAdd(l, r, "addtmp");
    if(strcmp(op, "-") == 0) return builder.CreateSub(l, r, "subtmp");
    if(strcmp(op, "*") == 0) return builder.CreateMul(l, r, "multmp");
    if(strcmp(op, "div") == 0) return builder.CreateSDiv(l, r, "divtmp");
    if(strcmp(op, "mod") == 0) return builder.CreateSRem(l, r, "modtmp");
    return nullptr;
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

  virtual Value* compile()  override {
    Value *l = left->compile();
    Value *r = right->compile();
    if(strcmp(op, "=") == 0) return builder.CreateICmpEQ(l, r, "ifcond");
    if(strcmp(op, "#") == 0) return builder.CreateICmpNE(l, r, "ifcond");
    if(strcmp(op, ">") == 0) return builder.CreateICmpSGT(l, r, "ifcond");
    if(strcmp(op, ">=") == 0) return builder.CreateICmpSGE(l, r, "ifcond");
    if(strcmp(op, "<") == 0) return builder.CreateICmpSLT(l, r, "ifcond");
    if(strcmp(op, "<=") == 0) return builder.CreateICmpSLE(l, r, "ifcond");
    if(strcmp(op, "and") == 0) return builder.CreateAnd(l, r, "ifcond");
    if(strcmp(op, "or") == 0) return builder.CreateOr(l, r, "ifcond");
    return nullptr;
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

  /* IT WILL NEED CHANGES */
  virtual Value *compile() override{
    if(expr_list.size()==1){
      return expr_list[0]->compile();
    }
    return nullptr;
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

  virtual Value *compile() override {
    if(strcmp(typeid(*expr1).name(), "2Id")==0) //Den 3erw giati 8eleri 2Id ??
      builder.CreateStore(expr2->compile(), NamedValues[expr1->getName()]);
    else{
   /*      int dimensions = type->getConstList().size();
        std::vector<int> sizes = type->getConstList();

        // Calculate the total size of the N-dimensional array
        Value *totalSize = ConstantInt::get(Type::getInt32Ty(TheContext),  type->getConstList()[0]);
        for (int i = 1; i < dimensions; ++i) {
            totalSize = builder.CreateMul(totalSize, ConstantInt::get(Type::getInt32Ty(TheContext), sizes[i]));
        }

        // Allocate memory for the N-dimensional array
        AllocaInst *arrayPtr = builder.CreateAlloca(Type::getInt32Ty(TheContext), totalSize, c);
        NamedValues[c] = arrayPtr;
 */



      char* id = expr1->getName();
      Value *firstElementPtr = builder.CreateGEP(Type::getInt32Ty(TheContext), NamedValues[id],  ConstantInt::get(Type::getInt32Ty(TheContext), 0  ));
      builder.CreateStore(expr2->compile(), firstElementPtr);
      //Value* index0 = ConstantInt::get(int32_t, 0);
      //Value* elementPtr = builder.CreateGEP(Type::getInt32Ty(TheContex`t), id, {ConstantInt::get(Type::getInt32Ty(TheContext), 0)}));
      //builder.CreateStore(ConstantInt::get(int32_t, 42), elementPtr);
      //printf("%s\n", id);
      //std::vector<int> indexes = expr1->getIndexes();
/*      ArrayType *dimensions = ArrayType::get(Type::getInt32Ty(TheContext), indexes[0]);
        for (long unsigned int i=1; i<indexes.size() ;i++) {
          dimensions = ArrayType::get(dimensions, indexes[i]);
        }
      //Value* elementPtr = builder.CreateGEP(NamedValues[id], dimensions);
      builder.CreateStore(expr2->compile(), NamedValues[id]); */
    }
    return nullptr;
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

  virtual Value *compile() override {
    Value *CondV = cond->compile();
    Function *TheFunction = builder.GetInsertBlock()->getParent();

    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
    BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
    BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else", TheFunction);
    BasicBlock *MergeBB = BasicBlock::Create(TheContext, "endif", TheFunction);

    builder.CreateCondBr(CondV, ThenBB, ElseBB);
    builder.SetInsertPoint(ThenBB);
    stmt1->compile();
    builder.CreateBr(MergeBB);
    builder.SetInsertPoint(ElseBB);
    if (stmt2!=nullptr) stmt2->compile();
    builder.CreateBr(MergeBB);
    builder.SetInsertPoint(MergeBB);
    return nullptr;
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

  virtual Value* compile() override {
    Value *n = expr->compile();
    BasicBlock *PrevBB = builder.GetInsertBlock();
    Function *TheFunction = PrevBB->getParent();
    BasicBlock *LoopBB =
      BasicBlock::Create(TheContext, "loop", TheFunction);
    BasicBlock *BodyBB =
      BasicBlock::Create(TheContext, "body", TheFunction);
    BasicBlock *AfterBB =
      BasicBlock::Create(TheContext, "endwhile", TheFunction);
    builder.CreateBr(LoopBB);
    builder.SetInsertPoint(LoopBB);
    PHINode *phi_iter = builder.CreatePHI(Type::getInt1Ty(TheContext), 2, "iter");
    phi_iter->addIncoming(n, PrevBB);
    Value *loop_cond = expr->compile();
    builder.CreateCondBr(loop_cond, BodyBB, AfterBB);
    builder.SetInsertPoint(BodyBB);
    stmt->compile();
    phi_iter->addIncoming(n, builder.GetInsertBlock());
    builder.CreateBr(LoopBB);
    builder.SetInsertPoint(AfterBB);
    return nullptr;
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

  virtual Value *compile() override{
    for (Stmt *s: stmt_list) s->compile();
    return nullptr;
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

  virtual Value *compile() override{
    if(strcmp(id,"writeInteger")==0){
      Value *n = expr_list->compile();
      Value *n64 = builder.CreateZExt(n, Type::getInt64Ty(TheContext));
      builder.CreateCall(writeInteger, std::vector<Value *> { n64 });
    }
    else if(strcmp(id,"writeChar")==0){
      Value *c = expr_list->compile();
      Value *c64 = builder.CreateZExt(c, Type::getInt64Ty(TheContext));
      builder.CreateCall(writeChar, std::vector<Value *> { c64 });
    }else if(strcmp(id,"writeString")==0){
      Value *s = expr_list->compile();
      Value *s64 = builder.CreateZExt(s, Type::getInt64PtrTy(TheContext));
      builder.CreateCall(writeString, std::vector<Value *> { s64 });
    }
    return nullptr;
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
    if (isDefined == true) {
      st.addScopeNameAndType(id, rettype);
      st.insertFunctionToPreviousScope(id, rettype, fpar->getFunctionParameters(), line);
      fpar->sem(); 
    } else {
      st.insertFunction(id, rettype, fpar->getFunctionParameters(), line, false);
    }
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

  virtual Value *compile() override {
    for(Func *ldef_elem : ldef_list) ldef_elem->compile();
    return nullptr;
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

  virtual Value *compile() override{
    local->compile();
    block->compile();
    return nullptr;
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
  VarDef(IdList *id, TypeBts *t, int l): id_list(id), type(t), line(l) {}
  ~VarDef() { delete id_list; delete type; }
  virtual void printOn(std::ostream &out) const override {
    out << "VarDef(" << *id_list << ", " << *type << ")";
  }
  virtual void sem() override {
    Datatype t = type->getTypeBts();
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

  virtual Value *compile() override {
    if(type->getConstList().size()==0){
      for (char *c : id_list->getIdList()) {
        AllocaInst *Alloca  = CreateEntryBlockAllocaInt(mainFunc, c);
        NamedValues[c] = Alloca;
      }
    }
    else{
      for (char *c : id_list->getIdList()) {

        int dimensions = type->getConstList().size();
        std::vector<int> sizes = type->getConstList();

        // Calculate the total size of the N-dimensional array
        Value *totalSize = ConstantInt::get(Type::getInt32Ty(TheContext),  type->getConstList()[0]);
        for (int i = 1; i < dimensions; ++i) {
            totalSize = builder.CreateMul(totalSize, ConstantInt::get(Type::getInt32Ty(TheContext), sizes[i]));
        }

        // Allocate memory for the N-dimensional array
        AllocaInst *arrayPtr = builder.CreateAlloca(Type::getInt32Ty(TheContext), totalSize, c);
        NamedValues[c] = arrayPtr;




     /*    Value *dimensions = ConstantInt::get(Type::getInt32Ty(TheContext), type->getConstList()[0]);
         for (long unsigned int i=1; i<type->getConstList().size() ;i++) {
          dimensions = ConstantInt::get(dimensions, type->getConstList()[i]);
        } 
        AllocaInst *ArrayAlloc = builder.CreateAlloca(Type::getInt32Ty(TheContext), dimensions, c);
        NamedValues[c] = ArrayAlloc; */
      }
    }
    return nullptr;
  }

private:
  IdList *id_list;
  TypeBts *type;
  int line;
};


#endif