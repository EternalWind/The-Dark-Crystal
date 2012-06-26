#ifndef THE_DARK_CRYSTAL_GUI_IMAGE_BUTTON
#define THE_DARK_CRYSTAL_GUI_IMAGE_BUTTON

#include <Gui/GuiWidget.hpp>
#include <Gui/GuiImageBox.hpp>

/**
  * ͼƬ��ť��
  */
class GuiImageButton : public dt::GuiWidget {

    Q_OBJECT

public:

	GuiImageButton(const QString name, const QString texture_normal, const QString texture_hover, const QString ntexture_pressed) ;

    void onInitialize();

    MyGUI::Widget* getMyGUIWidget();

	/**
	  * ��ť�¼���Ӧ����
	  * @param sender ��������¼��İ�ť
	  */
	void onClick(MyGUI::Widget* sender);

    /**
	  * ���������Ӧ����
	  * @param _sender �ؼ�
      * @param _new 
	  */
    void onMouseSetFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new);

    /**
	  * ����ƿ���Ӧ����
	  * @param _sender �ؼ�
      * @param _new 
	  */
    void onMouseLostFocus(MyGUI::Widget* _sender, MyGUI::Widget* _new);

private:
    MyGUI::ImageBox* mImageBox;                    //!< ͼƬ�ؼ�                     
    QString mTextureNormal;                        //!< ����ƽ��ͼƬ
    QString mTextureHover;                         //!< �������ȥͼƬ
    QString mTexturePressed;                       //!< �����ͼƬ

};

#endif