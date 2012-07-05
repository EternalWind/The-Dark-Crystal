#include "RecordManager.h"
#include "Definitions.h"

#include <QDir>
#include <QCoreApplication>
#include <QtXml/QtXml>
#include <QDateTime>

RecordManager* RecordManager::mInstance = nullptr;

RecordManager* RecordManager::get() {
    if (mInstance == nullptr) {
        mInstance = new RecordManager();
    }

    return mInstance;
}

void RecordManager::initialize() {
    QDir record_dir(QCoreApplication::applicationDirPath());

    record_dir.cdUp();
    
    if (!record_dir.cd(RECORD_DIR)) {
        record_dir.mkdir(RECORD_DIR);
        record_dir.cd(RECORD_DIR);
    }

    QStringList name_filter;
    name_filter << QString("*.") + RECORD_SUB_FIX;

    QFileInfoList file_infos = record_dir.entryInfoList(name_filter, QDir::Files);

    mCounter = 0;

    for (auto file_info = file_infos.begin() ; file_info != file_infos.end() ; ++file_info) {
        QFile file(file_info->absoluteFilePath());
        QDomDocument doc;

        if (file.open(QIODevice::ReadOnly) && doc.setContent(&file)) {
            QDomElement root = doc.documentElement();
            QDomElement level = root.firstChildElement(RECORD_LEVEL_INFO);

            if (!level.isNull()) {
                RecordInfo info;
                info.mLevelID = level.nodeValue();
                info.mFileName = file.fileName();

                mRecords[mCounter] = info;
                ++mCounter;
            }
        }

        file.close();
    }
}

void RecordManager::deinitialize() {
    if (mInstance)
        delete mInstance;
}

std::map<unsigned, RecordManager::RecordInfo> RecordManager::getRecords() const {
    return mRecords;
}

RecordManager::RecordInfo RecordManager::getRecord(const unsigned id) const {
    return mRecords.at(id);
}

void RecordManager::save(const BattleState* battle_state) {
    RecordInfo info;
    info.mLevelID = battle_state->getStage();

    QDir record_dir(QCoreApplication::applicationDirPath());

    record_dir.cdUp();
    record_dir.cd(RECORD_DIR);

    QFile file(record_dir.absolutePath() + "/" + dt::Utils::toString(QDateTime::currentDateTime().toTime_t()) + "." + RECORD_SUB_FIX);

    info.mFileName = file.fileName();

    mRecords[mCounter] = info;
    ++mCounter;

    if (!file.open(QIODevice::WriteOnly)) {
        QDomDocument doc;
        QDomElement root;
        QDomElement level;

        std::cout << dt::Utils::toStdString(file.fileName()) << std::endl;

        root = doc.createElement(RECORD_ROOT);
        doc.appendChild(root);

        level = doc.createElement(RECORD_LEVEL_INFO);
        level.setNodeValue(info.mLevelID);
        root.appendChild(level);

        QTextStream out(&file);
        out << doc.toString();

        file.close();
    }
}

BattleState* RecordManager::load(RecordManager::RecordInfo record) const {
    return new BattleState(record.mLevelID);
}

void RecordManager::remove(const unsigned id) {
    auto iter = mRecords.find(id);

    if (iter != mRecords.end()) {
        QDir record_dir(QCoreApplication::applicationDirPath());

        record_dir.cdUp();
        record_dir.cd(RECORD_DIR);

        record_dir.remove(iter->second.mFileName);

        mRecords.erase(iter);
    }
}