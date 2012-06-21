#ifndef THE_DARK_CRYSTAL_CTEADIT_STATE
#define THE_DARK_CRYSTAL_CTEADIT_STATE

#include <Scene/State.hpp>
#include <Gui/GuiImageBox.hpp>

/**
  * ������Ա�˵�
  */
class CreaditState : public dt::State {
    Q_OBJECT
public:
	/**
	  * ������ʼ������
	  */
    void onInitialize();

    	/**
	  * ��ť�¼���Ӧ����
	  * @param sender ��������¼��İ�ť
	  */
	void onClick(MyGUI::Widget* sender);

    void updateStateFrame(double simulation_frame_time);

};

#endif