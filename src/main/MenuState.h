#ifndef THE_DARK_CRYSTAL_MENU_STATE
#define THE_DARK_CRYSTAL_MENU_STATE

#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiImageBox.hpp>

/**
  * 游戏主菜单
  */
class MenuState : public dt::State {
    Q_OBJECT
public:
	/**
	  * 场景初始化函数
	  */
    void onInitialize();

	/**
	  * 按钮事件响应函数
	  * @params sender 触发点击事件的按钮
	  */
	void onClick(MyGUI::Widget* sender);

private:
	std::tr1::shared_ptr<dt::GuiButton> mSinglePlayerButton;	//!< 单人模式按钮
	std::tr1::shared_ptr<dt::GuiButton> mMultiPlayerButton;		//!< 多人模式按钮
	std::tr1::shared_ptr<dt::GuiButton> mSettingsButton;		//!< 设置按钮
	std::tr1::shared_ptr<dt::GuiButton> mNewGameButton;			//!< 单人模式开始新游戏
	std::tr1::shared_ptr<dt::GuiButton> mLoadRecordButton;		//!< 单人模式读取存档
	std::tr1::shared_ptr<dt::GuiButton> mExitButton;			//!< 退出按钮
	
}

#endif