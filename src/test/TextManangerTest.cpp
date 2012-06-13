#include "ConfigurationManager.h"
#include "QAManager.h"
#include "TextManager.h"

#include <Input/InputManager.hpp>
#include "iostream"

using namespace std;

void main()
{
    
    TextManager*text = TextManager::getInstance();
    if (text->loadTexts("Text.xml"))
    {
        cout << text->getText(3).toStdString();
    }


    int a;
    cin>>a;
}