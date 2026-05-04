// Grafico.h
#pragma once
#include "raylib.h"
#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include "../parsing/expressions.h"

using namespace expr;
using namespace std;

class Grafico {
public:
    Grafico(std::string funcao, Color cor = BLUE);

    void Calcular(int largura, int altura, float escala);
    void Desenhar() const;

private:
    string m_funcao;
    Color m_cor;
    std::vector<Vector2> m_pontos;
};

Grafico::Grafico(string funcao, Color cor)
    : m_funcao(funcao), m_cor(cor) {}

void Grafico::Calcular(int largura, int altura, float escala) {
    m_pontos.clear();
    
    auto expr = parse<float>(m_funcao);

    for (float x = -(float)largura / 2; x < (float)largura / 2; x += 0.1f) {
        float y = expr->eval(x);

        float telaX = x * escala + largura / 2.0f;
        float telaY = altura / 2.0f - y * escala;

        m_pontos.push_back({ telaX, telaY });
    }
}

void Grafico::Desenhar() const {
    if (!m_pontos.empty())
        DrawLineStrip(m_pontos.data(), (int)m_pontos.size(), m_cor);
}