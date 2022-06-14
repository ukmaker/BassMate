#ifndef SIMPLEGUI_GUI_H
#define SIMPLEGUI_GUI_H

class GUI;

#include "Core/GraphicsContext.h"
#include "Widgets/Window.h"

/**
* A singleton class to hold the framework components
**/
class GUI
{

public:

    GUI(GraphicsContext* context);

    GraphicsContext* getGraphicsContext();
    Window *rootWindow();
    
    void run();
    
private:

    GraphicsContext* _context;
    Window* _rootWindow;

};
#endif