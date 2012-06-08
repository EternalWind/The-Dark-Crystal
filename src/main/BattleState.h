#ifndef THE_DARK_CRYSTAL_BATTLE_STATE
#define THE_DARK_CRYSTAL_BATTLE_STATE

#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiLabel.hpp>
#include <Gui/GuiImageBox.hpp>
#include <cstdint>
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
	  * 初始化
	  * @params tot_enemy_num 地图上所有的怪兽数量
	  * @params tot_crystal_num 地图上所有的水晶数量
	  */
	BattleState(uint16_t tot_enemy_num = 0, uint16_t tot_crystal_num = 0);

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
	QString getBattleStateName() const;

	/**
	  * 返回对话窗口
	  * @returns 对话窗口
	  */
	dt::GuiLabel* getDialogLabel();

	/**
	  * 设置对话窗口
	  * @params 设置对话窗口
	  */
	void setDialogLabel(dt::GuiLabel* dialog_label);

	/**
	  * 返回总敌人数量
	  * @returns 总敌人数量
	  */
	uint16_t getTotalEnemyNum() const;

	/**
	  * 设置总敌人数量
	  * @params total_enemy_num
	  */
	void setTotalEnemyNum(uint16_t total_enemy_num);

	/**
	  * 返回剩下的敌人数量
	  * @returns 剩下的敌人数量
	  */
	uint16_t getRemainEnemyNum() const;

	/*
	 * 设置剩下敌人数量
	 * @params remain_enemy_num 剩余敌人数量
	 */
	void setRemainEnemyNum(uint16_t remain_enemy_num);

	/**
	  * 返回水晶总数
	  * @returns 水晶总数
	  */
	uint16_t getTotalCrystalNum() const;

	/**
	  * 设置总水晶数量
	  * @params total_crystal_num 水晶总数量
	  */
	void setTotalCrystalNum(uint16_t total_crystal_num);

	/** 
	  * 返回已获得水晶数量
	  * @returns 已获得水晶数量
	  */
	uint16_t getObtainedCrystalNum() const;

	/**
	  * 设置已获得水晶数量
	  * @params obtained_crystal_num 已获得水晶数量
	  */
	void setObtainedCrystalNum(uint16_t obtained_crystal_num);

	void onclick(MyGUI::Widget* _sender);

protected slots:

	void __onTriggerText(uint16_t text_id);

	void __onHealthChanged(uint16_t pre_health, uint16_t cur_health);

	void __onAmmoChanged(uint16_t pre_ammo, uint16_t cur_ammo);

	void __onClipNumChanged(uint16_t pre_num, uint16_t cur_num);

	void __onGetCrystal(); 

	void __onTriggerQA();

	void __onAnswerButtonClick(std::shared_ptr<MyGUI::Widget> sender);


private:
	std::vector<dt::GuiImageBox*> mHealthHUD;	//!< 生命值
	std::vector<dt::GuiImageBox*> mAmmoHUD;		//!< 弹药量 
	std::vector<dt::GuiImageBox*> mFrontSight;	//!< 准星
	std::vector<dt::GuiImageBox*> mClipNumHUD;	//!< 弹夹数
	std::vector<dt::GuiButton> mAnswerButtons;	//!< 答案选项
	dt::GuiLabel* mQuestionLabel;				//!< 问题显示窗口
	dt::GuiLabel* mDialogLabel;					//!< 对话显示窗口
	uint16_t mTotalEnemyNum;					//!< 地图中怪兽数量
	uint16_t mRemainEnemyNum;					//!< 地图中剩余怪兽数量
	uint16_t mTotalCrystalNum;					//!< 地图中水晶总数
	uint16_t mObtainedCrystalNum;				//!< 玩家已获得水晶总数
	

};

#endif
