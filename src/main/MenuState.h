#ifndef THE_DARK_CRYSTAL_MENU_STATE
#define THE_DARK_CRYSTAL_MENU_STATE

#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiImageBox.hpp>
#include <Audio/SoundComponent.hpp>
#include "GuiImageButton.h"

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
	  * @param sender ��������¼��İ�ť
	  */
	void onClick(MyGUI::Widget* sender);

    void updateStateFrame(double simulation_frame_time);


private:
    void __resetGui();

private:
    dt::GuiImageBox* mLogo;
    GuiImageButton*  mNewGameButton;
    GuiImageButton* mLoadRecordButton;
    GuiImageButton* mMultiPlayerButton;
    GuiImageButton* mSettingButton;
    GuiImageButton* mCreditButton;
    GuiImageButton* mExitButton;
	dt::SoundComponent* mButtonClickSound;

};

#endif