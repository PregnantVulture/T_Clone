#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define CELL_SIZE 30 
#define BOARD_X 285
#define BOARD_Y 52
#define WINDOW_WIDTH 920
#define WINDOW_HEIGHT 720
#define PIECE_COUNT 7
#define THEME_COUNT 5
#define SETTINGS_TAB_COUNT 2

typedef struct {
	int cells[4][4];
	int type;
	int rotation;
	int x;
	int y;
} Piece;

typedef struct {
	Color background;
	Color boardEmpty;
	Color panel;
	Color border;
	Color primary;
	Color accent;
	Color muted;
	Color grid;
	Color pieces[PIECE_COUNT];
} Theme;

typedef enum {
	SCREEN_LOADING,
	SCREEN_GAME,
	SCREEN_SETTINGS,
	SCREEN_DIFFICULTY
} AppScreen;

#include "themes/theme_monoshade_purple.h"
#include "themes/theme_monoshade_retro_green.h"
#include "themes/theme_monoshade_red.h"
#include "themes/theme_light.h"
#include "themes/theme_dark.h"

static const int SHAPES[PIECE_COUNT][4][4][4] = {
	{
		{{0, 0, 0, 0}, 
		 {1, 1, 1, 1}, 
		 {0, 0, 0, 0}, 
		 {0, 0, 0, 0}},

		{{0, 0, 1, 0},
		 {0, 0, 1, 0},
		 {0, 0, 1, 0},
		 {0, 0, 1, 0}},

		{{0, 0, 0, 0},
		 {0, 0, 0, 0},
		 {1, 1, 1, 1},
		 {0, 0, 0, 0}},

		{{0, 1, 0, 0},
		 {0, 1, 0, 0},
		 {0, 1, 0, 0},
		 {0, 1, 0, 0}}

	},
	{
		{{1, 1, 0, 0},
		 {0, 1, 1, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},

		{{0, 0, 1, 0},
		 {0, 1, 1, 0},
		 {0, 1, 0, 0},
		 {0, 0, 0, 0}},

		{{0, 0, 0, 0},
		 {1, 1, 0, 0},
		 {0, 1, 1, 0},
		 {0, 0, 0, 0}},

		{{0, 1, 0, 0},
		 {1, 1, 0, 0},
		 {1, 0, 0, 0}, 
		 {0, 0, 0, 0}}

	},

	{
		{{0, 1, 1, 0},
		 {1, 1, 0, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},

		{{0, 1, 0, 0},
		 {0, 1, 1, 0},
		 {0, 0, 1, 0},
		 {0, 0, 0, 0}},

		{{0, 0, 0, 0},
		 {0, 1, 1, 0},
		 {1, 1, 0, 0},
		 {0, 0, 0, 0}},

		{{1, 0, 0, 0},
		 {1, 1, 0, 0},
		 {0, 1, 0, 0},
		 {0, 0, 0, 0}}

	},
	{
		{{1, 0, 0, 0},
		 {1, 1, 1, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		
		{{0, 1, 0, 0},
		 {0, 1, 0, 0},
		 {1, 1, 0, 0},
		 {0, 0, 0, 0}},
		
		{{0, 0, 0, 0},
		 {1, 1, 1, 0},
		 {0, 0, 1, 0},
		 {0, 0, 0, 0}},
		
		{{0, 1, 1, 0},
		 {0, 1, 0, 0},
		 {0, 1, 0, 0},
		 {0, 0, 0, 0}}
		
	},
	{
		{{0, 0, 1, 0},
		 {1, 1, 1, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		
		{{0, 1, 0, 0},
		 {0, 1, 0, 0},
		 {0, 1, 1, 0},
		 {0, 0, 0, 0}},
		
		{{0, 0, 0, 0},
		 {1, 1, 1, 0},
		 {1, 0, 0, 0},
		 {0, 0, 0, 0}},
		
		{{1, 1, 0, 0},
		 {0, 1, 0, 0},
		 {0, 1, 0, 0},
		 {0, 0, 0, 0}}
		
	},
	
	{
		{{0, 1, 0, 0},
		 {1, 1, 1, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		
		{{0, 1, 0, 0},
		 {0, 1, 1, 0},
		 {0, 1, 0, 0},
		 {0, 0, 0, 0}},
		
		{{0, 0, 0, 0},
		 {1, 1, 1, 0},
		 {0, 1, 0, 0},
		 {0, 0, 0, 0}},
		
		{{0, 1, 0, 0},
		 {1, 1, 0, 0},
		 {0, 1, 0, 0},
		 {0, 0, 0, 0}}
		
	},
	{
		{{1, 1, 0, 0},
		 {1, 1, 0, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		
		{{1, 1, 0, 0},
		 {1, 1, 0, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		
		{{1, 1, 0, 0},
		 {1, 1, 0, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}},
		
		{{1, 1, 0, 0},
		 {1, 1, 0, 0},
		 {0, 0, 0, 0},
		 {0, 0, 0, 0}}
		
	}
};
static const Theme *theme = &THEME_MONOSHADE_PURPLE;
static const Theme *themes[THEME_COUNT] = {
	&THEME_MONOSHADE_PURPLE,
	&THEME_MONOSHADE_RETRO_GREEN,
	&THEME_MONOSHADE_RED,
	&THEME_LIGHT,
	&THEME_DARK
};

static const char *themeNames[THEME_COUNT] = {
	"MONOSHADE PURPLE",
	"RETRO GREEN",
	"MONOSHADE RED",
	"LIGHT",
	"DARK"
};

static int board[BOARD_HEIGHT][BOARD_WIDTH];
static int nextType;
static int heldType;
static int score;
static int lines;
static int level;
static bool canHold;
static bool paused;
static bool gameOver;
static int selectedTheme = 0;
static int selectedSettingsTab;
static bool soundEnabled = true;
static float masterVolume = 0.75f;
static AppScreen screen = SCREEN_LOADING;
static AppScreen settingsReturnScreen = SCREEN_LOADING;
static int selectedDifficulty = 1;
static float fallTimer;
static float lockTimer;
static Piece current;
static Sound bloopSound;
static Sound gameOverSound;
static Sound blockHitSound;
static bool audioReady;

static const char *settingsTabNames[SETTINGS_TAB_COUNT] = {
	"THEMES",
	"SOUND"
};

static const char *difficultyNames[] = {
	"EASY",
	"NORMAL",
	"HARD",
	"SHITTINGLY INSANE"
};

static const float difficultySpeedMultipliers[] = {
	0.8f,
	1.0f,
	2.0f,
	3.0f
};

static void UpdateHighscore(void)
{
	FILE *file = fopen("highscore.txt", "r+");
	int highscore = 0;

	if (file == NULL)
		file = fopen("highscore.txt", "w+");

	if (file == NULL)
		return;

	if (fscanf(file, "%d", &highscore) != 1) {
		highscore = 0;
		rewind(file);
		fprintf(file, "0\n");
	}

	if (score > highscore) {
		rewind(file);
		fprintf(file, "%d\n", score);
	}

	fclose(file);
}

static void SetPieceRotation(Piece *p, int rotation);
static void PlayBloop(void)
{
	if (audioReady && soundEnabled)
		PlaySound(bloopSound);
}

static void SetSoundVolumes(void)
{
	if (!audioReady)
		return;

	SetSoundVolume(bloopSound, masterVolume);
	SetSoundVolume(gameOverSound, masterVolume);
	SetSoundVolume(blockHitSound, masterVolume);
}

static Sound MakeBloop(void)
{
	const int sampleRate = 44100;
	const int frameCount = sampleRate / 8;
	unsigned char *samples = malloc((size_t)frameCount);

	for (int i = 0; i < frameCount; i++) {
		int period = sampleRate / (520 - (i * 260 / frameCount));
		int phase = i % period;
		int amplitude = 70 - (i * 55 / frameCount);
		samples[i] = (unsigned char)(128 + (phase < period / 2 ? amplitude : -amplitude));
	}

	Wave wave = {frameCount, sampleRate, 8, 1, samples};
	Sound sound = LoadSoundFromWave(wave);
	UnloadWave(wave);
	return sound;
}

static Piece MakePiece(int type)
{
	Piece piece = {0};
	piece.type = type;
	piece.x = 3;
	piece.y = -1;
	SetPieceRotation(&piece, 0);
	return piece;
}

static void SetPieceRotation(Piece *piece, int rotation)
{
	piece->rotation = (rotation + 4) % 4;

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			piece->cells[y][x] = SHAPES[piece->type][piece->rotation][y][x];
}

static bool Fits(Piece piece, int x, int y)
{
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			if (piece.cells[row][column]) {
				int boardX = x + column;
				int boardY = y + row;

				if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT ||
					(boardY >= 0 && board[boardY][boardX] != -1))
					return false;
			}
		}
	}

	return true;
}

static bool Move(int dx, int dy, bool playSound)
{
	if (!Fits(current, current.x + dx, current.y + dy))
		return false;

	current.x += dx;
	current.y += dy;

	if (playSound)
		PlayBloop();

	return true;
}

static void Rotate(void)
{
	Piece piece = current;
	const int kicks[] = {0, -1, 1, -2, 2};

	SetPieceRotation(&piece, current.rotation + 1);

	for (int i = 0; i < 5; i++) {
		if (Fits(piece, current.x + kicks[i], current.y)) {
			current = piece;
			current.x += kicks[i];
			PlayBloop();
			return;
		}
	}
}

static int DropDistance(void)
{
	int distance = 0;

	while (Fits(current, current.x, current.y + distance + 1))
		distance++;

	return distance;
}

static void ClearLines(void)
{
	int cleared = 0;

	for (int row = BOARD_HEIGHT - 1; row >= 0; row--) {
		bool full = true;

		for (int column = 0; column < BOARD_WIDTH; column++)
			if (board[row][column] == -1)
				full = false;

		if (full) {
			cleared++;

			for (int moveRow = row; moveRow > 0; moveRow--)
				for (int column = 0; column < BOARD_WIDTH; column++)
					board[moveRow][column] = board[moveRow - 1][column];

			for (int column = 0; column < BOARD_WIDTH; column++)
				board[0][column] = -1;

			row++;
		}
	}

	if (cleared) {
		const int points[] = {0, 100, 300, 500, 800};
		score += points[cleared] * level;
		lines += cleared;
		level = lines / 10 + 1;
	}
}

static void Spawn(void)
{
	current = MakePiece(nextType);
	nextType = GetRandomValue(0, PIECE_COUNT - 1);
	canHold = true;
	fallTimer = 0;
	lockTimer = 0;

	if (!Fits(current, current.x, current.y)) {
		gameOver = true;
		UpdateHighscore();
		if (audioReady && soundEnabled)
			PlaySound(gameOverSound);
	}
}

static void Lock(void)
{
	for (int row = 0; row < 4; row++)
		for (int column = 0; column < 4; column++)
			if (current.cells[row][column] && current.y + row >= 0)
				board[current.y + row][current.x + column] = current.type;

	if (audioReady && soundEnabled)
		PlaySound(blockHitSound);

	ClearLines();
	Spawn();
}

static void Hold(void)
{
	if (!canHold)
		return;

	int oldType = current.type;

	if (heldType == -1) {
		heldType = oldType;
		Spawn();
	} else {
		int swapType = heldType;
		heldType = oldType;
		current = MakePiece(swapType);
		fallTimer = 0;
		lockTimer = 0;
	}

	canHold = false;
}

static void Reset(void)
{
	for (int row = 0; row < BOARD_HEIGHT; row++)
		for (int column = 0; column < BOARD_WIDTH; column++)
			board[row][column] = -1;

	score = 0;
	lines = 0;
	level = 1;
	heldType = -1;
	canHold = true;
	paused = false;
	gameOver = false;
	nextType = GetRandomValue(0, PIECE_COUNT - 1);
	Spawn();
}

static void Cell(int x, int y, Color color, bool ghost)
{
	Rectangle rectangle = {
		(float)(BOARD_X + x * CELL_SIZE),
		(float)(BOARD_Y + y * CELL_SIZE),
		CELL_SIZE - 1.0f,
		CELL_SIZE - 1.0f
	};

	if (ghost) {
		DrawRectangleLinesEx(rectangle, 2, Fade(color, 0.35f));
	} else {
		DrawRectangleRec(rectangle, color);
		DrawRectangleLinesEx(rectangle, 1, Fade(theme->primary, 0.18f));
	}
}

static void Mini(int type, int x, int y)
{
	if (type < 0)
		return;

	for (int row = 0; row < 4; row++)
		for (int column = 0; column < 4; column++)
			if (SHAPES[type][0][row][column])
				DrawRectangle(x + column * 18, y + row * 18, 17, 17, theme->pieces[type]);
}

static void Panel(Rectangle rectangle, const char *title)
{
	DrawRectangleRec(rectangle, theme->panel);
	DrawRectangleLinesEx(rectangle, 1, theme->border);
	DrawText(title, (int)rectangle.x + 16, (int)rectangle.y + 14, 18, theme->primary);
}

static void Button(Rectangle rectangle, const char *label, bool selected)
{
	Color background = selected ? theme->accent : theme->panel;
	Color text = selected ? theme->background : theme->primary;
	int textWidth = MeasureText(label, 18);

	DrawRectangleRec(rectangle, background);
	DrawRectangleLinesEx(rectangle, 1, theme->border);
	DrawText(label, (int)(rectangle.x + (rectangle.width - textWidth) / 2),
		(int)rectangle.y + 14, 18, text);
}

static void CenteredText(const char *text, int centerX, int y, int fontSize, Color color)
{
	DrawText(text, centerX - MeasureText(text, fontSize) / 2, y, fontSize, color);
}

static void SettingsIcon(void)
{
	Rectangle rectangle = {842, 32, 48, 48};
	Color background = screen == SCREEN_SETTINGS ? theme->accent : theme->panel;

	DrawRectangleRec(rectangle, background);
	DrawRectangleLinesEx(rectangle, 1, theme->border);
	DrawCircle(866, 56, 9, theme->primary);
	DrawCircle(866, 56, 4, background);
	DrawLine(866, 42, 866, 47, theme->primary);
	DrawLine(866, 65, 866, 70, theme->primary);
	DrawLine(852, 56, 857, 56, theme->primary);
	DrawLine(875, 56, 880, 56, theme->primary);
	DrawLine(856, 46, 860, 50, theme->primary);
	DrawLine(872, 62, 876, 66, theme->primary);
	DrawLine(876, 46, 872, 50, theme->primary);
	DrawLine(860, 62, 856, 66, theme->primary);
}

static void DrawSettings(void)
{
	ClearBackground(theme->background);
	DrawText("SETTINGS", 44, 42, 30, theme->primary);
	DrawText("CONFIGURE YOUR GAME", 47, 78, 12, theme->accent);

	Rectangle panel = {154, 128, 612, 468};
	DrawRectangleRec(panel, theme->panel);
	DrawRectangleLinesEx(panel, 2, theme->border);

	for (int i = 0; i < SETTINGS_TAB_COUNT; i++) {
		Rectangle tab = {174 + i * 144.0f, 148, 128, 38};
		Button(tab, settingsTabNames[i], i == selectedSettingsTab);
	}

	if (selectedSettingsTab == 0) {
		DrawText("COLOR THEME", 190, 214, 20, theme->primary);

		for (int i = 0; i < THEME_COUNT; i++) {
			int y = 252 + i * 48;
			Rectangle option = {190, (float)y, 540, 38};
			Color optionColor = i == selectedTheme ? theme->accent : theme->boardEmpty;
			Color textColor = i == selectedTheme ? theme->background : theme->primary;

			DrawRectangleRec(option, optionColor);
			DrawRectangleLinesEx(option, 1, theme->border);
			DrawText(themeNames[i], 208, y + 11, 13, textColor);
		}
	} else {
		DrawText("SOUND EFFECTS", 190, 214, 20, theme->primary);
		DrawText(soundEnabled ? "ON" : "OFF", 650, 216, 14, theme->accent);

		Rectangle toggle = {190, 252, 540, 38};
		Button(toggle, soundEnabled ? "SOUND EFFECTS: ON" : "SOUND EFFECTS: OFF", soundEnabled);

		DrawText("MASTER VOLUME", 190, 332, 14, theme->primary);
		DrawText(TextFormat("%d%%", (int)(masterVolume * 100)), 680, 332, 14, theme->accent);
		DrawRectangle(190, 370, 540, 8, theme->boardEmpty);
		DrawRectangle(190, 370, (int)(540 * masterVolume), 8, theme->accent);
		DrawCircle(190 + 540 * masterVolume, 374, 10, theme->primary);
	}

	DrawText("ESC  back", 190, 556, 12, theme->muted);
}

static void DrawLoading(void)
{
	ClearBackground(theme->background);
	CenteredText("T shi", WINDOW_WIDTH / 2, 128, 72, theme->primary);
	CenteredText("RAYLIB EDITION", WINDOW_WIDTH / 2, 210, 16, theme->accent);
	CenteredText("THE CLASSIC FALLING BLOCK PUZZLE", WINDOW_WIDTH / 2, 252, 12, theme->muted);

	Button((Rectangle){300, 330, 320, 52}, "START GAME", true);
	Button((Rectangle){300, 400, 320, 52}, "SETTINGS", false);
	Button((Rectangle){300, 470, 320, 52}, "DIFFICULTY MODE", false);
	Button((Rectangle){300, 540, 320, 52}, "QUIT", false);
	CenteredText(TextFormat("SELECTED: %s", difficultyNames[selectedDifficulty]), WINDOW_WIDTH / 2, 640, 12, theme->muted);
}

static void DrawDifficulty(void)
{
	ClearBackground(theme->background);
	CenteredText("DIFFICULTY MODE", WINDOW_WIDTH / 2, 112, 36, theme->primary);
	CenteredText("CHOOSE HOW FAST THE BLOCKS FALL", WINDOW_WIDTH / 2, 174, 12, theme->muted);

	for (int i = 0; i < 4; i++) {
		Rectangle option = {250, 230 + i * 66.0f, 420, 50};
		Button(option, difficultyNames[i], i == selectedDifficulty);
	}

	CenteredText("NORMAL uses the original game speed", WINDOW_WIDTH / 2, 530, 12, theme->muted);
	CenteredText("TAB  back", WINDOW_WIDTH / 2, 590, 12, theme->muted);
}

static void DrawGame(void) {
	ClearBackground(theme->background);
	DrawText("T shi", 44, 44, 30, theme->primary);
	DrawText("Using Raylib, by Arshdeep", 47, 78, 12, theme->accent);
	SettingsIcon();

	Panel((Rectangle){36, 132, 190, 154}, "HOLD");
	Mini(heldType, 88, 190);
	DrawText("C   hold", 57, 258, 12, theme->muted);

	Panel((Rectangle){694, 132, 190, 154}, "NEXT");
	Mini(nextType, 766, 190);

	Panel((Rectangle){36, 316, 190, 176}, "SCORE");
	DrawText(TextFormat("%07d", score), 56, 359, 30, theme->primary);
	DrawText("LINES", 56, 411, 12, theme->muted);
	DrawText(TextFormat("%03d", lines), 137, 406, 20, theme->primary);
	DrawText("LEVEL", 56, 449, 12, theme->muted);
	DrawText(TextFormat("%02d", level), 137, 444, 20, theme->primary);

	Rectangle boardFrame = {
		BOARD_X - 8,
		BOARD_Y - 8,
		BOARD_WIDTH * CELL_SIZE + 16,
		BOARD_HEIGHT * CELL_SIZE + 16
	};
	DrawRectangleRec(boardFrame, theme->panel);
	DrawRectangleLinesEx(boardFrame, 2, theme->border);

	for (int row = 0; row < BOARD_HEIGHT; row++)
		for (int column = 0; column < BOARD_WIDTH; column++)
			Cell(column, row, board[row][column] == -1 ? theme->boardEmpty : theme->pieces[board[row][column]], false);

	if (!gameOver) {
		int ghostY = current.y + DropDistance();

		for (int row = 0; row < 4; row++)
			for (int column = 0; column < 4; column++)
				if (current.cells[row][column] && ghostY + row >= 0)
					Cell(current.x + column, ghostY + row, theme->pieces[current.type], true);

		for (int row = 0; row < 4; row++)
			for (int column = 0; column < 4; column++)
				if (current.cells[row][column] && current.y + row >= 0)
					Cell(current.x + column, current.y + row, theme->pieces[current.type], false);
	}

	DrawText("ARROW KEYS", 694, 345, 14, theme->primary);
	DrawText("move / soft drop", 694, 370, 13, theme->muted);
	DrawText("UP       rotate", 694, 404, 14, theme->primary);
	DrawText("SPACE    hard drop", 694, 438, 14, theme->primary);
	DrawText("C        hold piece", 694, 472, 14, theme->primary);
	DrawText("P  pause    R  restart", 694, 524, 13, theme->muted);

	if (paused || gameOver) {
		DrawRectangle(BOARD_X, BOARD_Y + 250, BOARD_WIDTH * CELL_SIZE, 90, Fade(theme->background, 0.94f));
		DrawText(gameOver ? "GAME OVER" : "PAUSED", BOARD_X + 64, BOARD_Y + 268, 30, theme->primary);
		DrawText(gameOver ? "Press R to play again" : "Press P to resume", BOARD_X + 65, BOARD_Y + 307, 13, theme->muted);
	}

}

static void DrawApp(void)
{
	if (screen == SCREEN_LOADING)
		DrawLoading();
	else if (screen == SCREEN_SETTINGS)
		DrawSettings();
	else if (screen == SCREEN_DIFFICULTY)
		DrawDifficulty();
	else
		DrawGame();
}

int main(void) {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "T shi");
	InitAudioDevice();
	audioReady = IsAudioDeviceReady();

	if (audioReady) {
		bloopSound = MakeBloop();
		gameOverSound = LoadSound("audley_fergine-game-over-classic-206486.wav");
		blockHitSound = LoadSound("mixkit-electronic-retro-block-hit-2185.wav");
		SetSoundVolumes();
	}

	SetTargetFPS(60);
	SetRandomSeed((unsigned int)time(NULL));
	Reset();

	while (!WindowShouldClose()) {
		float delta = GetFrameTime();

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mouse = GetMousePosition();

			if (screen == SCREEN_LOADING) {
				if (CheckCollisionPointRec(mouse, (Rectangle){300, 330, 320, 52})) {
					Reset();
					screen = SCREEN_GAME;
				} else if (CheckCollisionPointRec(mouse, (Rectangle){300, 400, 320, 52})) {
					settingsReturnScreen = SCREEN_LOADING;
					screen = SCREEN_SETTINGS;
				} else if (CheckCollisionPointRec(mouse, (Rectangle){300, 470, 320, 52})) {
					screen = SCREEN_DIFFICULTY;
				} else if (CheckCollisionPointRec(mouse, (Rectangle){300, 540, 320, 52})) {
					break;
				}
			} else if (screen == SCREEN_GAME) {
				if (CheckCollisionPointRec(mouse, (Rectangle){842, 32, 48, 48})) {
					settingsReturnScreen = SCREEN_GAME;
					screen = SCREEN_SETTINGS;
				}
			} else if (screen == SCREEN_SETTINGS) {
				for (int i = 0; i < SETTINGS_TAB_COUNT; i++) {
					if (CheckCollisionPointRec(mouse, (Rectangle){174 + i * 144.0f, 148, 128, 38})) {
						selectedSettingsTab = i;
						break;
					}
				}

				if (selectedSettingsTab == 0) {
					for (int i = 0; i < THEME_COUNT; i++) {
						if (CheckCollisionPointRec(mouse, (Rectangle){190, 252 + i * 48, 540, 38})) {
							selectedTheme = i;
							theme = themes[i];
							break;
						}
					}
				} else if (CheckCollisionPointRec(mouse, (Rectangle){190, 252, 540, 38})) {
					soundEnabled = !soundEnabled;
				} else if (CheckCollisionPointRec(mouse, (Rectangle){180, 350, 560, 48})) {
					masterVolume = (mouse.x - 190) / 540.0f;
					if (masterVolume < 0)
						masterVolume = 0;
					if (masterVolume > 1)
						masterVolume = 1;
					SetSoundVolumes();
				}
			} else if (screen == SCREEN_DIFFICULTY) {
				for (int i = 0; i < 4; i++) {
					if (CheckCollisionPointRec(mouse, (Rectangle){250, 230 + i * 66.0f, 420, 50})) {
						selectedDifficulty = i;
						screen = SCREEN_LOADING;
						break;
					}
				}
			}
		}

		if (IsKeyPressed(KEY_ESCAPE) && screen == SCREEN_DIFFICULTY)
			screen = SCREEN_LOADING;

		if (IsKeyPressed(KEY_TAB) && screen == SCREEN_SETTINGS)
			screen = settingsReturnScreen;

		if (screen == SCREEN_GAME) {
			if (IsKeyPressed(KEY_R))
				Reset();

			if (IsKeyPressed(KEY_P) && !gameOver)
				paused = !paused;

			if (!paused && !gameOver) {
			if (IsKeyPressed(KEY_C))
				Hold();

			if (IsKeyPressed(KEY_W))
				Rotate();

			if (IsKeyPressed(KEY_A))
				Move(-1, 0, true);

			if (IsKeyPressed(KEY_D))
				Move(1, 0, true);

			if (IsKeyPressed(KEY_SPACE)) {
				int distance = DropDistance();
				current.y += distance;
				score += distance * 2;
				Lock();
			}

			float interval = (0.8f - (level - 1) * 0.06f) / difficultySpeedMultipliers[selectedDifficulty];

			if (interval < 0.08f)
				interval = 0.08f;

			if (IsKeyDown(KEY_S))
				interval *= 0.08f;

			fallTimer += delta;

			if (fallTimer >= interval) {
				fallTimer = 0;

				if (!Move(0, 1, false))
					lockTimer += interval;
			} else if (Fits(current, current.x, current.y + 1)) {
				lockTimer = 0;
			}

			if (lockTimer >= 0.45f)
				Lock();
			}
		}

		BeginDrawing();
		DrawApp();
		EndDrawing();
	}

	if (audioReady) {
		UnloadSound(bloopSound);
		UnloadSound(gameOverSound);
		UnloadSound(blockHitSound);
		CloseAudioDevice();
	}

	CloseWindow();
	return 0;
}