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
	QString getBattleStateName();

	/**
	  * ���ضԻ�����
	  * @returns �Ի�����
	  */
	dt::GuiLabel* getDialogLabel();

	/**
	  * ���öԻ�����
	  * @params ���öԻ�����
	  */
	void setDialogDabel(dt::GuiLabel* dialog_label);


public slots:
	/**
	  * �����Ի��¼���Ӧ�Ĳ�
	  * @params text_id �Ի��ı�����Ӧ��ID
	  */
	void onTriggerText(uint16_t text_id);

	/**
	  * �������ֵ�ı��Ӧ�Ĳ�
	  */
	void onHealthChanged();

	/**
	  * ��ҩ���ı��Ӧ�Ĳ�
	  */
	void onAmmoChanged();

	/**
	  * �������ı��Ӧ�Ĳ�
	  */
	void onClipNumChanged();

	/**
	  * ��һ��ˮ���¼���Ӧ�Ĳ�
	  */
	void onGetCrystal(); 

	/**
	  * ��Ҵ����ʴ�ϵͳ�¼���Ӧ�Ĳ�
	  */
	void onTriggerQA();

	/**
	  * �ʴ�ģʽ�£���Ұ��»ش����ⰴ����Ӧ�Ĳ�
	  * @params sender ����
	  */
	void onAnswerButtonClick(std::shared_ptr<MyGUI::Widget> sender);


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
