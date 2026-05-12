#pragma once
#define RAYGUI_IMPLEMENTATION
#include "../../include/raygui.h"
#include <raylib.h>
#include <iostream>
#include "PlanoCartesiano.hpp"
#include "../ui/sidebar.hpp"
#include "../ui/menuHelper.hpp"
#include "../ui/rayGuiStyles.hpp"

class Janela {
public:
    Janela(int largura, int altura, const char* titulo);
    ~Janela();

    void Executar();

private:
    int m_larguraInicial;
    int m_alturaInicial;
    PlanoCartesiano m_plano;

    void ProcessarEntrada();
    void AlternarTelaCheia();
};

Janela::Janela(int largura, int altura, const char* titulo)
    : m_larguraInicial(largura), m_alturaInicial(altura), m_plano(20.0f)
{
    InitWindow(largura, altura, titulo);
    SetTargetFPS(60);

}

Janela::~Janela() {
    CloseWindow();
}

void Janela::Executar() {
    SetupGuiStyles();

    const int SIDEBAR_WIDTH = 350;
    int screenHeight = GetScreenHeight();
    int screenWidth = GetScreenWidth();

    int plotWidth = screenWidth - SIDEBAR_WIDTH;

    Color background = LIGHTGRAY;
    Sidebar sidebar(screenHeight, SIDEBAR_WIDTH);
    MenuHelper menuHelper({(float)screenWidth - 690, 15}, 610, 370);

    while (!WindowShouldClose()) {
        ProcessarEntrada();

        screenHeight = GetScreenHeight();
        screenWidth = GetScreenWidth();
        plotWidth = screenWidth - SIDEBAR_WIDTH;

        sidebar.Update();
        if(sidebar.GetInput().IsSubmitted()){
            const char* funcao = sidebar.GetInput().GetSubmittedFuncion();
            Color functionColor = BLUE;

            try {
                sidebar.ClearError();
                m_plano.Limpar();
                m_plano.AdicionarGrafico(funcao, functionColor);
                sidebar.GetInput().SetSubmittedColor(functionColor);
            } catch (const runtime_error& e) {
                sidebar.SetError(e.what());
            }
        }

        m_plano.Atualizar(plotWidth, screenHeight, SIDEBAR_WIDTH);

        BeginDrawing();
            ClearBackground(background);

            BeginScissorMode(SIDEBAR_WIDTH, 0, plotWidth, screenHeight);
                m_plano.Desenhar(plotWidth, screenHeight, SIDEBAR_WIDTH);
            EndScissorMode();
                    
            if (GuiButton({(float)screenWidth - 65, 15, 50, 50}, "?")) {
                menuHelper.Toggle();
            }
            menuHelper.Draw();       
            sidebar.Draw(screenHeight);

        EndDrawing();
    }
}

void Janela::ProcessarEntrada() {
    if (IsKeyPressed(KEY_F11))
        AlternarTelaCheia();
}

void Janela::AlternarTelaCheia() {
    if (!IsWindowFullscreen()) {
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    } else {
        SetWindowSize(m_larguraInicial, m_alturaInicial);
        ToggleFullscreen();
    }
}