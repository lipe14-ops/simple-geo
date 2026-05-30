#include "expressions.hpp"
#include <cmath>
#include <cctype>
#include <iostream>

namespace expr {

template<typename T>
ExprPtr<T> ExpressionParser<T>::parseExpression() {
    return parseAddSub();
}

template<typename T>
ExprPtr<T> ExpressionParser<T>::parseAddSub() {
    ExprPtr<T> left = parseMulDiv();

    while (true) {
        skipWhitespace();
        if (pos >= expr.length()) break;

        if (expr[pos] == '+') {
            pos++;
            auto op = ExpressaoFactory<T>::criarAdicao();
            op->addChild(left);
            op->addChild(parseMulDiv());
            left = op;
        } else if (expr[pos] == '-') {
            pos++;
            auto op = ExpressaoFactory<T>::criarSubtracao();
            op->addChild(left);
            op->addChild(parseMulDiv());
            left = op;
        } else {
            break;
        }
    }
    return left;
}

template<typename T>
ExprPtr<T> ExpressionParser<T>::parseMulDiv() {
    ExprPtr<T> left = parseExp();

    while (true) {
        skipWhitespace();
        if (pos >= expr.length()) break;

        if (expr[pos] == '*') {
            pos++;
            auto op = ExpressaoFactory<T>::criarMultiplicacao();
            op->addChild(left);
            op->addChild(parseExp());
            left = op;
        } else if (expr[pos] == '/') {
            pos++;
            auto op = ExpressaoFactory<T>::criarDivisao();
            op->addChild(left);
            op->addChild(parseExp());
            left = op;
        } else {
            break;
        }
    }
    return left;
}

template<typename T>
ExprPtr<T> ExpressionParser<T>::parseExp() {
    ExprPtr<T> left = parsePrimary();

    while (true) {
        skipWhitespace();
        if (pos >= expr.length() || expr[pos] != '^') break;
        pos++;
        ExprPtr<T> right = parsePrimary();
        auto op = ExpressaoFactory<T>::criarExponenciacao();
        op->addChild(left);
        op->addChild(right);
        left = op;
    }
    return left;
}

template<typename T>
ExprPtr<T> ExpressionParser<T>::parsePrimary() {
    skipWhitespace();

    if (pos >= expr.length())
        throw runtime_error("Expressão incompleta");

    // Subexpressão entre parênteses
    if (expr[pos] == '(') {
        pos++;
        ExprPtr<T> result = parseAddSub();
        skipWhitespace();
        if (pos >= expr.length() || expr[pos] != ')')
            throw runtime_error("Parêntese fechado esperado");
        pos++;
        return result;
    }

    // Negativo unário
    if (expr[pos] == '-') {
        pos++;
        auto negOne = ExpressaoFactory<T>::criarConstante(T(-1));
        auto op     = ExpressaoFactory<T>::criarMultiplicacao();
        op->addChild(negOne);
        op->addChild(parsePrimary());
        return op;
    }

    // Número literal
    if (isdigit(expr[pos]) || expr[pos] == '.') {
        size_t start = pos;
        while (pos < expr.length() && (isdigit(expr[pos]) || expr[pos] == '.'))
            pos++;
        return ExpressaoFactory<T>::criarConstante(
            static_cast<T>(stod(expr.substr(start, pos - start))));
    }

    // Identificador: função conhecida, constante (pi, e) ou variável
    if (isalpha(expr[pos]) || expr[pos] == '_') {
        size_t start = pos;
        while (pos < expr.length() && (isalnum(expr[pos]) || expr[pos] == '_'))
            pos++;
        string ident = expr.substr(start, pos - start);

        // ── Funções (exigem '(' a seguir) ────────────────────────────────────
        // Helper: consome '( arg )' e retorna a expressão unária
        auto unaryFunc = [&](ExprPtr<T> node) -> ExprPtr<T> {
            skipWhitespace();
            if (pos >= expr.length() || expr[pos] != '(')
                throw runtime_error("Esperado '(' após " + ident);
            pos++;
            node->addChild(parseAddSub());
            skipWhitespace();
            if (pos >= expr.length() || expr[pos] != ')')
                throw runtime_error("Esperado ')' após argumento de " + ident);
            pos++;
            return node;
        };

        if (ident == "sin")  return unaryFunc(ExpressaoFactory<T>::criarSeno());
        if (ident == "cos")  return unaryFunc(ExpressaoFactory<T>::criarCosseno());
        if (ident == "tan")  return unaryFunc(ExpressaoFactory<T>::criarTangente());
        if (ident == "cot")  return unaryFunc(ExpressaoFactory<T>::criarCotangente());
        if (ident == "sec")  return unaryFunc(ExpressaoFactory<T>::criarSecante());
        if (ident == "csc")  return unaryFunc(ExpressaoFactory<T>::criarCossecante());
        if (ident == "ln")   return unaryFunc(ExpressaoFactory<T>::criarLogNatural());
        if (ident == "log2") return unaryFunc(ExpressaoFactory<T>::criarLog2());
        if (ident == "sqrt") return unaryFunc(ExpressaoFactory<T>::criarSqrt());

        if (ident == "log") {
            // log(x) → base 10   |   log(a, b) → base arbitrária
            skipWhitespace();
            if (pos >= expr.length() || expr[pos] != '(')
                throw runtime_error("Esperado '(' após log");
            pos++;
            ExprPtr<T> first = parseAddSub();
            skipWhitespace();
            if (pos < expr.length() && expr[pos] == ',') {
                pos++;
                ExprPtr<T> base = parseAddSub();
                skipWhitespace();
                if (pos >= expr.length() || expr[pos] != ')')
                    throw runtime_error("Esperado ')' após argumentos de log(a, b)");
                pos++;
                auto op = ExpressaoFactory<T>::criarLogBase();
                op->addChild(first);
                op->addChild(base);
                return op;
            } else {
                if (pos >= expr.length() || expr[pos] != ')')
                    throw runtime_error("Esperado ')' após argumento de log");
                pos++;
                auto op = ExpressaoFactory<T>::criarLog10();
                op->addChild(first);
                return op;
            }
        }

        // ── Constantes matemáticas ────────────────────────────────────────────
        if (ident == "pi")
            return ExpressaoFactory<T>::criarConstante(static_cast<T>(M_PI));
        if (ident == "e")
            return ExpressaoFactory<T>::criarConstante(static_cast<T>(M_E));

        // ── Variável de nome arbitrário ───────────────────────────────────────
        return ExpressaoFactory<T>::criarVariavel(ident);
    }

    throw runtime_error(string("Token inesperado: ") + expr[pos]);
}

// ── Explicit instantiations ───────────────────────────────────────────────────

template class ExpressionParser<float>;
template class ExpressionParser<double>;
template class ExpressionParser<int>;

template class ExpressaoFactory<float>;
template class ExpressaoFactory<double>;
template class ExpressaoFactory<int>;

template class Constante<float>;
template class Constante<double>;
template class Constante<int>;

template class Variavel<float>;
template class Variavel<double>;
template class Variavel<int>;

template class Adicao<float>;
template class Adicao<double>;
template class Adicao<int>;

template class Subtracao<float>;
template class Subtracao<double>;
template class Subtracao<int>;

template class Multiplicacao<float>;
template class Multiplicacao<double>;
template class Multiplicacao<int>;

template class Divisao<float>;
template class Divisao<double>;
template class Divisao<int>;

template class Exponenciacao<float>;
template class Exponenciacao<double>;
template class Exponenciacao<int>;

template class Seno<float>;
template class Seno<double>;
template class Seno<int>;

template class Cosseno<float>;
template class Cosseno<double>;
template class Cosseno<int>;

template class Tangente<float>;
template class Tangente<double>;
template class Tangente<int>;

template class Cotangente<float>;
template class Cotangente<double>;
template class Cotangente<int>;

template class Secante<float>;
template class Secante<double>;
template class Secante<int>;

template class Cossecante<float>;
template class Cossecante<double>;
template class Cossecante<int>;

template class LogNatural<float>;
template class LogNatural<double>;
template class LogNatural<int>;

template class Log10<float>;
template class Log10<double>;
template class Log10<int>;

template class Log2<float>;
template class Log2<double>;
template class Log2<int>;

template class LogBase<float>;
template class LogBase<double>;
template class LogBase<int>;

template class RaizQuadrada<float>;
template class RaizQuadrada<double>;
template class RaizQuadrada<int>;

template<typename T>
ExprPtr<T> parse(const string& expressao) {
    ExpressionParser<T> parser(expressao);
    return parser.parse();
}

template ExprPtr<float>  parse<float> (const string&);
template ExprPtr<double> parse<double>(const string&);
template ExprPtr<int>    parse<int>   (const string&);

}  // namespace expr