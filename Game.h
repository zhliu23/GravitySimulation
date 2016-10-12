#ifndef GAME_H
#define	GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include "Particle.h"

const int DEFAULT_WIDTH = 800; //800
const int DEFAULT_HEIGHT = 600; //600
const int INITIAL_SIZE = 25; //use to be PARTICLE_COUNT
const int MAXIMUM_PARTICLE = 150; //run less smooth after 150
const int MINIMUM_PARTICLE = 0;
const int MESSAGE_WIDTH = 70; 
const int MESSAGE_HEIGHT = 30;
const std::string PAUSED = "PAUSED!";

const int HELP_SIZE = 8;
const std::string HELP[HELP_SIZE] = { "TAB=TOGGLE+/-", "Q=RED", "W=GREEN", "E=BLUE", 
								"LMB=+BALL", "RMB=-BALL", "R=RESET", "ESC=QUIT" };

// use this in your gravity calculations. original: 6.67e-11
// if -11 is increase by any amount, particles starting moving too fast and exit screen.
const double G = 6.07e-11;

class Game
{
private:
	// screen info
	int width;
	int height;

	// for timing frames
	unsigned int start;
	unsigned int last;
	unsigned int current;

	// for game status
	bool good;
	bool running;
	bool paused; //pause game
	bool incrementCG; //to increment/decrment color gradient values

	// SDL managed
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* particleTexture;
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };

	// TTF managed
	TTF_Font* font;

	// Color Gradient
	int Red;
	int Green;
	int Blue;

	// game data
	std::vector<Particle> particles;

	// Handle game logic updates here
	void update(double dt);

	// Render the game contents to the screen here
	void render();

	// Handle any user input events here
	void handleEvent(const SDL_Event& event);

	// Draw particle to screen
	void drawParticle(const Particle& p);

	// Create random position, radius and mass for particles
	Particle randomParticle() const;

	// Reset the game
	void reset();

	// Draw the particle count to screen using draw()
	void drawParticleCount();

	// Draw paused! to screen using draw()
	void drawPaused();

	// Draw the background color using draw()
	void drawColor();

	// Draw help message using draw()
	void drawHelp();

	// Draw to screen
	void draw(std::string str, SDL_Rect dst);
public:
	// This will act as our initialize function
	Game();

	// We don't want to be able to copy a game
	Game(const Game& orig) = delete;
	Game& operator=(const Game& right) = delete;

	// This will act as our cleanup function
	~Game();

	// Will handle actually running the game (the gameplay loop)
	int operator()();

};

#endif	/* GAME_H */
