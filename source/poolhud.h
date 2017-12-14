
#include "player.h"

class PoolHud{
public:
  PoolHud(const Player*);
 // ~PoolHud();
  void draw() const;
  void update();
  void toggle() {showPoolHud = !showPoolHud;}

private:
  const Player* player;
  SDL_Renderer* renderer;
  SDL_Rect hudFrame;
  bool showPoolHud;
};

