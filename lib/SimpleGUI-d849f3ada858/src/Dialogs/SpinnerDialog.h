#ifndef SIMPLEGUI_SPINNER_DIALOG_H
#define SIMPLEGUI_SPINNER_DIALOG_H

#include "Dialogs/OKCancelDialog.h"
#include "Widgets/SpinnerWidget.h"
#include "Widgets/BitmapWidget.h"

class SpinnerDialog : public OKCancelDialog
{
public:

    SpinnerDialog(GraphicsContext *context);
    SpinnerWidget *getSpinner();

private:

    SpinnerWidget *_spinner;

};

#endif