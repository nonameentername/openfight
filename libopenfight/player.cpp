#include "player.h"

using namespace std;

Player::Player() {
    current_state = "start";
    opponent = NULL;
    is_hurt = false;
    index = 0;

    animation_manager = new ObjectManager<Animation>();
    objects = new ObjectManager<Player>();
}

Player::Player(const Player &p) {
    x_pos = p.x_pos;
    y_pos = p.y_pos;
    alpha = p.alpha;
    scale = p.scale;

    x_initial = p.x_initial;
    y_initial = p.y_initial;
    alpha_initial = p.alpha_initial;
    scale_initial = p.scale_initial;

    type = p.type;
    inverted = p.inverted;
    is_hurt = p.is_hurt;
    index = p.index;

    current_state = p.current_state;
    previous_state = p.previous_state;

    states = p.states;
    next_state = p.next_state;

    animation_manager = new ObjectManager<Animation>(*p.animation_manager);
    opponent = p.opponent;

    objects = new ObjectManager<Player>(*p.objects);

    state = p.state;
}

Player::~Player() {
    delete animation_manager;
    delete objects;
}

bool Player::operator<(const Player &p) const {
    return this->index < p.index;
}

GLfloat Player::getXpos() {
    return x_pos;
}

bool Player::isInverted() {
    return inverted;
}

bool Player::isHurt() {
    return is_hurt;
}

Collision *Player::getOffense() {
    if (animation_manager->get(current_state) != NULL) {
        return animation_manager->get(current_state)->getOffense();
    }

    return NULL;
}

Collision *Player::getDefense() {
    if (animation_manager->get(current_state) != NULL) {
        return animation_manager->get(current_state)->getDefense();
    }

    return NULL;
}

void Player::setOpponent(Player *opponent) {
    this->opponent = opponent;
}

bool Player::updateState(string command) {
    return updateState(command, false);
}

bool Player::updateState(string command, bool from_opponent) {
    bool result = false;
    bool has_state = false;
    bool is_combo = false;
    bool is_continual = false;

    if (animation_manager->get(current_state) != NULL)
        is_combo = animation_manager->get(current_state)->isCombo();

    has_state = states[current_state][command] != "";

    if (from_opponent && has_state || (is_combo ? opponent->isHurt() && has_state : has_state)) {
        string state = states[current_state][command];

        if (animation_manager->get(state) != NULL) {
            result = true;

            if (from_opponent)
                is_hurt = true;

            is_continual = animation_manager->get(state)->isContinual();

            if (!is_continual)
                animation_manager->get(state)->restart();

            setCurrentState(state);
        }

        if (!from_opponent) {
            index = 1;
            opponent->index = 0;
        }
    }

    return result;
}

bool Player::update() {
    bool result = false;
    bool done = false;
    bool changed = false;

    if (animation_manager->get(current_state)->isContinual() && inverted != state["INVERT"]) {
        updateState("INVERT");
        inverted = state["INVERT"];
    }

    doAction(typeid(MoveAction));

    if (next_state[current_state] == "*previous") {
        bool temp;
        if (animation_manager->get(previous_state) != NULL)
            animation_manager->get(previous_state)->nextFrame(temp);
    }

    if (animation_manager->get(current_state) != NULL) {
        animation_manager->get(current_state)->update(x_pos, y_pos, 1, inverted);
        done = animation_manager->get(current_state)->nextFrame(changed);
    }

    if (done) {
        setCurrentState(next_state[current_state]);
        is_hurt = false;

        if (type == "temporary")
            result = true;
    }

    if (opponent != NULL && this->getOffense() != NULL && this->getOffense()->overlaps(opponent->getDefense())) {
        if (type == "projectile") {
            result = true;
            doAction(typeid(CollisionAction));
        } else if (changed) {
            doAction(typeid(CollisionAction));
        }
    }

    if (changed)
        doAction(typeid(CreateObjectAction));

    return result;
}

void Player::createObject(string object) {
    if (!objects->contains(object)) {
        Player *new_object = new Player();

        new_object->initialize(object, !inverted, 0, 0);
        new_object->setOpponent(opponent);

        objects->add(object, new_object);
    }
}

void Player::copyObject(string object, GLfloat x, GLfloat y, int index) {
    Player *obj_copy = new Player(*objects->get(object));
    obj_copy->setOpponent(opponent);
    obj_copy->state["INVERT"] = inverted;
    obj_copy->inverted = inverted;
    obj_copy->index = index;
    obj_copy->setLocation(x_pos + (inverted ? -x : x), y_pos + y);

    stringstream sstream;
    sstream << object << SDL_GetTicks();
    object_manager->add(sstream.str(), obj_copy);
}

void Player::draw() {
    if (animation_manager->get(current_state) != NULL)
        animation_manager->get(current_state)->draw(alpha);
}

void Player::doAction(const type_info &type) {
    Actions *actions;
    bool completed = false;

    if (animation_manager->get(current_state) != NULL)
        actions = animation_manager->get(current_state)->getActions();

    if (actions != NULL)
        completed = actions->doActions(this, opponent, type);

    if (type == typeid(MoveAction) && !completed) {
        if (animation_manager->get(previous_state) != NULL)
            actions = animation_manager->get(previous_state)->getActions();

        if (actions != NULL)
            actions->doActions(this, opponent, type);
    }
}

void Player::move(GLfloat x_vel, GLfloat y_vel) {
    moveXpos(x_vel);
    y_pos = y_pos + y_vel;

    checkCollisions(x_vel);

    if (y_vel < 0 && y_pos <= y_initial) {
        y_pos = y_initial;
        updateState("GROUND");
    }
}

void Player::setLocation(GLfloat x, GLfloat y) {
    if (state["INVERT"])
        x_pos = x - x_initial;
    else
        x_pos = x + x_initial;
    y_pos = y + y_initial;
}

void Player::checkCollisions(GLfloat action_x_vel) {
    if (action_x_vel == 0)
        action_x_vel = 0.1;

    if (opponent != NULL && this->getDefense() != NULL && this->getDefense()->overlaps(opponent->getDefense())) {
        if (action_x_vel > 0) {
            moveXpos(-action_x_vel / 2);
            opponent->moveXpos(-action_x_vel * 2);
        } else {
            moveXpos(-action_x_vel / 2);
        }
    }
}

void Player::moveXpos(GLfloat value) {
    if (inverted)
        x_pos -= value;
    else
        x_pos += value;
}

void Player::setCurrentState(string state) {
    if (state == "*previous") {
        current_state = previous_state;
    } else {
        previous_state = current_state;
        current_state = state;
    }
    animation_manager->get(current_state)->update(x_pos, y_pos, 1, inverted);
}
