#include "Dialogs/OKCancelDialog.h"
#include "Widgets/BitmapWidget.h"
#include "resources/GreenTick_64x64_bmp.h"
#include "resources/RedCross_64x64_bmp.h"

OKCancelDialog::OKCancelDialog(GraphicsContext *context) : Window(context)
{

    setLayout(FIXED);
    setSize(
        context->display()->width(),
        context->display()->height()
    );


    BitmapWidget *apply = new BitmapWidget(context);
    apply->setBitmap(GreenTick_64x64_bmp, 64, 64);
    apply->setLocation(
        0,
        context->display()->height() - apply->height()
    );
    attach(apply);

    BitmapWidget *cancel = new BitmapWidget(context);
    cancel->setBitmap(RedCross_64x64_bmp, 64, 64);
    cancel->setLocation(
        context->display()->width() - cancel->width(),
        context->display()->height() - cancel->height()
    );
    attach(cancel);
}


void OKCancelDialog::_handleOK(Event e) {
    _onOK.call(e);
}

void OKCancelDialog::_handleCancel(Event e) {
    _onCancel.call(e);
}