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
	  * @param sender 触发点击事件的按钮
	  */
	void onClick(MyGUI::Widget* sender);

    void updateStateFrame(double simulation_frame_time);
	
};


#endif