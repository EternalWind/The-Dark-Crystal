
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "BattleStateTest.h"
#include "MenuState.h"
#include "OptionState.h"
#include "AlienTest.h"
#include <Scene/Game.hpp>
#include "AITest.h"

int main(int argc, char** argv) {
    dt::Game game;
    game.run(new MenuState(), argc, argv);
    return 0;
}
