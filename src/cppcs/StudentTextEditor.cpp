#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <fstream>

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
	m_row = 0;
	m_col = 3;
	m_editor.push_back(">> ");
	m_currRow = m_editor.begin();
	m_upDown = m_editor.begin();
}

StudentTextEditor::~StudentTextEditor()
{
	reset();
}

bool StudentTextEditor::load(std::string file) 
{
	// load cpp or header files
	return true;
}

bool StudentTextEditor::save(std::string file)
{
	std::ofstream outfile(file);

	// Return false if file is inaccessible
	if (!outfile)
	{
		return false;
	}

	outfile << 
		"#include <iostream>" << std::endl <<
		"#include <string>" << std::endl <<
		"using namespace std;" << std::endl << std::endl <<
		"int main()" << std::endl << "{";

	std::list<std::string>::iterator itr = m_editor.begin();
	while (itr != --m_editor.end())
	{
		outfile << std::endl << '\t' << (*itr).substr(3);
		itr++;
	}

	outfile <<
		std::endl << "}" << std::endl;

	// run cache.cpp -> if there are errors, undo the changes and report error

	return true;
}


void StudentTextEditor::reset() 
{
	// Set all rows to empty strings and return cursor to (0,0)
	std::list<std::string>::iterator itr = m_editor.begin();
	while (!m_editor.empty())
	{
		itr = m_editor.erase(itr);
	}
	m_row = 0;
	m_col = 0;
	m_currRow = m_editor.begin();

	// Empty all the actions in the undo stack
	getUndo()->clear();
}

void StudentTextEditor::move(Dir dir) {
	// TODO
	switch (dir)
	{
	case Dir::UP:
		if (m_upDown != m_editor.begin())
		{
			m_upDown--;
			*m_currRow = *m_upDown;
			move(Dir::END);
		}
		break;
	case Dir::DOWN:
		if (m_upDown != m_currRow)
		{
			m_upDown++;
			*m_currRow = *m_upDown;
			move(Dir::END);
		}
		else
		{
			*m_currRow = ">> ";
			move(Dir::HOME);
		}
		break;
	case Dir::LEFT:
		// Move up and to the end if at the beginning of a row
		// if possible, otherwise decrease column by 1
		if (m_col == 0 && m_row != 0)
		{
			move(Dir::UP);
			move(Dir::END);
		}
		else if (m_col != 0)
		{
			m_col--;
		}
		break;
	case Dir::RIGHT:
		// Move down and to the beginning if at the end of a row
		// if possible, otherwise increase column by 1
		if (m_col == (*m_currRow).length() && m_row != m_editor.size() - 1)
		{
			move(Dir::DOWN);
			move(Dir::HOME);
		}
		else if (m_col != (*m_currRow).length())
		{
			m_col++;
		}
		break;
	case Dir::HOME:
		// Move to beginning of the row
		m_col = 3;
		break;
	case Dir::END:
		// Move to end of the row
		m_col = (*m_currRow).length();
		break;
	}
}

void StudentTextEditor::del() 
{
	// If the cursor is before the end of the line, remove current character
	if (m_col < (*m_currRow).length())
	{
		char deleted = (*m_currRow)[m_col];
		(*m_currRow) = (*m_currRow).substr(0, m_col) + (*m_currRow).substr(m_col + 1);
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, deleted);
	}
}

void StudentTextEditor::backspace() 
{
	// If the cursor is past the first character on a non-empty line,
	// remove the character at the cursor and move the cursor left
	if (m_col > 3)
	{
		char deleted = (*m_currRow)[m_col - 1];
		(*m_currRow) = (*m_currRow).substr(0, m_col - 1) + (*m_currRow).substr(m_col);
		move(Dir::LEFT);
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, deleted);
	}
}

void StudentTextEditor::insert(char ch) 
{
	// Add 4 spaces if the user inserts a tab character
	if (ch == '\t')
	{
		*m_currRow = (*m_currRow).substr(0, m_col) + "    " + (*m_currRow).substr(m_col);
		m_col += 4;
		getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
		return;
	}
	
	// Insert character into row and move cursor to the right
	*m_currRow = (*m_currRow).substr(0, m_col) + ch + (*m_currRow).substr(m_col);
	m_col++;
	getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
}

void StudentTextEditor::enter() 
{
	// Move everything to the right of the cursor to the next row
	std::string moveToNext = (*m_currRow).substr(m_col);
	*m_currRow = (*m_currRow).substr(0, m_col);
	if (m_currRow != m_editor.end())
	{
		m_currRow++;
		m_editor.insert(m_currRow, ">> " + moveToNext);
		m_currRow--;
	}
	else
	{
		m_editor.push_back(">> " + moveToNext);
		m_currRow = --m_editor.end();
	}

	getUndo()->clear();
	m_upDown = m_currRow;
	m_row++;
	m_col = 3;

	save("cache.cpp");
}

void StudentTextEditor::getPos(int& row, int& col) const {
	row = m_row;
	col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const 
{
	// If startRow or numRows is negative, or if startRow is greater than the number of lines, return -1
	if (startRow < 0 || numRows < 0 || startRow >= m_editor.size())
	{
		return -1;
	}

	// Clear all previous data in lines before adding requested lines
	lines.clear();

	// Set itr to point to startRow by incrementing/decrementing depending on the curr row
	std::list<std::string>::iterator itr = m_currRow;
	for (int i = 0; i < abs(m_row - startRow); i++)
	{
		if (startRow < m_row)
		{
			itr--;
		}
		else
		{
			itr++;
		}
	}
	
	// Append the rows prompted for to lines if they are valid rows in the editor
	for (int i = startRow; i < startRow + numRows; i++)
	{
		// Break out of the loop if there are no more rows available to read into lines
		if (itr == m_editor.end())
		{
			break;
		}

		// Otherwise, append the string value corresponding to the row to lines
		lines.push_back(*itr);
		itr++;
	}	

	return lines.size();
}

void StudentTextEditor::undo() 
{
	// Get undo from Undo class 
	int numChars = 0;
	std::string toAdd = "";
	int a_row;
	Undo::Action action = getUndo()->get(a_row, m_col, numChars, toAdd);

	// Do nothing if the undo returns error, otherwise move currRow to correct position
	if (action == Undo::Action::ERROR)
	{
		return;
	}
	for (int i = 0; i < abs(m_row - a_row); i++)
	{
		if (a_row < m_row)
		{
			m_currRow--;
		}
		else
		{
			m_currRow++;
		}
	}
	m_row = a_row;

	// Initialize iterator pointing to row after current row
	std::list<std::string>::iterator n_itr = m_currRow;
	n_itr++;

	switch (action)
	{
	case Undo::Action::DELETE:
		// Remove numChars from m_col if undo deletes previous insertion
		m_col -= numChars;
		*m_currRow = (*m_currRow).substr(0, m_col) + (*m_currRow).substr(m_col + numChars);
		break;
	case Undo::Action::INSERT:
		// Insert toAdd into the current row at m_col - numChars
		m_col += numChars;
		*m_currRow = (*m_currRow).substr(0, m_col - numChars) + toAdd + (*m_currRow).substr(m_col - numChars);
		break;
	default:
		break;
	}
}
