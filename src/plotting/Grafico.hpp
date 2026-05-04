// Grafico.hpp
#pragma once
#include "raylib.h"
#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include "../parsing/expressions.hpp"

using namespace expr;
using namespace std;

class Grafico {
public:
    Grafico(std::string funcao, Color cor = BLUE);
    // modificação: adicionado offset
    void Calcular(int largura, int altura, float escala, int offsetX);
    void Desenhar() const;

private:
    string m_funcao;
    Color m_cor;
    std::vector<Vector2> m_pontos;
};

Grafico::Grafico(string funcao, Color cor)
    : m_funcao(funcao), m_cor(cor) {}

// modificação: adicionado offset como parâmetro para calcular a distância da barra lateral da esquerda
void Grafico::Calcular(int largura, int altura, float escala, int offsetX = 0) {
    m_pontos.clear();
    
//    auto expr = parse<float>(m_funcao);

    //modificação: evita que um gráfico já inserido feche o programa
    ExprPtr<float> expr;
    try {
        expr = parse<float>(m_funcao);
    } catch (const runtime_error&) {
        return; // pontos vazios = nada desenhado, o programa não fecha sozinho
    }

    for (float x = -(float)largura / 2; x < (float)largura / 2; x += 0.1f) {
        //modificação: try exception
        try {
            float y = expr->eval(x);

            //modificação: ignora pontos com valores inválidos (inf, nan)
            if (!std::isfinite(y)) continue;

            float telaX = x * escala + largura / 2.0f + offsetX;
            float telaY = altura / 2.0f - y * escala;

            m_pontos.push_back({ telaX, telaY });
        } catch (const exception&){
            continue;
        }
    }
}

void Grafico::Desenhar() const {
    if (!m_pontos.empty())
        DrawLineStrip(m_pontos.data(), (int)m_pontos.size(), m_cor);
}