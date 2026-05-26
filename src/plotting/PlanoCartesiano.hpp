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
    void AlterarEscala(float novaEscala);
    float GetEscala() const { return m_escala; }
    void AlterarOffset(Vector2 novoOffset);
    Vector2 GetOffset() const { return m_offsetTranslacao; }

private:
    float m_escala;
    Vector2 m_offsetTranslacao = {0.0f, 0.0f};
    std::vector<Grafico> m_graficos;

    void DesenharEixos(int largura, int altura, int offsetX) const;
    void DesenharGrid(int largura, int altura, int offsetX) const;
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
        grafico.Calcular(largura, altura, m_escala, offsetX, m_offsetTranslacao);
}

void PlanoCartesiano::Desenhar(int largura, int altura, int offsetX=0) const {
    DesenharGrid(largura, altura, offsetX);
    DesenharEixos(largura, altura, offsetX);
    for (const auto& grafico : m_graficos)
        grafico.Desenhar();
}

void PlanoCartesiano::DesenharEixos(int largura, int altura, int offsetX) const {
    float centerX = offsetX + largura / 2.0f + m_offsetTranslacao.x;
    float centerY = altura / 2.0f + m_offsetTranslacao.y;
    int fontSize = 18;
    
    DrawLine(offsetX, centerY, offsetX + largura, centerY, BLACK);       // Eixo X
    DrawLine(centerX, 0, centerX, altura, BLACK);                         // Eixo Y
    
    // Desenhar valores no eixo X
    for (float screenX = centerX; screenX <= offsetX + largura; screenX += m_escala) {
        int xValue = (int)((screenX - centerX) / m_escala);
        if (xValue != 0) {  
            DrawText(TextFormat("%d", xValue), screenX - 6, centerY + 5, fontSize, DARKGRAY);
        }
    }
    for (float screenX = centerX - m_escala; screenX >= offsetX; screenX -= m_escala) {
        int xValue = (int)((screenX - centerX) / m_escala);
        if (xValue != 0) {
            DrawText(TextFormat("%d", xValue), screenX - 6, centerY + 5, fontSize, DARKGRAY);
        }
    }
    
    // Desenhar valores no eixo Y
    for (float screenY = centerY; screenY <= altura; screenY += m_escala) {
        int yValue = -(int)((screenY - centerY) / m_escala);
        if (yValue != 0) {
            DrawText(TextFormat("%d", yValue), centerX - 20, screenY - 5, fontSize, DARKGRAY);
        }
    }
    for (float screenY = centerY - m_escala; screenY >= 0; screenY -= m_escala) {
        int yValue = -(int)((screenY - centerY) / m_escala);
        if (yValue != 0) {
            DrawText(TextFormat("%d", yValue), centerX - 20, screenY - 5, fontSize, DARKGRAY);
        }
    }
    
    // Desenhar 0 na origem
    DrawText("0", centerX - 6, centerY + 5, fontSize, BLACK);
}

void PlanoCartesiano::DesenharGrid(int largura, int altura, int offsetX) const {
    float centerX = offsetX + largura / 2.0f + m_offsetTranslacao.x;
    float centerY = altura / 2.0f + m_offsetTranslacao.y;
  
    // Linhas verticais - começando de x = 0 no centro e indo para ambos os lados
    for (float screenX = centerX; screenX <= offsetX + largura; screenX += m_escala) {
        if (screenX <= offsetX + largura) {
            DrawLine(screenX, 0, screenX, altura, GRAY);
        }
    }
    for (float screenX = centerX - m_escala; screenX >= offsetX; screenX -= m_escala) {
        if (screenX >= offsetX) {
            DrawLine(screenX, 0, screenX, altura, GRAY);
        }
    }
    
    // Linhas horizontais - começando de y = 0 no centro e indo para ambos os lados
    for (float screenY = centerY; screenY <= altura; screenY += m_escala) {
        if (screenY <= altura) {
            DrawLine(offsetX, screenY, offsetX + largura, screenY, GRAY);
        }
    }
    for (float screenY = centerY - m_escala; screenY >= 0; screenY -= m_escala) {
        if (screenY >= 0) {
            DrawLine(offsetX, screenY, offsetX + largura, screenY, GRAY);
        }
    }
}

void PlanoCartesiano::Limpar() {
    m_graficos.clear();
}

void PlanoCartesiano::AlterarEscala(float novaEscala) {
    if (novaEscala > 1.0f) {  // Escala mínima para evitar zoom muito pequeno
        m_escala = novaEscala;
    }
}

void PlanoCartesiano::AlterarOffset(Vector2 novoOffset) {
    m_offsetTranslacao = novoOffset;
}