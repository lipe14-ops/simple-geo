#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"
#include <raylib.h>
#include <iostream>
#include "plotting/PlanoCartesiano.hpp"
#include "include/sidebar.hpp"
#include "include/menuHelper.hpp"
#include "include/rayGUIstyles.hpp"

using namespace std;

int main()
{
    InitWindow(1700, 900, "Simple GEO");
    SetTargetFPS(60);
    SetupGuiStyles();

    const int SIDEBAR_WIDTH = 350;
    int screenHeight = GetScreenHeight();
    int screenWidth = GetScreenWidth();
    int plotWidth = screenWidth - SIDEBAR_WIDTH;

    Color background = LIGHTGRAY;
    Sidebar sidebar(screenHeight, SIDEBAR_WIDTH);
    MenuHelper menuHelper({(float)screenWidth - 580, 15}, 500, 410);
    
    PlanoCartesiano plano(20.0f);

    while(WindowShouldClose() == false){

        sidebar.Update();
        if(sidebar.GetInput().IsSubmitted()){
            const char* funcao = sidebar.GetInput().GetSubmittedFuncion();
            
            plano = PlanoCartesiano(20.0f);
            Color functionColor = BLUE;

            try {
                sidebar.ClearError();
                plano.AdicionarGrafico(funcao, functionColor);
                sidebar.GetInput().SetSubmittedColor(functionColor);
            } catch (const runtime_error& e) {
                sidebar.SetError(e.what());
            }

        }

        plano.Atualizar(plotWidth, screenHeight, SIDEBAR_WIDTH);

        BeginDrawing();
            ClearBackground(background);

            BeginScissorMode(SIDEBAR_WIDTH, 0, plotWidth, screenHeight);
                plano.Desenhar(plotWidth, screenHeight, SIDEBAR_WIDTH);
            EndScissorMode();
                    
            if (GuiButton({(float)screenWidth - 65, 15, 50, 50}, "?")) {
                menuHelper.Toggle();
            }
            menuHelper.Draw();       
            sidebar.Draw(screenHeight);

        EndDrawing();
    }
    CloseWindow();
}