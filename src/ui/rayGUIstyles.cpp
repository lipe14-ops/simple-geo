#include "rayGuiStyles.hpp"
#include "include/raygui.h"
#include <raylib.h>

void SetupGuiStyles(){
    // Input Box - fundo
    GuiSetStyle(TEXTBOX, BACKGROUND_COLOR, ColorToInt(BLACK));
    GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED, ColorToInt(DARKGRAY));

    // Input Box - borda
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, ColorToInt(GRAY));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, ColorToInt(LIGHTGRAY));
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED, ColorToInt(WHITE));

    // Input Box -  texto
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, ColorToInt(LIGHTGRAY));
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, ColorToInt(WHITE));
    GuiSetStyle(DEFAULT, TEXT_SIZE, 22);

    // MenuHelper Button - fundo
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(LIGHTGRAY));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(WHITE));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(LIGHTGRAY));

    // MenuHelper Button - borda
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, ColorToInt(DARKGRAY));
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED, ColorToInt(BLACK));

    // MenuHelper Button - texto
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(DARKGRAY));
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
}