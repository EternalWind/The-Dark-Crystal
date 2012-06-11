#ifndef THE_DARK_CRYSTAL_MENU_STATE
#define THE_DARK_CRYSTAL_MENU_STATE

#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiImageBox.hpp>

/**
  * ��Ϸ���˵�
  */
class MenuState : public dt::State {
    Q_OBJECT
public:
	/**
	  * ������ʼ������
	  */
    void onInitialize();

	/**
	  * ��ť�¼���Ӧ����
	  * @params sender ��������¼��İ�ť
	  */
	void onClick(MyGUI::Widget* sender);

private:
	std::tr1::shared_ptr<dt::GuiButton> mSinglePlayerButton;	//!< ����ģʽ��ť
	std::tr1::shared_ptr<dt::GuiButton> mMultiPlayerButton;		//!< ����ģʽ��ť
	std::tr1::shared_ptr<dt::GuiButton> mSettingsButton;		//!< ���ð�ť
	std::tr1::shared_ptr<dt::GuiButton> mNewGameButton;			//!< ����ģʽ��ʼ����Ϸ
	std::tr1::shared_ptr<dt::GuiButton> mLoadRecordButton;		//!< ����ģʽ��ȡ�浵
	std::tr1::shared_ptr<dt::GuiButton> mExitButton;			//!< �˳���ť
	
}

#endif