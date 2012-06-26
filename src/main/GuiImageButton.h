#ifndef THE_DARK_CRYSTAL_GUI_IMAGE_BUTTON
#define THE_DARK_CRYSTAL_GUI_IMAGE_BUTTON

#include <Gui/GuiWidget.hpp>
#include <Gui/GuiImageBox.hpp>

/**
  * 图片按钮类
  */
class GuiImageButton : public dt::GuiWidget {

    Q_OBJECT

public:

	GuiImageButton(const QString name, const QString texture_normal, const QString texture_hover, const QString ntexture_pressed) ;

    void onInitialize();

    MyGUI::Widget* getMyGUIWidget();

	/**
	  * 按钮事件响应函数
	  * @param sender 触发点击事件的按钮
	  */
	void onClick(MyGUI::Widget* sender);

    /**
	  * 鼠标移上响应函数
	  * @param _sender 控件
      * @param _new 
	  */
    void onMouseSetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new);

    /**
	  * 鼠标移开响应函数
	  * @param _sender 控件
      * @param _new 
	  */
    void onMouseLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new);

private:
    MyGUI::ImageBox* mImageBox;                    //!< 图片控件                     
    QString mTextureNormal;                        //!< 按键平常图片
    QString mTextureHover;                         //!< 鼠标移上去图片
    QString mTexturePressed;                       //!< 鼠标点击图片

};

#endif