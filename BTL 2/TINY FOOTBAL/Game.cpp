#include "Game.h"

SDL_Renderer* Game::renderer = nullptr;
size_t Game::speed = 4;
Sound* Game::sound = nullptr;
bool Game::volumeOn = true;

/*game's components*/
Timer* timer;
Player* ronaldo;
Player* messi;
Player* bruno;
Player* mbappe;
Ball* ball;
Map* map;
Gate* gateR;
Gate* gateL;

Game::Game() : isRunning(false), window(nullptr)
{ }

Game::~Game()
{ }

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) /*SDL initialization*/
	{
		std::cout << "SDL subsystems initialized!..." << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags); /*create the window*/
		if (window != nullptr)
		{
			std::cout << "Window created!..." << std::endl;
		}
		else
		{
			std::cerr << "Unable to create window!" << std::endl;
			clean();
			exit(0);
		}
		renderer = SDL_CreateRenderer(window, -1, 0); /*create 2D render component*/
		if (renderer != nullptr)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /*white color*/
			std::cout << "Renderer created!..." << std::endl;
		}
		else
		{
			std::cerr << "Unable to create render component!" << std::endl;
			clean();
			exit(0);
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}
	resetGame = false;
	GameOver = false;

	/*set window's icon*/
	SDL_Surface* icon = IMG_Load("assets/icon.png");
	if(icon == nullptr)
	{ 
		std::cerr << "Unable to load the icon!" << std::endl;
		std::cerr << "IMG_Error: " << IMG_GetError() << std::endl;
	}
	else
	{
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}

	/*SDL_TTF library initialization*/
	if (TTF_Init() == -1)
	{
		std::cerr << "Unable to initialize SDL_TTF!" << std::endl;
		std::cerr << "TTF_Error: " << TTF_GetError() << std::endl;
	}

	/*SDL_IMG library initialization*/
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) /*initialize the library for .PNG images*/
	{
		std::cerr << "Unable to initialize SDL_IMG for PNG format!" << std::endl;
		std::cerr << "IMG_Error: " << IMG_GetError() << std::endl;
	}
	
	/*SDL_Mixer library initialization*/
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 204800) == -1) /*frequency = 44.1[kHz], number of channels = 2*/
	{
		std::cerr << "Unable to initialize SDL_Mixer!" << std::endl;
		std::cerr << "IMG_Error: " << Mix_GetError() << std::endl;
	}
	
	/*init game's components*/
	try {
		timer = new Timer("fonts/visitor.ttf");

		ronaldo = new Player("assets/ronaldo.png", new Score("fonts/visitor.ttf", 40, SCREEN_HEIGHT - 70), true);
		ronaldo->init(70.0f, SCREEN_HEIGHT - GROUND_LINE - (float)ronaldo->getBox().h);

		bruno = new Player("assets/bruno_fake.png", new Score("fonts/visitor.ttf", 40, SCREEN_HEIGHT - 70), false);
		bruno->init(170.0f, SCREEN_HEIGHT - GROUND_LINE - (float)bruno->getBox().h);

		messi = new Player("assets/messi.png", new Score("fonts/visitor.ttf", SCREEN_WIDTH - 90, SCREEN_HEIGHT - 70), true);
		messi->init(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - GROUND_LINE - (float)messi->getBox().h);

		mbappe = new Player("assets/mbappe_fake.png", new Score("fonts/visitor.ttf", SCREEN_WIDTH - 90, SCREEN_HEIGHT - 70), false);
		mbappe->init(SCREEN_WIDTH - 250.0f, SCREEN_HEIGHT - GROUND_LINE - (float)mbappe->getBox().h);

		ball = new Ball("assets/ball.png");
		ball->init((SCREEN_WIDTH - ball->getBox().w) / 2.0f, (SCREEN_HEIGHT - ball->getBox().h) / 2.0f);

		map = new Map("assets/map.png");

		gateR = new Gate("assets/gate_right.png");
		gateR->init(SCREEN_WIDTH - (float)gateR->getBox().w + 30.f, SCREEN_HEIGHT - GROUND_LINE - (float)gateR->getBox().h);

		gateL = new Gate("assets/gate_left.png");
		gateL->init(-30.0f, SCREEN_HEIGHT - GROUND_LINE - (float)gateL->getBox().h);

		sound = new Sound();
		Mix_PlayMusic(sound->music_background, -1);	/*play background music*/
	}
	catch (std::bad_alloc &e)
	{
		std::cerr << e.what() << std::endl;
		exit(0);
	}
	
}

/*handle inputs*/
void Game::handleEvents()
{
	SDL_Event event; /*an event is a union*/
	SDL_PollEvent(&event); /*extracts an event from the queue of events*/
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	/*a key was pressed*/
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
			/*keys control for player A*/
		case SDLK_1:
			ronaldo->isEnable = true;
			ronaldo->setTexture("assets/ronaldo.png");
			bruno->isEnable = false;
			bruno->setTexture("assets/bruno_fake.png");
			break;
		case SDLK_2:
			ronaldo->isEnable = false;
			ronaldo->setTexture("assets/ronaldo_fake.png");
			bruno->isEnable = true;
			bruno->setTexture("assets/bruno.png");
			break;
		case SDLK_KP_1:
			messi->setTexture("assets/messi.png");
			messi->isEnable = true;
			mbappe->setTexture("assets/mbappe_fake.png");
			mbappe->isEnable = false;
			break;
		case SDLK_KP_2:
			mbappe->setTexture("assets/mbappe.png");
			mbappe->isEnable = true;
			messi->setTexture("assets/messi_fake.png");
			messi->isEnable = false;
			break;
		case SDLK_w:
			if (!ronaldo->isJumping && !ronaldo->isFalling)
			{
				ronaldo->isJumping = true;
				ronaldo->isFalling = false;
			}
			if (!bruno->isJumping && !bruno->isFalling)
			{
				bruno->isJumping = true;
				bruno->isFalling = false;
			}
			break;
		case SDLK_a:
			ronaldo->velocity.x = -1;
			ronaldo->isRunning = true;
			
			bruno->velocity.x = -1;
			bruno->isRunning = true;

			break;
		case SDLK_d:
			ronaldo->velocity.x = 1;
			ronaldo->isRunning = true;
			
			bruno->velocity.x = 1;
			bruno->isRunning = true;
			
			break;
		case SDLK_s:
			ronaldo->isShooting = true;
			bruno->isShooting = true;
			break;
		case SDLK_z:
			ronaldo->isPowerShoot = true;
			bruno->isPowerShoot = true;
			break;
		case SDLK_m: /*pause/resume sounds*/
			sound->changeState();
			break;
		case SDLK_UP:
			if (!messi->isJumping && !messi->isFalling)
			{
				messi->isJumping = true;
				messi->isFalling = false;
			}
			if (!mbappe->isJumping && !mbappe->isFalling)
			{
				mbappe->isJumping = true;
				mbappe->isFalling = false;
			}
			break;
		case SDLK_LEFT:
			messi->velocity.x = -1;
			messi->isRunning = true;
			
			mbappe->velocity.x = -1;
			mbappe->isRunning = true;
			
			break;
		case SDLK_RIGHT:
			messi->velocity.x = 1;
			messi->isRunning = true;
			
			mbappe->velocity.x = 1;
			mbappe->isRunning = true;
			
			break;
		case SDLK_DOWN:
			messi->isShooting = true;
			mbappe->isShooting = true;
			break;
		case SDLK_KP_0:
			messi->isPowerShoot = true;
			mbappe->isPowerShoot = true;
			break;
	}
	break;
	/*a key was released*/
	case SDL_KEYUP:
		switch (event.key.keysym.sym)
		{
			/*keys control for player A*/
		case SDLK_w:
			ronaldo->velocity.y = 0;
			bruno->velocity.y = 0;
			break;
		case SDLK_s:
			ronaldo->isShooting = false;
			bruno->isShooting = false;
			break;
		case SDLK_a:
			ronaldo->velocity.x = 0;
			ronaldo->isRunning = false;
			bruno->velocity.x = 0;
			bruno->isRunning = false;
			break;
		case SDLK_d:
			ronaldo->velocity.x = 0;
			ronaldo->isRunning = false;
			bruno->velocity.x = 0;
			bruno->isRunning = false;
			break;
		case SDLK_z:
			ronaldo->isPowerShoot = false;
			bruno->isPowerShoot = false;
			break;
			/*keys control for player B*/
		case SDLK_UP:
			messi->velocity.y = 0;
			mbappe->velocity.y = 0;
			break;
		case SDLK_DOWN:
			messi->isShooting = false;
			mbappe->isShooting = false;
			break;
		case SDLK_LEFT:
			messi->velocity.x = 0;
			messi->isRunning = false;
			mbappe->velocity.x = 0;
			mbappe->isRunning = false;
			break;
		case SDLK_RIGHT:
			messi->velocity.x = 0;
			messi->isRunning = false;
			mbappe->velocity.x = 0;
			mbappe->isRunning = false;
			break;
		case SDLK_KP_0:
			messi->isPowerShoot = false;
			mbappe->isPowerShoot = false;
			break;
		}
		break;
	}
}

/*update game's components*/
void Game::update()
{
	if (timer->update()) /*time is over*/
	{
		timer->stop();
		if (ronaldo->scoreBoard->getScore() < messi->scoreBoard->getScore()) /*PSG is the winner, Game is over*/
		{
			winner1 = messi;
			winner2 = mbappe;
			GameOver = true;
		}			
		else if (ronaldo->scoreBoard->getScore() > messi->scoreBoard->getScore())	/*MU is the winner, Game is over*/
		{
			winner1 = ronaldo;
			winner2 = bruno;
			GameOver = true;
		}
		else /*draw*/
		{
			goldenGoal = true; /*start golden goal mode*/
			map->setTexture("assets/map_golden.png");
		}
	}
	if (Collision::AABB(ronaldo->getBox(), ball->getBox()))
	{ 
		std::cout << "Ronaldo hits the ball!" << std::endl;
		if (ronaldo->isEnable) 
		{ 
			ronaldo->hitsTheBall(ball); 
		}
	}
	if (Collision::AABB(messi->getBox(), ball->getBox()))
	{
		std::cout << "Messi hits the ball!" << std::endl;
		if (messi->isEnable)
		{
			messi->hitsTheBall(ball);
		}
	}
	if (Collision::AABB(bruno->getBox(), ball->getBox()))
	{
		std::cout << "Bruno hits the ball!" << std::endl;
		if (bruno->isEnable)
		{
			bruno->hitsTheBall(ball);
		}
	}
	if (Collision::AABB(mbappe->getBox(), ball->getBox()))
	{
		std::cout << "Mbappe hits the ball!" << std::endl;
		if (mbappe->isEnable)
		{
			mbappe->hitsTheBall(ball);
		}
	}
	if (Collision::AABB_GoalR(ball->getBox(), gateR->getBox()))
	{
		std::cout << "GOAL for MU !!!!!" << std::endl;
		timer->pause();
		if (volumeOn == true)
		{
			Mix_PlayChannel(-1, Game::sound->goal, 0);	/*play sound effect for goals*/
		}
		map->setTexture("assets/map_goal.png"); /*update map*/
		ronaldo->scoreBoard->increment();	/*update scoreboard*/
		if (goldenGoal) /*golden goal mode is active*/
		{
			winner1 = ronaldo;
			winner2 = bruno;
			GameOver = true;
		}
		else
		{
			resetGame = true;
		}
	}
	if (Collision::AABB_GoalL(ball->getBox(), gateL->getBox()))
	{
		std::cout << "GOAL for PSG !!!!!" << std::endl;
		timer->pause();
		if (volumeOn == true)
		{
			Mix_PlayChannel(-1, Game::sound->goal, 0);	/*play sound effect for goals*/
		}
		map->setTexture("assets/map_goal.png");	/*update map*/
		messi->scoreBoard->increment(); /*update scoreboard*/
		if (goldenGoal) /*golden goal mode is active*/
		{
			winner1 = messi;
			winner2 = mbappe;
			GameOver = true;
		}
		else
		{
			resetGame = true;
		}
	}
	if (Collision::AABB_GateL(ball->getBox(), gateL->getBox())) /*the ball hits the crossbar of left gate*/
	{
		ball->velocity.y = -ball->velocity.y - 0.7f;		/*the ball is rebounded*/
		ball->velocity.x += 1.0f;						/*the ball is forced to return on the field*/
	}
	if (Collision::AABB_GateR(ball->getBox(), gateR->getBox())) /*the ball hits the crossbar of right gate*/
	{
		ball->velocity.y = -ball->velocity.y - 0.7f;		/*the ball is rebounded*/
		ball->velocity.x += -1.0f;						/*the ball is forced to return on the field*/
	}

	ball->update();
	ronaldo->update();
	messi->update();
	bruno->update();
	mbappe->update();
	/*display information in console*/
	/*std::cout << "Ball: " << (*ball);
	std::cout << "Ronaldo: " << (*ronaldo);
	std::cout << "Bruno: " << (*bruno);
	std::cout << "Messi: " << (*messi);
	std::cout << "Mbappe: " << (*mbappe) << std::endl;*/
}

/*display game's components*/
void Game::render()
{
	SDL_RenderClear(renderer); /*clear window to the setted color*/
	map->draw();
	timer->draw();
	ball->draw();
	ronaldo->draw();
	bruno->draw();
	ronaldo->scoreBoard->draw();
	messi->draw();
	mbappe->draw();
	messi->scoreBoard->draw();
	gateR->draw();
	gateL->draw();

	SDL_RenderPresent(renderer); /*update window*/
}

/*delete game's components*/
void Game::clean()
{
	if (timer != nullptr)
	{
		delete timer;
		timer = nullptr;
	}
	if (ronaldo != nullptr)
	{
		delete ronaldo;
		ronaldo = nullptr;
	}
	if (messi != nullptr)
	{
		delete messi;
		messi = nullptr;
	}
	if (ball != nullptr)
	{
		delete ball;
		ball = nullptr;
	}
	if (gateL != nullptr)
	{
		delete gateL;
		gateL = nullptr;
	}
	if (gateR != nullptr)
	{
		delete gateR;
		gateR = nullptr;
	}
	if (map != nullptr)
	{
		delete map;
		map = nullptr;
	}
	if (winner1 != nullptr)
	{
		delete winner1;
		winner1 = nullptr;
	}
	if (winner2 != nullptr)
	{
		delete winner2;
		winner2 = nullptr;
	}
	if (sound != nullptr)
	{
		delete sound;
		sound = nullptr;
	}

	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();

	std::cout << "Game cleaned!..." << std::endl;
}

/*reinit the game after a player scored*/
void Game::reset()
{
	resetGame = false;
	GameOver = false;

	if (ronaldo->isEnable)
	{
		ronaldo->setTexture("assets/ronaldo.png");
	}
	else ronaldo->setTexture("assets/ronaldo_fake.png");
	ronaldo->init(70.0f, SCREEN_HEIGHT - GROUND_LINE - (float)ronaldo->getBox().h);

	if (messi->isEnable)
	{
		messi->setTexture("assets/messi.png");
	}
	else messi->setTexture("assets/messi_fake.png");
	messi->init(SCREEN_WIDTH - 150.0f, SCREEN_HEIGHT - GROUND_LINE - (float)messi->getBox().h);

	if (bruno->isEnable)
	{
		bruno->setTexture("assets/bruno.png");
	}
	else bruno->setTexture("assets/bruno_fake.png");
	bruno->init(170.0f, SCREEN_HEIGHT - GROUND_LINE - (float)bruno->getBox().h);

	if (mbappe->isEnable)
	{
		mbappe->setTexture("assets/mbappe.png");
	}
	else mbappe->setTexture("assets/mbappe_fake.png");
	mbappe->init(SCREEN_WIDTH - 250.0f, SCREEN_HEIGHT - GROUND_LINE - (float)mbappe->getBox().h);

	ball->init((SCREEN_WIDTH - ball->getBox().w) / 2.0f, (SCREEN_HEIGHT - ball->getBox().h) / 2.0f);
	map->setTexture("assets/map.png");
	timer->resume();
}

/*display the menu and handle input*/
void Game::showMenu()
{
	backToMenu = false;
	bool flag = true;
	struct {
		int x;
		int y;
	} mouse; /*mouse position*/
	SDL_Event event;
	Button* startBtn = new Button("assets/start_button.png", 400, 240);
	Button* exitBtn = new Button("assets/exit_button.png", 400, 340);
	SDL_Texture* background = TextureManager::LoadTexture("assets/background.png");
	
	while (flag)
	{
		/*display background and buttons*/
		SDL_RenderClear(renderer); 
		TextureManager::DrawTexture(background);
		startBtn->draw();
		exitBtn->draw();
		SDL_RenderPresent(renderer);

		/*handle user input*/
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			flag = false;
			break;
		case SDL_MOUSEMOTION:
			mouse.x = event.motion.x;
			mouse.y = event.motion.y;
			if (Collision::AABB(startBtn, mouse.x, mouse.y))
			{
				startBtn->mouseOver();
			}
			else
			{
				startBtn->mouseOut();
			}
			if (Collision::AABB(exitBtn, mouse.x, mouse.y))
			{
				exitBtn->mouseOver();
			}
			else
			{
				exitBtn->mouseOut();
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse.x = event.motion.x;
			mouse.y = event.motion.y;
			if (volumeOn)
			{
				Mix_PlayChannel(-1, sound->click, 0); /*sound effect on click*/
			}
			if (Collision::AABB(exitBtn, mouse.x, mouse.y))
			{
				isRunning = false;
				flag = false;
			}
			if (Collision::AABB(startBtn, mouse.x, mouse.y))
			{
				isRunning = true;
				goldenGoal = false;
				flag = false;
			}
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_m) /*pause/resume sounds*/
			{
				sound->changeState();
			}
		}
	}
	
	/*show instructions before game start*/
	if (isRunning)
	{
		SDL_RenderClear(renderer);
		SDL_Texture* infoPage = TextureManager::LoadTexture("assets/instructions.png");
		TextureManager::DrawTexture(infoPage);
		SDL_RenderPresent(renderer);
		Mix_PlayMusic(sound->music_playing, -1);	/*play playing music*/
		while (!flag)
		{
			SDL_PollEvent(&event);
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				flag = true;
				break;
			case SDL_KEYDOWN: /*press any key to continue*/
				flag = true;
				if (volumeOn)
				{
					Mix_PlayChannel(-1, sound->click, 0); /*sound effect*/
				}
				timer->start();	/*start timer*/
				break;
			}
		}
		if (infoPage != nullptr)
		{
			SDL_DestroyTexture(infoPage);
			infoPage = nullptr;
		}
	}

	/*delete menu's components*/
	if (startBtn != nullptr)
	{
		delete startBtn;
		startBtn = nullptr;
	}
	if (exitBtn != nullptr)
	{
		delete exitBtn;
		exitBtn = nullptr;
	}
	if (background != nullptr)
	{
		SDL_DestroyTexture(background);
		background = nullptr;
	}
}

/*show the winner when game is over*/
void Game::showWinner()
{
	bool flag = true;
	struct {
		int x;
		int y;
	} mouse; /*mouse position*/
	SDL_Event event;
	Button* backBtn = new Button("assets/back_button.png", SCREEN_WIDTH / 2 - 109, SCREEN_HEIGHT - 86);
	
	if (winner1 == ronaldo)
	{
		winner1->setTexture("assets/ronaldo.png");
		winner2->setTexture("assets/bruno.png");
	}		
	else
	{
		winner1->setTexture("assets/messi.png");
		winner2->setTexture("assets/mbappe.png");
	}		
	winner1->init((SCREEN_WIDTH - winner1->getBox().w) / 2.0f - 50.0f, SCREEN_HEIGHT - GROUND_LINE - winner1->getBox().h - 82.0f);
	winner2->init((SCREEN_WIDTH - winner2->getBox().w) / 2.0f + 30.0f, SCREEN_HEIGHT - GROUND_LINE - winner2->getBox().h - 82.0f);
	map->setTexture("assets/map_winner.png");
	if (volumeOn)
	{
		Mix_PlayChannel(-1, sound->win, 0); /*sound effect for winner*/
	}

	while (flag)
	{
		SDL_RenderClear(renderer);
		map->draw();
		winner1->draw();
		winner2->draw();
		ronaldo->scoreBoard->draw();
		messi->scoreBoard->draw();
		backBtn->draw();
		SDL_RenderPresent(Game::renderer);

		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			flag = false;
			break;
		case SDL_MOUSEMOTION:
			mouse.x = event.motion.x;
			mouse.y = event.motion.y;
			if (Collision::AABB(backBtn, mouse.x, mouse.y))
			{
				backBtn->mouseOver();
			}
			else
			{
				backBtn->mouseOut();
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse.x = event.motion.x;
			mouse.y = event.motion.y;
			if (volumeOn)
			{
				Mix_PlayChannel(-1, sound->click, 0); /*sound effect on click*/
			}			
			if (Collision::AABB(backBtn, mouse.x, mouse.y))
			{
				isRunning = false;
				backToMenu = true;
				flag = false;
			}
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_m)  /*pause/resume sounds*/
			{
				sound->changeState();
			}
		}
	}

	/*reset the game*/
	winner1 = nullptr;
	winner2 = nullptr;
	ronaldo->scoreBoard->resetScore();
	messi->scoreBoard->resetScore();
	reset();

	/*delete back button*/
	if (backBtn != nullptr)
	{
		delete backBtn;
		backBtn = nullptr;
	}
}

/*return game states*/
bool Game::running() const
{
	return isRunning;
}

bool Game::goReset() const
{
	return resetGame;
}

bool Game::isGameOver() const
{
	return GameOver;
}

bool Game::isGoingBack() const
{
	return backToMenu;
}
