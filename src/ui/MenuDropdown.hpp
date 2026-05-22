#ifndef MENU_DROPDOWN_HPP
#define MENU_DROPDOWN_HPP

#include <raylib.h>

enum class OpcaoMenu {
    TROCAR_COR,
    TOGGLE_VISIVEL,
    REMOVER,
    NENHUMA
};

class MenuDropdown {
private:
    bool ativo;
    Rectangle area;
    int idFuncao;
    int itemHover;

public:
    MenuDropdown();
    
    void Open(int idFunc, Rectangle posicao);
    void Close();
    bool IsAberto() const { return ativo; }
    
    void Update();
    void Draw() const;
    
    OpcaoMenu GetOpcaoClicada();
    int GetIDFuncao() const { return idFuncao; }
    
private:
    void LayoutItens();
};

#endif
