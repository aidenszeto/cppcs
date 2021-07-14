#include "EditorGui.h"
#include "TextIO.h"
#include <string>

// Choices are COLOR_x, where x is WHITE, BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN
const int FOREGROUND_COLOR = COLOR_WHITE;
const int BACKGROUND_COLOR = COLOR_BLACK;
const int HIGHLIGHT_COLOR  = COLOR_RED;

int main(int argc, char* argv[]) {
	TextIO ti;
	EditorGui editor;

	if (argc >= 2)
	{
		for (int i = 1; i < argc; i++)
		{
			editor.loadFileToEdit(argv[i]);
		}		
	}

	editor.run();
}       
