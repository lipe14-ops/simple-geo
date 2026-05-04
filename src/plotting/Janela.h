#pragma once
#include "raylib.h"
#include "PlanoCartesiano.h"

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

    // Adicione aqui as funções que deseja plotar
    m_plano.AdicionarGrafico("x^2 + 4", BLUE);
}

Janela::~Janela() {
    CloseWindow();
}

void Janela::Executar() {
    while (!WindowShouldClose()) {
        ProcessarEntrada();

        int largura = GetScreenWidth();
        int altura = GetScreenHeight();

        m_plano.Atualizar(largura, altura);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            m_plano.Desenhar(largura, altura);
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