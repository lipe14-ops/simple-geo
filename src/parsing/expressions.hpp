#ifndef EXPRESSIONS_HPP
#define EXPRESSIONS_HPP

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace std;

namespace expr {

// Alias conveniente para o mapa de variáveis
template<typename T>
using VarMap = map<string, T>;

enum class OpType {
    ADD, SUB, MUL, DIV, EXP, CONST, VAR,
    SIN, COS, TAN, COT, SEC, CSC,
    LN, LOG10, LOG2, LOGB, SQRT
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

    virtual T eval(const VarMap<T>& vars) = 0;
    virtual void print() const = 0;
};

template<typename T>
class Constante : public Expressao<T> {
public:
    explicit Constante(T val) : Expressao<T>(OpType::CONST, val) {}
    T eval(const VarMap<T>&) override { return this->value; }
    void print() const override { std::cout << this->value; }
};

template<typename T>
class Variavel : public Expressao<T> {
    string nome;
public:
    explicit Variavel(const string& n) : Expressao<T>(OpType::VAR), nome(n) {}

    const string& getNome() const { return nome; }

    T eval(const VarMap<T>& vars) override {
        auto it = vars.find(nome);
        if (it == vars.end())
            throw runtime_error("Variável não definida: " + nome);
        return it->second;
    }

    void print() const override { std::cout << nome; }
};

// ── Operações binárias ────────────────────────────────────────────────────────

template<typename T>
class Operacao : public Expressao<T> {
protected:
    virtual T executar(T left, T right) = 0;
    virtual string getOperador() const = 0;

public:
    Operacao(OpType t) : Expressao<T>(t) {}

    T eval(const VarMap<T>& vars) override {
        if (this->children.size() != 2)
            throw runtime_error("Operação binária requer 2 filhos");
        T left  = this->children[0]->eval(vars);
        T right = this->children[1]->eval(vars);
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
    T executar(T l, T r) override { return l + r; }
    string getOperador() const override { return "+"; }
public:
    Adicao() : Operacao<T>(OpType::ADD) {}
};

template<typename T>
class Subtracao : public Operacao<T> {
protected:
    T executar(T l, T r) override { return l - r; }
    string getOperador() const override { return "-"; }
public:
    Subtracao() : Operacao<T>(OpType::SUB) {}
};

template<typename T>
class Multiplicacao : public Operacao<T> {
protected:
    T executar(T l, T r) override { return l * r; }
    string getOperador() const override { return "*"; }
public:
    Multiplicacao() : Operacao<T>(OpType::MUL) {}
};

template<typename T>
class Divisao : public Operacao<T> {
protected:
    T executar(T l, T r) override {
        if (r == 0) throw runtime_error("Divisão por zero");
        return l / r;
    }
    string getOperador() const override { return "/"; }
public:
    Divisao() : Operacao<T>(OpType::DIV) {}
};

template<typename T>
class Exponenciacao : public Operacao<T> {
protected:
    T executar(T l, T r) override { return pow(l, r); }
    string getOperador() const override { return "^"; }
public:
    Exponenciacao() : Operacao<T>(OpType::EXP) {}
};

// ── Operações unárias ─────────────────────────────────────────────────────────

template<typename T>
class OperacaoUnaria : public Expressao<T> {
protected:
    virtual T executar(T val) = 0;
    virtual string getNome() const = 0;

public:
    OperacaoUnaria(OpType t) : Expressao<T>(t) {}

    T eval(const VarMap<T>& vars) override {
        if (this->children.size() != 1)
            throw runtime_error("Operação unária requer 1 filho");
        return executar(this->children[0]->eval(vars));
    }

    void print() const override {
        std::cout << getNome() << "(";
        this->children[0]->print();
        std::cout << ")";
    }
};

// Trigonométricas
template<typename T>
class Seno : public OperacaoUnaria<T> {
protected:
    T executar(T v) override { return sin(v); }
    string getNome() const override { return "sin"; }
public:
    Seno() : OperacaoUnaria<T>(OpType::SIN) {}
};

template<typename T>
class Cosseno : public OperacaoUnaria<T> {
protected:
    T executar(T v) override { return cos(v); }
    string getNome() const override { return "cos"; }
public:
    Cosseno() : OperacaoUnaria<T>(OpType::COS) {}
};

template<typename T>
class Tangente : public OperacaoUnaria<T> {
protected:
    T executar(T v) override { return tan(v); }
    string getNome() const override { return "tan"; }
public:
    Tangente() : OperacaoUnaria<T>(OpType::TAN) {}
};

template<typename T>
class Cotangente : public OperacaoUnaria<T> {
protected:
    T executar(T v) override {
        T t = tan(v);
        if (t == 0) throw runtime_error("Cotangente indefinida (tan = 0)");
        return T(1) / t;
    }
    string getNome() const override { return "cot"; }
public:
    Cotangente() : OperacaoUnaria<T>(OpType::COT) {}
};

template<typename T>
class Secante : public OperacaoUnaria<T> {
protected:
    T executar(T v) override {
        T c = cos(v);
        if (c == 0) throw runtime_error("Secante indefinida (cos = 0)");
        return T(1) / c;
    }
    string getNome() const override { return "sec"; }
public:
    Secante() : OperacaoUnaria<T>(OpType::SEC) {}
};

template<typename T>
class Cossecante : public OperacaoUnaria<T> {
protected:
    T executar(T v) override {
        T s = sin(v);
        if (s == 0) throw runtime_error("Cossecante indefinida (sin = 0)");
        return T(1) / s;
    }
    string getNome() const override { return "csc"; }
public:
    Cossecante() : OperacaoUnaria<T>(OpType::CSC) {}
};

// Logarítmicas
template<typename T>
class LogNatural : public OperacaoUnaria<T> {
protected:
    T executar(T v) override {
        if (v <= 0) throw runtime_error("ln indefinido para valor <= 0");
        return log(v);
    }
    string getNome() const override { return "ln"; }
public:
    LogNatural() : OperacaoUnaria<T>(OpType::LN) {}
};

template<typename T>
class Log10 : public OperacaoUnaria<T> {
protected:
    T executar(T v) override {
        if (v <= 0) throw runtime_error("log10 indefinido para valor <= 0");
        return log10(v);
    }
    string getNome() const override { return "log"; }
public:
    Log10() : OperacaoUnaria<T>(OpType::LOG10) {}
};

template<typename T>
class Log2 : public OperacaoUnaria<T> {
protected:
    T executar(T v) override {
        if (v <= 0) throw runtime_error("log2 indefinido para valor <= 0");
        return log2(v);
    }
    string getNome() const override { return "log2"; }
public:
    Log2() : OperacaoUnaria<T>(OpType::LOG2) {}
};

// log(a, b) — base arbitrária (binário)
template<typename T>
class LogBase : public Expressao<T> {
public:
    LogBase() : Expressao<T>(OpType::LOGB) {}

    T eval(const VarMap<T>& vars) override {
        if (this->children.size() != 2)
            throw runtime_error("log(a, b) requer 2 argumentos");
        T a = this->children[0]->eval(vars);
        T b = this->children[1]->eval(vars);
        if (a <= 0)      throw runtime_error("log(a, b): argumento a deve ser > 0");
        if (b <= 0 || b == 1) throw runtime_error("log(a, b): base b deve ser > 0 e != 1");
        return log(a) / log(b);
    }

    void print() const override {
        std::cout << "log(";
        this->children[0]->print();
        std::cout << ", ";
        this->children[1]->print();
        std::cout << ")";
    }
};

// Raiz quadrada
template<typename T>
class RaizQuadrada : public OperacaoUnaria<T> {
protected:
    T executar(T v) override {
        if (v < 0) throw runtime_error("sqrt indefinido para valor < 0");
        return sqrt(v);
    }
    string getNome() const override { return "sqrt"; }
public:
    RaizQuadrada() : OperacaoUnaria<T>(OpType::SQRT) {}
};

// ── Factory ───────────────────────────────────────────────────────────────────

template<typename T>
class ExpressaoFactory {
public:
    static ExprPtr<T> criarConstante(T valor) {
        return make_shared<Constante<T>>(valor);
    }

    static ExprPtr<T> criarVariavel(const string& nome) {
        return make_shared<Variavel<T>>(nome);
    }

    static ExprPtr<T> criarAdicao()          { return make_shared<Adicao<T>>(); }
    static ExprPtr<T> criarSubtracao()       { return make_shared<Subtracao<T>>(); }
    static ExprPtr<T> criarMultiplicacao()   { return make_shared<Multiplicacao<T>>(); }
    static ExprPtr<T> criarDivisao()         { return make_shared<Divisao<T>>(); }
    static ExprPtr<T> criarExponenciacao()   { return make_shared<Exponenciacao<T>>(); }
    static ExprPtr<T> criarSeno()            { return make_shared<Seno<T>>(); }
    static ExprPtr<T> criarCosseno()         { return make_shared<Cosseno<T>>(); }
    static ExprPtr<T> criarTangente()        { return make_shared<Tangente<T>>(); }
    static ExprPtr<T> criarCotangente()      { return make_shared<Cotangente<T>>(); }
    static ExprPtr<T> criarSecante()         { return make_shared<Secante<T>>(); }
    static ExprPtr<T> criarCossecante()      { return make_shared<Cossecante<T>>(); }
    static ExprPtr<T> criarLogNatural()      { return make_shared<LogNatural<T>>(); }
    static ExprPtr<T> criarLog10()           { return make_shared<Log10<T>>(); }
    static ExprPtr<T> criarLog2()            { return make_shared<Log2<T>>(); }
    static ExprPtr<T> criarLogBase()         { return make_shared<LogBase<T>>(); }
    static ExprPtr<T> criarSqrt()            { return make_shared<RaizQuadrada<T>>(); }

    static ExprPtr<T> criarOperacao(OpType op) {
        switch (op) {
            case OpType::ADD:   return criarAdicao();
            case OpType::SUB:   return criarSubtracao();
            case OpType::MUL:   return criarMultiplicacao();
            case OpType::DIV:   return criarDivisao();
            case OpType::EXP:   return criarExponenciacao();
            case OpType::CONST: return criarConstante(0);
            case OpType::VAR:   return criarVariavel("x");
            case OpType::SIN:   return criarSeno();
            case OpType::COS:   return criarCosseno();
            case OpType::TAN:   return criarTangente();
            case OpType::COT:   return criarCotangente();
            case OpType::SEC:   return criarSecante();
            case OpType::CSC:   return criarCossecante();
            case OpType::LN:    return criarLogNatural();
            case OpType::LOG10: return criarLog10();
            case OpType::LOG2:  return criarLog2();
            case OpType::LOGB:  return criarLogBase();
            case OpType::SQRT:  return criarSqrt();
            default: throw runtime_error("Tipo de operação desconhecido");
        }
    }
};

// ── Parser ────────────────────────────────────────────────────────────────────

template<typename T>
class ExpressionParser {
private:
    string expr;
    size_t pos;

    void skipWhitespace() {
        while (pos < expr.length() && isspace(expr[pos])) pos++;
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
        if (pos < expr.length())
            throw runtime_error("Caractere inesperado na expressão");
        return result;
    }
};

template<typename T>
ExprPtr<T> parse(const string& expressao);

}  // namespace expr

#endif