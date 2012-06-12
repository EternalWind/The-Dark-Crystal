#include "ConfigurationManager.h"
#include "QAManager.h"

#include <Input/InputManager.hpp>
#include "iostream"

using namespace std;

void main()
{
    /*ConfigurationManager*conf = ConfigurationManager::getInstance();*/
    //
    //if (conf->loadConfig())
    //{
    //    
    //    cout<<conf->getScreenSetting().getFullScreen()<<endl;


    //    SoundSetting*sound = new SoundSetting();
    //    sound->setMusic(99);
    //    conf->setSoundSetting(*sound);
    //    SoundSetting mSound = conf->getSoundSetting();

    //    conf->saveConfig();
    //    cout<<conf->getSoundSetting().getSoundEffect()<<endl;
    //    cout<<conf->getSoundSetting().getMusic()<<endl;
    //    cout<<conf->getSoundSetting().getMainVolume()<<endl;

    //    cout<<conf->getControlSetting().getKey((ControlSetting::KeyFunction)4)<<endl;
    //    QString m = conf->getControlSetting().getKeyName((ControlSetting::KeyFunction)4);
    //    QByteArray da(m.toAscii());//toAscii()返回一个QByteArray类型   
    //    std::cout << da.data()<<endl;

    //    cout<<conf->getControlSetting().getSensitivity()<<endl;
    //    cout<<conf->getControlSetting().getYInverted()<<endl;


    //}

    //conf->saveConfig();

    QAManager*qa = QAManager::getInstance();
    if (qa->loadQuestions("QA.xml"))
    {
        std::shared_ptr<Question> q = qa->getRandomQuestion();
        cout << q->getQuestion().toStdString(); //防止迭代器失效
        vector<QString> option = q->getAnswers();
        //for (int i = 0; i <option.size(); ++i)
        //{
        //    cout << option[i].toStdString() << " ";
        //}
        for (vector <QString>::iterator iter = option.begin(); iter != option.end(); iter++) {
            cout << (*iter).toStdString() << " ";
        }
        cout << endl;
        if (q->evaluate(1)) {
            cout <<"You are right";
        } else {
            cout << "You are wrong";
        };
        cout<<endl;
        cout <<"选项" << qa->getRandomQuestion()->getCorrectAnswer() <<"正确";
    }

    int a;
    cin>>a;
}