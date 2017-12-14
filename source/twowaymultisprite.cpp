#include "twowaymultisprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
           if(goingForward){
              currentFrame =(currentFrame+1) % (numberOfFrames/2);
	   }
           else{
              currentFrame =(numberOfFrames/2) + ((currentFrame+1) % (numberOfFrames/2));
           }
		timeSinceLastFrame = 0;
	}
}

TwoWayMultiSprite::~TwoWayMultiSprite() { if ( explosion ) delete explosion; } 

TwoWayMultiSprite::TwoWayMultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  explosion(nullptr),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  goingForward(true)
{ }

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) :
  Drawable(s), 
  images(s.images),
  explosion(nullptr),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  goingForward(s.goingForward)
  { }

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  explosion = s.explosion;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  goingForward = (s.goingForward);
  return *this;
}

void TwoWayMultiSprite::draw() const { 
  if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void TwoWayMultiSprite::explode() {
  if ( !explosion ) {
    Sprite s(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(s);
  }

}

void TwoWayMultiSprite::update(Uint32 ticks) { 
   if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    goingForward = true;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    goingForward = false;
  }  

}
