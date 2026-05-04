#include "menuHelper.hpp"

MenuHelper::MenuHelper(Vector2 position, float width, float height){
    background = { position.x, position.y, width, height };
    active = true;

    fontSize = 20;
    lineSpacing = 30;
    padding = 15;
}

void MenuHelper::Toggle(){
    active = !active;
}

void MenuHelper::SetActive(bool state){
    active = state;
}

bool MenuHelper::IsActive() const{
    return active;
}

void MenuHelper::Draw() const{
    if (!active) return;

    DrawRectangleRec(background, LIGHTGRAY);
    DrawRectangleLinesEx(background, 2, BLACK);

    int startX = background.x + padding;
    int startY = background.y + padding;

    DrawText(
        "Clique no botão ' ? ' para fechar e abrir essa aba  ---->", startX,
        startY,
        fontSize, BLACK
    );

    DrawText(
        "Utilizando x como variável, insira uma função polinomial à", startX,
        startY + lineSpacing * 2,
        fontSize, BLACK
    );
    DrawText(
        "esquerda. Utilize os seguinte símbolos para operações:", startX,
        startY + lineSpacing * 3,
        fontSize, BLACK
    );

    DrawText(
        "      adição:           +        |        subtração:      -", startX,
        startY + lineSpacing * 5,
        fontSize, BLACK
    );
    DrawText(
        "      multiplicação:   *        |        divisão:          /", startX,
        startY + lineSpacing * 6,
        fontSize, BLACK
    );

    DrawText(
        "      potenciação:    ^        |", startX,
        startY + lineSpacing * 7,
        fontSize, BLACK
    );

    DrawText(
        "Use parênteses () para indicar prioridade.", startX,
        startY + lineSpacing * 9,
        fontSize, BLACK
    );

    DrawText(
        "Clique em ''Desenhar Função'' para visualisar o gráfico.", startX,
        startY + lineSpacing * 10 + 10,
        fontSize, BLACK
    );

}
