#ifndef FUNCAO_REGISTRO_HPP
#define FUNCAO_REGISTRO_HPP

#include <raylib.h>
#include <string>

using namespace std;

struct FuncaoRegistro {
    int id;
    string expressao;
    string descricao;
    Color cor;
    bool visivel;
    bool eValida;
    string ultimoErro;
    
    FuncaoRegistro(int id, const std::string& expr, Color cor = BLUE);
    
    bool IsValida() const { return eValida; }
    void SetValida(bool v) { eValida = v; }
    void SetErro(const std::string& erro) { 
        ultimoErro = erro;
        eValida = false;
    }
};

#endif
