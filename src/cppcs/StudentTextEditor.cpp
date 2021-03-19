#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdio>


std::string execute(const std::string& command);
const std::string CACHE = "cache.cpp";
const std::string TEMP = "temp.txt";

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
	m_includes.insert("#include <iostream>");
	m_includes.insert("#include <fstream>");
	m_includes.insert("#include <string>");
	m_includes.insert("using namespace std;");
	m_includes.insert("std::ofstream outfile(\"" + TEMP + "\");");
}

StudentTextEditor::~StudentTextEditor()
{
	reset();
}

bool StudentTextEditor::load(std::string file) 
{
	m_includes.insert("#include \"" + file + "\"");
	return true;
}

bool StudentTextEditor::save(std::string file)
{
	std::ofstream outfile(file);
	std::ifstream infile(TEMP);

	// Return false if file is inaccessible
	if (!outfile)
	{
		return false;
	}

	// No file on first iteration, need to create one
	if (!infile)
	{
		std::ofstream temp(TEMP);
	}

	std::unordered_set<std::string>::iterator inc_itr = m_includes.begin();
	while (inc_itr != m_includes.end())
	{
		outfile << *inc_itr << std::endl;
		inc_itr++;
	}

	outfile << std::endl << "int main()" << std::endl << "{";

	std::list<std::string>::iterator itr = m_compile.begin();
	while (itr != m_compile.end())
	{
		std::string line = *itr;
		if ((*itr).substr(3, 8) != "#include" || *itr == "\n")
		{			
			size_t f_cout = (*itr).find("cout");
			size_t f_op = (*itr).find("<<");
			if (f_cout != std::string::npos && f_op != std::string::npos && f_op > f_cout)
			{
				line = "outfile <<" + (*itr).substr(f_op + 2); // need to check if file is accessible first
				itr = m_compile.erase(itr);
			}
			outfile << std::endl << '\t' << line;	
		}
		if (itr != m_compile.end()) itr++;
	}

	outfile << std::endl << "}" << std::endl;

	// Compile cache.cpp and run executable
	if (!system(nullptr))
	{
		return false;
	}
	// TODO: catch comiplation errors and writeError() if applicable
	system("g++ -o cache cache.cpp"); 
	system("cache.exe");
	std::string s;
	while (getline(infile, s))
	{
		writeOutput(s);
	}

	return true;
}


void StudentTextEditor::reset() 
{
	// Set all rows to empty strings and return cursor to (0,0)
	m_editor.clear();
	m_includes.clear();
	m_row = 0;
	m_editor.push_back(">> ");
	m_currRow = m_editor.begin();
	m_col = 3;

	// Empty all the actions in the undo stack
	getUndo()->clear();
	save(CACHE);
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
	if (m_currRow != --m_editor.end())
	{
		return;
	}	

	if ((*m_currRow).substr(3, 8) == "#include")
	{
		if (m_includes.find((*m_currRow).substr(3)) == m_includes.end())
		{
			m_includes.insert((*m_currRow).substr(3));
		}
		else
		{
			writeError((*m_currRow).substr(12) + " already included");
			return;
		}
	}
	else
	{
		m_compile.push_back((*m_currRow).substr(3));
		if (save(CACHE))
		{
			m_editor.push_back(">> ");
			m_currRow++;
			m_upDown = m_currRow;
			m_row++;
			m_col = 3;
			getUndo()->clear();
		}
		else
		{
			writeError("Invalid command. Please try again.");
		}
	}
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

void StudentTextEditor::writeError(std::string error)
{
	m_editor.push_back("\n");
	m_editor.push_back("    ERROR: " + error);
	m_editor.push_back("\n");
	m_editor.push_back(">> ");
	m_currRow = --m_editor.end();
	m_row += 4;
	move(Dir::HOME);
}

void StudentTextEditor::writeOutput(std::string out)
{
	m_editor.push_back("\n");
	m_editor.push_back("    " + out);
	m_editor.push_back("\n");
	m_currRow = --m_editor.end();
	m_row += 3;
	move(Dir::HOME);
}

std::string execute(const std::string& command) 
{
	system((command + " > temp.txt").c_str());

	std::ifstream ifs("temp.txt");
	std::string ret{ std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
	ifs.close(); // must close the inout stream so the file can be cleaned up
	if (std::remove("temp.txt") != 0) 
	{
		perror("Error deleting temporary file");
	}
	return ret;
}