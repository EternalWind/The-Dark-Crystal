#ifndef THE_DARK_CRYSTAL_QUESTION
#define THE_DARK_CRYSTAL_QUESTION

#include <QString>
#include <cstdint>
#include <vector>

 /**
   * 问答系统的问题类
   */
class Question {
public:
    /**
      * 获取问题内容
      * @return 问题的内容
      */
    QString getQuestion() const;

    /** 
      * 获取问题选项
      * @return 选项内容数组
      */
    std::vector<QString> getAnswers() const;

    /** 
      * 获取问题的正确答案
      * @return 正确答案
      */
    QString getCorrectAnswer() const;

    /** 
      * 设置问题内容
      * @param 要设置的问题的内容
      */
    void setQuestion(const QString question);

    /** 
      * 设置答案选项
      * @param 要设置的答案选项数组
      */
    void setAnswers(const std::vector<QString> answers);

    /** 
      * 设置正确答案
      * @param 设置正确答案序号
      */
    void setCorrectAnswer(const uint16_t index);

    /** 
      * 判断回答是否正确
      * @param 选择的答案序号
      * @return 回答是否正确
      */
    bool evaluate(const uint16_t index);

private:
    QString mQustion;               //!< 问题内容
    uint16_t mCorrectAnswser;        //!< 正确答案序号
    std::vector<QString> mAnswers;    //!< 答案选项

};

#endif