#ifndef THE_DARK_CRYSTAL_RECORD_MANAGER
#define THE_DARK_CRYSTAL_RECORD_MANAGER

#include "BattleState.h"

#include <Core/Manager.hpp>

#include <map>


class RecordManager : public dt::Manager {

    Q_OBJECT

public:

	struct RecordInfo {
        QString mLevelID;
        QString mFileName;
    };

    /**
      * ����RecordManager�ĵ���ָ�롣
      * @returns RecordManager�ĵ���ָ��
      */
    static RecordManager* get();

    /**
      * ��ʼ��������Record��Ϣ��
      */
    void initialize();

    /**
      * ����ʼ����
      */
    void deinitialize();

    /**
      * ��ȡȫ��Record��
      * @returns ȫ��Record
      */
    std::map<unsigned, RecordInfo> getRecords() const;

    /**
      * ��ȡĳһRecord��
      * @param id Ҫ��ȡ��Record��ID
      * @returns Ҫ��ȡ��Record
      */
    RecordInfo getRecord(const unsigned id) const;

    /**
      * ����һ��BattleState��
      * @param battle_state Ҫ�����BattleState
      */
    void save(const BattleState* battle_state);

    /**
      * ����Record��ȡһ��BattleState��
      * @param record Ҫ��ȡ��Record
      * @returns ��ȡ������BattleState
      */
    BattleState* load(const RecordInfo record) const;

    /**
      * �Ƴ�һ��Record��
      * @param id Ҫ�Ƴ���Record��id
      */
    void remove(const unsigned id);

private:
    std::map<unsigned, RecordInfo> mRecords;           //!< Record�б�
    unsigned mCounter;                                 //!< Record��������
    static RecordManager* mInstance;                   //!< RecordManager����ָ�롣
};

#endif