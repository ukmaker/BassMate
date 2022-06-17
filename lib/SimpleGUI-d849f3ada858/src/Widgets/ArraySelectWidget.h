#ifndef SIMPLEGUI_ARRAY_SELECT_WIDGET_H
#define SIMPLEGUI_ARRAY_SELECT_WIDGET_H

#include "Widgets/AbstractSelectWidget.h"

/**
* a select widget allows the user to choose one of a set of choices
* Choices are presented as scrolling text, with the selected choice centred and bolded
* Choices are mapped to a backing value
**/
template<typename V>
class ArraySelectWidget : public AbstractSelectWidget<V>
{

public:

    ArraySelectWidget(GraphicsContext *context, const GFXfont* font, const char** text, const V* values, const uint16_t numChoices) : 
        AbstractSelectWidget<V>(context),
        _text(text),
        _values(values),
        _numChoices(numChoices)
    {
        this->setFont(font);
    }


    virtual const char* getSelectedText() {
        if(!this->isSelected()) {
            return nullptr;
        }

        return _text[this->getSelectedIndex()];
    }

    virtual const V getSelectedValue() {
        if(!this->isSelected()) {
            return 0;
        }

        return _values[this->getSelectedIndex()];
    }
    virtual const char* getText(uint16_t idx) {
        return _text[idx];
    }

    virtual const V getValue(uint16_t idx) {
        return _values[idx];
    }

    virtual uint16_t getNumChoices() {
        return _numChoices;
    }

private:
    const char **_text;
    const V *_values;
    const uint16_t _numChoices;

};
#endif