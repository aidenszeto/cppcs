#ifndef STUDENTTEXTEDITOR_H_
#define STUDENTTEXTEDITOR_H_

#include "TextEditor.h"
#include <list>
#include <unordered_set>

class Undo;

class StudentTextEditor : public TextEditor {
public:

	StudentTextEditor(Undo* undo);
	~StudentTextEditor();
	bool load(std::string file);
	bool save(std::string file);
	void reset();
	void move(Dir dir);
	void del();
	void backspace();
	void insert(char ch);
	void enter();
	void getPos(int& row, int& col) const;
	int getLines(int startRow, int numRows, std::vector<std::string>& lines) const;
	void undo();

private:
	void writeError(std::string error);

	int m_row;
	int m_col;
	std::list<std::string> m_editor;
	std::list<std::string>::iterator m_currRow;
	std::list<std::string>::iterator m_upDown;
	std::unordered_set<std::string> m_includes;
};

#endif // STUDENTTEXTEDITOR_H_
