#ifndef SIMPLEGUI_OK_CANCEL_DIALOG_H
#define SIMPLEGUI_OK_CANCEL_DIALOG_H
#include "Widgets/Window.h"
/**
* A Dialog takes over the whole screen and offers some sort of control
* together with apply and cancel buttons
**/
class OKCancelDialog : public Window
{
public:

    OKCancelDialog(GraphicsContext *context);

    void onOK(void(* fn)(Event e)) {
        _onOK.attach(fn);
    }
    
    void onCancel(void(* fn)(Event e)) {
        _onCancel.attach(fn);
    }
    
    template<typename T>
    void onOK(T* tptr, void (T::*mptr)(Event e)) {
        _onOK.attach(tptr, mptr);
    }

    template<typename T>
    void onCancel(T* tptr, void (T::*mptr)(Event e)) {
        _onCancel.attach(tptr, mptr);
    }
    
private:

    FunctionPointerArg1<void,Event> _onOK;
    FunctionPointerArg1<void,Event> _onCancel;
    
    void _handleOK(Event e);
    void _handleCancel(Event e);


};

#endif