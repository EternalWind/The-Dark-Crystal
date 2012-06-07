#include "StateManager.h"

void NewStateManager::push(StateName stateName)
{
    dt::State * res = NULL;
    switch ( stateName )
    {
<<<<<<< HEAD

=======
>>>>>>> cd7b85e445ad5735e34b804e03bcedb53f2dac35
    case FIRST_STATE:
        res = new FirstTestState();
        break;
    //case SECOND_STATE:
    //    res = new SecondTestState();
    //    break;
    default :
        break;
    }
    if ( res != NULL )
        dt::Root::getInstance().getStateManager()->setNewState(res);
}

NewStateManager & NewStateManager::getInstance()
{
    static NewStateManager instance;
    return instance;
}

void NewStateManager::pop(uint16_t count)
{
    dt::Root::getInstance().getStateManager()->pop();
}