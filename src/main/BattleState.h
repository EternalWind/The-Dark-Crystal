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
  * ��Ϸ��ս������
  * Ĭ�ϵ�һ�˳��ӽ�
  * ��ʾѪ����������������
  */
class BattleState : public dt::State {
    Q_OBJECT
public:
    void onInitialize();
	
    void updateStateFrame(double simulation_frame_time);

	/**
	  * ��ʼ��
	  * @params tot_enemy_num ��ͼ�����еĹ�������
	  * @params tot_crystal_num ��ͼ�����е�ˮ������
	  */
	BattleState(uint16_t tot_enemy_num = 0, uint16_t tot_crystal_num = 0);

	/**
	  * �ж���ҵ�ǰ�Ƿ��Ѿ�ȡ��ʤ��
	  * @returns ʤ�����
	  */
	bool isVictory();

	/**
	  * ���ȡʤ������£���һ������
	  */
	void win();

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
	  * @params ���öԻ�����
	  */
	void setDialogLabel(dt::GuiLabel* dialog_label);

	/**
	  * �����ܵ�������
	  * @returns �ܵ�������
	  */
	uint16_t getTotalEnemyNum() const;

	/**
	  * �����ܵ�������
	  * @params total_enemy_num
	  */
	void setTotalEnemyNum(uint16_t total_enemy_num);

	/**
	  * ����ʣ�µĵ�������
	  * @returns ʣ�µĵ�������
	  */
	uint16_t getRemainEnemyNum() const;

	/*
	 * ����ʣ�µ�������
	 * @params remain_enemy_num ʣ���������
	 */
	void setRemainEnemyNum(uint16_t remain_enemy_num);

	/**
	  * ����ˮ������
	  * @returns ˮ������
	  */
	uint16_t getTotalCrystalNum() const;

	/**
	  * ������ˮ������
	  * @params total_crystal_num ˮ��������
	  */
	void setTotalCrystalNum(uint16_t total_crystal_num);

	/** 
	  * �����ѻ��ˮ������
	  * @returns �ѻ��ˮ������
	  */
	uint16_t getObtainedCrystalNum() const;

	/**
	  * �����ѻ��ˮ������
	  * @params obtained_crystal_num �ѻ��ˮ������
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
	std::vector<dt::GuiImageBox*> mHealthHUD;	//!< ����ֵ
	std::vector<dt::GuiImageBox*> mAmmoHUD;		//!< ��ҩ�� 
	std::vector<dt::GuiImageBox*> mFrontSight;	//!< ׼��
	std::vector<dt::GuiImageBox*> mClipNumHUD;	//!< ������
	std::vector<dt::GuiButton> mAnswerButtons;	//!< ��ѡ��
	dt::GuiLabel* mQuestionLabel;				//!< ������ʾ����
	dt::GuiLabel* mDialogLabel;					//!< �Ի���ʾ����
	uint16_t mTotalEnemyNum;					//!< ��ͼ�й�������
	uint16_t mRemainEnemyNum;					//!< ��ͼ��ʣ���������
	uint16_t mTotalCrystalNum;					//!< ��ͼ��ˮ������
	uint16_t mObtainedCrystalNum;				//!< ����ѻ��ˮ������
	

};

#endif
