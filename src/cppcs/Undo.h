#ifndef UNDO_H_
#define UNDO_H_

#include <string>

class Undo {
public:
	enum Action {
		ERROR = 0,
		INSERT = 1,
		SPLIT = 2,
		DELETE = 3,
		JOIN = 4	// deleting last character on line to join with below line; backspacing backward on first character on the line to join with above line	
	};

	Undo() { }
	virtual ~Undo() { }

	virtual void submit(const Action action, int row, int col, char ch = 0) = 0;
	virtual Action get(int& row, int& col, int& count, std::string& text) = 0;
	virtual void clear() = 0;
};

Undo* createUndo();

#endif // UNDO_H_
