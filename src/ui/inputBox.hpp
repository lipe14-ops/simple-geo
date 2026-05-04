#ifndef INPUTBOX_HPP
#define INPUTBOX_HPP

#include <raylib.h>

class InputBox{
private:
    Rectangle box;
    Rectangle buttonBox;
    
    char text[256];
    char submittedFunction[256];
    bool editMode;
    bool submitted;
    Color submittedColor;

public:
    InputBox(Rectangle box, Rectangle buttonBox);

    void Update();
    void Draw();
    bool IsSubmitted();    
    void ClearText();
    void SetSubmittedColor(Color color);

    const char* GetText() const;
    const char* GetSubmittedFuncion() const;
    Color GetSubmittedColor() const;
};

#endif