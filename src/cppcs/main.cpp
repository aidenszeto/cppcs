#include "EditorGui.h"
#include "TextIO.h"
#include <iostream>
#include <string>

// Do not change anything in this file other than these initializer values
const int FOREGROUND_COLOR = COLOR_WHITE;
const int BACKGROUND_COLOR = COLOR_BLACK;
const int HIGHLIGHT_COLOR  = COLOR_RED;
// Choices are COLOR_x, where x is WHITE, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN

int main(int argc, char* argv[]) {
	TextIO ti(FOREGROUND_COLOR, BACKGROUND_COLOR, HIGHLIGHT_COLOR);
	EditorGui editor(LINES, COLS);

	if (argc >= 2)
	{
		for (int i = 1; i < argc; i++)
		{
			editor.loadFileToEdit(argv[i]);
		}		
	}

	editor.run();
}       
