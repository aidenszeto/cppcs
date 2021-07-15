#include "EditorGui.h"
#include "TextIO.h"
#include <string>

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
