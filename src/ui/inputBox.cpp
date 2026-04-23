#include "../include/inputBox.hpp"
#include "../include/raygui.h"
#include "../include/inputBox.hpp"
#include <cstring>

InputBox::InputBox(Rectangle box, Rectangle buttonBox){
    this->box = box;
    this->buttonBox = buttonBox;
    text[0] = '\0';
    submittedFunction[0] = '\0';
    editMode = false;
    submitted = false;
}

void InputBox::Update(){
    if(GuiTextBox(box, text, 256, editMode)){
        editMode = !editMode;
    }

    if(GuiButton(buttonBox, "Desenhar função")){
        strcpy(submittedFunction, text);
        submitted = true;
        editMode = false;
        text[0] = '\0';
    }
}

void InputBox::Draw(){
    GuiTextBox(box, text, 256, editMode);
}

void InputBox::ClearText(){
    text[0] = '\0';
}

void InputBox::SetSubmittedColor(Color color){
    submittedColor = color;
}

const char* InputBox::GetText() const{
    return text;
}

const char* InputBox::GetSubmittedFuncion() const{
    return submittedFunction;
}

Color InputBox::GetSubmittedColor() const{
    return submittedColor;
}

bool InputBox::IsSubmitted(){
    if(submitted){
        submitted = false;
        return true;
    }
    return false;
}