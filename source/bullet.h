#include <iostream>
#include "multisprite.h"
#include "gamedata.h"


class Bullet : public MultiSprite {
public:
  Bullet(const std::string&);
  Bullet(const std::string&, const Vector2f&, const Vector2f&);
 // Bullet(const Bullet&);
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset();
private:
  float distance;
  float maxDistance;
  bool tooFar;
};
