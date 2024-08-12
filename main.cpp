#include <SDL.h>        
#include <SDL_image.h>
#include <iostream>
#include <time.h>
#include <Windows.h>


using namespace std;

void doKeyDown(SDL_KeyboardEvent* event,bool &up,bool &down,bool &left,bool &right,bool &fire)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			up = true;
		}

		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		{
			down = true;
		}

		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		{
			left = true;
		}

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			right = true;
		}
		if (event->keysym.scancode == SDL_SCANCODE_SPACE)
		{
			fire = true;
		}
	}
}

void doKeyUp(SDL_KeyboardEvent* event, bool& up, bool& down, bool& left, bool& right,bool &fire)
{
	if (event->repeat == 0)
	{
		if (event->keysym.scancode == SDL_SCANCODE_UP)
		{
			up = false;
		}

		if (event->keysym.scancode == SDL_SCANCODE_DOWN)
		{
			down = false;
		}

		if (event->keysym.scancode == SDL_SCANCODE_LEFT)
		{
			left = false;
		}

		if (event->keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			right = false;
		}
		if (event->keysym.scancode == SDL_SCANCODE_SPACE)
		{
			fire = true;
		}
	}
}
void run_all(SDL_Rect &player, bool &up, bool &down, bool &left, bool &right) {
	if (up)
	{
		player.y -= 4;
		if(player.y <=0)
		player.y += 4;
	}
	if (down)
	{
		player.y += 4;
		if(player.y>= 630)
			player.y -= 4;
	}
	if (left)
	{
		player.x -= 4;
		if (player.x <= 0)
			player.x+=4;
	}
	if (right)
	{
		player.x += 4;
		if (player.x >= 410)
			player.x -= 4;
	}
}

void wait(SDL_Event &event, bool &up, bool &down, bool &left, bool &right, bool &fire, bool &quit ) {
	//SDL_WaitEvent(&event);
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		quit = true;
		break;
	case SDL_KEYDOWN:
		doKeyDown(&event.key, up, down, left, right, fire);
		break;
	case SDL_KEYUP:
		doKeyUp(&event.key, up, down, left, right, fire);
		break;
	default:
		break;
	}
}
//, int  & bullet_health, int & test_bum, SDL_Rect& bum, int& bos2_health
void bum_intersect(SDL_Rect & rectangle_1, SDL_Rect & rectangle_2, int &rect1, int &thiss, int& rect2, int& thiss2)
{
	if (rectangle_1.x + rectangle_1.w >= rectangle_2.x + 10 &&
		rectangle_2.x + rectangle_2.w >= rectangle_1.x + 10 &&
		rectangle_1.y <= rectangle_2.y + rectangle_2.h - 15 &&
		rectangle_2.y <= rectangle_1.y + rectangle_1.h - 15
		)
	{
		if (thiss == 1)
		{
			rect1 = 1;
		}
		if (thiss2 == 1)
		{
			rect2 = 1;
		}
	}
	
}

void bos_move(SYSTEMTIME & time_bosmove, LONG &time_2,int & test_timebm_1, SDL_Rect & bos2_td)
{
	GetSystemTime(&time_bosmove);
			LONG test_timebm = (time_bosmove.wSecond * 1000) + time_bosmove.wMilliseconds;
			if (time_2<=5000)
			{
				if (test_timebm - test_timebm_1 > 1000/60 )
				{
					bos2_td.y += 1;
					bos2_td.x += 1;
					test_timebm_1 = test_timebm;
					
					if (bos2_td.x > 440 - 80)
						bos2_td.x--;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 10000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.y -= 1;
					bos2_td.x -= 1;
					test_timebm_1 = test_timebm;
					if (bos2_td.y <= 0)
						bos2_td.y += 1;
					if (bos2_td.x <= -10)
						bos2_td.x++;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 15000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.x += 2;
					test_timebm_1 = test_timebm;
					if (bos2_td.x >= 440 - 80)
						bos2_td.x -= 2;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 20000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.x -= 2;
					test_timebm_1 = test_timebm;
					if (bos2_td.x <= -10)
						bos2_td.x += 2;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 25000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.y += 1;
					bos2_td.x += 1;
					test_timebm_1 = test_timebm;
					if (bos2_td.x >= 440 - 80)
						bos2_td.x--;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 30000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.y -= 1;
					bos2_td.x -= 1;
					test_timebm_1 = test_timebm;
					if (bos2_td.y <= 0)
						bos2_td.y += 1;
					if (bos2_td.x <= -10)
						bos2_td.x++;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			if (time_2 <= 35000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.x += 2;
					test_timebm_1 = test_timebm;
					if (bos2_td.x >= 440 - 80)
						bos2_td.x -= 2;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 40000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.x -= 2;
					test_timebm_1 = test_timebm;
					if (bos2_td.x <= -10)
						bos2_td.x += 2;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 45000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.y += 1;
					bos2_td.x += 1;
					test_timebm_1 = test_timebm;
					if (bos2_td.x >= 440 - 80)
						bos2_td.x--;
					
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 50000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.y -= 1;
					bos2_td.x -= 1;
					test_timebm_1 = test_timebm;
					if (bos2_td.y <= 0)
						bos2_td.y += 1;
					if (bos2_td.x <= -10)
						bos2_td.x++;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 55000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.x += 2;
					test_timebm_1 = test_timebm;
					if (bos2_td.x >= 440 - 80)
						bos2_td.x -= 2;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
			else if (time_2 <= 60000)
			{
				if (test_timebm - test_timebm_1 > 1000 / 60)
				{
					bos2_td.x -= 2;
					test_timebm_1 = test_timebm;
					if (bos2_td.x <= -10)
						bos2_td.x += 2;
				}
				if (test_timebm > 59990)
				{
					test_timebm_1 = 0;
				}
			}
}

int main(int argc, char** argv)
{

	bool quit = false;
	SDL_Event event;
	//SDL_KeyboardEvent kevent;
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG );
	SDL_Window* window = SDL_CreateWindow("SDL2 Displaying Image",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 440, 680, 0);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Surface* image = IMG_Load("bk_space.jpg");
	SDL_Surface* image2 = IMG_Load("alien.png");
	SDL_Surface* image_bullet = IMG_Load("bullet.png");
	SDL_Surface* image_bos2 = IMG_Load("bos10.png");
	SDL_Surface* image_bum = IMG_Load("bum2.png");
	SDL_Surface* image_bum3 = IMG_Load("bum3.png");
	SDL_Surface* image_bullet2 = IMG_Load("bullet2.png");
	SDL_Surface* image_win = IMG_Load("winx.jpg");
	SDL_Surface* image_over = IMG_Load("game-over.png");

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, image2);
	SDL_Texture* texture_bullet = SDL_CreateTextureFromSurface(renderer, image_bullet);
	SDL_Texture* texture_bos2 = SDL_CreateTextureFromSurface(renderer, image_bos2);
	SDL_Texture* texture_bum = SDL_CreateTextureFromSurface(renderer, image_bum);
	SDL_Texture* texture_bum3 = SDL_CreateTextureFromSurface(renderer, image_bum3);
	SDL_Texture* texture_bullet2 = SDL_CreateTextureFromSurface(renderer, image_bullet2);
	SDL_Texture* texture_win = SDL_CreateTextureFromSurface(renderer, image_win);
	SDL_Texture* texture_over = SDL_CreateTextureFromSurface(renderer, image_over);

	SDL_Rect td;
	td.x = 0;
	td.y = 0;
	td.w = 440;
	td.h = 680;

	SDL_Rect td2;
	td2.x = 0;
	td2.y = -680;
	td2.w = 440;
	td2.h = 680;

	SDL_Rect player;
	player.x = 205;
	player.y = 600;
	player.w = 30;
	player.h = 30;
	int player_health = 3;

	SDL_Rect bullet;
	bullet.x = player.x;
	bullet.y = player.y-30;
	bullet.w = 30;
	bullet.h = 30;
	

	SDL_Rect bos2_td;
	bos2_td.x = 185;
	bos2_td.y = 100;
	bos2_td.w = 80;
	bos2_td.h = 80;
	int bos2_health = 50 ;

	SDL_Rect bullet2;
	bullet2.x = bos2_td.x + 20;
	bullet2.y = bos2_td.y +10;
	bullet2.w = 40;
	bullet2.h = 40;

	SDL_Rect bum;
	bum.x = 200;
	bum.y = 30;
	bum.w = 40;
	bum.h = 40;

	SDL_Rect bum3;
	bum3.x = 0;
	bum3.y = 0;
	bum3.w = bos2_td.w + 100;;
	bum3.h = bos2_td.h + 100;
	int bullet_dx=0, bullet_dy=0, bullet_health=0,
	bullet2_dx = 0,bullet2_dy = 26, bullet2_health = 0;

	SDL_Rect player_bum;
	player_bum.x = 0;
	player_bum.y = 0;
	player_bum.w = bos2_td.w + 100;;
	player_bum.h = bos2_td.h + 100;

	SDL_Rect life1;
	life1.x = 0;
	life1.y = 0;
	life1.w = 20;
	life1.h = 20;
	SDL_Rect life2;
	life2.x = 30;
	life2.y = 0;
	life2.w = 20;
	life2.h = 20;
	SDL_Rect life3;
	life3.x = 60;
	life3.y = 0;
	life3.w = 20;
	life3.h = 20;

	SDL_Rect wintd;
	wintd.x = 0;
	wintd.y = 200;
	wintd.w = 440;
	wintd.h = 200;

	bool up = false, down = false, left = false, right = false, fire=false;

	SYSTEMTIME timex;
	SYSTEMTIME time_bum;
	SYSTEMTIME time_bum2;

	SYSTEMTIME time_bosmove;
	int  time_1= 0;
	int time_bumdl_1 = -100000;
	int time_bumdl_2 = -100000;
	int time_win1 = -100000;
	int win_time = 1000000;
	int over_time = 1000000;

	int test_timebm_1 = 0;
	int bos_timebum = 0;
	int time_start_bullet2 = 0;
	int rect1 = 0,rect2=0;
	while (!quit)
	{
			wait(event, up, down, left, right, fire, quit);
			//SDL_Rect dstrect = { 110, 120, 320, 240 };

			SDL_RenderCopy(renderer, texture, NULL, &td);
			SDL_RenderCopy(renderer, texture, NULL, &td2);
			//SDL_RenderCopy(renderer, texture2, NULL, &player);

			if (bullet_health == 0)
			{
				bullet.x = player.x;
				bullet.y = player.y;
				bullet_dx = 0;
				bullet_dy = 20;
				bullet_health = 1;
			}

			if (bullet2_health == 0)
			{
				if (bos2_health > 0)
				{
					bullet2.x = bos2_td.x + 22;
					bullet2.y = bos2_td.y + 20;
					bullet2_dx = 0;
					bullet2_dy = 8;
					bullet2_health = 1;
				}
				
			}
		
			GetSystemTime(&timex);
			LONG time_2 = (timex.wSecond * 1000) + timex.wMilliseconds;
			
			int a2 = (int)time_2;
			
			    // fps1 
				if (a2 - time_1 > 1000 / 110)
				{
					run_all(player, up, down, left, right);
					time_1 = a2;
					bullet.x += bullet_dx;
					bullet.y -= bullet_dy;

					td.y+=3;
					td2.y+=3;
					if (td.y >= 680)
						td.y = -680;
					if (td2.y >= 680)
						td2.y = -680;

				}
				if (a2>59990 )
				{
					time_1 = 0;
				}
				//fps2_bullet2
				if (time_2 - time_start_bullet2 > 1000/120 )
				{
					bullet2.x += bullet_dx;
					bullet2.y += bullet2_dy;
					time_start_bullet2 = time_2;
				}
				if (time_2 > 59990)
				{
					time_start_bullet2 = 0;
				}
			
			//bullet.x += bullet_dx;
			//bullet.y -= bullet_dy;

			if (bullet.y < 0)
			{
				bullet_health = 0;
			}
			if (bullet2.y > 680)
			{
				bullet2_health = 0;
			}
			//int re_playerx = 0;
			// player live
			if (player_health > 0)
			{
				SDL_RenderCopy(renderer, texture2, NULL, &player);
				time_win1 = time_2;
				over_time = time_2;
				
			}
			// lose
			else {
				player_bum.x = player.x-38;
				player_bum.y = player.y-26;
				player_bum.w = 100;
				player_bum.h = 100;
				if(time_2-time_win1<300 )
				SDL_RenderCopy(renderer, texture_bum3, NULL, &player_bum);

				player.w = -100;
				player.h = -100;
			}
			//bos live
			if (bos2_health >= 0) {
				SDL_RenderCopy(renderer, texture_bos2, NULL, &bos2_td);
				bos_timebum = time_2;
				win_time = time_2;
			}
			// bos die
			else
			{
				//cout << "ok";
				bum3.x = bos2_td.x-60;
				bum3.y = bos2_td.y;
				
				if(time_2- bos_timebum <=240 && time_2 - bos_timebum>=1)
					SDL_RenderCopy(renderer, texture_bum3, NULL, &bum3);

				bos2_td.w =-100;
				bos2_td.h =-100;
			}

			//time bum
			int test_bum = 0;
			int test_bum2 = 0;

			int thiss2 = 100;
			int thiss = 1;// call function now
			bum_intersect(bullet, bos2_td,rect1,thiss,rect2,thiss2);
			thiss = 0;
			if (rect1 == 1 && player_health>0)
			{
				bum.x = bullet.x - 5;
				bum.y = bullet.y;
				bullet_health = 0;
				test_bum = 1;
				bos2_health--;
				rect1 = 0;
			}
			if (test_bum == 1) {
				GetSystemTime(&time_bum);
				LONG time_bumdl = (time_bum.wSecond * 1000) + time_bum.wMilliseconds;
				time_bumdl_1 = time_bumdl;
				test_bum == 0;
			}
			if (time_2 >= 59990)
				time_bumdl_1 = 0;
			
			GetSystemTime(&time_bum);
			LONG time_bumdl = (time_bum.wSecond * 1000) + time_bum.wMilliseconds;
			if (time_bumdl - time_bumdl_1 < 30)
				{
					SDL_RenderCopy(renderer, texture_bum, NULL, &bum);
			}
			int i = 0;

			// bosmove
			bos_move(time_bosmove, time_2, test_timebm_1, bos2_td);
			
			if (bullet_health > 0 && player_health>0)
			{
				SDL_RenderCopy(renderer, texture_bullet, NULL, &bullet);
			}
			
			if (bullet2_health > 0)
			{
				SDL_RenderCopy(renderer, texture_bullet2, NULL, &bullet2);
			}

			thiss2 = 1;// call function now
			bum_intersect(bullet2, player,rect1,thiss, rect2,thiss2);
			thiss2 = 0;
			if (rect2 == 1)
			{
				bullet2_health = 0;
				test_bum2 = 1;
				player_health--;

				rect2 = 0;
			}
			
			if (test_bum2 == 1) {
				GetSystemTime(&time_bum2);
				LONG time_bumdll = (time_bum2.wSecond * 1000) + time_bum2.wMilliseconds;
				time_bumdl_2 = time_bumdll;
				test_bum2 == 0;

			}
			GetSystemTime(&time_bum2);
			LONG time_bumdll = (time_bum2.wSecond * 1000) + time_bum2.wMilliseconds;
			if (time_bumdll - time_bumdl_2 < 150)
			{
				bum.x = player.x - 5;
				bum.y = player.y;
				SDL_RenderCopy(renderer, texture_bum, NULL, &bum);
			}
			if (player_health == 3) {
				SDL_RenderCopy(renderer, texture2, NULL, &life1);
				SDL_RenderCopy(renderer, texture2, NULL, &life2);
				SDL_RenderCopy(renderer, texture2, NULL, &life3);
			}
			else if (player_health == 2) {
				SDL_RenderCopy(renderer, texture2, NULL, &life1);
				SDL_RenderCopy(renderer, texture2, NULL, &life2);
			}
			else if (player_health == 1) {
				SDL_RenderCopy(renderer, texture2, NULL, &life1);
			}
			
			if (time_2 - win_time >= 2000)
			{
				SDL_RenderCopy(renderer, texture_win, NULL, &wintd);
				win_time = -10000;
			}
			if (time_2 - over_time >= 2000)
			{
				SDL_RenderCopy(renderer, texture_over, NULL, &wintd);
				over_time = -10000;
			}


			SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
