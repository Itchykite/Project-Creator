#include "window.h"
#include "ui.h"
#include "settings.h"

int main()
{
    init_templates();

    WindowContext wc = initWindow();
    init_ui(wc.window, wc.renderer, wc.font_scale);

    whileLoop(wc.window, wc.renderer);

    free_templates();

    return 0;
}
