#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twowaymultisprite.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"


Engine::~Engine() { 
  delete player;
  delete treasure; 
  for( auto s : JellyFish){
    delete(s);
  }
  for( auto s : Sharks){
    delete(s);
  }
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  water("water", Gamedata::getInstance().getXmlInt("water/factor") ),
  sand("sand", Gamedata::getInstance().getXmlInt("sand/factor") ),
  viewport( Viewport::getInstance() ),
  HudVisible(true),
  HudHasFaded(false),
  JellyFish(),
  Sharks(),
  strategies(),
  treasure(new Sprite("treasure")),
  player(new Player("boy")),
  poolhud(player),
  currentStrategy(0),
  currentSprite(0),
  collision(false),
  godMode(false),
  sound(),
  makeVideo( false )
{


  int nj = Gamedata::getInstance().getXmlInt("numberOfJelly");
  JellyFish.reserve(nj);
  //Vector2f pos = player->getPosition();
  //int w = player->getScaledWidth();
  //int h = player->getScaledHeight();
  for (int i = 0; i < nj; ++i) {
    JellyFish.push_back( new Sprite("jellyfish") );
    //player->attach( SmartSprites[i] );
  } 

  int ns = Gamedata::getInstance().getXmlInt("numberOfSharks");
  Sharks.reserve(ns);

  for (int i = 0; i <ns; i++){
    Sharks.push_back(new TwoWayMultiSprite("shark"));
  }

  for(Drawable *s : Sharks) {
        //give each star a pseudorandom velocity
      s->setVelocity(Vector2f(
          (std::rand()%130)+50, (std::rand()%130)+50));
      s->setPosition(Vector2f(
          (std::rand()%(Gamedata::getInstance().getXmlInt("world/width"))), (std::rand()%(Gamedata::getInstance().getXmlInt("world/height")))));
  }
  for(Drawable *s : JellyFish) {
      s->setScale(Gamedata::getInstance().getRandFloat(0.5,2));
        //give each star a pseudorandom velocity
      s->setVelocity(Vector2f(
          (std::rand()%60)+10, (std::rand()%60)+10));
      s->setPosition(Vector2f(
          (std::rand()%(Gamedata::getInstance().getXmlInt("world/width"))), (std::rand()%(Gamedata::getInstance().getXmlInt("world/height")))));
  }

  treasure->setPosition(Vector2f(
		((Gamedata::getInstance().getXmlInt("treasure/startLoc/x"))), 	
		((Gamedata::getInstance().getXmlInt("treasure/startLoc/y")))));

  strategies.push_back( new PerPixelCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  water.draw();
  sand.draw();

  for( auto s : Sharks){
    s->draw();
  }
  for( auto s : JellyFish){
    s->draw();
  }

/*  strategies[currentStrategy]->draw();
  if ( collision ) {
    IOmod::getInstance().writeText("Collision!", 500, 90);
  }*/
  if(findTreasure()){
    io.writeText("Congrats! You found the treasure!", 250,200);
    io.writeText("Press R to Restart the game", 250,250);
    clock.pause();
  }
  if (godMode){
    io.writeText("God Mode", 250,50);
  }
  
  treasure->draw();
  player->draw();
  viewport.draw();
  if (HudVisible) poolhud.draw();
  SDL_RenderPresent(renderer);
}


void Engine::checkForCollisions() {
  auto it = Sharks.begin();
  while ( it != Sharks.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      player->explode();
      sound[0];
      //it = Sharks.erase(it);
      ++it;
    }
    else ++it;
  }
}

bool Engine::findTreasure() const{
  if(strategies[currentStrategy]->execute(*player, *treasure)){
    return true;
  }
  return false;
}

void Engine::update(Uint32 ticks) {
  if(!godMode) checkForCollisions();
  player->IsSharkShot(Sharks);
  player->update(ticks);
  if(findTreasure()) sound[1];
  for( auto s : Sharks){
    s->update(ticks);
  }
  for( auto s : JellyFish){
    s->update(ticks);
  }
  water.update();
  sand.update();
  viewport.update();
 // always update viewport last
}
/*
void Engine::switchSprite(){
  ++currentSprite;
  int current = currentSprite % 3;
  Viewport::getInstance().setObjectToTrack(Sprites.at(current));
}*/

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDL_SCANCODE_F1]) {
            if (HudVisible)  poolhud = NULL;
            else poolhud = PoolHud(player);
            HudVisible = !HudVisible;
        }
	if (keystate[SDL_SCANCODE_F2])
	    sound.toggleMusic();
        if (keystate[SDL_SCANCODE_SPACE]){
            player->shoot();
        }
        if (keystate[SDL_SCANCODE_E]){
            player->explode();
        }
        if (keystate[SDL_SCANCODE_R]){
          if ( clock.isPaused() ) clock.unpause();
	    return true;
	}
        if ( keystate[SDL_SCANCODE_G] ) {
          godMode = !godMode;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

      if(clock.getTicks() >7000 && !HudHasFaded){
            if (HudVisible)  poolhud = NULL;
            else poolhud = PoolHud(player);
            HudVisible = !HudVisible;
            HudHasFaded = true;
      }


    // In this section of the event loop we allow key bounce:
      ticks = clock.getElapsedTicks();
      if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }

      draw();
      update(ticks);	
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
