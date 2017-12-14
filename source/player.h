#ifndef PLAYER__H
#define PLAYER__H

#include "twowaymultisprite.h"
#include "bullets.h"
#include <list>

class ExplodingSprite;

class Player : public TwoWayMultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);
  virtual void draw() const;
  ~Player();

  void collided() { collision = true; }
  void missed() { collision = false; }

  unsigned int bulletCount() const {return bullets.bulletCount();}
  unsigned int freeCount() const {return bullets.freeCount();}

  Player& operator=(const Player&) = delete;

  void right();
  void left();
  void up();
  void down();
  void stop();

  void shoot();
  void IsSharkShot(std::vector<TwoWayMultiSprite*>& Sharks);

  virtual void explode();
//  void attach( Sprite* o ) { observers.push_back(o); } 
//  void detach( Sprite* o );
private:
  ExplodingSprite* explosion;
  bool collision;
  Vector2f initialVelocity;

  std::string bulletName;
  //std::list<Bullet> bullets;
  Bullets bullets;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastFrame;


  //std::list<Sprite*> observers;
};
#endif
