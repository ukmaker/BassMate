#include "GUI.h"

GUI::GUI(GraphicsContext* context) : _context(context)
{
    _rootWindow = new Window(context);
}

GraphicsContext* GUI::getGraphicsContext() {
    return _context;
}

Window* GUI::rootWindow() {
    return _rootWindow;
}

void GUI::run() {
    _context->eventDispatcher()->pumpEvents();
    _rootWindow->draw();   
    
}
