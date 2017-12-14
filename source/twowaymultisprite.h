#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&);
  TwoWayMultiSprite(const TwoWayMultiSprite&);

 ~TwoWayMultiSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

  virtual void explode();

protected:

  std::vector<Image *> images;
  ExplodingSprite* explosion;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  bool goingForward;
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
};
#endif
