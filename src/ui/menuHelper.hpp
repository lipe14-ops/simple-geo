#ifndef MENU_HELPER_HPP
#define MENU_HELPER_HPP

#include <raylib.h>
#include <string>

class MenuHelper {
private:
    float posX;
    float posY;
    float width;

    int fontSize;
    int lineSpacing;
    int padding;

    bool active;
public:
    MenuHelper(float screenWidth);

    void Toggle();
    void SetActive(bool state);
    bool IsActive() const;

    void Draw(float screenWidth);
};

#endif