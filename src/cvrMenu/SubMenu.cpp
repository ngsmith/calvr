#include <cvrMenu/SubMenu.h>
#include <cvrMenu/MenuSystem.h>
#include <cvrUtil/WebConnection.h>
#include "stringprintf.hpp"

using namespace cvr;

SubMenu::SubMenu(std::string name, std::string title) :
        MenuItem()
{
    _name = name;
    if(title.empty())
    {
        WebSingletons::menu()->send(stringprintf("[\"SubMenu\", \"%p\", \"%s\", \"%s\"]\n", this, name.c_str(), name.c_str()));
        _title = name;
    }
    else
    {
        WebSingletons::menu()->send(stringprintf("[\"SubMenu\", \"%p\", \"%s\", \"%s\"]\n", this, name.c_str(), title.c_str()));
        _title = title;
    }
}

SubMenu::~SubMenu()
{
}

void SubMenu::addItem(MenuItem * item)
{
    if(!item)
    {
        return;
    }

    for(int i = 0; i < _children.size(); i++)
    {
        if(_children[i] == item)
        {
            return;
        }
    }
    WebSingletons::menu()->send(stringprintf("[\"addItem\", %d, \"%p\", \"%p\"]\n", -1, this, item));
    _children.push_back(item);
    _dirty = true;
}

void SubMenu::addItem(MenuItem * item, int position)
{
    if(!item)
    {
	return;
    }

    if(getItemPosition(item) >= 0)
    {
	return;
    }

    if(position < 0 || position >= _children.size())
    {
        WebSingletons::menu()->send(stringprintf("[\"addItem\", %d, \"%p\", \"%p\"]\n", -1, this, item));
	_children.push_back(item);
    }
    else
    {
	std::vector<MenuItem*>::iterator it = _children.begin();
	it += position;
        WebSingletons::menu()->send(stringprintf("[\"addItem\", %d, \"%p\", \"%p\"]\n", position, this, item));
	_children.insert(it,item);
    }

    _dirty = true;
}

void SubMenu::removeItem(MenuItem * item)
{
    for(std::vector<MenuItem*>::iterator it = _children.begin();
            it != _children.end(); it++)
    {
        if((*it) == item)
        {
             WebSingletons::menu()->send(stringprintf("[\"removeItem\", \"%p\", \"%p\"]\n", this, item));
            _children.erase(it);
            _dirty = true;
            return;
        }
    }
}

int SubMenu::getItemPosition(MenuItem * item)
{
    for(int i = 0; i < _children.size(); i++)
    {
	if(_children[i] == item)
	{
	    return i;
	}
    }
    return -1;
}

MenuItem * SubMenu::getChild(int i)
{
    if(i < 0 || i >= _children.size())
    {
        return NULL;
    }
    return _children[i];
}

std::vector<MenuItem*> & SubMenu::getChildren()
{
    return _children;
}

int SubMenu::getNumChildren()
{
    return _children.size();
}

std::string SubMenu::getName()
{
    return _name;
}

std::string SubMenu::getTitle()
{
    return _title;
}
