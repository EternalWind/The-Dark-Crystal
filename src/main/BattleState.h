#ifndef THE_DARK_CRYSTAL_BATTLE_STATE
#define THE_DARK_CRYSTAL_BATTLE_STATE

#include "Weapon.h"
#include "GuiImageButton.h"

#include <Input/InputManager.hpp>
#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiLabel.hpp>
#include <Gui/GuiEditBox.hpp>
#include <Gui/GuiImageBox.hpp>
#include <Gui/GuiProgressBar.hpp>

#include <QString>

#include <cstdint>
#include <vector>

/**
  * 游戏的战斗场景
  * 默认第一人称视角
  * 显示血量，武器，弹夹数
  */
class BattleState : public dt::State {
    Q_OBJECT
public:
    BattleState(const QString stage_name);

    void onInitialize();

    void onDeinitialize();
	
    void updateStateFrame(double simulation_frame_time);

	/**
	  * 初始化
	  * @param tot_enemy_num 地图上所有的怪兽数量
	  * @param tot_crystal_num 地图上所有的水晶数量
	  */
	BattleState(uint16_t tot_enemy_num = 0, uint16_t tot_crystal_num = 0);

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
	  * @param 设置对话窗口
	  */
	void setDialogLabel(dt::GuiLabel* dialog_label);

	/**
	  * 返回问答窗口
	  * @returns 问答窗口
	  */
	dt::GuiEditBox* getQuestionLabel();

	/**
	  * 设置问答窗口
	  * @param label 问答窗口
	  */
	void setQuestionLabel(dt::GuiEditBox* edit_box);

signals:
    void sVictory();

public slots:
	/**
	  * 玩家取胜的情况下，下一步动作
	  */
	void win();

    /**
      * 失败函数
      */
    void fail();

    /**
      * 设置当前关卡。
      * @param stage 要设置的关卡
      */
    void setStage(const QString stage);

    /**
      * 获取当前关卡。
      * @returns 当前关卡
      */
    QString getStage() const;

    /**
      * 获取下一关卡。
      * @returns 下一关卡
      */
    QString getNextStage() const;

    /**
      * 设置下一关卡。
      * @param next_stage 要设置的下一关卡
      */
    void setNextStage(const QString next_stage);

    /**
	  * 返回总敌人数量
	  * @returns 总敌人数量
	  */
	int getTotalEnemyNum() const;

	/**
	  * 设置总敌人数量
	  * @param total_enemy_num
	  */
	void setTotalEnemyNum(int total_enemy_num);

	/**
	  * 返回剩下的敌人数量
	  * @returns 剩下的敌人数量
	  */
	int getRemainEnemyNum() const;

	/*
	 * 设置剩下敌人数量
	 * @param remain_enemy_num 剩余敌人数量
	 */
	void setRemainEnemyNum(int remain_enemy_num);

	/**
	  * 返回水晶总数
	  * @returns 水晶总数
	  */
	int getTotalCrystalNum() const;

	/**
	  * 设置总水晶数量
	  * @param total_crystal_num 水晶总数量
	  */
	void setTotalCrystalNum(int total_crystal_num);

	/** 
	  * 返回已获得水晶数量
	  * @returns 已获得水晶数量
	  */
	int getObtainedCrystalNum() const;

	/**
	  * 设置已获得水晶数量
	  * @param obtained_crystal_num 已获得水晶数量
	  */
	void setObtainedCrystalNum(int obtained_crystal_num);

    /**
      * 设置场景参数1
      * @param param1 参数1
      */
    void setSceneParam1(double param1);

    /**
      * 设置场景参数2
      * @param param2 参数2
      */
    void setSceneParam2(double param2);

private:
    /**
      * 重置Gui元素的位置、大小
      */
    void __resetGui();

    void __onClick(MyGUI::Widget* sender);

    /**
      * 暂停玩家控制并呼出菜单
      */
    void __showMenu();

    /**
      * 恢复控制并隐藏菜单
      */
    void __hideMenu();

protected slots:

	void __onTriggerText(uint16_t text_id);

	void __onHealthChanged(uint16_t cur_health);

	void __onAmmoChanged(uint16_t cur_ammo);

	void __onClipNumChanged(uint16_t cur_num);

	void __onGetCrystal(); 

	void __onTriggerQA();

	void __onAnswerButtonClick(std::shared_ptr<MyGUI::Widget> sender);

    void __changeDigits(std::vector<dt::GuiImageBox*>& pics, uint16_t number);

	void __onAmmoClipChange(uint16_t cur_ammo, uint16_t cur_clip);

    void __onKeyPressed(dt::InputManager::InputCode code, const OIS::EventArg& event);

protected:
	std::vector<dt::GuiImageBox*> mHealthHUD;	//!< 生命值
	std::vector<dt::GuiImageBox*> mAmmoHUD;		//!< 弹药量 
    dt::GuiImageBox* mFrontSight;	            //!< 准星
	std::vector<dt::GuiImageBox*> mClipNumHUD;	//!< 弹夹数
	std::vector<dt::GuiButton*> mAnswerButtons;	//!< 答案选项
	dt::GuiEditBox* mQuestionLabel;				//!< 问题显示窗口
	dt::GuiLabel* mDialogLabel;					//!< 对话显示窗口
	dt::GuiProgressBar* mPickUpCrystalBar;      //!< 捡起水晶进度条
	uint16_t mTotalEnemyNum;					//!< 地图中怪兽数量
	uint16_t mRemainEnemyNum;					//!< 地图中剩余怪兽数量
	uint16_t mTotalCrystalNum;					//!< 地图中水晶总数
	uint16_t mObtainedCrystalNum;				//!< 玩家已获得水晶总数
    QString mStage;                             //!< 当前关卡
    QString mNextStage;                         //!< 下一关卡
    double mSceneParam1;
    double mSceneParam2;

private:
	double mCrystalBarPosition;                 //!< 进度条position
    dt::GuiButton* mResumeButton;               //!< 返回游戏按钮
    dt::GuiButton* mSaveButton;                 //!< 保存游戏按钮
    dt::GuiButton* mLoadButton;                 //!< 读取游戏按钮
    dt::GuiButton* mReturnMenuButton;           //!< 返回主菜单按钮
    dt::GuiButton* mExitButton;                 //!< 退出游戏按钮
    bool mHasPaused;                            //!< 是否已经暂停玩家控制
};

#endif