#include <cmath>
#include <cstdlib>
#include <ctime>

#include "Game.h"

Game::Game() : start(0), last(0), current(0), good(true), running(false),
width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT), particles(std::vector<Particle>())
{
	// Seed the random number generator
	srand(time(0));

	// initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init = FAILED" << std::endl;
		good = false;
		return;
	}

	// initialize TTF
	if (TTF_Init() != 0)
	{
		std::cout << "TTF_Init = FAILED" << std::endl;
		good = false;
		return;
	}

	// initialize SDL window
	window = SDL_CreateWindow("Gravity     P = Pause/Help", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		std::cout << "CreateWindow = FAILED" << std::endl;
		good = false;
		return;
	}

	// initialize SDL renderer
	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		std::cout << "CreateRenderer = FAILED" << std::endl;
		good = false;
		return;
	}

	// create font and size
	font = TTF_OpenFont("emulogic.ttf", 14);
	if (font == NULL)
	{
		std::cout << "TTF_OpenFont = FAILED" << std::endl;
		good = false;
		return;
	}

	// initialize particle texture
	SDL_Surface* bmp = SDL_LoadBMP("particle.bmp");
	if (bmp == NULL)
	{
		std::cout << "LoadBMP = FAILED" << std::endl;
		good = false;
		return;
	}
	particleTexture = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);
	if (particleTexture == NULL)
	{
		std::cout << "CreateTextureFromSurface = FAILED" << std::endl;
		good = false;
		return;
	}

	// initialize our particles
	for (int i = 0; i < INITIAL_SIZE; ++i)
	{
		particles.push_back(randomParticle());
	}

	// initialize color
	Red = 106;
	Green = 90;
	Blue = 205;
}

Game::~Game()
{
	if (!good)
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
	}
	if (particleTexture != NULL)
	{
		SDL_DestroyTexture(particleTexture);
	}
	if (renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}
	TTF_Quit();
	SDL_Quit();
}

int Game::operator()()
{
	if (!good)
	{
		return -1;
	}
	running = true;
	paused = false;
	incrementCG = true; //default to increment color gradient

	SDL_Event event;
	SDL_SetRenderDrawColor(renderer, Red, Green, Blue, 255); //Slate Blue as default background color
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	start = SDL_GetTicks();
	last = start;

	while (running) // every iteration is 1 frame
	{
			current = SDL_GetTicks();
			while (SDL_PollEvent(&event))
			{
				handleEvent(event);
			}
			if (paused != true) //if game is paused, update will not be call
				update((current - last) / 1000.0);
			render();
			last = current;
	}
	return 0;
}

void Game::reset()
{
	particles.clear();
	for (int i = 0; i < INITIAL_SIZE; ++i)
		particles.push_back(randomParticle());
}

void Game::drawParticleCount()
{
	SDL_Rect dst;
	dst.x = DEFAULT_WIDTH - MESSAGE_WIDTH;
	dst.y = 0;
	dst.w = MESSAGE_WIDTH;
	dst.h = MESSAGE_HEIGHT;

	std::ostringstream os;
	os << particles.size();
	std::string str = "Count: " + os.str();

	draw(str, dst);
}

void Game::drawPaused()
{
	SDL_Rect dst;
	dst.x = DEFAULT_WIDTH - MESSAGE_WIDTH;
	dst.y = MESSAGE_HEIGHT;
	dst.w = MESSAGE_WIDTH;
	dst.h = MESSAGE_HEIGHT;

	draw(PAUSED, dst);
}

void Game::drawColor()
{
	SDL_SetRenderDrawColor(renderer, Red, Green, Blue, 255);

	SDL_Rect dst;
	dst.x = 0;
	dst.y = 0;
	dst.w = MESSAGE_WIDTH * 3;
	dst.h = MESSAGE_HEIGHT;

	std::ostringstream os;
	os << "Red:" << Red << " Green:" << Green << " Blue:" << Blue;
	std::string str = os.str();

	draw(str, dst);
}

void Game::drawHelp()
{
	std::string str;
	SDL_Rect dst;
	SDL_Surface *message;
	SDL_Texture* fontTexture;

	dst.x = DEFAULT_WIDTH -  MESSAGE_WIDTH;
	dst.w = MESSAGE_WIDTH;
	dst.h = MESSAGE_HEIGHT;

	for (int i = 0; i < HELP_SIZE; i++)
	{
		dst.y = MESSAGE_HEIGHT * (2 + i);
		draw(HELP[i], dst);
	}
}

void Game::draw(std::string str, SDL_Rect dst)
{
	SDL_Surface *message = TTF_RenderText_Solid(font, str.c_str(), black);
	if (message == NULL)
	{
		std::cout << "RenderText_Solid = FAILED" << std::endl;
		return;
	}

	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, message);
	SDL_FreeSurface(message);
	SDL_RenderCopy(renderer, fontTexture, NULL, &dst);
	SDL_DestroyTexture(fontTexture);
}

void Game::update(double dt)
{
	std::cout << dt << " sec since last frame.\n";

	// Replace with your game logic!
	
	//Force due to gravity
	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = i; j < particles.size(); j++)
		{
			if (i != j)
			{
				double accel = G * (particles[j].getMass() / (pow(particles[i].getPos().getDistance(particles[j].getPos()), 2)));
				double angleRad = particles[i].getPos().getAngleRad(particles[j].getPos());				
				
				double accelX = accel * cos(angleRad);
				double accelY = accel * sin(angleRad);
				
				double velocX = particles[i].getVel().getX() + (accelX * dt);
				double velocY = particles[i].getVel().getY() + (accelY * dt);
				particles[i].newVelXY(velocX, velocY);
			}
		}
	}

	//Elastic collisions
	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = i; j < particles.size(); j++)
		{
			if (i != j)
			{
				if (particles[i].collide(particles[j]))
				{
					Point newVel1(particles[i].calcVelElastic(particles[j]));
					Point newVel2(particles[j].calcVelElastic(particles[i]));

					particles[i].newVelXY(newVel1.getX(), newVel1.getY());
					particles[j].newVelXY(newVel2.getX(), newVel2.getY());
				}
			}
		}
	}

	//Boundary Check
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i].getPos().getX() <= particles[i].getRadius() || particles[i].getPos().getX() >= DEFAULT_WIDTH - particles[i].getRadius())
			particles[i].reverseVelX();

		if (particles[i].getPos().getY() <= particles[i].getRadius() || particles[i].getPos().getY() >= DEFAULT_HEIGHT - particles[i].getRadius())
			particles[i].reverseVelY();	
	}

	//Change Position
	for (int i = 0; i < particles.size(); i++)
		particles[i].calcPos(dt);

}

void Game::render()
{
	SDL_RenderClear(renderer);

	
	// rendering here would place objects beneath the particles
	for (const Particle& p : particles)
	{
		drawParticle(p);
	}

	drawParticleCount();
	drawColor();

	if (paused == true)
	{
		drawPaused();
		drawHelp();
	}
	// rendering here would place objects on top of the particles

	SDL_RenderPresent(renderer);
}

void Game::handleEvent(const SDL_Event& event)
{
	switch (event.type)
	{
		// Add your own event handling here if desired
	case SDL_QUIT:
		running = false;
		break;
	case SDL_MOUSEBUTTONDOWN: //Add or remove particles on screen
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			if(particles.size() < MAXIMUM_PARTICLE && paused != true)
				particles.push_back(randomParticle());
		}
		else if (event.button.button == SDL_BUTTON_RIGHT && paused != true)
		{
			if(particles.size() == MINIMUM_PARTICLE)
				running = false;
			else
				particles.pop_back();
		}
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_p)
		{
			if (paused == true)
				paused = false;
			else
				paused = true;
		}
		if (event.key.keysym.sym == SDLK_r)
			reset();
		if (event.key.keysym.sym == SDLK_ESCAPE)
			running = false;
		if (event.key.keysym.sym == SDLK_q)
		{
			if (incrementCG == true && Red < 255)
				Red++;
			else if (incrementCG == false && Red > 0)
				Red--;
		}
		if (event.key.keysym.sym == SDLK_w)
		{
			if (incrementCG == true && Green < 255)
				Green++;
			else if (incrementCG == false && Green > 0)
				Green--;
		}
		if (event.key.keysym.sym == SDLK_e)
		{
			if (incrementCG == true && Blue < 255)
				Blue++;
			else if (incrementCG == false && Blue > 0)
				Blue--;
		}
		if (event.key.keysym.sym == SDLK_TAB)
		{
			if (incrementCG == true)
				incrementCG = false;
			else
				incrementCG = true;
		}
		break; 
	default:
		break;
	}
}

void Game::drawParticle(const Particle& p)
{
	SDL_Rect dst;
	double shift = p.getRadius();
	dst.x = (int)(p.getPos().getX() - shift);
	dst.y = (int)(p.getPos().getY() - shift);
	dst.w = shift * 2;
	dst.h = shift * 2;
	SDL_RenderCopy(renderer, particleTexture, NULL, &dst);
}

Particle Game::randomParticle() const
{
	Point pos(rand() % width, rand() % height);
	// using some percentage of the mass of Jupiter. Original:1.9e27 Best:1.9e15
	double mass = ((double)rand() / RAND_MAX) * 1.9e15 + 1.0;
	return Particle(pos, mass);
}