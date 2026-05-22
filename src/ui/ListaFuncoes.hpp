#ifndef LISTA_FUNCOES_HPP
#define LISTA_FUNCOES_HPP

#include <raylib.h>
#include "FuncaoRegistro.hpp"
#include "MenuDropdown.hpp"
#include <vector>

using namespace std;

class ListaFuncoes {
private:
    Rectangle area;
    const vector<FuncaoRegistro>* funcoes;
    
    float yOffset;
    const int alturaItem = 40;
    
    MenuDropdown menu;
    int idFuncaoComMenu;

public:
    ListaFuncoes(Rectangle area);
    
    void SetFuncoes(const vector<FuncaoRegistro>* f) { funcoes = f; }
    void Update();
    void Draw() const;
    
    OpcaoMenu GetOpcaoSelecionada(int& outID);
    
private:
    void RenderItem(const FuncaoRegistro& f, int index, float yPos) const;
    Rectangle GetBotaoMenu(int index, float yPos) const;
};

#endif
