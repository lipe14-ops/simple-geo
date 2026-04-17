#pragma once
#include "raylib.h"
#include "Grafico.h"
#include <vector>
#include <memory>

class PlanoCartesiano {
public:
    PlanoCartesiano(float escala = 20.0f);

    void AdicionarGrafico(string funcao, Color cor = BLUE);
    void Atualizar(int largura, int altura);
    void Desenhar(int largura, int altura) const;

private:
    float m_escala;
    std::vector<Grafico> m_graficos;

    void DesenharEixos(int largura, int altura) const;
};

PlanoCartesiano::PlanoCartesiano(float escala)
    : m_escala(escala) {}

void PlanoCartesiano::AdicionarGrafico(string funcao, Color cor) {
    m_graficos.emplace_back(funcao, cor);
}

void PlanoCartesiano::Atualizar(int largura, int altura) {
    for (auto& grafico : m_graficos)
        grafico.Calcular(largura, altura, m_escala);
}

void PlanoCartesiano::Desenhar(int largura, int altura) const {
    DesenharEixos(largura, altura);
    for (const auto& grafico : m_graficos)
        grafico.Desenhar();
}

void PlanoCartesiano::DesenharEixos(int largura, int altura) const {
    DrawLine(0, altura / 2, largura, altura / 2, BLACK);       // Eixo X
    DrawLine(largura / 2, 0, largura / 2, altura, BLACK);      // Eixo Y
}