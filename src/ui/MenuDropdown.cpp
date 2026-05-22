#include "MenuDropdown.hpp"
#include "include/raygui.h"

MenuDropdown::MenuDropdown()
    : ativo(false), idFuncao(-1), itemHover(-1)
{
    area = {0, 0, 150, 35};
}

void MenuDropdown::Open(int idFunc, Rectangle posicao) {
    ativo = true;
    idFuncao = idFunc;
    area = posicao;
    itemHover = -1;
    LayoutItens();
}

void MenuDropdown::Close() {
    ativo = false;
}

void MenuDropdown::Update() {
    if (!ativo) return;
    
    Vector2 mouse = GetMousePosition();
    itemHover = -1;
    
    for (int i = 0; i < 3; i++) { //hover
        Rectangle itemBounds = {area.x, area.y + i * 35, area.width, 35};
        if (CheckCollisionPointRec(mouse, itemBounds)) {
            itemHover = i;
            break;
        }
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { //clicar fora
        if (itemHover == -1) { ativo = false; }
    }
}

OpcaoMenu MenuDropdown::GetOpcaoClicada() {
    if (!ativo || itemHover == -1) return OpcaoMenu::NENHUMA;
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        OpcaoMenu opcoes[] = {
            OpcaoMenu::TROCAR_COR,
            OpcaoMenu::TOGGLE_VISIVEL,
            OpcaoMenu::REMOVER
        };
        
        OpcaoMenu op = opcoes[itemHover];
        ativo = false;
        return op;
    }
    
    return OpcaoMenu::NENHUMA;
}

void MenuDropdown::Draw() const {
    if (!ativo) return;
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 100});

    float menuHeight = 3 * 35;

    DrawRectangleRec({area.x, area.y, area.width, menuHeight}, DARKGRAY);
    DrawRectangleLinesEx({area.x, area.y, area.width, menuHeight}, 2, WHITE);

    const char* labels[] = {"Trocar Cor", "Mostrar / Ocultar", "Remover"};
    
    for (int i = 0; i < 3; i++) {
        Rectangle itemBounds = {area.x, area.y + i * 35, area.width, 35};
        Color bgColor = (i == itemHover) ? GRAY : DARKGRAY;
        DrawRectangleRec(itemBounds, bgColor);
        DrawText(labels[i], area.x + 10, area.y + i * 35 + 8, 14, WHITE);
    }
}

void MenuDropdown::LayoutItens() {
    if (area.x + area.width > GetScreenWidth()) { 
        area.x = GetScreenWidth() - area.width - 10;
    }
    if (area.y + 3 * 35 > GetScreenHeight()) {
        area.y = GetScreenHeight() - 3 * 35 - 10;
    }
}
