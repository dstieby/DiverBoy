#include <sstream>
#include "poolhud.h"
#include "gamedata.h"
#include "renderContext.h"
#include "ioMod.h"

//PoolHud::~PoolHud(){}

PoolHud::PoolHud(const Player* p) : 
  player (p),
  renderer(RenderContext::getInstance()->getRenderer()),
  hudFrame(  {Gamedata::getInstance().getXmlInt("poolHud/x"),
		Gamedata::getInstance().getXmlInt("poolHud/y"),
		Gamedata::getInstance().getXmlInt("poolHud/width"),
		Gamedata::getInstance().getXmlInt("poolHud/height")
	      }
  ),
  showPoolHud(true)
{}

void PoolHud::update(){}

void PoolHud::draw() const {
  if( !showPoolHud) return;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 255,255,255,255/2);

  SDL_RenderFillRect(renderer, &hudFrame);

  IOmod::getInstance().
    writeText("Toggle this HUD with F1", hudFrame.x+15, hudFrame.y+10);
  IOmod::getInstance().
    writeText("Use spacebar to throw tridents at sharks!",hudFrame.x+15, hudFrame.y+85);
IOmod::getInstance().writeText("Toggle music with F2", hudFrame.x+15, hudFrame.y+35);
/*
  std::stringstream strm;
  strm << "Trident list: " << player->bulletCount();
  IOmod::getInstance().writeText(strm.str(), hudFrame.x+15, hudFrame.y+60);

  strm.clear();
  strm.str("");
  strm << "Free list: " << player->freeCount();
  IOmod::getInstance().writeText(strm.str(), hudFrame.x+15, hudFrame.y +85);*/
  IOmod::getInstance().
    writeText("Toggle god mode with g",hudFrame.x+15, hudFrame.y+60);
  IOmod::getInstance().
    writeText("Find the treasure chest!",hudFrame.x+15, hudFrame.y+110);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255/2);
  SDL_RenderDrawRect(renderer, &hudFrame); 
}
