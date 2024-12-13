
internal void Clear_Screen(u32 color)
{
	unsigned int* pixel = (u32*)render_state.memory;

	for (int y = 0; y < render_state.height; y++)
	{
		for (int x = 0; x < render_state.width; x++)
		{
			*pixel++ = color;
		}
	}
}

internal void Draw_Rect_in_Pixels(int x0, int y0, int x1, int y1, u32 color)
{
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++)
	{
		unsigned int* pixel = (u32*)render_state.memory + x0 + y * render_state.width;
		for (int x = x0; x < x1; x++)
		{
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void Draw_Rect(float x, float y, float halfSizeX, float halfSizeY, u32 color)
{
	x *= render_state.height * render_scale;
	y *= render_state.height * render_scale;
	halfSizeX *= render_state.height * render_scale;
	halfSizeY *= render_state.height * render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	int x0 = x - halfSizeX;
	int x1 = x + halfSizeX;
	int y0 = y - halfSizeY;
	int y1 = y + halfSizeY;


	Draw_Rect_in_Pixels(x0, y0, x1, y1, color);
}

const char* letters[][7] =
{
	" 000 ",
	"0   0",
	"0   0",
	"00000",
	"0   0",
	"0   0",
	"0   0",

	"0000 ",
	"0   0",
	"0   0",
	"0000 ",
	"0   0",
	"0   0",
	"0000 ",

	" 000",
	"0   0",
	"0    ",
	"0    ",
	"0    ",
	"0   0",
	" 000 ",

	"0000 ",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0000 ",

	"00000",
	"0    ",
	"0    ",
	"000  ",
	"0    ",
	"0    ",
	"00000",

	"00000",
	"0    ",
	"0    ",
	"000  ",
	"0    ",
	"0    ",
	"0    ",

	" 000 ",
	"0   0",
	"0   0",
	"0    ",
	"0  00",
	"0   0",
	" 000 ",

	"0   0",
	"0   0",
	"0   0",
	"00000",
	"0   0",
	"0   0",
	"0   0",

	"00000",
	"  0  ",
	"  0  ",
	"  0  ",
	"  0  ",
	"  0  ",
	"00000",

	"00000",
	"    0",
	"    0",
	"    0",
	"    0",
	"0   0",
	" 000 ",

	"0   0",
	"0  0 ",
	"0 0  ",
	"00   ",
	"0 0  ",
	"0  0 ",
	"0   0",

	"0    ",
	"0    ",
	"0    ",
	"0    ",
	"0    ",
	"0    ",
	"00000",

	"0   0",
	"00 00",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"0   0",
	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",
	"0   0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 000 ",

	"0000 ",
	"0   0",
	"0   0",
	"0000 ",
	"0    ",
	"0    ",
	"0    ",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	" 000 ",
	"0    ",

	"0000 ",
	"0   0",
	"0   0",
	"0000 ",
	"0   0",
	"0   0",
	"0   0",

	" 0000",
	"0    ",
	"0    ",
	" 000 ",
	"    0",
	"    0",
	"0000 ",

	"00000",
	"  0  ",
	"  0  ",
	"  0  ",
	"  0  ",
	"  0  ",
	"  0  ",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 000 ",

	"0   0",
	"0   0",
	"0   0",
	" 0 0 ",
	" 0 0 ",
	"  0  ",
	"     ",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"00 00",
	"0   0",

	"0   0",
	"0   0",
	" 0 0 ",
	"  0  ",
	" 0 0 ",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0 ",
	"  0  ",
	"  0  ",
	"  0  ",
	"  0  ",

	"00000",
	"    0",
	"   0 ",
	"  0  ",
	" 0   ",
	"0    ",
	"00000",
};

internal void Draw_text(const char* text, float x, float y, float size, u32 color)
{
	float half_size = size * .5f;
	float original_y = y;

	while (*text)
	{
		if (*text != 32)
		{
			const char** A_letter = letters[*text-'A'];
			float original_x = x;

			for (int i = 0; i < 7; i++)
			{
				const char* row = A_letter[i];
				while (*row)
				{
					if (*row == '0')
					{
						Draw_Rect(x, y, half_size, half_size, color);
					}
					x += size;
					row++;
				}
				y -= size;
				x = original_x;
			}
		}
		text++;
		x += size * 6.f;
		y = original_y;
	}
	
}

internal void Draw_number(int number, float x, float y, float size, u32 color)
{
	float half_size = size * .5f;
	bool drew_number = false;



	while (number or !drew_number)
	{
		int digit = number % 10;
		number = number / 10;

		drew_number = true;

		switch (digit)
		{
		case 0:
		{
			Draw_Rect(x - size, y, half_size, 2.5f * size, color);
			Draw_Rect(x + size, y, half_size, 2.5f * size, color);
			Draw_Rect(x, y - size * 2.f, half_size, half_size * size, color);
			Draw_Rect(x, y + size * 2.f, half_size, half_size * size, color);
			x -= size * 4.f;
		}
		break;
		case 1:
		{
			Draw_Rect(x, y, half_size, 2.5f * size, color);
			Draw_Rect(x - size, y + size, half_size, half_size * size, color);
			x -= size * 4.f;
		}
		break;
		case 2:
		{
			Draw_Rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y, 1.5f * size, half_size, color);
			Draw_Rect(x + size, y + size, half_size, half_size, color);
			Draw_Rect(x - size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		}
		break;
		case 3:
		{
			Draw_Rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y, 1.5f * size, half_size, color);
			Draw_Rect(x + size, y + size, half_size, half_size, color);
			Draw_Rect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;

		}
		break;
		case 4:
		{
			Draw_Rect(x + size, y, half_size, 2.5f * size, color);
			Draw_Rect(x - size, y + size, half_size, 1.5f * size, color);
			Draw_Rect(x, y, half_size, half_size, color);
			x -= size * 4.f;
		}
		break;
		case 5:
		{
			Draw_Rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y, 1.5f * size, half_size, color);
			Draw_Rect(x - size, y + size, half_size, half_size, color);
			Draw_Rect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		}
		break;
		case 6:
		{
			Draw_Rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y, 1.5f * size, half_size, color);
			Draw_Rect(x - size, y + size, half_size, half_size, color);
			Draw_Rect(x + size, y - size, half_size, half_size, color);
			Draw_Rect(x - size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		}
		break;
		case 7:
		{
			Draw_Rect(x + size, y, half_size, 2.5f * size, color);
			Draw_Rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			x -= size * 4.f;
		}
		break;
		case 8:
		{
			Draw_Rect(x - size, y, half_size, 2.5f * size, color);
			Draw_Rect(x + size, y, half_size, 2.5f * size, color);
			Draw_Rect(x, y, 1.5f * size, half_size, color);
			Draw_Rect(x, y - size * 2.f, half_size, half_size * size, color);
			Draw_Rect(x, y + size * 2.f, half_size, half_size * size, color);
			x -= size * 4.f;
		}
		break;
		case 9:
		{
			Draw_Rect(x, y + size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y - size * 2.f, 1.5f * size, half_size, color);
			Draw_Rect(x, y, 1.5f * size, half_size, color);
			Draw_Rect(x - size, y + size, half_size, half_size, color);
			Draw_Rect(x + size, y + size, half_size, half_size, color);
			Draw_Rect(x + size, y - size, half_size, half_size, color);
			x -= size * 4.f;
		}
		break;
		}
	}
}