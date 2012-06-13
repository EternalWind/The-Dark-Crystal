#include <QAManager.h>
#include <Utils/Random.hpp>
#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>
#include <iostream>

std::shared_ptr<QAManager> QAManager::mInstance = std::shared_ptr<QAManager>(new QAManager());

QAManager::~QAManager() {

}

void QAManager::initialize() {

}

void QAManager::deinitialize() {

}

bool QAManager::loadQuestions(QString path) {

    QFile config_file("QATestFile.xml");
    QDomDocument doc;

    // If the file doesn't exist...
    if(!config_file.open(QIODevice::ReadOnly)) {
        dt::Logger::get().info("The configuration file doesn't exist. Trying to create a new one.");
        return false;
    }

    if (doc.setContent(&config_file)) {
        QDomElement root = doc.documentElement();
        for (QDomElement QA_node = root.firstChildElement("Question"); !QA_node.isNull(); QA_node = QA_node.nextSiblingElement()) {

            std::shared_ptr<Question> question= std::shared_ptr<Question>(new Question());

            auto id = QA_node.firstChildElement("id");
            uint16_t index = id.text().toUInt();

            auto content_node = QA_node.firstChildElement("content");
            QString question_content = content_node.text();
            question->setQuestion(question_content);

            std::vector<QString> option_vec;
            auto option1_node = QA_node.firstChildElement("option1");
            option_vec.push_back(option1_node.text());

            auto option2_node = QA_node.firstChildElement("option2");
            option_vec.push_back(option2_node.text());

            auto option3_node = QA_node.firstChildElement("option3");
            option_vec.push_back(option3_node.text());


            auto option4_node = QA_node.firstChildElement("option4");
            option_vec.push_back(option4_node.text());

            question->setAnswers(option_vec);

            auto correctAnswerIndex_node = QA_node.firstChildElement("correctAnswerIndex");
            uint16_t correctAnswer_index = correctAnswerIndex_node.text().toUInt();
            question->setCorrectAnswer(correctAnswer_index);

            mQuestions.insert(std::pair<uint16_t, std::shared_ptr<Question> > (index, question));
        } 
    } else {
        dt::Logger::get().error("Failed to read from the configuration file.");
        return false;
    }
    return true;
}


QAManager* QAManager::getInstance() {
    return mInstance.get();
}

std::shared_ptr<Question> QAManager::getRandomQuestion() {

    dt::Random::initialize();
    int32_t num = mQuestions.size();
    int32_t index = 1;

    if (0 != num) {
        index = dt::Random::get(1, num);
    }
    return mQuestions[index];

}




