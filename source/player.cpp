#include "player.h"
#include <cmath>
#include "explodingSprite.h"

Player::Player( const std::string& name) :
  TwoWayMultiSprite(name),
  explosion(nullptr),
  collision(false),
  initialVelocity(getVelocity()),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(bulletName),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0)
{ }

Player::~Player( ) { if (explosion) delete explosion; }

Player::Player(const Player& s) :
  TwoWayMultiSprite(s), 
  explosion(nullptr),
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastFrame(s.timeSinceLastFrame)
  { }


void Player::stop() { 
  setVelocity(Vector2f(0,0));
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
  goingForward = true;
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
  goingForward = false; 
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::explode() {
  if ( !explosion ) {
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

void Player::shoot() { 
  if ( timeSinceLastFrame < bulletInterval ) return;

  Vector2f vel = getVelocity();
  float x; // = getScaledWidth();
  float y = getY() + getScaledHeight()/2;


  // I need to add some minSpeed to velocity:
  if (goingForward){
    //bullet.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
    //bullet.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
    x = getX() + getScaledWidth() - 20;
    vel[0] += minSpeed;
  }
  else {
    //bullet.setPosition( getPosition() +Vector2f(-(deltaX-100), deltaY) );
    //bullet.setVelocity(getVelocity() - Vector2f(minSpeed, 0) );
    x = getX() - 70;
    vel[0] -=minSpeed;
  }
  bullets.shoot(Vector2f(x,y), vel);
  timeSinceLastFrame = 0;
}

void Player::draw() const {
  if ( explosion ) explosion->draw();
  else {
    images[currentFrame]->draw(getX(), getY(), getScale());
    bullets.draw();
  }
}

void Player::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  timeSinceLastFrame += ticks;
  if ( !collision ) advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  
  bullets.update(ticks);
 


  stop();
}

void Player::IsSharkShot(std::vector<TwoWayMultiSprite*>& Sharks){
  auto it = Sharks.begin();
  while(it != Sharks.end()){
     if (bullets.collided(*it)){
        it = Sharks.erase(it);
     }
     else ++it;
  }
  
}
