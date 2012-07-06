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
      * 返回RecordManager的单例指针。
      * @returns RecordManager的单例指针
      */
    static RecordManager* get();

    /**
      * 初始化。加载Record信息。
      */
    void initialize();

    /**
      * 反初始化。
      */
    void deinitialize();

    /**
      * 获取全部Record。
      * @returns 全部Record
      */
    std::map<unsigned, RecordInfo> getRecords() const;

    /**
      * 获取某一Record。
      * @param id 要获取的Record的ID
      * @returns 要获取的Record
      */
    RecordInfo getRecord(const unsigned id) const;

    /**
      * 保存一个BattleState。
      * @param battle_state 要保存的BattleState
      */
    void save(const BattleState* battle_state);

    /**
      * 根据Record读取一个BattleState。
      * @param record 要读取的Record
      * @returns 读取出来的BattleState
      */
    BattleState* load(const RecordInfo record) const;

    /**
      * 移除一个Record。
      * @param id 要移除的Record的id
      */
    void remove(const unsigned id);

private:
    std::map<unsigned, RecordInfo> mRecords;           //!< Record列表。
    unsigned mCounter;                                 //!< Record计数器。
    static RecordManager* mInstance;                   //!< RecordManager单例指针。
};

#endif