#ifndef MENU_HELPER_HPP
#define MENU_HELPER_HPP

#include <raylib.h>
#include <string>

class MenuHelper {
private:
    Rectangle background;
    bool active;

    int fontSize;
    int lineSpacing;
    int padding;

public:
    MenuHelper(Vector2 position, float width, float height);

    void Toggle();
    void SetActive(bool state);
    bool IsActive() const;

    void Draw() const;
};

#endif
