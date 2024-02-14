#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>

typedef struct {
	int x; 
	int y;
}vec2;

int score = 0;
vec2 segments[256];

void drawBordersAndScore(WINDOW *win, int screen_width, int screen_height) {
    erase();

	attron(COLOR_PAIR(4));
    // top border and score
    char score_text[20];
    sprintf(score_text, "  SCORE: %d  ", score);
    int score_pos = (screen_width / 2) - (strlen(score_text) / 2);
    mvaddstr(0, score_pos, score_text);
    for (int x = 0; x < screen_width; x++) {
        if (x < score_pos || x >= score_pos + strlen(score_text)) {
            mvaddch(0, x, '#');
        }
    }
    // side borders
    for (int y = 1; y < screen_height - 1; y++) {
        mvaddch(y, 0, '#');
        mvaddch(y, screen_width - 1, '#');
    }
    // bottom border
    for (int x = 0; x < screen_width; x++) {
        mvaddch(screen_height - 1, x, '#');
    }
	attron(COLOR_PAIR(4));
}

void drawSnakeAndBerry(vec2 berry, vec2 head, int sem, int screen_width, int screen_height) {
    attron(COLOR_PAIR(3));
	mvaddch(berry.y, berry.x, '@');
	attron(COLOR_PAIR(3));

	attron(COLOR_PAIR(2));
    for (int i = 0; i < score; i++) {
        mvaddch(segments[i].y, segments[i].x, 'o');
    }
	attron(COLOR_PAIR(2));

	attron(COLOR_PAIR(1));
    switch(sem) {
        case 1: mvaddch(head.y, head.x, '<'); break;
        case 2: mvaddch(head.y, head.x, '>'); break;
        case 3: mvaddch(head.y, head.x, '^'); break;
        case 4: mvaddch(head.y, head.x, 'v'); break;
    }
	attron(COLOR_PAIR(1));
}

bool checkSelfCollision(vec2 head, int score) {
	// Checks for collision with itself
    for (int i = 1; i < score; i++) {
        if (head.x == segments[i].x && head.y == segments[i].y) {
            return true;
        }
    }
    return false;
}

bool checkBerryCollision(vec2 berry, vec2 *segments, int score) {
	for (int i = 0; i < score; i++) {
        if (berry.x == segments[i].x && berry.y == segments[i].y) {
            return true;
        }
    }
    return false;
}

int main(){
	// initialize screen
	WINDOW *win = initscr();
	keypad(win, true);
	nodelay(win, true);
	curs_set(0);

	// start color functionality
	start_color();
	if (!has_colors()) {
		endwin();
		fprintf(stderr, "Error: colors");
		exit(1);
	}
	init_pair(1, COLOR_GREEN, COLOR_BLACK); // Snake head
	init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Snake body
	init_pair(3, COLOR_RED, COLOR_BLACK); // Berry
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK); // Game box

	// Get terminal dimensions
    int screen_height, screen_width;
    getmaxyx(win, screen_height, screen_width);
	int total_play_area = (screen_width - 2) * (screen_height - 2);

	// snake
	vec2 head = { screen_width / 2, screen_height / 2 }; 
	vec2 dir = { 1, 0 };
	int sem = 2;

	// berry
	vec2 berry = { rand() % (screen_width - 2) + 1, rand() % (screen_height - 2) + 1 };

	bool MOVING = false;

	while (true) {
		int press = wgetch(win);
		// direction logic
		if (press == KEY_LEFT) {
			if (dir.x == 1) continue;
			sem = 1;
			dir.x = -1;	dir.y = 0;

			MOVING = false;
		}
		if (press == KEY_RIGHT) {
			if (dir.x == -1) continue;
			sem = 2;
			dir.x = 1;	dir.y = 0;

			MOVING = false;
		}
		if(press == KEY_UP) {
			if (dir.y == 1) continue;
			sem = 3;
			dir.x = 0;	dir.y = -1;

			MOVING = true;
		}
		if (press == KEY_DOWN) {
			if (dir.y == -1) continue;
			sem = 4;
			dir.x = 0;	dir.y = 1;

			MOVING = true;
		}
		if(press == '\e') {
			break;
		}

		// update segemnts
		for (int i = score; i > 0; i--) {
			segments[i] = segments[i - 1];
		}
		segments[0] = head;

		head.x += dir.x;
		head.y += dir.y;

		// self collision
		if (checkSelfCollision(head, score)) {
			endwin();
			printf("Game Over! Final Score: %d\n", score);
			exit(0); // exit
		}

		// wall wrapping
		if (head.x <= 0) {
			head.x = screen_width - 2; // move head to the opposite side
		} else if (head.x >= screen_width - 1) {
			head.x = 1; // move head to the opposite side
		}
		if (head.y <= 0) {
			head.y = screen_height - 2; // move head to the opposite side
		} else if (head.y >= screen_height - 1) {
			head.y = 1; // move head to the opposite side
		}

		if (head.x == berry.x && head.y == berry.y) {
			score++;

			// check for win
			if (score >= total_play_area) {
				endwin();
				printf("Congratulations, You Won! Final Score: %d\n", score);
				exit(0); // exit
			}

			do {
				berry.x = rand() % (screen_width - 2) + 1;
				berry.y = rand() % (screen_height - 2) + 1;
			} while (checkBerryCollision(berry, segments, score));
		}

		// draw
		drawBordersAndScore(win, screen_width, screen_height);
        drawSnakeAndBerry(berry, head, sem, screen_width, screen_height);

		if (MOVING) {
			usleep(200000);
		} else {
			usleep(100000);
		}

	}
	endwin();

	return 0;
}
