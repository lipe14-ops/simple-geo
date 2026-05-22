#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include <raylib.h>
#include "inputBox.hpp"
#include <string>
#include "FuncaoRegistro.hpp"
#include "GerenciadorFuncoes.hpp"
#include "ListaFuncoes.hpp"
#include "MenuDropdown.hpp"

using namespace std;

class Sidebar {
private:
    int width;
    float paddingWidth;
    InputBox inputBox;
    ListaFuncoes lista;
    GerenciadorFuncoes gerenciador;
    int idSelecionandoCor;
    string errorMessage;
    string successMessage;
    Rectangle colorPickerArea;
    
    public:
    Sidebar(int screenHeight, int width);
    
    Color corSelecionada;
    bool selecionandoCor;
    void Update();
    void Draw(int screenHeight);
    InputBox& GetInput();
    GerenciadorFuncoes& GetGerenciador() { return gerenciador; }

    void SetError(const string& msg);
    void ClearError();
    void SetSuccess(const string& msg);
    
private:
    void ProcessarInputBox();
    void ProcessarMenuDropdown();
    void NovaFuncao(const string& expr);
    void RemoverFuncao(int id);
    void TrocarCorFuncao(int id, Color novaCor);
    void ToggleVisibilidadeFuncao(int id);
};

#endif
