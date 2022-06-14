#include "SpinnerDialog.h"
SpinnerDialog::SpinnerDialog(GraphicsContext *context) : OKCancelDialog(context)
{

    // Now add a Spinner
    _spinner = new SpinnerWidget(context);
    _spinner->setLocation(0,0);
    attach(_spinner);
}

SpinnerWidget *SpinnerDialog::getSpinner()
{
    return _spinner;
}
