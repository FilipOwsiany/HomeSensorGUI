#include "CSegfaultHandler.h"
#include "CGuiApp.h"


int main()
{
    CSegfaultHandler::install();

    CGuiApp app;
    return app.run();
}

