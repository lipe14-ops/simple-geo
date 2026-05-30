#include "gerenciadorFuncoes.hpp"
#include "../parsing/expressions.hpp"
#include <algorithm>
#include <stdexcept>

using namespace std;

int GerenciadorFuncoes::AdicionarFuncao(
    const string& expr, 
    Color cor
) {
    string erro;
    if (!ValidarExpressao(expr, erro)) { return -1; }

    FuncaoRegistro novaFuncao(
        proximoID,
        expr,
        (cor.r == -1 && cor.g == -1 && cor.b == -1) 
            ? ProximaCor()
            : cor           
    );
    
    novaFuncao.SetValida(true);
    funcoes.push_back(novaFuncao);
    int idCriado = proximoID;
    proximoID++;
    
    return idCriado;
}

bool GerenciadorFuncoes::RemoverFuncao(int id) {
    auto it = find_if(
        funcoes.begin(), 
        funcoes.end(),
        [id](const FuncaoRegistro& f) { 
            return f.id == id; 
        }
    );
    if (it == funcoes.end()) { return false; }
    
    funcoes.erase(it);
    if (idSelecionada == id) { idSelecionada = -1; }
    
    return true;
}

bool GerenciadorFuncoes::ToggleVisibilidade(int id) {
    FuncaoRegistro* f = FindById(id);
    if (!f) return false;
    f->visivel = !f->visivel;
    return true;
}

vector<FuncaoRegistro> GerenciadorFuncoes::GetFuncoesVisiveis() const {
    vector<FuncaoRegistro> resultado;
    
    for (const auto& f : funcoes) {
        if (f.visivel) {
            resultado.push_back(f);
        }
    }
    return resultado;
}

const FuncaoRegistro* GerenciadorFuncoes::GetFuncao(int id) const {
    for (const auto& f : funcoes) {
        if (f.id == id) {
            return &f;
        }
    }
    return nullptr;
}

FuncaoRegistro* GerenciadorFuncoes::GetFuncaoMutavel(int id) {
    for (auto& f : funcoes) {
        if (f.id == id) {
            return &f;
        }
    }
    return nullptr;
}

Color GerenciadorFuncoes::ProximaCor() {
    Color c = coresDisponiveis[proximaCorIndex % 8];
    proximaCorIndex++;
    return c;
}

FuncaoRegistro* GerenciadorFuncoes::FindById(int id) {
    for (auto& f : funcoes) {
        if (f.id == id) {
            return &f;
        }
    }
    return nullptr;
}

bool GerenciadorFuncoes::ValidarExpressao(
    const std::string& expr, 
    std::string& outErro
) {
    try {
        // Testa parsing da expressão
        auto teste = expr::parse<float>(expr);
        return true;
    } catch (const std::runtime_error& e) {
        outErro = e.what();
        return false;
    }
}
