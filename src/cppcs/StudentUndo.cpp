#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) 
{
	// Check for batching if stack is not empty and action and row match previous action
	if (!m_actions.empty() && m_actions.top().m_action == action && m_actions.top().m_row == row)
	{
		switch (action)
		{
		case Action::DELETE:
			// Append characters to end of string if batched delete is detected	
			if (m_actions.top().m_col == col)
			{
				m_actions.top().m_chs += ch;	
				m_actions.top().m_row = row;
				m_actions.top().m_col = col;
				return;
			}
			// Append characters to beginning of string if batched backspace is detected
			else if (m_actions.top().m_col - 1 == col)
			{
				m_actions.top().m_chs = ch + m_actions.top().m_chs;
				m_actions.top().m_row = row;
				m_actions.top().m_col = col;
				return;
			}
			break;
		case Action::INSERT:
			// Append characters to end of string if adjacent insert detected
			if (m_actions.top().m_col + 1 == col || (ch == '\t' && m_actions.top().m_col + 4 == col))
			{
				if (ch == '\t')
				{
					m_actions.top().m_chs += "    ";
				}
				else
				{
					m_actions.top().m_chs += ch;
				}
				m_actions.top().m_row = row;
				m_actions.top().m_col = col;
				return;
			}
			break;
		default:
			break;
		}
	}
	KeyPress kp(row, col, action, ch);
	m_actions.push(kp);
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text) 
{
	// Return an error if there are no actions in the undo stack
	if (m_actions.empty())
	{
		return Action::ERROR;
	}

	// Store the action that is being undone and reset count and text
	Action undoAction = m_actions.top().m_action;
	count = 0;
	text = "";

	// Set row and col to the action's position
	row = m_actions.top().m_row;
	col = m_actions.top().m_col;

	// Increment count for every character undone
	count += m_actions.top().m_chs.length();

	// If we're undoing a deletion, append the char deleted to text
	if (m_actions.top().m_action == Action::DELETE)
	{
		text += m_actions.top().m_chs;
	}
	m_actions.pop();

	// Return the action corresponding to the original change made
	switch (undoAction)
	{
	case Action::INSERT:
		return Action::DELETE;
	case Action::DELETE:
		return Action::INSERT;
	default:
		return Action::ERROR;
	}
}

void StudentUndo::clear() 
{
	// Pop from stack until m_actions is empty
	while (!m_actions.empty())
	{
		m_actions.pop();
	}
}

StudentUndo::~StudentUndo()
{
	clear();
}
