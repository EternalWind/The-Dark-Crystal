#include "Question.h"

QString Question::getQuestion() const {
    return mQustion;
}

std::vector<QString> Question::getAnswers() const {
    return mAnswers;
}

uint16_t  Question::getCorrectAnswer() const {
    return mCorrectAnswser;
}

void Question::setQuestion(const QString question) {
    mQustion = question;
}

void Question::setCorrectAnswer(const uint16_t index) {
    mCorrectAnswser = index;
}

void Question::setAnswers(const std::vector<QString> answers) {
    mAnswers = answers;
}

bool Question::evaluate(const uint16_t index) {
    return (index == mCorrectAnswser);
}
