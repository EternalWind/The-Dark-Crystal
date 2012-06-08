#ifndef THE_DARK_CRYSTAL_BATTLE_STATE
#define THE_DARK_CRYSTAL_BATTLE_STATE

#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiLabel.hpp>
#include <Gui/GuiImageBox.hpp>
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

	/**
	  * ���öԻ�����
	  */

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
	std::vector<dt::GuiImageBox*> mHealthHUD;		//!< ����ֵ
	std::vector<dt::GuiImageBox*> mAmmoHUD;		//!< ��ҩ�� 
	std::vector<std::shared_ptr<dt::GuiImageBox> > mFrontSight; //!< ׼��
	std::vector<std::shared_ptr<int> > mClipNumHUD; //!< ������
	std::vector<std::shared_ptr<dt::GuiButton> > mAnswerButtons; //!< ��ѡ��
	std::shared_ptr<dt::GuiLabel> mQuestionLabel;	//!< ������ʾ����
	std::shared_ptr<dt::GuiLabel> mDialogLabel;	//!< �Ի���ʾ����
	uint16_t mTotalEnemyNum;	//!< ��ͼ�й�������
	uint16_t mRemainEnemyNum;	//!< ��ͼ��ʣ���������
	uint16_t mTotalCrystalNum;	//!< ��ͼ��ˮ������
	uint16_t mObtainedCrystalNum;	//!< ����ѻ��ˮ������
	

};

#endif
