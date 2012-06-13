#ifndef THE_DARK_CRYSTAL_SAVELOAD_STATE
#define THE_DARK_CRYSTAL_SAVELOAD_STATE

#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiImageBox.hpp>

/**
  * 加载存档界面
  */
class SaveLoadState : public dt::State {
    Q_OBJECT
public:
	/**
	  * 初始化函数
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