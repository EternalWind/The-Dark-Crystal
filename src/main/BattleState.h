#ifndef THE_DARK_CRYSTAL_BATTLE_STATE
#define THE_DARK_CRYSTAL_BATTLE_STATE

#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiLabel.hpp>
#include <Gui/GuiImageBox.hpp>
#include <QString>
#include <vector>

/**
  * 游戏的战斗场景
  * 默认第一人称视角
  * 显示血量，武器，弹夹数
  */
class BattleState : public dt::State {
    Q_OBJECT
public:
    void onInitialize();
	
    void updateStateFrame(double simulation_frame_time);

	/**
	  * 判断玩家当前是否已经取得胜利
	  * @returns 胜利与否
	  */
	bool isVictory();

	/**
	  * 玩家取胜的情况下，下一步动作
	  */
	void win();

	/**
	  * 返回当前BattleState的名字
	  * @returns 当前BattleState的名字
	  */
	QString getBattleStateName();

	/**
	  * 返回对话窗口
	  * @returns 对话窗口
	  */

	/**
	  * 设置对话窗口
	  */

public slots:
	/**
	  * 触发对话事件对应的槽
	  * @params text_id 对话文本所对应的ID
	  */
	void onTriggerText(uint16_t text_id);

	/**
	  * 玩家生命值改变对应的槽
	  */
	void onHealthChanged();

	/**
	  * 弹药量改变对应的槽
	  */
	void onAmmoChanged();

	/**
	  * 弹夹数改变对应的槽
	  */
	void onClipNumChanged();

	/**
	  * 玩家获得水晶事件对应的槽
	  */
	void onGetCrystal(); 

	/**
	  * 玩家触发问答系统事件对应的槽
	  */
	void onTriggerQA();

	/**
	  * 问答模式下，玩家按下回答问题按键对应的槽
	  * @params sender 按键
	  */
	void onAnswerButtonClick(std::shared_ptr<MyGUI::Widget> sender);


private:
	std::vector<dt::GuiImageBox*> mHealthHUD;		//!< 生命值
	std::vector<dt::GuiImageBox*> mAmmoHUD;		//!< 弹药量 
	std::vector<std::shared_ptr<dt::GuiImageBox> > mFrontSight; //!< 准星
	std::vector<std::shared_ptr<int> > mClipNumHUD; //!< 弹夹数
	std::vector<std::shared_ptr<dt::GuiButton> > mAnswerButtons; //!< 答案选项
	std::shared_ptr<dt::GuiLabel> mQuestionLabel;	//!< 问题显示窗口
	std::shared_ptr<dt::GuiLabel> mDialogLabel;	//!< 对话显示窗口
	uint16_t mTotalEnemyNum;	//!< 地图中怪兽数量
	uint16_t mRemainEnemyNum;	//!< 地图中剩余怪兽数量
	uint16_t mTotalCrystalNum;	//!< 地图中水晶总数
	uint16_t mObtainedCrystalNum;	//!< 玩家已获得水晶总数
	

};

#endif
