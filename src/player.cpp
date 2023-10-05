#include "player.h"

using namespace std;

Player::Player()
{
   current_state = "start";
   opponent      = NULL;
   is_hurt       = false;
   index         = 0;

   animation_manager = new ObjectManager<Animation>();
   objects = new ObjectManager<Player>();
}

Player::Player(const Player &p)
{
   x_pos = p.x_pos;
   y_pos = p.y_pos;
   alpha = p.alpha;
   scale = p.scale;

   x_initial     = p.x_initial;
   y_initial     = p.y_initial;
   alpha_initial = p.alpha_initial;
   scale_initial = p.scale_initial;

   type     = p.type;
   inverted = p.inverted;
   is_hurt  = is_hurt;
   index    = index;

   current_state  = p.current_state;
   previous_state = p.previous_state;

   states     = p.states;
   next_state = p.next_state;

   animation_manager = new ObjectManager<Animation>(*p.animation_manager);
   opponent = p.opponent;

   objects = new ObjectManager<Player>(*p.objects);
 
   state = p.state;
}

Player::~Player()
{
   delete animation_manager;
   delete objects;
}

bool Player::operator< (const Player& p) const
{
   return this->index < p.index;
}

GLfloat Player::getXpos()
{
   return x_pos;
}

bool Player::isInverted()
{
   return inverted;
}

bool Player::isHurt()
{
   return is_hurt;
}

Collision *Player::getOffense()
{
   if(animation_manager->get(current_state) != NULL)
   {
      return animation_manager->get(current_state)->getOffense();
   }

   return NULL;
}

Collision *Player::getDefense()
{
   if(animation_manager->get(current_state) != NULL)
   {
      return animation_manager->get(current_state)->getDefense();
   }

   return NULL;
}

void Player::initialize(std::string file_name, bool player_one, GLfloat x_pos, GLfloat y_pos)
{
   xmlDoc  *doc            = NULL;
   xmlNode *root_element   = NULL;
   xmlNode *type_node      = NULL;
   xmlNode *actions_node   = NULL;
   xmlNode *position_node  = NULL;

   Animation *animation;
   Actions   *actions;
   Collision *defense;
   Collision *offense;

   if(!player_one)
   {
      state["INVERT"] = true;
      inverted        = true;
   }
   else
   {
      state["INVERT"] = false;
      inverted        = false;
   }

   this->x_pos = x_pos;
   this->y_pos = y_pos;

   doc = xmlReadFile(file_name.c_str(), NULL, 0);
   root_element = xmlDocGetRootElement(doc);

   type_node      = findChild(root_element, "type");
   position_node  = findChild(root_element, "position");

   if(type_node != NULL)
      type = getContent<string>(type_node);

   if(position_node != NULL)
   {
      y_initial     = getProperty<float>(position_node, "y_pos");
      x_initial     = getProperty<float>(position_node, "x_pos");
      alpha_initial = getProperty<float>(position_node, "alpha");
      scale_initial = getProperty<float>(position_node, "scale");

      moveXpos(x_initial);
      this->y_pos += y_initial;
      alpha = alpha_initial;
      scale = scale_initial;
   }

   vector<xmlNode*> action_node = findChildren(root_element, "action");

   for(int i = 0; i < action_node.size(); i++)
   {
      string name    = getProperty<string>(action_node[i], "name");
      string trigger = getProperty<string>(action_node[i], "trigger");
      string from    = getProperty<string>(action_node[i], "from");
      string to      = getProperty<string>(action_node[i], "to");

      vector<string> from_tokens    = splitString(from, ", ");
      vector<string> trigger_tokens = splitString(trigger, ", ");

      for(int j = 0; j < from_tokens.size(); j++)
         for(int k = 0; k < trigger_tokens.size(); k++)
            states[from_tokens[j]][trigger_tokens[k]] = name;

      next_state[name] = to;
   }

   vector<xmlNode*> animation_node = findChildren(root_element, "animation");
   for(int i = 0; i < animation_node.size(); i++)
   {
      string name       = getProperty<string>(animation_node[i], "name");
      bool is_combo     = getProperty<bool>(animation_node[i], "combo");
      bool is_continual = getProperty<bool>(animation_node[i], "continual");

      animation = new Animation(is_combo, is_continual);

      vector<xmlNode*> frame_node = findChildren(animation_node[i], "frame");
      for(int j = 0; j < frame_node.size(); j++)
      {
         string path  = getProperty<string>(frame_node[j], "path");
         float width  = getProperty<float>(frame_node[j], "width");
         float height = getProperty<float>(frame_node[j], "height");
         float delay  = getProperty<float>(frame_node[j], "delay");

         defense = new Collision();
         offense = new Collision();
         actions = new Actions();

         vector<xmlNode*> defense_node = findChildren(frame_node[j], "defense");
         for(int k = 0; k < defense_node.size(); k++)
         {
            float x_pos  = getProperty<float>(defense_node[k], "x_pos");
            float y_pos  = getProperty<float>(defense_node[k], "y_pos");
            float width  = getProperty<float>(defense_node[k], "width");
            float height = getProperty<float>(defense_node[k], "height");

            defense->addCollision(new CollisionBox(x_pos, y_pos, width, height));
         }

         vector<xmlNode*> offense_node = findChildren(frame_node[j], "offense");
         for(int k = 0; k < offense_node.size(); k++)
         {
            float x_pos  = getProperty<float>(offense_node[k], "x_pos");
            float y_pos  = getProperty<float>(offense_node[k], "y_pos");
            float width  = getProperty<float>(offense_node[k], "width");
            float height = getProperty<float>(offense_node[k], "height");

            offense->addCollision(new CollisionBox(x_pos, y_pos, width, height));
         }

         vector<xmlNode*> object_node = findChildren(frame_node[j], "create_object");
         for(int k = 0; k < object_node.size(); k++)
         {
            string obj_path = getProperty<string>(object_node[k], "path");
            float x_pos     = getProperty<float>(object_node[k], "x_pos");
            float y_pos     = getProperty<float>(object_node[k], "y_pos");
            int   index     = getProperty<int>(object_node[k], "index");

            createObject(obj_path);
            actions->addAction(new CreateObjectAction(obj_path, x_pos, y_pos, index));
         }

         vector<xmlNode*> collision_node = findChildren(frame_node[j], "collision");
         for(int k = 0; k < collision_node.size(); k++)
         {
            string obj_path    = getProperty<string>(collision_node[k], "path");
            float x_pos        = getProperty<float>(collision_node[k], "x_pos");
            float y_pos        = getProperty<float>(collision_node[k], "y_pos");
            int   index        = getProperty<int>(collision_node[k], "index");
            string to          = getProperty<string>(collision_node[k], "to");
            string to_opponent = getProperty<string>(collision_node[k], "opponent");

            createObject(obj_path);
            actions->addAction(new CollisionAction(obj_path, x_pos, y_pos, index, to, to_opponent));
         }

         xmlNode *move_node = findChild(frame_node[j], "move");
         if(move_node != NULL)
         { 
            GLfloat x_vel = getProperty<float>(move_node, "x_vel");
            GLfloat y_vel = getProperty<float>(move_node, "y_vel");
            
            actions->addAction(new MoveAction (x_vel, y_vel));
         }

         animation->addFrame(path, width, height, offense, defense, actions, delay);
      }

      animation_manager->add(name, animation);
   }

   xmlFreeDoc(doc);
}

void Player::setOpponent(Player *opponent)
{
   this->opponent = opponent;
}

bool Player::updateState(string command)
{
   return updateState(command, false);
}

bool Player::updateState(string command, bool from_opponent)
{
   bool result       = false;
   bool has_state    = false;
   bool is_combo     = false;
   bool is_continual = false;

   if(animation_manager->get(current_state) != NULL)
      is_combo = animation_manager->get(current_state)->isCombo();

   has_state = states[current_state][command] != "";

   if(from_opponent && has_state || (is_combo? opponent->isHurt() && has_state : has_state))
   {
      string state = states[current_state][command];

      if(animation_manager->get(state) != NULL)
      {
         result = true;

         if(from_opponent)
            is_hurt = true;

         is_continual = animation_manager->get(state)->isContinual();

         if(!is_continual)
            animation_manager->get(state)->restart();

         setCurrentState(state);
      }

      if(!from_opponent)
      {
         index = 1;
         opponent->index = 0;
      }
   }

   return result;
}

bool Player::update()
{
   bool result  = false;
   bool done    = false;
   bool changed = false;

   if(animation_manager->get(current_state)->isContinual() && inverted != state["INVERT"])
   {
      updateState("INVERT");
      inverted = state["INVERT"];
   }

   doAction(typeid(MoveAction));

   if(next_state[current_state] == "*previous")
   {
      bool temp;
      if(animation_manager->get(previous_state) != NULL)
         animation_manager->get(previous_state)->nextFrame(temp);
   }

   if(animation_manager->get(current_state) != NULL)
   {
      animation_manager->get(current_state)->update(x_pos, y_pos, 1, inverted);
      done = animation_manager->get(current_state)->nextFrame(changed);
   }

   if(done)
   {
      setCurrentState(next_state[current_state]);
      is_hurt = false;

      if(type == "temporary")
         result = true;
   }

   if(opponent != NULL && this->getOffense() != NULL &&
      this->getOffense()->overlaps(opponent->getDefense()))
   {
      if(type == "projectile")
      {
         result = true;
         doAction(typeid(CollisionAction));
      }
      else if(changed)
      {
         doAction(typeid(CollisionAction));
      }
   }

   if(changed)
      doAction(typeid(CreateObjectAction));

   return result;
}

void Player::createObject(string object)
{
   if(!objects->contains(object))
   {
      Player *new_object = new Player();

      new_object->initialize(object, !inverted, 0, 0);
      new_object->setOpponent(opponent);

      objects->add(object, new_object);
   }
}

void Player::copyObject(string object, GLfloat x, GLfloat y, int index)
{
   Player *obj_copy = new Player(*objects->get(object));
   obj_copy->setOpponent(opponent);
   obj_copy->state["INVERT"] = inverted;
   obj_copy->inverted        = inverted;
   obj_copy->index           = index;
   obj_copy->setLocation(x_pos + (inverted? -x : x), y_pos + y);

   stringstream sstream;
   sstream << object << SDL_GetTicks();
   object_manager->add(sstream.str(), obj_copy);
}

void Player::draw()
{
   if(animation_manager->get(current_state) != NULL)
      animation_manager->get(current_state)->draw(alpha);
}

void Player::doAction(const type_info& type)
{
   Actions *actions;
   bool completed = false;

   if(animation_manager->get(current_state) != NULL)
      actions = animation_manager->get(current_state)->getActions();

   if(actions != NULL)
      completed = actions->doActions(this, opponent, type);

   if(type == typeid(MoveAction) && !completed)
   {
      if(animation_manager->get(previous_state) != NULL)
         actions = animation_manager->get(previous_state)->getActions();

      if(actions != NULL)
         actions->doActions(this, opponent, type);
   }
}

void Player::move(GLfloat x_vel, GLfloat y_vel)
{
   moveXpos(x_vel);
   y_pos = y_pos + y_vel;

   checkCollisions(x_vel);

   if(y_vel < 0 && y_pos <= y_initial)
   {
      y_pos = y_initial;
      updateState("GROUND");
   }
}

void Player::setLocation(GLfloat x, GLfloat y)
{
   if(state["INVERT"])
      x_pos = x - x_initial;
   else
      x_pos = x + x_initial;
   y_pos = y + y_initial;
}

void Player::checkCollisions(GLfloat action_x_vel)
{
   if(action_x_vel == 0)
      action_x_vel = 0.1;

   if(opponent != NULL && this->getDefense() != NULL &&
      this->getDefense()->overlaps(opponent->getDefense()))
   {
      if(action_x_vel > 0)
      {
         moveXpos(-action_x_vel/2);
         opponent->moveXpos(-action_x_vel*2);
      }
      else
      {
         moveXpos(-action_x_vel/2);
      }
   }
}

void Player::moveXpos(GLfloat value)
{
   if(inverted)
      x_pos -= value;
   else
      x_pos += value;
}

void Player::setCurrentState(string state)
{
   if(state == "*previous")
   {
      current_state = previous_state;
   }
   else
   {
      previous_state = current_state;
      current_state  = state;
   }
   animation_manager->get(current_state)->update(x_pos, y_pos, 1, inverted);
}
