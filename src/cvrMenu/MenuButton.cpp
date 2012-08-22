#include <cvrMenu/MenuButton.h>
#include <cvrUtil/WebConnection.h>
#include "stringprintf.hpp"

using namespace cvr;

MenuButton::MenuButton(std::string text) :
        MenuItem()
{
    _text = text;
    WebSingletons::menu()->send(stringprintf("[\"MenuButton\", \"%p\", \"%s\"]\n", this, text.c_str()));
}

MenuButton::~MenuButton()
{
}

std::string & MenuButton::getText()
{
    return _text;
}

void MenuButton::setText(std::string text)
{
    _text = text;
    setDirty(true);
}

MenuItemType MenuButton::getType()
{
    return BUTTON;
}
