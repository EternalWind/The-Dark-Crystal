#ifndef THE_DARK_CRYSTAL_OPTION_STATE
#define THE_DARK_CRYSTAL_OPTION_STATE

#include "ControlSetting.h"
#include "SoundSetting.h"
#include "ScreenSetting.h"
#include "QASetting.h"

#include <QString>
#include <Scene/State.hpp>
#include <Gui/GuiLabel.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiImageBox.hpp>
#include <Gui/GuiCheckBox.hpp>
#include <Gui/GuiScrollBar.hpp>
#include <Audio/SoundComponent.hpp>

/**
  * 游戏设置菜单
  */
class OptionState : public dt::State {
    Q_OBJECT
public:
    OptionState();
	/**
	  * 场景初始化函数
	  */
    void onInitialize();

	/**
	  * 按钮事件响应函数
	  * @param sender 触发点击事件的按钮
	  */
	void onClick(MyGUI::Widget* sender);

    /**
      * 控制按钮事件响应函数
      * @param sender 触发点击事件的按钮
      */
	void onSettingsButtonClick(MyGUI::Widget* sender);

	/**
	  * 滚动条事件响应函数
	  * @param sender 触发点击事件的按钮
	  * @param position 滚动条当前位置
	  */
	void onScrollChangePosition(MyGUI::ScrollBar* sender, size_t position);

    void updateStateFrame(double simulation_frame_time);

public slots:
    /**
	  * 键盘事件响应函数
	  * @param code 键盘的按键
	  * @param event 触发事件
	  */
    void onKeyDown(dt::InputManager::InputCode code, const OIS::EventArg& event);

private:

    /**
	  * 按键设置辅助函数
	  */
    void addNewFuncButton(const QString name, const QString font_text, int x, int y);


private:
    dt::GuiCheckBox* mQASettingCheckBox;          //!< 问答系统开关
    dt::GuiCheckBox* mDisplaySettingCheckBox;     //!< 分辨率设置
    dt::GuiScrollBar* mSoundVolumeScrollBar;      //!< 音效大小滚动条
    dt::GuiScrollBar* mMasterVolumeScrollBar;     //!< 主音量大小滚动条
    dt::GuiScrollBar* mMusicVolumeScrollBar;      //!< 背景音乐大小滚动条
    dt::GuiScrollBar* mMouseSensitivityScrollBar; //!< 鼠标灵敏度滚动条
    dt::GuiLabel* mDisplaySettingsLabel;          //!< 显示是否开启全屏
    dt::GuiLabel* mMasterVolumeLabel;             //!< 显示主音量大小
    dt::GuiLabel* mSoundVolumeLabel;              //!< 显示音效大小
    dt::GuiLabel* mMusicVolumeLabel;              //!< 显示背景音乐大小
    dt::GuiLabel* mMessageLabel;                  //!< 显示消息
    dt::GuiLabel* mMouseSensitivityLabel;         //!< 显示鼠标灵敏度
    MyGUI::Widget* mActionButton;                 //!< 记录功能设置按键
    ControlSetting::KeyFunction mActionToChange;  //!< 记录当前要设置的功能
    ControlSetting mControlSettings;              //!< 记录当前的按键设定
    SoundSetting mSoundSettings;                  //!< 记录当前的音量设定
    ScreenSetting mScreenSettings;                //!< 记录当前的显示设定
    QASetting mQASettings;                        //!< 记录当前的QA设定
	dt::SoundComponent* mButtonClickSound;        //!< 按键音效
};

#endif