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
      * @return ���������
      */
    QString getQuestion() const;

    /** 
      * ��ȡ����ѡ��
      * @return ѡ����������
      */
    std::vector<QString> getAnswers() const;

    /** 
      * ��ȡ�������ȷ��
      * @return ��ȷ��
      */
    QString getCorrectAnswer() const;

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
      * @return �ش��Ƿ���ȷ
      */
    bool evaluate(const uint16_t index);

private:
    QString mQustion;               //!< ��������
    uint16_t mCorrectAnswser;        //!< ��ȷ�����
    std::vector<QString> mAnswers;    //!< ��ѡ��

};

#endif