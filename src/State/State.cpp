/*
 * Copyright 2012-2015 Falltergeist Developers.
 *
 * This file is part of Falltergeist.
 *
 * Falltergeist is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Falltergeist is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Falltergeist.  If not, see <http://www.gnu.org/licenses/>.
 */

// Related headers
#include "../State/State.h"

// C++ standard includes

// Falltergeist includes
#include "../Event/State.h"
#include "../Game/Game.h"
#include "../Graphics/ActiveUI.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/UI.h"
#include "../UI/ImageList.h"
#include "../UI/SmallCounter.h"
#include "../UI/TextArea.h"

// Third party includes

namespace Falltergeist
{
namespace State
{

State::State() : Event::Emitter()
{
    addEventHandler("activate",   [this](Event::Event* event){ this->onStateActivate(dynamic_cast<Event::State*>(event)); });
    addEventHandler("deactivate", [this](Event::Event* event){ this->onStateDeactivate(dynamic_cast<Event::State*>(event)); });
}

State::~State()
{
    for (auto ui : _ui)
    {
        delete ui;
    }
}

void State::init()
{
    _initialized = true;
}

void State::think()
{
    for (auto ui : _ui)
    {
        ui->think();
    }
}

int State::x() const
{
    return _x;
}

void State::setX(int x)
{
    _x = x;
}

int State::y() const
{
    return _y;
}

void State::setY(int y)
{
    _y = y;
}

bool State::initialized()
{
    return _initialized;
}

bool State::fullscreen() const
{
    return _fullscreen;
}

void State::setFullscreen(bool value)
{
    _fullscreen = value;
}

bool State::modal() const
{
    return _modal;
}

void State::setModal(bool value)
{
    _modal = value;
}

ActiveUI* State::addUI(ActiveUI* ui)
{
    addUI((UI*)ui);
    return ui;
}

UI* State::addUI(UI* ui)
{
    // Add to UI state position
    if (x()) ui->setX(ui->x() + x());
    if (y()) ui->setY(ui->y() + y());

    _ui.push_back(ui);
    return ui;
}

UI* State::addUI(const std::string& name, UI* ui)
{
    addUI(ui);
    _labeledUI.insert(std::pair<std::string, UI*>(name, ui));
    return ui;
}

ActiveUI* State::addUI(const std::string& name, ActiveUI* ui)
{
    addUI(name, (UI*)ui);
    return ui;
}


void State::addUI(std::vector<UI*> uis)
{
    for (auto ui : uis)
    {
        addUI(ui);
    }
}

ActiveUI* State::getActiveUI(const std::string& name)
{
    return dynamic_cast<ActiveUI*>(getUI(name));
}

TextArea* State::getTextArea(const std::string& name)
{
    return dynamic_cast<TextArea*>(getUI(name));
}

ImageList* State::getImageList(const std::string& name)
{
    return dynamic_cast<ImageList*>(getUI(name));
}

SmallCounter* State::getSmallCounter(const std::string& name)
{
    return dynamic_cast<SmallCounter*>(getUI(name));
}

UI* State::getUI(const std::string& name)
{
    if (_labeledUI.find(name) != _labeledUI.end())
    {
        return _labeledUI.at(name);
    }
    return nullptr;
}

void State::handle(Event::Event* event)
{
    if (event->handled()) return;
    if (auto keyboardEvent = dynamic_cast<Event::Keyboard*>(event))
    {
        if (keyboardEvent->name() == "keydown")
        {
            onKeyDown(keyboardEvent);
        }
    }
    for (auto it = _ui.rbegin(); it != _ui.rend(); ++it)
    {
        if (event->handled()) return;
        if (auto activeUI = dynamic_cast<ActiveUI*>(*it))
        {
            activeUI->handle(event);
        }
    }
}

void State::render()
{
    for (auto it = _ui.begin(); it != _ui.end(); ++it)
    {
        if ((*it)->visible())
        {
            (*it)->render(false);
        }
    }
    while (!_uiToDelete.empty())
    {
        delete _uiToDelete.back();
        _uiToDelete.pop_back();
    }
}

void State::popUI()
{
    if (_ui.size() == 0) return;
    _uiToDelete.push_back(_ui.back());
    _ui.pop_back();
}

void State::onStateActivate(Event::State* event)
{
}

void State::onStateDeactivate(Event::State* event)
{
}

void State::onKeyDown(Event::Keyboard* event)
{
}

bool State::active()
{
    return _active;
}

void State::setActive(bool value)
{
    _active = value;
}

}
}
