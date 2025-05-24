#ifndef _actions_h
#define _actions_h

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>

class Player;

class Action
{
   public:
      Action();
      ~Action();

      virtual void doAction(Player *player, Player *opponent) = 0;
};

class MoveAction : public Action
{
   protected:
      GLfloat x_vel;
      GLfloat y_vel;

   public:
      MoveAction(GLfloat x_vel, GLfloat y_vel);
      ~MoveAction();

      void doAction(Player *player, Player *opponent);
};

class CreateObjectAction : public Action
{
   protected:
      std::string object;
      GLfloat x_pos;
      GLfloat y_pos;
      int index;

   public:
      CreateObjectAction(std::string object, GLfloat x_pos, GLfloat y_pos, int index);
      ~CreateObjectAction();

      void doAction(Player *player, Player *opponent);
};

class CollisionAction : public Action
{
   protected:
      std::string object;
      GLfloat x_pos;
      GLfloat y_pos;
      int index;
      std::string to;
      std::string to_opponent;

   public:
      CollisionAction(std::string object, GLfloat x_pos, GLfloat y_pos, int index, std::string to, std::string to_opponent);
      ~CollisionAction();

      void doAction(Player *player, Player *opponent);
};

class Actions
{
   protected:
      std::vector<Action*> actions;

   public:
      Actions();
      Actions(const Actions &a);
      ~Actions();

      void addAction(Action *a);
      bool doActions(Player *player, Player *opponent, const std::type_info& type);
};

#endif
