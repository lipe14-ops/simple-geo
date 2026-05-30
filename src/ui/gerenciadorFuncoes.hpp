#ifndef GERENCIADOR_FUNCOES_HPP
#define GERENCIADOR_FUNCOES_HPP

#include "FuncaoRegistro.hpp"
#include <vector>
#include <memory> //smart pointers

using namespace std;

class GerenciadorFuncoes {
private:
    vector<FuncaoRegistro> funcoes; // mantém histórico mesmo se removidas
    
    int proximoID = 1;
    int idSelecionada = -1;
    int proximaCorIndex = 0;

    Color coresDisponiveis[8] = {
        BLUE,       // 0
        RED,        // 1
        GREEN,      // 2
        ORANGE,     // 3
        PURPLE,     // 4
        BROWN,      // 5
        MAGENTA,    // 6
        DARKGREEN   // 7
    };
    
    
public:
    int AdicionarFuncao(const string& expr, Color cor = {0, 0, 0, 0});
    bool RemoverFuncao(int id);
    bool ToggleVisibilidade(int id);
    
    const vector<FuncaoRegistro>& GetFuncoes() const {
        return funcoes;
    }
    vector<FuncaoRegistro> GetFuncoesVisiveis() const;
    const FuncaoRegistro* GetFuncao(int id) const;
    FuncaoRegistro* GetFuncaoMutavel(int id);
    
    void SetSelecionada(int id) { idSelecionada = id; }
    int GetSelecionada() const { return idSelecionada; }
    void Limpar() {
        funcoes.clear();
        idSelecionada = -1;
    }
    int GetQuantidade() const { return funcoes.size(); }
    
    Color ProximaCor();
    
private:
    FuncaoRegistro* FindById(int id);
    bool ValidarExpressao(const string& expr, string& outErro);
};

#endif
