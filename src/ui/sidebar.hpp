#ifndef SIDEBAR_HPP
#define SIDEBAR_HPP

#include <raylib.h>
#include "inputBox.hpp"
#include <string>

using namespace std;

class Sidebar {
private:
    int width;
    float paddingWidth;
    InputBox inputBox;
    string errorMessage;
    
public:
    Sidebar(int screenHeight, int width);

    void Update();
    void Draw(int screenHeight);
    InputBox& GetInput();

    void SetError(const string& msg);
    void ClearError();
};

#endif
