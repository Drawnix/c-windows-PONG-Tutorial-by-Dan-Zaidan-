#define is_down(b) input -> buttons[b].is_down
#define pressed(b) (input -> buttons[b].is_down && input -> buttons[b].changed)
#define released(b) (!input -> buttons[b].is_down && input -> buttons[b].changed)

//PLAYER RELATIVES
float player_1_p, player_1_dp, player_2_p, player_2_dp;
float player_halfsize_x = 2.5, player_halfsize_y = 12;
int player_1_score, player_2_score;
float player_speed = 10.f;

//ARENA SIZE
float arena_halfsize_x = 85, arena_halfsize_y = 45;

//BALL RELATIVES
float ball_p_x = 0.f, ball_p_y = 0.f, ball_dp_x = 10, ball_dp_y;
float ball_halfsize = 2;

internal void simulate_player(float *p, float *dp, float ddp, float dt)
{
	ddp -= *dp * 1.5f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_halfsize_y > arena_halfsize_y)
	{
		*p = arena_halfsize_y - player_halfsize_y;
		*dp = -*dp;
	}
	else if (*p - player_halfsize_y < -arena_halfsize_y)
	{
		*p = -arena_halfsize_y + player_halfsize_y;
		*dp = -*dp;
	}
}

internal bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y)
{
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

enum GameMode
{
	GM_MENU,
	GM_GAMEPLAY
};

GameMode current_gamemode;

int hot_button = 0;
bool enemy_is_ai;

internal void simulate_game(Input* input, float delta)
{
	Clear_Screen(0xEB6821);	//BACKGROUND
	//Draw_Rect(0, 0, 87, 45, 0xeb9b34);	//SCREEN 
	Draw_Rect(0, 0, arena_halfsize_x, arena_halfsize_y, 0xeb9b34);	//SCREEN 

	float player_1_ddp = 0.f;
	float player_2_ddp = 0.f;

	if (current_gamemode == GM_GAMEPLAY)
	{
		if (!enemy_is_ai)
		{
			if (is_down(BUTTON_UP))
			{
				player_1_ddp += player_speed;
			}
			if (is_down(BUTTON_DOWN))
			{
				player_1_ddp -= player_speed;
			}
		}
		
		else
		{
			player_1_ddp = (ball_p_y - player_1_p);
			if (player_1_ddp > 8)
			{
				player_1_ddp = 8;
			}
			if (player_1_ddp < -8)
			{
				player_1_ddp = -8;
			}
		}
		
		if (is_down(BUTTON_W))
		{
			player_2_ddp += player_speed;
		}
		if (is_down(BUTTON_S))
		{
			player_2_ddp -= player_speed;
		}

		simulate_player(&player_1_p, &player_1_dp, player_1_ddp, delta);
		simulate_player(&player_2_p, &player_2_dp, player_2_ddp, delta);


		ball_p_x += ball_dp_x * delta;
		ball_p_y += ball_dp_y * delta;

		Draw_Rect(80, player_1_p, player_halfsize_x, player_halfsize_y, 0xEB5B34);	//PLAYER 1
		Draw_Rect(-80, player_2_p, player_halfsize_x, player_halfsize_y, 0xEB5B34);	//PLAYER 2

		if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_halfsize, ball_halfsize, 80, player_1_p, player_halfsize_x, player_halfsize_y))
		{
			ball_p_x = (arena_halfsize_x - 5) - player_halfsize_x - ball_halfsize;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_1_p) / 2.5 + player_1_dp * 0.75f;
		}
		else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_halfsize, ball_halfsize, -80, player_2_p, player_halfsize_x, player_halfsize_y))
		{
			ball_p_x = -(arena_halfsize_x - 5) + player_halfsize_x + ball_halfsize;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_2_p) / 2.5 + player_2_dp * 0.75f;
		}

		if (ball_p_y + ball_halfsize > arena_halfsize_y)
		{
			ball_p_y = arena_halfsize_y - ball_halfsize;
			ball_dp_y *= -1;

		}
		else if (ball_p_y - ball_halfsize < -arena_halfsize_y)
		{
			ball_p_y = -arena_halfsize_y + ball_halfsize;
			ball_dp_y *= -1;

		}

		if (ball_p_x + ball_halfsize > arena_halfsize_x)
		{
			ball_dp_x *= -1;
			ball_dp_y = 0;

			ball_p_x = 0;
			ball_p_y = 0;
			player_1_score++;
		}
		else if (ball_p_x - ball_halfsize < -arena_halfsize_x)
		{
			ball_dp_x *= -1;
			ball_dp_y = 0;

			ball_p_x = 0;
			ball_p_y = 0;
			player_2_score++;
		}

		/*float at_x = -80;
		for (int i = 0; i < player_1_score; i++)
		{
			Draw_Rect(at_x, 47.f, 1.f, 1.f, 0xaaaaaa);
			at_x += 2.5f;
		}

		at_x = 80;
		for (int i = 0; i < player_2_score; i++)
		{
			Draw_Rect(at_x, 47.f, 1.f, 1.f, 0xaaaaaa);
			at_x -= 2.5f;
		}*/

		Draw_number(player_1_score, -10, 40, 1.f, 0xbbffbb);
		Draw_number(player_2_score, 10, 40, 1.f, 0xbbffbb);

		Draw_Rect(ball_p_x, ball_p_y, ball_halfsize, ball_halfsize, 0xFFFFFF);	//BALL
	}
	else
	{
		if (pressed(BUTTON_LEFT) or pressed(BUTTON_RIGHT))
		{
			hot_button = !hot_button;
		}

		

		if (pressed(BUTTON_ENTER))
		{
			current_gamemode = GM_GAMEPLAY;
			enemy_is_ai = hot_button ? 0 : 1;
		}

		Draw_text("PONG", -35, 40, 2, 0xff0000);
		Draw_text("BY DAN ZAIDAN", 25, 40, .5f, 0xff0000);
		Draw_text("DRAWNIXS VERSION", 25, 30, .5f, 0xff0000);

		if (hot_button == 0)
		{
			Draw_Rect(-37, -3, 40, 6, 0xEBB234);
			Draw_text("SINGLE PLAYER", -75, 0, 1, 0xff0000);
			Draw_text("MULTIPLAYER", 10, 0, 1, 0xaa0000);
		}
		else
		{
			Draw_Rect(42, -3, 35, 6, 0xEBB234);
			Draw_text("SINGLE PLAYER", -75, 0, 1, 0xaa0000);
			Draw_text("MULTIPLAYER", 10, 0, 1, 0xff0000);
		}
		
	}
}