#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "poolhud.h"
#include "sprite.h"
#include "sound.h"

class Player;
class CollisionStrategy;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World water; 
  World sand;
  Viewport& viewport;
  bool HudVisible;
  bool HudHasFaded;

  void checkForCollisions();
  bool findTreasure() const;

  std::vector<Drawable*> JellyFish;
  std::vector<TwoWayMultiSprite*> Sharks;
  
  std::vector<CollisionStrategy*> strategies;
  Sprite* treasure;
  Player* player;
  PoolHud poolhud;
  int currentStrategy;
  int currentSprite;
  bool collision;
  bool godMode;
 
  SDLSound sound; 

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  void printScales() const;

};
