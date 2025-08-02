#include "playerBridge.h"

using namespace std;

PlayerBridge::PlayerBridge() {
}

PlayerBridge::~PlayerBridge() {
}

void PlayerBridge::initialize(Player *player_one, Player *player_two) {
    this->player_one = player_one;
    this->player_two = player_two;
}

void PlayerBridge::update() {
    GLfloat x1 = player_one->getXpos();
    GLfloat x2 = player_two->getXpos();

    if (x1 < x2) {
        if (player_one->isInverted() || !player_two->isInverted()) {
            player_one->state["INVERT"] = false;
            player_two->state["INVERT"] = true;
        }
    } else {
        if (!player_one->isInverted() || player_two->isInverted()) {
            player_one->state["INVERT"] = true;
            player_two->state["INVERT"] = false;
        }
    }
}
