#ifndef THE_DARK_CRYSTAL_OPTION_STATE
#define THE_DARK_CRYSTAL_OPTION_STATE

#include "ControlSetting.h"
#include <QString>
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
    OptionState();
	/**
	  * ������ʼ������
	  */
    void onInitialize();

	/**
	  * ��ť�¼���Ӧ����
	  * @param sender ��������¼��İ�ť
	  */
	void onClick(MyGUI::Widget* sender);

    /**
      * ���ư�ť�¼���Ӧ����
      * @param sender ��������¼��İ�ť
      */
	void onSettingsButtonClick(MyGUI::Widget* sender);

	/**
	  * �������¼���Ӧ����
	  * @param sender ��������¼��İ�ť
	  * @param position ��������ǰλ��
	  */
	void onScrollChangePosition(MyGUI::ScrollBar* sender, size_t position);

    void updateStateFrame(double simulation_frame_time);

public slots:
    /**
	  * �����¼���Ӧ����
	  * @param code ���̵İ���
	  * @param event �����¼�
	  */
    void onKeyDown(dt::InputManager::InputCode code, const OIS::EventArg& event);

private:
    dt::GuiCheckBox* mQASettingCheckBox;        //!< �ʴ�ϵͳ����
    dt::GuiCheckBox* mDisplaySettingsCheckBox;  //!< �ֱ�������
    dt::GuiScrollBar* mSoundVolumeScrollBar;    //!< ��Ч��С������
    dt::GuiScrollBar* mMusicVolumeScrollBar;    //!< �������ִ�С������
    dt::GuiLabel* mDisplaySettingsLabel;        //!< ��ʾ�Ƿ���ȫ��
    dt::GuiLabel* mSoundVolumeLabel;            //!< ��ʾ��Ч��С
    dt::GuiLabel* mMusicVolumeLabel;            //!< ��ʾ�������ִ�С
    dt::GuiLabel* mMessageLabel;                //!< ��ʾ��Ϣj
    MyGUI::Widget* mActionButton;               //!< ��¼�������ð���
    ControlSetting::KeyFunction mActionToChange;//!< ��¼��ǰҪ���õĹ���
    
};

#endif