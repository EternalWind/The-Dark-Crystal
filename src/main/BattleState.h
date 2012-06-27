#ifndef THE_DARK_CRYSTAL_BATTLE_STATE
#define THE_DARK_CRYSTAL_BATTLE_STATE

#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiLabel.hpp>
#include <Gui/GuiEditBox.hpp>
#include <Gui/GuiImageBox.hpp>
#include <cstdint>
#include <QString>
#include <vector>
#include "Weapon.h"
/**
  * ��Ϸ��ս������
  * Ĭ�ϵ�һ�˳��ӽ�
  * ��ʾѪ����������������
  */
class BattleState : public dt::State {
    Q_OBJECT
public:
    BattleState(const QString stage_name);

    void onInitialize();
	
    void updateStateFrame(double simulation_frame_time);

	/**
	  * ��ʼ��
	  * @param tot_enemy_num ��ͼ�����еĹ�������
	  * @param tot_crystal_num ��ͼ�����е�ˮ������
	  */
	BattleState(uint16_t tot_enemy_num = 0, uint16_t tot_crystal_num = 0);

	/**
	  * ���ص�ǰBattleState������
	  * @returns ��ǰBattleState������
	  */
	QString getBattleStateName() const;

	/**
	  * ���ضԻ�����
	  * @returns �Ի�����
	  */
	dt::GuiLabel* getDialogLabel();

	/**
	  * ���öԻ�����
	  * @param ���öԻ�����
	  */
	void setDialogLabel(dt::GuiLabel* dialog_label);

	/**
	  * �����ʴ𴰿�
	  * @returns �ʴ𴰿�
	  */
	dt::GuiEditBox* getQuestionLabel();

	/**
	  * �����ʴ𴰿�
	  * @param label �ʴ𴰿�
	  */
	void setQuestionLabel(dt::GuiEditBox* edit_box);

public slots:
    /**
	  * �ж���ҵ�ǰ�Ƿ��Ѿ�ȡ��ʤ��
	  * @returns ʤ�����
	  */
	//bool isVictory();

	/**
	  * ���ȡʤ������£���һ������
	  */
	void win();

    /**
      * ���õ�ǰ�ؿ���
      * @param stage Ҫ���õĹؿ�
      */
    void setStage(const QString stage);

    /**
      * ��ȡ��ǰ�ؿ���
      * @returns ��ǰ�ؿ�
      */
    QString getStage() const;

    /**
      * ��ȡ��һ�ؿ���
      * @returns ��һ�ؿ�
      */
    QString getNextStage() const;

    /**
      * ������һ�ؿ���
      * @param next_stage Ҫ���õ���һ�ؿ�
      */
    void setNextStage(const QString next_stage);

    /**
	  * �����ܵ�������
	  * @returns �ܵ�������
	  */
	int getTotalEnemyNum() const;

	/**
	  * �����ܵ�������
	  * @param total_enemy_num
	  */
	void setTotalEnemyNum(int total_enemy_num);

	/**
	  * ����ʣ�µĵ�������
	  * @returns ʣ�µĵ�������
	  */
	int getRemainEnemyNum() const;

	/*
	 * ����ʣ�µ�������
	 * @param remain_enemy_num ʣ���������
	 */
	void setRemainEnemyNum(int remain_enemy_num);

	/**
	  * ����ˮ������
	  * @returns ˮ������
	  */
	int getTotalCrystalNum() const;

	/**
	  * ������ˮ������
	  * @param total_crystal_num ˮ��������
	  */
	void setTotalCrystalNum(int total_crystal_num);

	/** 
	  * �����ѻ��ˮ������
	  * @returns �ѻ��ˮ������
	  */
	int getObtainedCrystalNum() const;

	/**
	  * �����ѻ��ˮ������
	  * @param obtained_crystal_num �ѻ��ˮ������
	  */
	void setObtainedCrystalNum(int obtained_crystal_num);

private:
    void __resetGui();

protected slots:

	void __onTriggerText(uint16_t text_id);

	void __onHealthChanged(uint16_t pre_health, uint16_t cur_health);

	void __onAmmoChanged(uint16_t pre_ammo, uint16_t cur_ammo);

	void __onClipNumChanged(uint16_t pre_num, uint16_t cur_num);

	void __onGetCrystal(); 

	void __onTriggerQA();

	void __onAnswerButtonClick(std::shared_ptr<MyGUI::Widget> sender);

    void __changeDigits(std::vector<dt::GuiImageBox*>& pics, uint16_t number);

	void __onAmmoClipChange(uint16_t cur_ammo, uint16_t cur_clip);

protected:
	std::vector<dt::GuiImageBox*> mHealthHUD;	//!< ����ֵ
	std::vector<dt::GuiImageBox*> mAmmoHUD;		//!< ��ҩ�� 
    dt::GuiImageBox* mFrontSight;	            //!< ׼��
	std::vector<dt::GuiImageBox*> mClipNumHUD;	//!< ������
	std::vector<dt::GuiButton*> mAnswerButtons;	//!< ��ѡ��
	dt::GuiEditBox* mQuestionLabel;				//!< ������ʾ����
	dt::GuiLabel* mDialogLabel;					//!< �Ի���ʾ����
	uint16_t mTotalEnemyNum;					//!< ��ͼ�й�������
	uint16_t mRemainEnemyNum;					//!< ��ͼ��ʣ���������
	uint16_t mTotalCrystalNum;					//!< ��ͼ��ˮ������
	uint16_t mObtainedCrystalNum;				//!< ����ѻ��ˮ������
    QString mStage;                             //!< ��ǰ�ؿ�
    QString mNextStage;                         //!< ��һ�ؿ�

};

#endif