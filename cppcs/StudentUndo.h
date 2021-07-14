#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();
	virtual ~StudentUndo();

private:
	struct KeyPress
	{
		int m_row;
		int m_col;
		Action m_action;
		std::string m_chs;
		KeyPress(int row, int col, Action action, char ch) :
			m_row(row), m_col(col), m_action(action) 
		{
			// Initialize m_chs to 4 spaces if tab character is added
			if (ch == '\t')
			{
				m_chs = "    ";
			}
			else
			{
				m_chs = std::string(1, ch);
			}
		}
	};
	std::stack<KeyPress> m_actions;
};

#endif // STUDENTUNDO_H_
