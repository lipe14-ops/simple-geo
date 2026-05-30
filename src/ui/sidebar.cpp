#include "sidebar.hpp"
#include "include/raygui.h"
#include <stdexcept>

using namespace std;

Sidebar::Sidebar(int screenHeight, int width)
    : width(width), paddingWidth(20),
    inputBox({paddingWidth, 60, (float)(width - paddingWidth*2), 50}, //inputbox
        {paddingWidth, 120, (float)(width - paddingWidth*2), 40}),// button "desenhar funcao"
    lista({paddingWidth, 180, (float)(width - paddingWidth*2), (float)(screenHeight - 230)}),
    idSelecionandoCor(-1), corSelecionada(BLUE), selecionandoCor(false)
{
    colorPickerArea = {paddingWidth, 180, (float)(width - paddingWidth*2), 120};
    lista.SetFuncoes(&gerenciador.GetFuncoes());
}

void Sidebar::Update(){
    inputBox.Update();
    lista.Update();
    ProcessarInputBox();
    ProcessarMenuDropdown();
}

void Sidebar::ProcessarInputBox() {
    if (!inputBox.IsSubmitted()) return;
    
    const char* textoDigitado = inputBox.GetSubmittedFuncion();
    try {
        ClearError();
        int novoID = gerenciador.AdicionarFuncao(textoDigitado, gerenciador.ProximaCor());
        
        if (novoID > 0) {
            SetSuccess("Função adicionada!");
            inputBox.ClearText();
        } else {
            SetError("Expressão inválida");
        }
    } catch (const runtime_error& e) {
        SetError(string("Erro: ") + e.what());
    }
}

void Sidebar::ProcessarMenuDropdown() {
    int idSelecionada;
    OpcaoMenu opcao = lista.GetOpcaoSelecionada(idSelecionada);
    
    if (opcao == OpcaoMenu::NENHUMA) return;
    
    switch (opcao) {
        case OpcaoMenu::REMOVER:
            RemoverFuncao(idSelecionada);
            break;

        case OpcaoMenu::TOGGLE_VISIVEL:
            ToggleVisibilidadeFuncao(idSelecionada);
            break;
            
        case OpcaoMenu::TROCAR_COR:
            idSelecionandoCor = idSelecionada;
            if (const FuncaoRegistro* f = gerenciador.GetFuncao(idSelecionandoCor)) {
                selecionandoCor = true;
                corSelecionada = f->cor;
            } else {
                idSelecionandoCor = -1;
                SetError("Função não encontrada");
            }
            break;
            
        default:
            break;
    }
}

void Sidebar::NovaFuncao(const string& expr) {
    int novoID = gerenciador.AdicionarFuncao(expr, gerenciador.ProximaCor());
    if (novoID > 0) {
        SetSuccess("Função adicionada!");
    } else {
        SetError("Expressão inválida");
    }
}

void Sidebar::RemoverFuncao(int id) {
    if (gerenciador.RemoverFuncao(id)) {
        SetSuccess("Função removida");
    } else {
        SetError("Erro ao remover");
    }
}

void Sidebar::TrocarCorFuncao(int id, Color novaCor) {
    FuncaoRegistro* f = gerenciador.GetFuncaoMutavel(id);
    if (f) {
        f->cor = novaCor;
        SetSuccess("Cor alterada!");
        selecionandoCor = false;
    }
}

void Sidebar::ToggleVisibilidadeFuncao(int id) {
    if (gerenciador.ToggleVisibilidade(id)) {
        SetSuccess("Visibilidade alterada");
    }
}

void Sidebar::SetError(const string& msg){
    errorMessage = msg;
    successMessage = "";
}

void Sidebar::ClearError(){
    errorMessage.clear();
}

void Sidebar::SetSuccess(const string& msg) {
    successMessage = msg;
    errorMessage = "";
}

void Sidebar::Draw(int screenHeight){

    DrawRectangle(0, 0, width, screenHeight, BLACK);

    DrawText("Digite uma função abaixo:", paddingWidth, 20, 20, WHITE);
    inputBox.Draw();
    
    GuiButton({paddingWidth, 120, (float)(width - paddingWidth*2), 40}, "Desenhar função");
    
    
    if (selecionandoCor) { //color picker
        DrawText("Selecione a cor:", paddingWidth, 180, 16, WHITE);
        GuiColorPicker(colorPickerArea, "", &corSelecionada);
        
        if (GuiButton({paddingWidth, 310, (float)(width - paddingWidth*2)/2, 30}, "Aplicar")) {
            TrocarCorFuncao(idSelecionandoCor, corSelecionada);
            idSelecionandoCor = -1;
        }
        
        if (GuiButton({paddingWidth + (width - paddingWidth*2)/2 + 5, 310, (float)(width - paddingWidth*2)/2 - 5, 30}, "Cancelar")) {
            selecionandoCor = false;
            idSelecionandoCor = -1;
        }
    }else{
        lista.Draw();
    }

    if (!errorMessage.empty()) {
        DrawText(errorMessage.c_str(), paddingWidth, screenHeight - 35, 16, RED);
    }
    if (!successMessage.empty()) {
        DrawText(successMessage.c_str(), paddingWidth, screenHeight - 35, 16, GREEN);
    }
}

InputBox& Sidebar::GetInput(){
    return inputBox;
}
