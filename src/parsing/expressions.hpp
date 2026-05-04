#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace std;

namespace expr {

enum class OpType {
    ADD, SUB, MUL, DIV, EXP, CONST, VAR
};

template<typename T> class Expressao;
template<typename T> using ExprPtr = shared_ptr<Expressao<T>>;

template<typename T>
class Expressao {
protected:
    OpType type;
    T value;
    vector<ExprPtr<T>> children;

    Expressao(OpType t, T v = 0) : type(t), value(v) {}

public:
    virtual ~Expressao() = default;

    OpType getType() const { return type; }
    T getValue() const { return value; }
    void setValue(T v) { value = v; }
    
    void addChild(ExprPtr<T> child) {
        children.push_back(child);
    }

    const vector<ExprPtr<T>>& getChildren() const {
        return children;
    }

    virtual T eval(T x) = 0;
    virtual void print() const = 0;
};

template<typename T>
class Constante : public Expressao<T> {
public:
    explicit Constante(T val) : Expressao<T>(OpType::CONST, val) {}
    T eval(T x) override { return this->value; }
    void print() const override { std::cout << this->value; }
};

template<typename T>
class Variavel : public Expressao<T> {
public:
    Variavel() : Expressao<T>(OpType::VAR) {}
    T eval(T x) override { return x; }
    void print() const override { std::cout << "x"; }
};

template<typename T>
class Operacao : public Expressao<T> {
protected:
    virtual T executar(T left, T right) = 0;
    virtual string getOperador() const = 0;

public:
    Operacao(OpType t) : Expressao<T>(t) {}
    
    T eval(T x) override {
        if (this->children.size() != 2) {
            throw runtime_error("Operação binária requer 2 filhos");
        }
        T left = this->children[0]->eval(x);
        T right = this->children[1]->eval(x);
        return executar(left, right);
    }

    void print() const override {
        std::cout << "(";
        this->children[0]->print();
        std::cout << " " << getOperador() << " ";
        this->children[1]->print();
        std::cout << ")";
    }
};

template<typename T>
class Adicao : public Operacao<T> {
protected:
    T executar(T left, T right) override { return left + right; }
    string getOperador() const override { return "+"; }
public:
    Adicao() : Operacao<T>(OpType::ADD) {}
};

template<typename T>
class Subtracao : public Operacao<T> {
protected:
    T executar(T left, T right) override { return left - right; }
    string getOperador() const override { return "-"; }
public:
    Subtracao() : Operacao<T>(OpType::SUB) {}
};

template<typename T>
class Multiplicacao : public Operacao<T> {
protected:
    T executar(T left, T right) override { return left * right; }
    string getOperador() const override { return "*"; }
public:
    Multiplicacao() : Operacao<T>(OpType::MUL) {}
};

template<typename T>
class Divisao : public Operacao<T> {
protected:
    T executar(T left, T right) override {
        if (right == 0) throw runtime_error("Divisão por zero");
        return left / right;
    }
    string getOperador() const override { return "/"; }
public:
    Divisao() : Operacao<T>(OpType::DIV) {}
};

template<typename T>
class Exponenciacao : public Operacao<T> {
protected:
    T executar(T left, T right) override { return pow(left, right); }
    string getOperador() const override { return "^"; }
public:
    Exponenciacao() : Operacao<T>(OpType::EXP) {}
};

// Factory para criar expressões
template<typename T>
class ExpressaoFactory {
public:
    static ExprPtr<T> criarConstante(T valor) {
        return make_shared<Constante<T>>(valor);
    }

    static ExprPtr<T> criarVariavel() {
        return make_shared<Variavel<T>>();
    }

    static ExprPtr<T> criarAdicao() {
        return make_shared<Adicao<T>>();
    }

    static ExprPtr<T> criarSubtracao() {
        return make_shared<Subtracao<T>>();
    }

    static ExprPtr<T> criarMultiplicacao() {
        return make_shared<Multiplicacao<T>>();
    }

    static ExprPtr<T> criarDivisao() {
        return make_shared<Divisao<T>>();
    }

    static ExprPtr<T> criarExponenciacao() {
        return make_shared<Exponenciacao<T>>();
    }

    static ExprPtr<T> criarOperacao(OpType op) {
        switch (op) {
            case OpType::ADD: return criarAdicao();
            case OpType::SUB: return criarSubtracao();
            case OpType::MUL: return criarMultiplicacao();
            case OpType::DIV: return criarDivisao();
            case OpType::EXP: return criarExponenciacao();
            case OpType::CONST: return criarConstante(0);
            case OpType::VAR: return criarVariavel();
            default: throw runtime_error("Tipo de operação desconhecido");
        }
    }
};

template<typename T>
class ExpressionParser {
private:
    string expr;
    size_t pos;

    void skipWhitespace() {
        while (pos < expr.length() && isspace(expr[pos])) {
            pos++;
        }
    }

    ExprPtr<T> parseExpression();
    ExprPtr<T> parseAddSub();
    ExprPtr<T> parseMulDiv();
    ExprPtr<T> parseExp();
    ExprPtr<T> parsePrimary();

public:
    explicit ExpressionParser(const string& e) : expr(e), pos(0) {}

    ExprPtr<T> parse() {
        ExprPtr<T> result = parseExpression();
        skipWhitespace();
        if (pos < expr.length()) {
            throw runtime_error("Caractere inesperado na expressão");
        }
        return result;
    }
};

template<typename T>
ExprPtr<T> parse(const string& expressao);

}  

#endif 