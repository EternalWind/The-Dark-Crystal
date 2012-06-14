#ifndef THE_DARK_CRYSTAL_SAVELOAD_STATE
#define THE_DARK_CRYSTAL_SAVELOAD_STATE

#include <Scene/State.hpp>
#include <Gui/GuiButton.hpp>
#include <Gui/GuiImageBox.hpp>
#include <Gui/GuiListBox.hpp>

/**
  * ���ش浵����
  */
class SaveLoadState : public dt::State {
    Q_OBJECT
public:
	/**
	  * ��ʼ������
	  */
    void onInitialize();

	/**
	  * ��ť�¼���Ӧ����
	  * @param sender ��������¼��İ�ť
	  */
	void onClick(MyGUI::Widget* sender);

    /**
	  * ListBox�¼���Ӧ����
	  * @param sender ��������¼���ListBox
      * @param index �б��index
	  */
    void onListClick(MyGUI::ListBox* sender, size_t index);

    void updateStateFrame(double simulation_frame_time);

private:
    dt::GuiListBox* mRecordList;
	
};

#endif