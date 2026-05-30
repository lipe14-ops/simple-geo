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
    float m_velocidadeZoom = 5.0f;  // Controla a sensibilidade do zoom
    Vector2 m_posicaoMouseAnterior = {0.0f, 0.0f};
    bool m_arrastando = false;

    void ProcessarEntrada();
    void AlternarTelaCheia();
    void ProcessarZoom();
    void ProcessarTranslacao();
};

Janela::Janela(int largura, int altura, const char* titulo)
    : m_larguraInicial(largura), m_alturaInicial(altura), m_plano(80.0f)
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

    Color background = WHITE;  //LIGTHRAY
    Sidebar sidebar(screenHeight, SIDEBAR_WIDTH);
    MenuHelper menuHelper((float)screenWidth);

    while (!WindowShouldClose()) {
        ProcessarEntrada();

        screenHeight = GetScreenHeight();
        screenWidth = GetScreenWidth();
        plotWidth = screenWidth - SIDEBAR_WIDTH;

        sidebar.Update();
        m_plano.Limpar();
        auto funcoesVisiveis = sidebar.GetGerenciador().GetFuncoesVisiveis();
        for (const auto& f : funcoesVisiveis) {
            try {
                m_plano.AdicionarGrafico(f.expressao, f.cor);
            } catch (const std::runtime_error& e) { }
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
            menuHelper.Draw((float)screenWidth);       
            sidebar.Draw(screenHeight);

        EndDrawing();
    }
}

void Janela::ProcessarEntrada() {
    if (IsKeyPressed(KEY_F11))
        AlternarTelaCheia();
    ProcessarZoom();
    ProcessarTranslacao();
}

void Janela::ProcessarZoom() {
    float mouseWheelMove = GetMouseWheelMove();
    if (mouseWheelMove != 0.0f) {
        float escalaAtual = m_plano.GetEscala();
        float novaEscala = escalaAtual + (mouseWheelMove * m_velocidadeZoom);
        m_plano.AlterarEscala(novaEscala);
    }
}

void Janela::ProcessarTranslacao() {
    Vector2 mousePosAtual = GetMousePosition();
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_arrastando = true;
        m_posicaoMouseAnterior = mousePosAtual;
    }
    
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        m_arrastando = false;
    }
    
    if (m_arrastando) {
        Vector2 diferenca = {mousePosAtual.x - m_posicaoMouseAnterior.x,
                             mousePosAtual.y - m_posicaoMouseAnterior.y};
        Vector2 offsetAtual = m_plano.GetOffset();
        m_plano.AlterarOffset({offsetAtual.x + diferenca.x, offsetAtual.y + diferenca.y});
        m_posicaoMouseAnterior = mousePosAtual;
    }
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