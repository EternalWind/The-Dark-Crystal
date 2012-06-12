#ifndef THE_DARK_CRYSTAL_QUESTION
#define THE_DARK_CRYSTAL_QUESTION

#include <QString>
#include <cstdint>
#include <vector>

 /**
   * �ʴ�ϵͳ��������
   */
class Question {
public:
    /**
      * ��ȡ��������
      * @returns ���������
      */
    QString getQuestion() const;

    /** 
      * ��ȡ����ѡ��
      * @returns ѡ����������
      */
    std::vector<QString> getAnswers() const;

    /** 
      * ��ȡ�������ȷ��
      * @returns ��ȷ��
      */
    uint16_t getCorrectAnswer() const;

    /** 
      * ������������
      * @param Ҫ���õ����������
      */
    void setQuestion(const QString question);

    /** 
      * ���ô�ѡ��
      * @param Ҫ���õĴ�ѡ������
      */
    void setAnswers(const std::vector<QString> answers);

    /** 
      * ������ȷ��
      * @param ������ȷ�����
      */
    void setCorrectAnswer(const uint16_t index);

    /** 
      * �жϻش��Ƿ���ȷ
      * @param ѡ��Ĵ����
      * @returns �ش��Ƿ���ȷ
      */
    bool evaluate(const uint16_t index);

private:
    QString mQustion;                 //!< ��������
    uint16_t mCorrectAnswser;         //!< ��ȷ�����
    std::vector<QString> mAnswers;    //!< ��ѡ��

};

#endif