#include <cvrMenu/MenuCheckbox.h>
#include <cvrUtil/WebConnection.h>
#include "stringprintf.hpp"

using namespace cvr;

MenuCheckbox::MenuCheckbox(std::string text, bool value) :
        MenuItem()
{
    _text = text;
    _value = value;
    WebSingletons::menu()->send(stringprintf("[\"MenuCheckbox\", \"%p\", \"%s\", %d]\n", this, text.c_str(), value));
}

MenuCheckbox::~MenuCheckbox()
{
}

bool MenuCheckbox::getValue()
{
    return _value;
}

void MenuCheckbox::setValue(bool value)
{
    _value = value;
    setDirty(true);
}

std::string MenuCheckbox::getText()
{
    return _text;
}

void MenuCheckbox::setText(std::string text)
{
    _text = text;
    setDirty(true);
}
