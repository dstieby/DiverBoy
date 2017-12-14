#include <iostream>
#include <cmath>
#include "ioMod.h"
#include "bullets.h"
#include "imageFactory.h"
#include "collisionStrategy.h"

Bullets::~Bullets(){}
Bullets::Bullets(const std::string& n) :
  name(n),
  myVelocity(
    Gamedata::getInstance().getXmlInt(name+"/speedX"),
    Gamedata::getInstance().getXmlInt(name+"/speedX")
  ),
  bulletImages(ImageFactory::getInstance().getImages(name) ),
  bulletList(),
  freeList(),
  strategy( new PerPixelCollisionStrategy )
{
} 

Bullets::Bullets(const Bullets& b) :
  name(b.name),
  myVelocity(b.myVelocity),
  bulletImages(b.bulletImages),
  bulletList(b.bulletList),
  freeList(b.freeList),
  strategy(b.strategy)
{}

void Bullets::shoot(const Vector2f& pos, const Vector2f& objVel){
  if( freeList.empty() ){
    Bullet b(name, pos, objVel);
    bulletList.push_back(b);
  }
  else {
    Bullet b = freeList.front();
    freeList.pop_front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    bulletList.push_back(b);
}
}
bool Bullets::collided(const Drawable* obj) const {
  for (const auto& bullet: bulletList) {
    if ( strategy->execute(bullet, *obj) ) {
      return true;
    }
  }  
  return false;
}

void Bullets::draw() const{
  for (const auto& bullet: bulletList){
    bullet.draw();
  }
}

void Bullets::update(Uint32 ticks){
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while(ptr != bulletList.end()){
    ptr->update(ticks);
    if (ptr->goneTooFar()) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    } 
    else ++ptr;
  }
}














