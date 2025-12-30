#include "window.h"
#include "ui.h"

int main() 
{
    WindowContext wc = initWindow();
    init_ui(wc.window, wc.renderer, wc.font_scale);

    whileLoop(wc.window, wc.renderer);

    return 0;
}
