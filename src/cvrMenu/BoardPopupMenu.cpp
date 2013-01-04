#include <cvrMenu/BoardPopupMenu.h>
#include <cvrKernel/SceneManager.h>
#include <cvrInput/TrackingManager.h>
#include <cvrUtil/OsgMath.h>

#include <iostream>

using namespace cvr;

BoardPopupMenu::BoardPopupMenu() :
        BoardMenu()
{
}

BoardPopupMenu::~BoardPopupMenu()
{
}

void BoardPopupMenu::setMenu(SubMenu * menu)
{
    BoardMenu::setMenu(menu);
}

void BoardPopupMenu::updateStart()
{
    BoardMenu::updateStart();
}

bool BoardPopupMenu::processIsect(IsectInfo & isect, int hand)
{
    return BoardMenu::processIsect(isect,hand);
}

void BoardPopupMenu::updateEnd()
{
    BoardMenu::updateEnd();
}

bool BoardPopupMenu::processEvent(InteractionEvent * event)
{
    if(!_menuActive || !_myMenu || !event->asTrackedButtonEvent())
    {
        return false;
    }

    TrackedButtonInteractionEvent * tie = event->asTrackedButtonEvent();

    if(_clickActive)
    {
        if(tie->getHand() == _activeHand)
        {
            if(tie->getInteraction() == BUTTON_DRAG
                    || tie->getInteraction() == BUTTON_UP)
            {
                if(tie->getButton() == _primaryButton)
                {
                    BoardMenuSubMenuGeometry * smg =
                            dynamic_cast<BoardMenuSubMenuGeometry *>(_activeItem);
                    if(smg && smg->isMenuHead())
                    {
                        updateMovement(tie);
                    }

                    _activeItem->processEvent(event);
                    if(tie->getInteraction() == BUTTON_UP)
                    {
                        _clickActive = false;
                    }
                    return true;
                }
            }
        }
        return false;
    }
    else if(tie->getHand() == _activeHand && tie->getButton() == _primaryButton)
    {
        if(tie->getInteraction() == BUTTON_DOWN
                || tie->getInteraction() == BUTTON_DOUBLE_CLICK)
        {
            if(_activeItem)
            {
                BoardMenuSubMenuGeometry * smg =
                        dynamic_cast<BoardMenuSubMenuGeometry *>(_activeItem);
                if(smg && smg->isMenuHead())
                {
                    osg::Vec3 ray;
                    ray = _currentPoint[tie->getHand()]
                            - tie->getTransform().getTrans();

		    if(!tie->asPointerEvent())
		    {
			_moveDistance = ray.length();
		    }
		    else
		    {
			_moveDistance = ray.y();
		    }
                    _menuPoint = _currentPoint[tie->getHand()]
                            * osg::Matrix::inverse(_menuRoot->getMatrix());
                    updateMovement(tie);
                }
                else if(smg && !smg->isMenuHead())
                {
                    if(smg->isMenuOpen())
                    {
                        closeMenu((SubMenu*)smg->getMenuItem());
                    }
                    else
                    {
                        openMenu(smg);
                    }
                }
		_clickActive = true;
                _activeItem->processEvent(event);
                return true;
            }

            return false;
        }
    }

    return false;
}

void BoardPopupMenu::itemDelete(MenuItem * item)
{
    BoardMenu::itemDelete(item);
}

void BoardPopupMenu::clear()
{
    BoardMenu::clear();
}

void BoardPopupMenu::close()
{
    setVisible(false);
}

void BoardPopupMenu::setScale(float scale)
{
    BoardMenu::setScale(scale);
}

float BoardPopupMenu::getScale()
{
    return BoardMenu::getScale();
}

void BoardPopupMenu::setPosition(osg::Vec3 pos)
{
    osg::Matrix m = _menuRoot->getMatrix();
    m.setTrans(pos);
    _menuRoot->setMatrix(m);
}

osg::Vec3 BoardPopupMenu::getPosition()
{
    return _menuRoot->getMatrix().getTrans();
}

void BoardPopupMenu::setRotation(osg::Quat rot)
{
    osg::Matrix m = _menuRoot->getMatrix();
    m.setRotate(rot);
    _menuRoot->setMatrix(m);
}

osg::Quat BoardPopupMenu::getRotation()
{
    return _menuRoot->getMatrix().getRotate();
}

void BoardPopupMenu::setTransform(osg::Matrix m)
{
    _menuRoot->setMatrix(m);
}

osg::Matrix BoardPopupMenu::getTransform()
{
    return _menuRoot->getMatrix();
}

void BoardPopupMenu::setVisible(bool v)
{
    if(v == _menuActive)
    {
        return;
    }

    if(v)
    {
        SceneManager::instance()->getMenuRoot()->addChild(_menuRoot);
    }
    else
    {
	BoardMenu::close();
	_clickActive = false;
    }

    _menuActive = v;
}

bool BoardPopupMenu::isVisible()
{
    return _menuActive;
}
