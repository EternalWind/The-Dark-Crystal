#ifndef STATE_CHANGE_TEST_H
#define STATE_CHANGE_TEST_H
#pragma once

#include <Config.hpp>
#include <Core/Root.hpp>
#include <Scene/Game.hpp>
#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiManager.hpp>
#include <Gui/GuiRootWindow.hpp>
#include <Graphics/TextComponent.hpp>
#include <OgreFontManager.h>
#include <memory>

// first test state
class FirstTestState : public dt::State {
    Q_OBJECT
public:
    void onInitialize();

    void onDeinitialize();

    void onClick(MyGUI::Widget* sender);

    void updateStateFrame(double simulation_frame_time);

private:
    std::shared_ptr<dt::GuiButton> mNextButton;
};

// second test state
//class SecondTestState : public dt::State {
//    Q_OBJECT
//public:
//    void onInitialize();
//
//    void onDeinitialize();
//
//    void onClick(MyGUI::Widget* sender);
//
//    void updateStateFrame(double simulation_frame_time);
//
//private:
//    std::shared_ptr<dt::GuiButton> mReturnButton;
//};

#endif