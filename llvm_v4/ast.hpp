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
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

using namespace llvm;

// Global LLVM variables related to the LLVM suite.
static LLVMContext TheContext;
static std::unique_ptr<Module> TheModule;

static std::map<std::string, Value *> NamedValues;
static std::map<std::string, Type *> NamedValuesTypes;
static std::map<std::string, std::vector<int>> ArrDimensions;
static std::map<std::string, int> isCharVar;

static Function* firstFunc;
static bool firstFuncDefined=false;
static std::map<std::string, Function*> FuncPtr;
static std::map<std::string, std::vector<Type *>>  funcParamTypes;
static std::map<std::string, std::vector<int>>  funcParamRefs;
static std::map<std::string, Datatype> ReturnTypesOfFunc;

static IRBuilder<> builder(TheContext);
static Function* mainFunc;

static Function *writeInteger;
static Function *writeChar;
static Function *writeString;

static Function *readInteger;
static Function *readChar;
static Function *readString;

static Function *ascii;
static Function *chr;

static Function *strlenGrace;
static Function *strcmpGrace;
static Function *strcpyGrace;
static Function *strcatGrace;

// Useful LLVM types.
static Type *i8;
static Type *i32;
static Type *i64;

static bool optimOption;
static std::unique_ptr<legacy::FunctionPassManager> TheFPM;

class AST {
public:
  virtual void printOn(std::ostream &out) const = 0;
  virtual ~AST() {}
  virtual void sem(){}
  virtual Value *compile(){return 0;}

  void llvm_compile_and_dump() {  

    i8 = Type::getInt8Ty(TheContext);
    i32 = Type::getInt32Ty(TheContext);
    i64 = Type::getInt64Ty(TheContext);

     // Open a new context and module.
    TheModule = std::make_unique<Module>("Grace IR", TheContext);

    TheFPM = std::make_unique<legacy::FunctionPassManager>(TheModule.get());

    printf("- \033[1;33mOptimization Selected\033[0m: ");
    if(optimOption) printf("\033[1;32mYES\033[0m\n");
    else printf("\033[1;31mNO\033[0m\n");

    if(optimOption){
      TheFPM->add(createPromoteMemoryToRegisterPass());
      TheFPM->add(createInstructionCombiningPass());
      TheFPM->add(createReassociatePass());
      TheFPM->add(createGVNPass()); 
      TheFPM->add(createCFGSimplificationPass());
    }

    TheFPM->doInitialization();
 
    // declare void @writeInteger()
    FunctionType *writeInteger_type = FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *> { i32 }, false);
    writeInteger = Function::Create(writeInteger_type, Function::ExternalLinkage, "writeInteger", TheModule.get());

     // declare void @writeChar()
    FunctionType *writeChar_type = FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *> { i8 }, false);
    writeChar = Function::Create(writeChar_type, Function::ExternalLinkage, "writeChar", TheModule.get());

    // declare void @writeString()
    FunctionType *writeString_type = FunctionType::get(Type::getVoidTy(TheContext),  std::vector<Type *> { PointerType::get(i8, 0) }, false);
    writeString = Function::Create(writeString_type, Function::ExternalLinkage, "writeString", TheModule.get());

    // declare void @readInteger()
    FunctionType *readInteger_type = FunctionType::get(Type::getInt32Ty(TheContext), false);
    readInteger = Function::Create(readInteger_type, Function::ExternalLinkage, "readInteger", TheModule.get());

    // declare void @readChar()
    FunctionType *readChar_type = FunctionType::get(Type::getInt8Ty(TheContext), false);
    readChar = Function::Create(readChar_type, Function::ExternalLinkage, "readChar", TheModule.get());

    // declare void @readString()
    FunctionType *readString_type = FunctionType::get(Type::getVoidTy(TheContext),  std::vector<Type *> { PointerType::get(i8, 0) }, false);
    readString = Function::Create(readString_type, Function::ExternalLinkage, "readString", TheModule.get());

    // declare void @ascii()
    FunctionType *ascii_type = FunctionType::get(Type::getInt32Ty(TheContext),  std::vector<Type *> { i8 }, false);
    ascii = Function::Create(ascii_type, Function::ExternalLinkage, "ascii", TheModule.get());

    // declare void @chr()
    FunctionType *chr_type = FunctionType::get(Type::getInt8Ty(TheContext), std::vector<Type *> { i32 }, false);
    chr = Function::Create(chr_type, Function::ExternalLinkage, "chr", TheModule.get());

    // declare void @strlenGrace()
    FunctionType *strlenGrace_type = FunctionType::get(Type::getInt32Ty(TheContext), std::vector<Type *> { PointerType::get(i8, 0) }, false);
    strlenGrace = Function::Create(strlenGrace_type, Function::ExternalLinkage, "strlenGrace", TheModule.get());
    
    // declare void @strcmpGrace()
    FunctionType *strcmpGrace_type = FunctionType::get(Type::getInt32Ty(TheContext), std::vector<Type *> { PointerType::get(i8, 0), PointerType::get(i8, 0) }, false);
    strcmpGrace = Function::Create(strcmpGrace_type, Function::ExternalLinkage, "strcmpGrace", TheModule.get());

    // declare void @strcpyGrace()
    FunctionType *strcpyGrace_type = FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *> { PointerType::get(i8, 0), PointerType::get(i8, 0) }, false);
    strcpyGrace = Function::Create(strcpyGrace_type, Function::ExternalLinkage, "strcpyGrace", TheModule.get());

    // declare void @strcatGrace()
    FunctionType *strcatGrace_type = FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *> { PointerType::get(i8, 0), PointerType::get(i8, 0) }, false);
    strcatGrace = Function::Create(strcatGrace_type, Function::ExternalLinkage, "strcatGrace", TheModule.get());


   
    // Emit the program code.
    compile();
    FunctionType *funcType = FunctionType::get(Type::getInt32Ty(TheContext), false);
    mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", TheModule.get());
    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", mainFunc);
    builder.SetInsertPoint(BB);
    builder.CreateCall(firstFunc);
    Value *returnValue = ConstantInt::get(Type::getInt32Ty(TheContext), 0);
    builder.CreateRet(returnValue);

    printf("- \033[1;33mIR Generation\033[0m: \033[1;32mPASSED\033[0m\n");

    // Open the file for writing
    std::error_code EC;
    llvm::raw_fd_ostream outputFile("temp.imm", EC);
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
    // sem();
    if (type != t) {
      showSemanticError(code, line, op, t, type);
    }
  }
  Datatype type_check_return(std::vector<Datatype> t, int code, int line, char* op = const_cast<char*>("")) {
    // sem();
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

  virtual Value *getOffset(char* c, int i) {
    return 0;
  }

  virtual int numOfDimensionsPassed(int recursion) {
    return 0;
  }

  virtual SymbolEntry * getSymbolEntry() {
    return new VarEntry();  
  }

  virtual std::string getTypeOfExpr() {
    return "";
  }

  virtual void semArrayCall(std::vector<int> dimensions) {
    return;
  }
  

protected:
  Datatype type;
};

class Stmt: public AST {

};

class Func: public AST {
  public:

    virtual Datatype getDataype() {return TYPE_nothing;}

    virtual std::string getFuncName() {return nullptr;}

    virtual std::string getTypeOfFunc() {return "";}

    virtual void findFparTypes(std::string funcName) {}
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

  virtual std::string getTypeOfExpr() override{
    return "Const";
  }

  int getConst() {
    return num;
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
    return builder.CreateLoad(NamedValuesTypes[str], NamedValues[str], str);
  }

  virtual std::string getTypeOfExpr() override{
    return "Id";
  }
private:
  char *str;
  int line;
};

class String: public Expr {
public:
  String(char *s): str(s) {
    int i=1, j=0;
    new_str = str;
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
          else n1 = str[i+2] -'A' +10;
          if(str[i+3] >= '0' && str[i+3] <= '9') n2 = str[i+3] -'0';
          else n2 = int(str[i+3]) -'A' +10;
          new_str[j] = char(n1* 16 + n2);
          i++;i++;
        }
        i++;
      }
      i++;
      j++;
    }while(str[i]!='\0');
  }
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
    return new_str;
  }

  virtual Value *compile() override {
    return builder.CreateGlobalStringPtr(getString());
  }

  virtual std::string getTypeOfExpr() override{
    return "String";
  }

private:
    char *str, *new_str;
};

class ConstChar: public Expr {
public:
  ConstChar(char *c): constchar(c) {
    // GIATI EKANES TO CONSTCHAR CHAR* ???????
    int len;
    for(int i=1;i<8;i++){
      if(constchar[i]=='\''&& constchar[i+1]!='\'') //case of '\''
      {
        len = i+1;
        break;
      }
    }
    if(len == 3) {newConstchar = constchar[1]; return;}
    if(len == 4){
      if(constchar[2]=='n') {newConstchar = '\n'; return;}
      if(constchar[2]=='t') {newConstchar = '\t'; return;}
      if(constchar[2]=='r') {newConstchar = '\r'; return;}
      if(constchar[2]=='0') {newConstchar = '\0'; return;}
      if(constchar[2]=='\\') {newConstchar = '\\'; return;}
      if(constchar[2]=='\'') {newConstchar = '\''; return;}
      else {newConstchar = '\"'; return;}
    }
    int n1, n2;
    if(constchar[3] >= '0' && constchar[3] <= '9') n1 = constchar[3] -'0';
    else n1 = constchar[3] -'A' +10;
    if(constchar[4] >= '0' && constchar[4] <= '9') n2 = constchar[4] -'0';
    else n2 = int(constchar[4]) -'A' +10;
    newConstchar =  char(n1* 16 + n2);
  }
  virtual void printOn(std::ostream &out) const override {
    out << "ConstChar(" << constchar << ")";
  }
  virtual void sem() override {
    type = TYPE_char;
  }
  virtual char *getName() override {
    return constchar;
  }

  char getChar(){
    return newConstchar;
  }

  virtual Value *compile() override{
    std::string str = constchar;
    return  ConstantInt::get(Type::getInt8Ty(TheContext), getChar());
  }


private:
  char *constchar;
  char newConstchar;
};

class ArrayCall: public Expr {
public:
  ArrayCall(Expr *l, Expr *r, int li): lvalue(l), expr(r), line(li) {}
  ~ArrayCall() { delete lvalue; delete expr; }
  virtual void printOn(std::ostream &out) const override {
    out << "ArrayCall(" << *lvalue << ", " << *expr << ")";
  }
  virtual void sem() override {
    return;
  }
  virtual char *getName() override {
    return lvalue->getName();
  }
  virtual SymbolEntry *getSymbolEntry() override {
    return st.lookup(lvalue->getName(), line, {TYPE_array});
  }
  virtual std::string getTypeOfExpr() override{
    return "Array";
  }
  virtual void semArrayCall(std::vector<int> dimensions) override {
    int current = dimensions.back();
    dimensions.pop_back();

    if (expr->getTypeOfExpr() == "Const") {
      if (current != 0 && dynamic_cast<Const *>(expr)->getConst() >= current) {
        showSemanticWarning(1, line, lvalue->getName());
      }
    }

    if (dimensions.size() > 0) {
      if (lvalue->getTypeOfExpr() == "Array") {
        lvalue->semArrayCall(dimensions);
      } else {
        showSemanticError(25, line, lvalue->getName());
      }
    } else {
      if (lvalue->getTypeOfExpr() == "Array") {
        showSemanticError(25, line, lvalue->getName());
      } else {
        lvalue->sem();
      }
    }

    SymbolEntry *e = expr->getSymbolEntry();
    std::vector<int> innerDimensions = e->getDimensions();
    if (innerDimensions.size() == 0 && expr->getTypeOfExpr() == "Array") {
      showSemanticError(28, line, expr->getName());
    } else if (innerDimensions.size() > 0 && expr->getTypeOfExpr() != "Array") {
      showSemanticError(25, line, expr->getName());
    } else if (innerDimensions.size() > 0 && expr->getTypeOfExpr() == "Array") {
      expr->semArrayCall(innerDimensions);
      expr->sem();
    } else if (expr->getTypeOfExpr() == "String") {
      showSemanticError(2, line, expr->getName());
    } else {
      expr->sem();
    }

    type = st.lookup(lvalue->getName(), line, {TYPE_array})->getDatatype();
    expr->type_check(TYPE_int, 2, line);
  }



  virtual Value *getOffset(char* c, int levelOfRecursion) override{
    int sizeOfTheRestOftheArray = 1;
    for(long unsigned int j=levelOfRecursion; j<ArrDimensions[c].size(); j++){
      sizeOfTheRestOftheArray *= ArrDimensions[c][j];
    }
    if(lvalue->getTypeOfExpr() == "Id"){
        return builder.CreateMul(expr->compile(), ConstantInt::get(Type::getInt32Ty(TheContext),sizeOfTheRestOftheArray));      
    }
    else{
        return builder.CreateAdd(lvalue->getOffset(c, levelOfRecursion-1),
                                  builder.CreateMul(expr->compile(), ConstantInt::get(Type::getInt32Ty(TheContext),sizeOfTheRestOftheArray)));
    } 
  }


  virtual int numOfDimensionsPassed(int recursion) override{
    if(lvalue->getTypeOfExpr() == "Id") return recursion;
    return lvalue->numOfDimensionsPassed(recursion+1);
  }

  virtual Value *compile() override{
    char* id = lvalue->getName();
    if(strcmp(typeid(*lvalue).name(), "2Id")==0){
      if(isCharVar[id] == 1){
        return builder.CreateLoad(NamedValuesTypes[id], builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {expr->compile()}), id);
      }
      else{
        return builder.CreateLoad(NamedValuesTypes[id], builder.CreateGEP(Type::getInt32Ty(TheContext), NamedValues[id],  {expr->compile()}), id);
      }
      
    }
    else{
      Value *offset = getOffset(id, ArrDimensions[id].size());
      Value *elementPtr;
      if(isCharVar[id] == 1){
        elementPtr = builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {offset});
        return builder.CreateLoad(NamedValuesTypes[id],  elementPtr, id);
      }
      else{
        elementPtr = builder.CreateGEP(Type::getInt32Ty(TheContext), NamedValues[id],  {offset});
        return builder.CreateLoad(NamedValuesTypes[id],  elementPtr, id);
      }
    }
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
    SymbolEntry *e = expr->getSymbolEntry();
    std::vector<int> innerDimensions = e->getDimensions();
    if (innerDimensions.size() == 0 && expr->getTypeOfExpr() == "Array") {
      showSemanticError(28, line, expr->getName());
    } else if (innerDimensions.size() > 0 && expr->getTypeOfExpr() != "Array") {
      showSemanticError(25, line, expr->getName());
    } else if (innerDimensions.size() > 0 && expr->getTypeOfExpr() == "Array") {
      expr->semArrayCall(innerDimensions);
      expr->sem();
    } else if (expr->getTypeOfExpr() == "String") {
      showSemanticError(29, line, expr->getName());
    } else {
      expr->sem();
    }

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
    SymbolEntry *e = expr->getSymbolEntry();
    std::vector<int> innerDimensions = e->getDimensions();
    if (innerDimensions.size() == 0 && expr->getTypeOfExpr() == "Array") {
      showSemanticError(28, line, expr->getName());
    } else if (innerDimensions.size() > 0 && expr->getTypeOfExpr() != "Array") {
      showSemanticError(25, line, expr->getName());
    } else if (innerDimensions.size() > 0 && expr->getTypeOfExpr() == "Array") {
      expr->semArrayCall(innerDimensions);
      expr->sem();
    } else if (expr->getTypeOfExpr() == "String") {
      showSemanticError(29, line, expr->getName());
    } else {
      expr->sem();
    }

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
    expr->sem();
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
    SymbolEntry *e1 = left->getSymbolEntry();
    std::vector<int> innerDimensions1 = e1->getDimensions();
    if (innerDimensions1.size() == 0 && left->getTypeOfExpr() == "Array") {
      showSemanticError(28, line, left->getName());
    } else if (innerDimensions1.size() > 0 && left->getTypeOfExpr() != "Array") {
      showSemanticError(25, line, left->getName());
    } else if (innerDimensions1.size() > 0 && left->getTypeOfExpr() == "Array") {
      left->semArrayCall(innerDimensions1);
      left->sem();
    } else if (left->getTypeOfExpr() == "String") {
      showSemanticError(29, line, left->getName());
    } else {
      left->sem();
    }

    SymbolEntry *e2 = right->getSymbolEntry();
    std::vector<int> innerDimensions2 = e2->getDimensions();
    if (innerDimensions2.size() == 0 && right->getTypeOfExpr() == "Array") {
      showSemanticError(28, line, right->getName());
    } else if (innerDimensions2.size() > 0 && right->getTypeOfExpr() != "Array") {
      showSemanticError(25, line, right->getName());
    } else if (innerDimensions2.size() > 0 && right->getTypeOfExpr() == "Array") {
      right->semArrayCall(innerDimensions2);
      right->sem();
    } else if (right->getTypeOfExpr() == "String") {
      showSemanticError(29, line, right->getName());
    } else {
      right->sem();
    }

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
    SymbolEntry *e1 = left->getSymbolEntry();
    std::vector<int> innerDimensions1 = e1->getDimensions();
    if (innerDimensions1.size() == 0 && left->getTypeOfExpr() == "Array") {
      showSemanticError(28, line, left->getName());
    } else if (innerDimensions1.size() > 0 && left->getTypeOfExpr() != "Array") {
      showSemanticError(25, line, left->getName());
    } else if (innerDimensions1.size() > 0 && left->getTypeOfExpr() == "Array") {
      left->semArrayCall(innerDimensions1);
      left->sem();
    } else if (left->getTypeOfExpr() == "String") {
      showSemanticError(29, line, left->getName());
    } else {
      left->sem();
    }

    SymbolEntry *e2 = right->getSymbolEntry();
    std::vector<int> innerDimensions2 = e2->getDimensions();
    if (innerDimensions2.size() == 0 && right->getTypeOfExpr() == "Array") {
      showSemanticError(28, line, right->getName());
    } else if (innerDimensions2.size() > 0 && right->getTypeOfExpr() != "Array") {
      showSemanticError(25, line, right->getName());
    } else if (innerDimensions2.size() > 0 && right->getTypeOfExpr() == "Array") {
      right->semArrayCall(innerDimensions2);
      right->sem();
    } else if (right->getTypeOfExpr() == "String") {
      showSemanticError(29, line, right->getName());
    } else {
      right->sem();
    }

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
  ExprList(int l): expr_list(), line(l) {}
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
  virtual void exprListSem(SymbolEntry* func, int l, char* f) {
    std::vector<Datatype> datatypes = func->getParameterDatatypes();
    std::vector<std::vector<int>> dimensions = func->getParameterDimensions();
    if (expr_list.size() != datatypes.size()) {
      showSemanticError(14, l, f);
    }
    int i = 0;
    for (Expr *expr : expr_list) {
      SymbolEntry *e = expr->getSymbolEntry();
      std::vector<int> exprDimensions = e->getDimensions();
      // var is not an array, but gets array call
      if (expr->getTypeOfExpr() == "Array" && exprDimensions.size() == 0) {
        showSemanticError(28, line, expr->getName());
      // var is an array, does not get array call
      } else if (expr->getTypeOfExpr() != "Array" && exprDimensions.size() > 0) {
        if (dimensions[i].size() == exprDimensions.size()) {
          for (long unsigned int j = 0; j < dimensions[i].size(); j++) {
            if (dimensions[i][j] != 0 && exprDimensions[j] != 0 && dimensions[i][j] != exprDimensions[j]) {
              showSemanticError(30, line, func->getParameterNames()[i]);
            }
          }
          expr->sem();
        } else {
          showSemanticError(30, line, func->getParameterNames()[i]);
        }
      // var is an array, gets array call
      } else if (expr->getTypeOfExpr() == "Array" && exprDimensions.size() > 0) {
        if (dimensions[i].size() >= exprDimensions.size()) {
          showSemanticError(30, line, func->getParameterNames()[i]);
        } else if (dimensions[i].size() < exprDimensions.size()) {
          std::vector<int> newDimensions = {};
          for (long unsigned int j = 0; j < exprDimensions.size() - dimensions[i].size(); j++) {
            newDimensions.push_back(exprDimensions[j]);
          }

          expr->semArrayCall(newDimensions);
          for (long unsigned int j = 0; j < dimensions[i].size(); j++) {
            if (dimensions[i][j] != 0 && exprDimensions[j + exprDimensions.size() - dimensions[i].size()] != 0 && dimensions[i][j] != exprDimensions[j + exprDimensions.size() - dimensions[i].size()]) {
              showSemanticError(30, line, func->getParameterNames()[i]);
            }
          }
          expr->sem();
        }
      // var is not an array, does not get array call
      } else if (expr->getTypeOfExpr() != "Array" && exprDimensions.size() == 0) {
        if (expr->getTypeOfExpr() == "String" && dimensions[i].size() == 0) {
          showSemanticError(31, line, func->getParameterNames()[i]);
        } else if (expr->getTypeOfExpr() == "String" && dimensions[i].size() == 1) {
          String *string = dynamic_cast<String *>(expr);
          if (dimensions[i][0] != 0 && dimensions[i][0] < (static_cast<int>(strlen(string->getString()))+1)) {
            showSemanticError(32, line, expr->getName());
          }
          expr->sem();
        } else if (expr->getTypeOfExpr() == "String" && dimensions[i].size() > 0) {
          showSemanticError(30, line, func->getParameterNames()[i]);
        } else if (dimensions[i].size() > 0) {
          showSemanticError(30, line, func->getParameterNames()[i]);
        } else {
          expr->sem();
        }
      }

      expr->type_check(datatypes[i], 15, l, expr->getName());
      ++i;
    }
  }

  /* IT WILL NEED CHANGES */
  std::vector<Value *> compile_ExprList(bool libFunc = true, std::string funcName = ""){
    std::vector<Value *> v;
    if(libFunc){
      for (Expr *c : expr_list) {
        v.push_back(c->compile());
      }
    }
    else{
      if(funcParamRefs.find(funcName)== funcParamRefs.end()) 
        return v;
      std::vector<int> paramRefs = funcParamRefs[funcName];
      long unsigned int i=-1;
      for (Expr *c : expr_list) {      
        i++;
        if(paramRefs[i] == 0){ v.push_back(c->compile()); continue;}
        
        /* FOR REF*/
        char *id = c->getName();
        std::string typeOfExpr = c->getTypeOfExpr();
        
        /* IF IT IS ID*/
        if(typeOfExpr == "Id"){
          if(isCharVar[id] == 1)
           v.push_back(builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {ConstantInt::get(Type::getInt32Ty(TheContext), 0)}));
          else
            v.push_back(builder.CreateGEP(Type::getInt32Ty(TheContext), NamedValues[id],  {ConstantInt::get(Type::getInt32Ty(TheContext), 0)}));
          continue;
        }
        
        /*IF IT IS STRING*/
        if(typeOfExpr == "String"){
          v.push_back(c->compile());
          continue;
        }
        
        /*IF IT IS ARRAY*/
        int numOfDimensionPassed = c->numOfDimensionsPassed(1);
        Value *offset = c->getOffset(id, numOfDimensionPassed);
        if(isCharVar[id] == 1)
           v.push_back( builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {offset}));
          else
            v.push_back( builder.CreateGEP(Type::getInt32Ty(TheContext), NamedValues[id],  {offset}));

      }
    }
    return v;
  }

  
  std::vector<Expr *> getExprList(){
    std::vector<Expr *> v;
    for (Expr *c : expr_list) {
      v.push_back(c);
    }
    return v;
  }

private:
  std::vector<Expr *> expr_list;
  int line;
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
      SymbolEntry *e = expr->getSymbolEntry();
      std::vector<int> innerDimensions = e->getDimensions();
      if (innerDimensions.size() == 0 && expr->getTypeOfExpr() == "Array") {
        showSemanticError(28, line, expr->getName());
      } else if (innerDimensions.size() > 0 && expr->getTypeOfExpr() != "Array") {
        showSemanticError(25, line, expr->getName());
      } else if (innerDimensions.size() > 0 && expr->getTypeOfExpr() == "Array") {
        expr->semArrayCall(innerDimensions);
        expr->sem();
      } else if (expr->getTypeOfExpr() == "String") {
        showSemanticError(17, line, st.getCurrentScopeName(), t);
      } else {
        expr->sem();
      }

      expr->type_check(t, 16, line, st.getCurrentScopeName());
    } else if (t != TYPE_nothing) {
      showSemanticError(17, line, st.getCurrentScopeName(), t);
    }
    st.setCurrentScopeReturned();
  }
  virtual Value *compile() override {
    if(expr != nullptr)
      builder.CreateStore(expr->compile(), NamedValues["_returnValue"]);
    builder.CreateBr(BasicBlock::Create(TheContext, "return"));

    return nullptr;
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
    SymbolEntry *e1 = expr1->getSymbolEntry();
    std::vector<int> dimensions = e1->getDimensions();
    if (dimensions.size() == 0 && expr1->getTypeOfExpr() == "Array") {
      showSemanticError(28, line, expr1->getName());
    } else if (dimensions.size() > 0 && expr1->getTypeOfExpr() != "Array") {
      showSemanticError(25, line, expr1->getName());
    } else if (dimensions.size() > 0 && expr1->getTypeOfExpr() == "Array") {
      expr1->semArrayCall(dimensions);
      expr1->sem();
    } else if (expr1->getTypeOfExpr() == "String") {
      showSemanticError(26, line, expr1->getName());
    } else {
      expr1->sem();
    }

    SymbolEntry *e2 = expr2->getSymbolEntry();
    std::vector<int> dimensions2 = e2->getDimensions();
    if (dimensions2.size() == 0 && expr2->getTypeOfExpr() == "Array") {
      showSemanticError(28, line, expr2->getName());
    } else if (dimensions2.size() > 0 && expr2->getTypeOfExpr() != "Array") {
      showSemanticError(25, line, expr2->getName());
    } else if (dimensions2.size() > 0 && expr2->getTypeOfExpr() == "Array") {
      expr2->semArrayCall(dimensions2);
      expr2->sem();
    } else if (expr2->getTypeOfExpr() == "String") {
      showSemanticError(27, line, expr2->getName());
    } else {
      expr2->sem();
    }

    expr2->type_check(e1->getDatatype(), 9, line);
  }

  virtual Value *compile() override {
    if(strcmp(typeid(*expr1).name(), "2Id")==0) //Den 3erw giati 8eleri 2Id ??
      builder.CreateStore(expr2->compile(), NamedValues[expr1->getName()]);
    else{
      char* id = expr1->getName();
      std::vector<int> dimensions = ArrDimensions[id];
      Value *offset = expr1->getOffset(id, dimensions.size());
      Value *elementPtr;
      if(isCharVar[id] == 1){
        elementPtr = builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {offset});
      }
      else{
        elementPtr = builder.CreateGEP(Type::getInt32Ty(TheContext), NamedValues[id],  {offset});
      }
      builder.CreateStore(expr2->compile(), elementPtr);
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
    cond->sem();
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
    expr->sem();
    expr->type_check(TYPE_bool, 8, line);
    stmt->sem();
  }

  virtual Value* compile() override {

    Value *CondV = expr->compile();
    Function *TheFunction = builder.GetInsertBlock()->getParent();

    BasicBlock *DoBB = BasicBlock::Create(TheContext, "do", TheFunction);
    BasicBlock *EndWhileBB = BasicBlock::Create(TheContext, "endwhile", TheFunction);

    builder.CreateCondBr(CondV, DoBB, EndWhileBB);
    builder.SetInsertPoint(DoBB);
    stmt->compile();
    CondV = expr->compile();
    builder.CreateCondBr(CondV, DoBB, EndWhileBB);
    builder.SetInsertPoint(EndWhileBB);
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
      expr_list->exprListSem(e, line, id);
    } 
    else if (e->getParameterDatatypes().size() != 0) {
      showSemanticError(14, line, id);
    }
    type = e->getDatatype();
    if (isVoid == true && type != TYPE_nothing) {
      showSemanticError(13, line, id);
    }
  }

  virtual Value *compile() override{
    if(strcmp(id,"writeInteger")==0){
      builder.CreateCall(writeInteger, expr_list->compile_ExprList());
      return nullptr;
    }
    else if(strcmp(id,"writeChar")==0){
      builder.CreateCall(writeChar, expr_list->compile_ExprList());
      return nullptr;
    }
    else if(strcmp(id,"writeString")==0){
      if (((expr_list->getExprList())[0])->getTypeOfExpr() == "String") {
        builder.CreateCall(writeString, expr_list->compile_ExprList());
        return nullptr;
      }
      else if(((expr_list->getExprList())[0])->getTypeOfExpr() == "Id"){
        char* id = (expr_list->getExprList())[0]->getName();
        Value *s =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {ConstantInt::get(Type::getInt32Ty(TheContext), 0)});
        builder.CreateCall(writeString, std::vector<Value *> { s });
        return nullptr;
      }
      else{
        char* id = (expr_list->getExprList())[0]->getName();
        Value *offset = ((expr_list->getExprList())[0])->getOffset(id, ArrDimensions[id].size()-1);
        Value *s =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {offset});
        builder.CreateCall(writeString, std::vector<Value *> { s });
        return nullptr;
      }
    }
    else if(strcmp(id,"readInteger")==0){
      return builder.CreateCall(readInteger);
    }
    else if(strcmp(id,"readChar")==0){
      return builder.CreateCall(readChar);
    }
    else if(strcmp(id,"readString")==0){
      if(((expr_list->getExprList())[0])->getTypeOfExpr() == "Id"){
        char* id = (expr_list->getExprList())[0]->getName();
        Value *s =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {ConstantInt::get(Type::getInt32Ty(TheContext), 0)});
        builder.CreateCall(readString, std::vector<Value *> { s });
        return nullptr;
      }
      else{
        char* id = (expr_list->getExprList())[0]->getName();
        Value *offset = ((expr_list->getExprList())[0])->getOffset(id, ArrDimensions[id].size()-1);
        Value *s =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {offset});
        builder.CreateCall(readString, std::vector<Value *> { s });
        return nullptr;
      }
    }
    else if(strcmp(id,"ascii")==0){
      Value *c = expr_list->compile_ExprList()[0];
      return builder.CreateCall(ascii, std::vector<Value *> { c });
    }
    else if(strcmp(id,"chr")==0){
      Value *c = expr_list->compile_ExprList()[0];
      return builder.CreateCall(chr, std::vector<Value *> { c });
    }
    else if(strcmp(id,"strlen")==0){
      if (((expr_list->getExprList())[0])->getTypeOfExpr() == "String") {
        Value *s = expr_list->compile_ExprList()[0];
        return builder.CreateCall(strlenGrace, std::vector<Value *> { s });
      }
      else if(((expr_list->getExprList())[0])->getTypeOfExpr() == "Id"){
        char* id = (expr_list->getExprList())[0]->getName();
        Value *s =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {ConstantInt::get(Type::getInt32Ty(TheContext), 0)});
        return builder.CreateCall(strlenGrace, std::vector<Value *> { s });
      }
      else{
        char* id = (expr_list->getExprList())[0]->getName();
        Value *offset = ((expr_list->getExprList())[0])->getOffset(id, ArrDimensions[id].size()-1);
        Value *s =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {offset});
        return builder.CreateCall(strlenGrace, std::vector<Value *> { s });
      }
    }
    else if(strcmp(id,"strcmp")==0 || strcmp(id,"strcpy")==0 || strcmp(id,"strcat")==0){
    Value *s_1, *s_2;
    std::vector<Value *> listValueOfParam = expr_list->compile_ExprList();
    
    if (((expr_list->getExprList())[0])->getTypeOfExpr() == "String") {
      s_1 = listValueOfParam[0];
    }
    else if(((expr_list->getExprList())[0])->getTypeOfExpr() == "Id"){
      char* id = (expr_list->getExprList())[0]->getName();
      s_1 =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {ConstantInt::get(Type::getInt32Ty(TheContext), 0)});
    }
    else{
      char* id = (expr_list->getExprList())[0]->getName();
      Value *offset = ((expr_list->getExprList())[0])->getOffset(id, ArrDimensions[id].size()-1);
      s_1 =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {offset});
    }

    if (((expr_list->getExprList())[1])->getTypeOfExpr() == "String") {
      s_2 = listValueOfParam[1];
    }
    else if(((expr_list->getExprList())[1])->getTypeOfExpr() == "Id"){
      char* id = (expr_list->getExprList())[1]->getName();
      s_2 =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {ConstantInt::get(Type::getInt32Ty(TheContext), 0)});
    }
    else{
      char* id = (expr_list->getExprList())[1]->getName();
      Value *offset = ((expr_list->getExprList())[1])->getOffset(id, ArrDimensions[id].size()-1);
      s_2 =  builder.CreateGEP(Type::getInt8Ty(TheContext), NamedValues[id],  {offset});
    }

      if(strcmp(id,"strcmp")==0)
        return builder.CreateCall(strcmpGrace, std::vector<Value *> { s_1, s_2 });
      if(strcmp(id,"strcpy")==0){
        builder.CreateCall(strcpyGrace, std::vector<Value *> { s_1, s_2 });
        return nullptr;
      }
      if(strcmp(id,"strcat")==0){
        builder.CreateCall(strcatGrace, std::vector<Value *> { s_1, s_2 });
        return nullptr;
      }
      
    }

    else{
      std::vector<Value *> parameters = expr_list->compile_ExprList(false, id);
      if(ReturnTypesOfFunc[id] != TYPE_nothing)
        return builder.CreateCall(FuncPtr[id], parameters);
      else{
        builder.CreateCall(FuncPtr[id], parameters);
        return nullptr;
      }
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
      for (int i : v) {
          if (i == -1) {
            showSemanticError(33, line, id_list->getIdList()[0]);
          }
      }
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
          s.push_back(FunctionParameter(c, t, false, ref));
        }
      } else {
        for (char *c : id_list->getIdList()) {
          s.push_back(FunctionParameter(c, t, true, ref, v));
        }
      }
    }
    return s;
  }


  virtual void findFparTypes(std::string funcName) override{
    Datatype type = fpar_type->getType();
    int IdListSize = id_list->getIdList().size();
    for(int i = 0; i < IdListSize; i++){
      if(type == TYPE_int&& !ref){
        funcParamTypes[funcName].push_back(i32);
        funcParamRefs[funcName].push_back(0);
      }
      else if(type == TYPE_int&& ref){
        funcParamTypes[funcName].push_back(PointerType::get(i32, 0));
        funcParamRefs[funcName].push_back(1);
      }
      else if(type == TYPE_char && !ref){
        funcParamTypes[funcName].push_back(i8);
        funcParamRefs[funcName].push_back(0);
      }
      else if(type == TYPE_char && ref){
        funcParamTypes[funcName].push_back(PointerType::get(i8, 0));
        funcParamRefs[funcName].push_back(1);
      }
    }
  }

  std::vector<std::vector<int>> getConstListFparDef() {
    std::vector<std::vector<int>> v;
    int IdListSize = id_list->getIdList().size();
    for(int i = 0; i < IdListSize; i++){
      v.push_back(fpar_type->getConstList());
    }
    return v;
  }


  std::vector<char *> getIdListFromFparDef(){
    return id_list->getIdList();
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


  virtual void findFparTypes(std::string funcName) override{
    for(Func *ldef_elem : fdef_list)
      ldef_elem->findFparTypes(funcName);
  }

  std::vector<char *> getIdListFromFparDefList(){
    std::vector<char *> v, temp;
    for (FparDef *fdef: fdef_list){
      temp = fdef->getIdListFromFparDef();
      v.insert(v.end(), temp.begin(), temp.end());
    }
      
    return v;
  }
  
  std::vector<std::vector<int>> getConstListFparDefList(){
    std::vector<std::vector<int>> v, temp;
    for (FparDef *fdef: fdef_list){
      temp = fdef->getConstListFparDef();
      v.insert(v.end(), temp.begin(), temp.end());
    }
    return v;
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

  virtual Value *compile() override{
    std::string nameOfFunc = id;

    if(!firstFuncDefined){
      if(strcmp(id, "main")==0){
        nameOfFunc = "_main";
      }
      ReturnTypesOfFunc[id] = rettype;
      FunctionType *funcType = FunctionType::get(Type::getVoidTy(TheContext), false);
      FuncPtr[id] = Function::Create(funcType, Function::ExternalLinkage, nameOfFunc, TheModule.get());
      firstFunc = FuncPtr[id];
    }
    
    builder.SetInsertPoint(BasicBlock::Create(TheContext, "entry", FuncPtr[id]));

    if(ReturnTypesOfFunc[id] == TYPE_int)
    {
      isCharVar["_returnValue"] = 0;
      AllocaInst *Alloca  = builder.CreateAlloca(Type::getInt32Ty(TheContext), nullptr, "_returnValue");
      NamedValues["_returnValue"] = Alloca;
      NamedValuesTypes["_returnValue"] = i32;
    } 

    if(ReturnTypesOfFunc[id] == TYPE_char)
    {
      isCharVar["_returnValue"] = 1;
      AllocaInst *Alloca  = builder.CreateAlloca(Type::getInt8Ty(TheContext), nullptr, "_returnValue");
      NamedValues["_returnValue"] = Alloca;
      NamedValuesTypes["_returnValue"] = i8;
    } 

    if(firstFuncDefined){
      if(funcParamRefs.find(id)!= funcParamRefs.end()) {
        std::vector<char *> fparamsVector = fpar->getIdListFromFparDefList();
        std::vector<std::vector<int>> fparamsVectorDimensions = fpar->getConstListFparDefList();
        int i =0;
        for(char *c: fparamsVector) {

          if(funcParamTypes[id][i] == i32){
            isCharVar[c] = 0;
            AllocaInst *Alloca  = builder.CreateAlloca(Type::getInt32Ty(TheContext), nullptr, c);
            NamedValues[c] = Alloca;
            NamedValuesTypes[c] = i32;
            builder.CreateStore( &(*std::next(FuncPtr[id]->arg_begin(), i)), NamedValues[c]);
          }
          else if(funcParamTypes[id][i] == i8){
            isCharVar[c] = 1;
            AllocaInst *Alloca  = builder.CreateAlloca(Type::getInt8Ty(TheContext), nullptr, c);
            NamedValues[c] = Alloca;
            NamedValuesTypes[c] = i8;
            builder.CreateStore( &(*std::next(FuncPtr[id]->arg_begin(), i)), NamedValues[c]);
          }
          else if(funcParamTypes[id][i] == PointerType::get(i32, 0)){
            isCharVar[c] = 0;
            NamedValues[c] = std::next(FuncPtr[id]->arg_begin(), i);
            NamedValuesTypes[c] = i32;
            ArrDimensions[c] = fparamsVectorDimensions[i];
            
          }
          else if(funcParamTypes[id][i] == PointerType::get(i8, 0)){
            isCharVar[c] = 1;
            NamedValues[c] = std::next(FuncPtr[id]->arg_begin(), i);
            NamedValuesTypes[c] = i8;
            ArrDimensions[c] = fparamsVectorDimensions[i];
          }

          i++;
        }

      }
    }     
    firstFuncDefined = true;
    return nullptr;
  }

  virtual void findFparTypes(std::string funcName) override{
    fpar->findFparTypes(funcName);
  }

  Datatype getDataype(){
    return rettype;
  }

  std::string getFuncName(){
    return id;
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

  std::vector<Func *> compile_LocalDef(){
    std::vector<Func *> functions;
    for(Func *ldef_elem : ldef_list){
      if(ldef_elem->getTypeOfFunc() == "VarDef")
        ldef_elem->compile();
     else if(ldef_elem->getTypeOfFunc() == "FuncDef")
      {
        std::string id = ldef_elem->getFuncName();
        std::string nameOfFunc = id;
        
        
        //FIND PARAMS 
        ldef_elem->findFparTypes(id);

        //DEFINE FUNCTION
        if(id == "main"){
          nameOfFunc = "_main";
        }
        ReturnTypesOfFunc[id] = ldef_elem->getDataype();
        FunctionType *funcType;
        if(ReturnTypesOfFunc[id] == TYPE_int) 
          funcType = FunctionType::get(Type::getInt32Ty(TheContext), funcParamTypes[id], false);
        else if(ReturnTypesOfFunc[id] == TYPE_char) 
          funcType = FunctionType::get(Type::getInt8Ty(TheContext), funcParamTypes[id], false);
        else
          funcType = FunctionType::get(Type::getVoidTy(TheContext), funcParamTypes[id], false);
        FuncPtr[id] = Function::Create(funcType, Function::ExternalLinkage, nameOfFunc, TheModule.get());
        functions.push_back(ldef_elem);
      }
    }
 
    return functions;
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
    head->compile();
    std::vector<Func *> nestedFunctions = local->compile_LocalDef();
    block->compile();

    BasicBlock *ReturnBlock = BasicBlock::Create(TheContext, "return", FuncPtr[head->getFuncName()]);
    builder.CreateBr(ReturnBlock);
    builder.SetInsertPoint(ReturnBlock);
    if(head->getDataype() == TYPE_nothing)
      builder.CreateRetVoid();
    if(head->getDataype() == TYPE_int)
      builder.CreateRet(builder.CreateLoad(NamedValuesTypes["_returnValue"], NamedValues["_returnValue"], "_returnValue"));
    if(head->getDataype() == TYPE_char)
      builder.CreateRet(builder.CreateLoad(NamedValuesTypes["_returnValue"], NamedValues["_returnValue"], "_returnValue"));

    TheFPM->run(*FuncPtr[head->getFuncName()]);
    
    for(Func *function : nestedFunctions)
      function->compile();
    return nullptr;
  }
  
  virtual void findFparTypes(std::string funcName) override{
    head->findFparTypes(funcName);
  }

  virtual std::string getTypeOfFunc() override{
    return "FuncDef";
  }

  virtual Datatype getDataype() override{
    return head->getDataype();

  }

  virtual std::string getFuncName() override{
    return head->getFuncName();
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
    for (int i : v) {
        if (i == -1) {
          showSemanticError(33, line, id_list->getIdList()[0]);
        }
    }
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
        if(type->getTypeBts() == TYPE_char){
          isCharVar[c] = 1;
          AllocaInst *Alloca  = builder.CreateAlloca(Type::getInt8Ty(TheContext), nullptr, c);
          NamedValues[c] = Alloca;
          NamedValuesTypes[c] = i8;
        }
        else{
          isCharVar[c] = 0;
          AllocaInst *Alloca  = builder.CreateAlloca(Type::getInt32Ty(TheContext), nullptr, c);
          NamedValues[c] = Alloca;
          NamedValuesTypes[c] = i32;
        }
      }
    }
    else{
      for (char *c : id_list->getIdList()) {
        if(type->getTypeBts() == TYPE_char){
          isCharVar[c] = 1;
          int dimensions = type->getConstList().size();
          std::vector<int> sizes = type->getConstList();

          // Calculate the total size of the N-dimensional array
          Value *totalSize = ConstantInt::get(Type::getInt32Ty(TheContext),  type->getConstList()[0]);
          for (int i = 1; i < dimensions; ++i) {
              totalSize = builder.CreateMul(totalSize, ConstantInt::get(Type::getInt32Ty(TheContext), sizes[i]));
          }

          // Allocate memory for the N-dimensional array
          AllocaInst *arrayPtr = builder.CreateAlloca(Type::getInt8Ty(TheContext), totalSize, c);
          NamedValues[c] = arrayPtr;
          NamedValuesTypes[c] = i8;
          ArrDimensions[c] = sizes;
        }
        else{
          isCharVar[c] = 0;
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
          NamedValuesTypes[c] = i32;
          ArrDimensions[c] = sizes;

        }
      }
    }
    return nullptr;
  }

  virtual std::string getTypeOfFunc() override{
    return "VarDef";
  }

private:
  IdList *id_list;
  TypeBts *type;
  int line;
};


#endif