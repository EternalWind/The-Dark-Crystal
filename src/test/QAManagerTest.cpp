#include "ConfigurationManager.h"
#include "QAManager.h"

#include <Input/InputManager.hpp>
#include "iostream"

using namespace std;

//void main()
//{
//
//
//    QAManager*qa = QAManager::getInstance();
//    if (qa->loadQuestions("QATestFile.xml"))
//    {
//        std::shared_ptr<Question> q = qa->getRandomQuestion();
//        cout << q->getQuestion().toStdString(); //��ֹ������ʧЧ
//        vector<QString> option = q->getAnswers();
//        
//        for (vector <QString>::iterator iter = option.begin(); iter != option.end(); iter++) {
//            cout << (*iter).toStdString() << " ";
//        }
//        cout << endl;
//        if (q->evaluate(1)) {
//            cout <<"You are right";
//        } else {
//            cout << "You are wrong";
//        };
//        cout << endl;
//        cout <<"ѡ��" << qa->getRandomQuestion()->getCorrectAnswer() <<"��ȷ";
//    }
//
//    int a;
//    cin>>a;
//}