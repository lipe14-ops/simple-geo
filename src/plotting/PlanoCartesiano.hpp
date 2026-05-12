#pragma once
#include "raylib.h"
#include "Grafico.hpp"
#include <vector>
#include <memory>

class PlanoCartesiano {
public:
    PlanoCartesiano(float escala = 20.0f);

    void AdicionarGrafico(string funcao, Color cor = BLUE);
    // modificação: adicionado offset
    void Atualizar(int largura, int altura, int offsetX);
    void Desenhar(int largura, int altura, int offsetX) const;
    void Limpar();

private:
    float m_escala;
    std::vector<Grafico> m_graficos;

    void DesenharEixos(int largura, int altura, int offsetX) const;
};

PlanoCartesiano::PlanoCartesiano(float escala)
    : m_escala(escala) {}

void PlanoCartesiano::AdicionarGrafico(string funcao, Color cor) {
    //modificação: testa se a função é válida antes de adicioná-la na fila
    auto teste = expr::parse<float>(funcao);
    m_graficos.emplace_back(funcao, cor);
}

// modificação: adicionado offset como parâmetro para calcular a distância da barra lateral da esquerda
void PlanoCartesiano::Atualizar(int largura, int altura, int offsetX=0) {
    for (auto& grafico : m_graficos)
        grafico.Calcular(largura, altura, m_escala, offsetX);
}

void PlanoCartesiano::Desenhar(int largura, int altura, int offsetX=0) const {
    DesenharEixos(largura, altura, offsetX);
    for (const auto& grafico : m_graficos)
        grafico.Desenhar();
}

void PlanoCartesiano::DesenharEixos(int largura, int altura, int offsetX) const {
    DrawLine(0, altura / 2, largura + offsetX, altura / 2, BLACK);       // Eixo X
    DrawLine(largura / 2 + offsetX, 0, largura / 2 + offsetX, altura, BLACK);      // Eixo Y
}

void PlanoCartesiano::Limpar() {
    m_graficos.clear();
}