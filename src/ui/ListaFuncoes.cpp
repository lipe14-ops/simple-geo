#include "ListaFuncoes.hpp"
#include "include/raygui.h"
#include <cstdio>

using namespace std;

ListaFuncoes::ListaFuncoes(Rectangle area)
    : area(area), funcoes(nullptr), yOffset(0.0f), idFuncaoComMenu(-1)
{
}

void ListaFuncoes::Update() {
    if (!funcoes) return;
    
    float wheel = GetMouseWheelMove(); //scroll
    if (wheel != 0) {
        yOffset -= wheel * 20;
        int maxScroll = max(0, (int)(funcoes->size() * alturaItem) - (int)area.height);
        if (yOffset < 0) yOffset = 0;
        if (yOffset > maxScroll) yOffset = maxScroll;
    }
    menu.Update();
    Vector2 mouse = GetMousePosition();
    int index = 0;
    
    for (const auto& f : *funcoes) {
        float yPos = area.y + 5 - yOffset + (index * alturaItem);
        
        if (yPos < area.y || yPos > area.y + area.height) {
            index++;
            continue;
        }
        
        Rectangle btnMenu = GetBotaoMenu(index, yPos);
        
        if (CheckCollisionPointRec(mouse, btnMenu) && 
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            menu.Open(f.id, {btnMenu.x + 35, btnMenu.y, 150, 105});
            idFuncaoComMenu = f.id;
        }
        index++;
    }
}

void ListaFuncoes::Draw() const {
    if (!funcoes) return;

    DrawRectangle(area.x, area.y, area.width, area.height, {20, 20, 20, 255});
    DrawRectangleLinesEx(area, 1, GRAY);

    BeginScissorMode(area.x, area.y, area.width, area.height);
        float yPos = area.y + 5 - yOffset;
        int index = 0;
        
        for (const auto& f : *funcoes) {
            RenderItem(f, index, yPos);
            yPos += alturaItem;
            index++;
        } 
    EndScissorMode();

    menu.Draw();
}

void ListaFuncoes::RenderItem(const FuncaoRegistro& f, int index, float yPos) const {
    Color bgColor = (index % 2 == 0) ? Color{30, 30, 30, 255} : Color{40, 40, 40, 255};
    
    DrawRectangle(area.x + 5, yPos, area.width - 10, alturaItem - 5, bgColor); //bg funcão
    DrawRectangle(area.x + 10, yPos + 5, 20, 20, f.cor); // Quadrado de cor
    
    Color textColor = f.visivel ? WHITE : GRAY;
    DrawText(f.expressao.c_str(), area.x + 40, yPos + 8, 18, textColor); // Expressão
    
    GuiButton(GetBotaoMenu(index, yPos), ":");
}

Rectangle ListaFuncoes::GetBotaoMenu(int index, float yPos) const {
    return {area.x + area.width - 40, yPos + 2, 30, 30};
}

OpcaoMenu ListaFuncoes::GetOpcaoSelecionada(int& outID) {
    OpcaoMenu opcao = menu.GetOpcaoClicada();
    outID = menu.GetIDFuncao();
    return opcao;
}
