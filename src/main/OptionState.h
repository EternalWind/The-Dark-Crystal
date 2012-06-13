#ifndef THE_DARK_CRYSTAL_OPTION_STATE
#define THE_DARK_CRYSTAL_OPTION_STATE

#include "ControlSetting.h"

#include <Scene/State.hpp>
#include <Gui/GuiLabel.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiImageBox.hpp>
#include <Gui/GuiCheckBox.hpp>
#include <Gui/GuiScrollBar.hpp>

/**
  * ��Ϸ���ò˵�
  */
class OptionState : public dt::State {
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

	/**
	  * �������¼���Ӧ����
	  * @params sender ��������¼��İ�ť
	  * @params position ��������ǰλ��
	  */
	void onScrollChangePosition(MyGUI::ScrollBar* sender, size_t position);

private:
	dt::GuiCheckBox* mQASettingCheckBox;		//!< �ʴ�ϵͳ����
	dt::GuiScrollBar* mDisplaySettingsScrollBar;//!< �ֱ������ù�����
	dt::GuiScrollBar* mSoundVolumeScrollBar;	//!< ��Ч��С������
	dt::GuiScrollBar* mMusicVolumeScrollBar;	//!< �������ִ�С������
	dt::GuiLabel* mDisplaySettingsLabel;		//!< ��ʾ�ֱ��ʴ�С
	dt::GuiLabel* mSoundVolumeLabel;			//!< ��ʾ��Ч��С
	dt::GuiLabel* mMusicVolumeLabel;			//!< ��ʾ�������ִ�С
	dt::GuiLabel* mMessageLabel;				//!< ��ʾ��Ϣ
	ControlSetting::KeyFunction mActionToChange;//!< ��¼��ǰҪ���õĹ���
}

#endif