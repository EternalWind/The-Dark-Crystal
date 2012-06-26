#include "GuiImageButton.h"

#include <Utils/Utils.hpp>
#include <iostream>

GuiImageButton::GuiImageButton(const QString name, const QString texture_normal, const QString texture_hover, const QString ntexture_pressed) 
    : GuiWidget(name),
      mImageBox(nullptr),
      mTextureNormal(texture_normal),
      mTextureHover(texture_hover),
      mTexturePressed(ntexture_pressed) {}

void GuiImageButton::onInitialize() {
    mImageBox = getParent()->getMyGUIWidget()->createWidget<MyGUI::ImageBox>("ImageBox", 0, 0, 100, 100, MyGUI::Align::Default, dt::Utils::toStdString(getFullName()));
    mImageBox->setImageTexture(dt::Utils::toStdString(mTextureNormal));
    dynamic_cast<MyGUI::ImageBox*>(getMyGUIWidget())->eventMouseButtonClick += MyGUI::newDelegate(this, &GuiImageButton::onClick);
    dynamic_cast<MyGUI::ImageBox*>(getMyGUIWidget())->eventMouseSetFocus += MyGUI::newDelegate(this, &GuiImageButton::onMouseSetFocus);
    dynamic_cast<MyGUI::ImageBox*>(getMyGUIWidget())->eventMouseLostFocus += MyGUI::newDelegate(this, &GuiImageButton::onMouseLostFocus);
}

MyGUI::Widget* GuiImageButton::getMyGUIWidget() {
    return mImageBox;
}


void GuiImageButton::onClick(MyGUI::Widget* sender) {
    mImageBox->setImageTexture(dt::Utils::toStdString(mTexturePressed));
}


void GuiImageButton::onMouseSetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new) {
    mImageBox->setImageTexture(dt::Utils::toStdString(mTextureHover));
}


void GuiImageButton::onMouseLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new) {
    mImageBox->setImageTexture(dt::Utils::toStdString(mTextureNormal));
}