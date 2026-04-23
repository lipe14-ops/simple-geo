#include "../include/sidebar.hpp"
#include "../include/raygui.h"


Sidebar::Sidebar(int screenHeight, int width)
    : width(width), paddingWidth(20),
    inputBox({paddingWidth, 60, (float)(width - paddingWidth*2), 50}, //inputbox
    {paddingWidth, 120, (float)(width - paddingWidth*2), 40}) // button "desenhar funcao"
{
}

void Sidebar::Update(){
    inputBox.Update();
}

void Sidebar::SetError(const string &msg){
    errorMessage = msg;
}

void Sidebar::ClearError(){
    errorMessage.clear();
}

void Sidebar::Draw(int screenHeight){
    
    DrawRectangle(0, 0, width, screenHeight, BLACK);

    DrawText("Digite uma função abaixo:", paddingWidth, 20, 20, WHITE);
    inputBox.Draw();
    
    GuiButton({paddingWidth, 120, (float)(width - paddingWidth*2), 40}, "Desenhar função");

    const char* submittedFunc = inputBox.GetSubmittedFuncion();

    if(submittedFunc[0] != '\0' && errorMessage.empty()){
        DrawText("Função 01:", paddingWidth, 195, 20, LIGHTGRAY);
        DrawText(submittedFunc, paddingWidth, 220, 22, inputBox.GetSubmittedColor());
    }

    if(!errorMessage.empty()){
        DrawText(errorMessage.c_str(), paddingWidth, 170, 18, RED);
    }
}

InputBox &Sidebar::GetInput(){
    return inputBox;
}

