#ifndef TEXTEDITOR_H_
#define TEXTEDITOR_H_

#include <string>
#include <vector>

class Undo;

class TextEditor {
public:
	enum Dir { UP, DOWN, LEFT, RIGHT, HOME, END };

	TextEditor(Undo* undo)
		: undo_(undo) { }
	virtual ~TextEditor() { }
	virtual bool load(std::string file) = 0;
	virtual bool save(std::string file) = 0;
	virtual void reset() = 0;

	virtual void insert(char ch) = 0;
	virtual void enter() = 0;
	virtual void del() = 0;
	virtual void backspace() = 0;
	virtual void move(Dir dir) = 0;
	virtual void getPos(int& row, int& col) const = 0;
	virtual int getLines(int startRow, int numRows, std::vector<std::string>& lines) const = 0;
	virtual void undo() = 0;

protected:
	Undo* getUndo() { return undo_; }

private:
	Undo* undo_;
};

TextEditor* createTextEditor(Undo* un);

#endif // TEXTEDITOR_H_
