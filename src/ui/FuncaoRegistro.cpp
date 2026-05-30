#include "FuncaoRegistro.hpp"

using namespace std;

FuncaoRegistro::FuncaoRegistro(int id, const string& expr, Color cor)
    : id(id), expressao(expr), cor(cor), visivel(true), eValida(false)
{
    descricao = expr;
    ultimoErro = "";
}