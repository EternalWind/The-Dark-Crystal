#include <QAManager.h>
#include <Utils/Random.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>

std::shared_ptr<QAManager> QAManager::mInstance = std::shared_ptr<QAManager>(new QAManager());

QAManager::~QAManager() {

}

void QAManager::initialize() {

}

void QAManager::deinitialize() {

}

bool QAManager::loadQuestions(QString path) {

    QFile config_file("QA.xml");
    QDomDocument doc;

    // If the file doesn't exist...
    if(!config_file.open(QIODevice::ReadOnly)) {
        dt::Logger::get().info("The configuration file doesn't exist. Trying to create a new one.");
        return false;
    }

    if (doc.setContent(&config_file)) {
        QDomElement root = doc.documentElement();
    }
    


}

QAManager* QAManager::get() {
     return mInstance.get();
}

Question* QAManager::getRandomQuestion() {

    uint16_t num = mQuestions.size();
    uint16_t index = 1;

    if (0 != num) {
        index = dt::Random.get(1, num);
    }
    
    return mQuestions[index];

}




