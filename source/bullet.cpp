#include <iostream>
#include <cmath>
#include "bullet.h"
#include "imageFactory.h"

Bullet::Bullet( const string& name):
  MultiSprite(name),
  distance(0),
  maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
  tooFar(false) 
{}

Bullet::Bullet( const string& name, 
		const Vector2f& pos, const Vector2f& vel):
  MultiSprite(name, pos, vel),
  distance(0),
  maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
  tooFar(false) 
{}

void Bullet::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if (getVelocityX() >0) currentFrame = 0;
  else currentFrame = 1;
/*
  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
    currentFrame = 0;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -std::abs( getVelocityX() ) );
    currentFrame = 1;
  }  */


  if (getY() < 0 || getY()+getScaledHeight() > worldHeight ) {
    tooFar = true;
  }
  if (getX() < 0 || getX() > worldWidth) {
    tooFar = true;
  }

  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}

void Bullet::reset(){
   tooFar = false;  
   distance = 0;
}
