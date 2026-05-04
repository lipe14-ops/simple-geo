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
            ExprPtr<T> right = parseMulDiv();
            auto op = ExpressaoFactory<T>::criarAdicao();
            op->addChild(left);
            op->addChild(right);
            left = op;
        } else if (expr[pos] == '-') {
            pos++;
            ExprPtr<T> right = parseMulDiv();
            auto op = ExpressaoFactory<T>::criarSubtracao();
            op->addChild(left);
            op->addChild(right);
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
            ExprPtr<T> right = parseExp();
            auto op = ExpressaoFactory<T>::criarMultiplicacao();
            op->addChild(left);
            op->addChild(right);
            left = op;
        } else if (expr[pos] == '/') {
            pos++;
            ExprPtr<T> right = parseExp();
            auto op = ExpressaoFactory<T>::criarDivisao();
            op->addChild(left);
            op->addChild(right);
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

    if (pos >= expr.length()) {
        throw runtime_error("Expressão incompleta");
    }

    if (expr[pos] == '(') {
        pos++;
        ExprPtr<T> result = parseAddSub();
        skipWhitespace();
        if (pos >= expr.length() || expr[pos] != ')') {
            throw runtime_error("Parêntese fechado esperado");
        }
        pos++;
        return result;
    }

    if (expr[pos] == '-') {
        pos++;
        ExprPtr<T> operand = parsePrimary();
        auto negOne = ExpressaoFactory<T>::criarConstante(-1);
        auto op = ExpressaoFactory<T>::criarMultiplicacao();
        op->addChild(negOne);
        op->addChild(operand);
        return op;
    }

    if (expr[pos] == 'x') {
        pos++;
        return ExpressaoFactory<T>::criarVariavel();
    }

    if (isdigit(expr[pos]) || expr[pos] == '.') {
        size_t start = pos;
        while (pos < expr.length() && (isdigit(expr[pos]) || expr[pos] == '.')) {
            pos++;
        }
        return ExpressaoFactory<T>::criarConstante(stod(expr.substr(start, pos - start)));
    }

    throw runtime_error(string("Token inesperado: ") + expr[pos]);
}

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

template<typename T>
ExprPtr<T> parse(const string& expressao) {
    ExpressionParser<T> parser(expressao);
    return parser.parse();
}

template ExprPtr<float> parse<float>(const string&);
template ExprPtr<double> parse<double>(const string&);
template ExprPtr<int> parse<int>(const string&);


}  
