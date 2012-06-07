/****
	�������Ĺ�����ע��һ������Ҫ��������
	1. include �������ͷ�ļ�
	2. ��StateName�ж���ö�ٳ�������
	3. ��push������дcase������ɡ�
****/


#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <Scene/State.hpp>
#include <Scene/StateManager.hpp>
#include <Core/Root.hpp>

#include "StateChangeTest.h"

enum StateName {
    TEST,
    FIRST_STATE,
    SECOND_STATE
};

class NewStateManager {
public:
    void push(StateName stateName);
    void pop(uint16_t count = 1);
    static NewStateManager & getInstance();

//for singleton
private:
    NewStateManager(){}
    NewStateManager & operator = (const NewStateManager &){}
    NewStateManager(const NewStateManager &){}
};

#endif