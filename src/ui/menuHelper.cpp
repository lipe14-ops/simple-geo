#include "menuHelper.hpp"

MenuHelper::MenuHelper(Vector2 position, float width, float height){
    background = { position.x, position.y, width, height };
    active = false;

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
        "Utilizando x como variável, insira uma função", startX,
        startY,
        fontSize, BLACK
    );
    DrawText(
        "polinomial utilizando as seguintes operações:", startX,
        startY + lineSpacing * 1,
        fontSize, BLACK
    );

    DrawText(
        "adição:           +", startX,
        startY + lineSpacing * 3,
        fontSize, BLACK
    );
    DrawText(
        "subtração:      -", startX,
        startY + lineSpacing * 4,
        fontSize, BLACK
    );
    DrawText(
        "multiplicação:   *", startX,
        startY + lineSpacing * 5,
        fontSize, BLACK
    );
    DrawText(
        "divisão:          /", startX,
        startY + lineSpacing * 6,
        fontSize, BLACK
    );
    DrawText(
        "potenciação:    ^", startX,
        startY + lineSpacing * 7,
        fontSize, BLACK
    );

    DrawText(
        "Use parênteses () para indicar prioridade.", startX,
        startY + lineSpacing * 9,
        fontSize, BLACK
    );

    DrawText(
        "Clique em ''Desenhar Função'' para visualisar", startX,
        startY + lineSpacing * 10 + 10,
        fontSize, BLACK
    );

    DrawText(
        "o gráfico.", startX,
        startY + lineSpacing * 11 + 10,
        fontSize, BLACK
    );
}
