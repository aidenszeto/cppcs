#ifndef TEXTIO_H_
#define TEXTIO_H_

#ifndef _MSC_VER
#include <curses.h>		// https://www.linuxjournal.com/content/getting-started-ncurses
#else
#include "curses.h"
#endif 

#include <string>

const int CTRL_D = 'D' - 'A' + 1;
const int CTRL_S = 'S' - 'A' + 1;
const int CTRL_L = 'L' - 'A' + 1;
const int CTRL_X = 'X' - 'A' + 1;
const int CTRL_Z = 'Z' - 'A' + 1;

class TextIO {
public:
	TextIO(int fgcolor, int bgcolor, int hilite) {
		initscr();
		start_color();
		cbreak();
		noecho();
		raw();
		init_pair(COLOR::WHITE, fgcolor, bgcolor);
		init_pair(COLOR::RED, hilite, bgcolor);
		keypad(stdscr, TRUE);
		refresh();
	}

	~TextIO() {
		echo();
		endwin();
	}

	static void clear() {
		::clear();
	}

	enum COLOR {
		WHITE = COLOR_WHITE,
		RED = COLOR_RED
	};

	static void print(char ch, COLOR fcolor = COLOR::WHITE) {
		attron(COLOR_PAIR(fcolor));
		addch(ch);
	}

	static void print(const std::string& s, COLOR fcolor = COLOR::WHITE) {
		attron(COLOR_PAIR(fcolor));
		addstr(s.c_str());
	}

	static void refresh() {
		::refresh();
	}

	static void move(int row, int col) {
		::move(row, col);
	}

	/*
		   key code        description

		   KEY_DOWN        The four arrow keys ...
		   KEY_UP
		   KEY_LEFT
		   KEY_RIGHT
	   KEY_PPAGE	   Previous page
	   KEY_NPAGE	   Next page
		   KEY_HOME        Home key
		   KEY_BACKSPACE   Backspace
		   KEY_F(n)        Function keys, for 0 <= n >= 63
		   KEY_DC          Delete character
		   KEY_IC          Insert char or enter insert mode
		   KEY_ENTER       Enter or send
	*/

	static int getChar() {
		int ch = 0;
		ch = getch();
#ifdef _MBCS
		const int kEnter = '\r';
		const int kBackspace = '\b';
#else
		const int kBackspace = 127;
		const int kEnter = '\n';
#endif
		if (ch == kBackspace)
			return KEY_BACKSPACE;
		if (ch == kEnter)
			return KEY_ENTER;
		return ch;
	}

	static void getString(std::string& str) {
		const int kMaxFilenameLength = 1024;
		char temp[kMaxFilenameLength] = "";
		echo();
		getnstr(temp, kMaxFilenameLength);
		noecho();
		str = temp;
	}

private:
	static const int kDefaultPair = 1;
};

#endif // TEXTIO_H_
