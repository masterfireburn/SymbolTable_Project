#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//Symbol class to represent a symbol in the table;
class Symbol 
{
    public:
        string name;
        string i_type;
        string d_type;
        string is_array;
        string array_size;
        string scope;
        Symbol *next_parameter;
        Symbol *next_symbol;
   
   // Default constructor
   Symbol() 
   {
       name = "";
       i_type = "";
       d_type = "";
       is_array = "";
       array_size = "";
       scope = "";
       next_parameter = NULL;
       next_symbol = NULL;
   }
   // Parameterised Constructor
   Symbol(string name, string i_type, string d_type, string is_array, string array_size, string scope) 
   {
       this->name = name;
       this->i_type = i_type;
       this->d_type = d_type;
       this->is_array = is_array;
       this->array_size = array_size;
       this->scope = scope;
       this->next_parameter = NULL;
       this->next_symbol = NULL;
       
   }
};

//Class to implement symbol table
class SymbolTable
{
    Symbol *head;

    public:
    // Default constructor
    SymbolTable() 
    {
        head = NULL;
    }
    
    Symbol* getHead()
    {
        return head;
    }
    
    void append(Symbol *new_symbol, bool is_param)
    {
        // If the table is empty, make
        // the new symbol as the head and return
        if (head == nullptr) 
        {
            head = new_symbol;
        }
        else
        {
            // Store the head reference in a temporary variable
            Symbol* last = head;
            
            while (last->next_symbol != nullptr)
            {
                last = last->next_symbol;
            }
            if(is_param)
            {
                while (last->next_parameter != nullptr)
                {
                    last = last->next_parameter;
                }
                last->next_parameter = new_symbol;
            }
            else
            {
                last->next_symbol = new_symbol;
            }
        }
    }
};

// Node class to represent a node of the tree.
class Node 
{
  public:
    string token;
    string token_type;
    int line_num;
    Node *child;
    Node *sibling;

    // Default constructor
    Node() 
    {
        token = "";
        token_type = "";
        line_num = 0;
        child = NULL;
        sibling = NULL;
    }

    // Parameterised Constructor
    Node(string token, string token_type, int line_num) 
    {
        this->token = token;
        this->token_type = token_type;
        this->line_num = line_num;
        this->child = NULL;
        this->sibling = NULL;
    }
};

// Class to implement a Left Child Right Sibling tree
class LCRS_Tree 
{
    Node *head;

    public:
    // Default constructor
    LCRS_Tree() 
    {
        head = NULL;
    }
    
    Node* getHead()
    {
        return head;
    }

    // Function to insert a node at the start of the
    // RSLC tree
    int append(string token, string token_type, int line_num, bool check) 
    {

        // Create a new node
        Node* new_node = new Node(token, token_type, line_num);

        // If the LCRS tree is empty, make
        // the new node as the head and return
        if (head == nullptr) 
        {
            head = new_node;
        }
        else
        {

            // Store the head reference in a temporary variable
            Node* last = head;

            // Traverse till the last node
            while (last->sibling != nullptr || last->child != nullptr) 
            {
                if(last->sibling != nullptr)
                {
                    last = last->sibling;
                }
                else if (last->child != nullptr)
                {
                    last = last->child;
                }
            }

            if(check)
            {
                //This section can be expanded to hold more reserved words
                if(new_node->token_type == "IDENTIFIER" && (new_node->token == "int"      || new_node->token == "char"      || 
                                                            new_node->token == "bool"     || new_node->token == "float"     || 
                                                            new_node->token == "double"   || new_node->token == "void"      ||
                                                            new_node->token == "for"      || new_node->token == "while"     || 
                                                            new_node->token == "if"       || new_node->token == "else"      || 
                                                            new_node->token == "printf"   || new_node->token == "procedure" ||
                                                            new_node->token == "function" || new_node->token == "return"    ))
                {
                    if(last->token_type == "IDENTIFIER" && (last->token == "int"    || last->token == "char"  || 
                                                            last->token == "bool"   || last->token == "float" || 
                                                            last->token == "double" || last->token == "void"  ))
                    {
                        return -10;
                    }
                }
                last->sibling = new_node;
                
            }
            else
            {
                last->child = new_node;
            }
        }
        return 0;
    }

    // Function to print the LCRS tree.
    void print() 
    {
        Node *temp = head;

        // Check for empty tree
        if (head != NULL) 
        {
             // Traverse the tree
            while (temp->sibling != nullptr || temp->child != nullptr) 
            {
                cout << temp->token << " -> ";
                
                if(temp->sibling != nullptr)
                {
                    temp = temp->sibling;
                }
                else if (temp->child != nullptr)
                {
                    temp = temp->child;
                    cout << endl << endl;
                }
            }
            cout << temp->token << " -> ";
        }
    };
};

char findBlockTerminator(ifstream& in_file, ofstream& out_file, int& line)
{
	char c;
	char prev_c;
	int original_line = line;
	out_file << "  ";
	while (in_file.get(c))
	{
		if(c == '\n')
		{
			out_file << c;
			line++;
		}
		if(c == '*')
		{
			in_file.get(c);
			if(c == '/')
			{
				out_file << "  ";
				return c;
			}
			if(c == '\n')
			{
				out_file << c;
				line++;
			}
			else
			{
				out_file << " ";
			}
		}
		if(c == '/' && prev_c == '*')
		{
			out_file << " ";
			return c;
		}
		else
		{
			out_file << " ";
		}
		prev_c = c;
	}
	cerr << "ERROR: Program contains C-style, unterminated comment on line " << original_line << std::endl;
	return -2;
}

int findLineTerminator(ifstream& in_file, ofstream& out_file, int line)
{
	char c;
	while (in_file.get(c))
	{
		if(c == '\n')
		{
			return c;
		}
	}
	cerr << "ERROR: Program contains C-style, unterminated comment on line " << line << std::endl;
	return -3;
}

char starFunction(ifstream& in_file, ofstream& out_file, char prev_c, int line)
{
	char c;
	in_file.get(c);
	if(c == '/')
	{
		cerr << "ERROR: Program contains C-style, unterminated comment on line " << line << std::endl;
		return -4;
	}
	out_file << prev_c;
	if (c == '*')
	{
		starFunction(in_file, out_file, c, line);
	}
	return c;
}

char findStringTerminator(int mode, ifstream& in_file, ofstream& out_file, char c, int line)
{
	string string_to_print;
	string_to_print += c;
	if(mode == 0)
	{
		while (in_file.get(c))
		{
		    if(c == '\\')
		    {
		        string_to_print += c;
		        in_file.get(c);
		        string_to_print += c;
		    }
			else if(c == '"')
			{
			    string_to_print += c; 
				out_file << string_to_print;
				return c;
			}
			else
			{
			   string_to_print += c; 
			}
		}
		cerr << "ERROR: Program contains C-style, unterminated string on line " << line << std::endl;
		return -5;
	}
	else
	{
		while (in_file.get(c))
		{
		    if(c == '\\')
		    {
		        string_to_print += c;
		        in_file.get(c);
		        string_to_print += c;
		    }
			else if(c == '\'')
			{
			    string_to_print += c;
				out_file << string_to_print;
				return c;
			}
			else
			{
			   string_to_print += c; 
			}
		}
		cerr << "ERROR: Program contains C-style, unterminated string on line " << line << std::endl;
		return -6;
	}
}

char removeComments(ifstream& in_f, ofstream& out_f)
{
	char c;
	char prev_c;
	int line_num = 1;

	while (in_f.get(c))
	{
		if(c == '/')
		{
			prev_c = c;
			in_f.get(c);
			if(c == '/')
			{
				c = findLineTerminator(in_f, out_f, line_num);
				if(c == -3)
				{
					in_f.close();
					out_f.close();
					return c;
				}
			}
			else if(c == '*')
			{
				c = findBlockTerminator(in_f, out_f, line_num);
				if(c == -2)
				{
					in_f.close();
					out_f.close();
					return c;
				}
			}
			else
			{
				out_f << prev_c;
			}

		}
		if(c == '*')
		{
			c = starFunction(in_f, out_f, c, line_num);
			if(c == -4)
			{
				in_f.close();
				out_f.close();
				return c;
			}
		}
		if(c == '"')
		{
			c = findStringTerminator(0, in_f, out_f, c, line_num);
			if(c == -5)
			{
				in_f.close();
				out_f.close();
				return c;
			}
		}
		if(c == '\'')
		{
			c = findStringTerminator(1, in_f, out_f, c, line_num);
			if(c == -6)
			{
				in_f.close();
				out_f.close();
				return c;
			}
		}
		if(c == '\n') {
			line_num++;
		}
		if(c != '/' && c != '*' && c != '"' && c != '\'' && !in_f.eof())
		{
			out_f << c;
		}
	}

	return 0;
}

string identifierCheck(ifstream& in_f, char& c, string identifier)
{
    
    
    if(!(isalpha(c)) && !(isdigit(c)) && c != '_' && c != '-'){
        return identifier;
    }
    else
    {
        identifier += c;
        in_f.get(c);
        return identifierCheck(in_f, c, identifier);
    }
}

string intCheck(ifstream& in_f, char& c, string integer, int line, bool& err)
{
    if(!(isdigit(c))){
        if(c == ',' || c == ';' || c == ']' || c == ')' || c == '}' || c == ' ')
        {
            return integer;
        }
        else
        {
            cerr << "INTEGER error on line: " << line;
		    err = true;
		    return integer;
        }   
    }
    else
    {
        integer += c;
        in_f.get(c);
        return intCheck(in_f, c, integer, line, err);
    }
}

string stringCheck(ifstream& in_f, char c, bool mode)
{
    string toReturn;
    
    if(mode)
    {
        in_f.get(c);
        while(c != '\'')
        {
            toReturn += c;
            in_f.get(c);
        }
    }
    else
    {
        in_f.get(c);
        while(c != '"')
        {
            toReturn += c;
            in_f.get(c);
        } 
    }
    return toReturn;
}

char createTokens(ifstream& in_f, LCRS_Tree& LCRS, char c, int& line, bool& check)
{
    char prev_c = ' ';
    
    if(isalpha(c))
	{
	    string identifier;
	    
	    identifier = identifierCheck(in_f, c, identifier);
	    
	    if(identifier == "TRUE")
	    {
	        LCRS.append(identifier, "BOOLEAN_TRUE", line, check);
	        if(!check)
	        {
	            check = true;
	        }
	        c = createTokens(in_f, LCRS, c, line, check);
		    return c;
	    }
	    if(identifier == "FALSE")
	    {
	        LCRS.append(identifier, "BOOLEAN_FALSE", line, check);
	        if(!check)
	        {
	            check = true;
	        }
	        c = createTokens(in_f, LCRS, c, line, check);
		    return c;
	    }
	    int err = LCRS.append(identifier, "IDENTIFIER", line, check);
	    if(err < 0)
	    {
	        cerr << "Invalid use of reserved word: '" << identifier << "' on line: " << line;
	        return -10;
	    }
	    if(!check)
	    {
	        check = true;
	    }
	    c = createTokens(in_f, LCRS, c, line, check);
		return c;
	}
	if(isdigit(c))
	{
	    string integer;
	    bool err = false; 
	    string intWithoutError = intCheck(in_f, c, integer, line, err);
	    
	    if(err)
	    {
	        return -8;
	    }
	    
	    LCRS.append(intWithoutError, "INTEGER", line, check);
	    if(!check)
	    {
	        check = true;
	    }
	    c = createTokens(in_f, LCRS, c, line, check);
		return c;
	}
	if(c == '"')
	{
	   LCRS.append("\"", "DOUBLE_QUOTE", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	   
	   LCRS.append(stringCheck(in_f, c, false), "STRING", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	   
	   LCRS.append("\"", "DOUBLE_QUOTE", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	   
	}
	if(c == '\'')
	{
	   LCRS.append("'", "SINGLE_QUOTE", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	   
	   LCRS.append(stringCheck(in_f, c, true), "STRING", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	   
	   LCRS.append("'", "SINGLE_QUOTE", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	}
	if(c == '(')
	{
	   LCRS.append("(", "L_PAREN", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	   
	}
	if(c == ')')
	{
	   LCRS.append(")", "R_PAREN", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	}
	if(c == '[')
	{
	   prev_c = c;
	   in_f.get(c);
	   if(c == '-')
	   {
	        cerr << "Invalid array size on line: " << line;
		    c = -9;
		    return c;
	   }
	   else
	   {
	        LCRS.append("[", "L_BRACKET", line, check);
	        if(!check)
	        {
	            check = true;
	        }
	        c = createTokens(in_f, LCRS, c, line, check);
	        return c; 
	   }
	}
	if(c == ']')
	{
	   LCRS.append("]", "R_BRACKET", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	}
	if(c == '{')
	{
	   LCRS.append("{", "L_BRACE", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	}
	if(c == '}')
	{
	   LCRS.append("}", "R_BRACE", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	}
	if(c == ';')
	{
	   LCRS.append(";", "SEMICOLON", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	}
	if(c == ',')
	{
	   LCRS.append(",", "COMMA", line, check);
	   if(!check)
	   {
	        check = true;
	   }
	}
	if(c == '=')
	{
	    prev_c = c;
	    in_f.get(c);
	    if(c == '=')
		{
	        LCRS.append("==", "BOOLEAN_EQUAL", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		}
		else
		{
	        LCRS.append("=", "ASSIGNMENT_OPERATOR", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		    c = createTokens(in_f, LCRS, c, line, check);
		    return c;
		    
		}
	}
	if(c == '+')
	{
	    prev_c = c;
	    in_f.get(c);
	    if(isdigit(c))
		{
		    bool err = false;
		    string integer;
		    
		    string intWithoutError = intCheck(in_f, c, integer, line, err);
		    
		    if(err)
	        {
	            return -8;
	        }
		    
	        LCRS.append(intWithoutError, "INTEGER", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		}
		else
		{
	        LCRS.append("+", "PLUS", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		    c = createTokens(in_f, LCRS, c, line, check);
		    return c;
		}
	}
	if(c == '-')
	{
	    prev_c = c;
	    in_f.get(c);
	    if(isdigit(c))
		{
		    bool err = false;
		    string integer;
		    
		    string intWithoutError = intCheck(in_f, c, integer, line, err);
		    
		    if(err)
	        {
	            return -8;
	        }
		    
	        LCRS.append("-" + intWithoutError, "INTEGER", line, check);
	        if(!check)
	        {
	            check = true;
	        }
	        c = createTokens(in_f, LCRS, c, line, check);
		    return c;
		}
		else
		{
	        LCRS.append("-", "MINUS", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		    c = createTokens(in_f, LCRS, c, line, check);
		    return c;
		}
	}
	if(c == '*')
	{
	    LCRS.append("*", "ASTERISK", line, check);
	    if(!check)
	    {
	        check = true;
	    }
	}
	if(c == '/')
	{
	    LCRS.append("/", "DIVIDE", line, check);
	    if(!check)
	    {
	        check = true;
	    }
	}
	if(c == '%')
	{
	    LCRS.append("%", "MODULO", line, check);
	    if(!check)
	    {
	        check = true;
	    }
	}
	if(c == '^')
	{
	    LCRS.append("^", "CARET", line, check);
	    if(!check)
	    {
	        check = true;
	    }
	}
	if(c == '<')
	{
	    prev_c = c;
	    in_f.get(c);
	    if(c == '=')
		{
	        LCRS.append("<=", "LT_EQUAL", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		}
		else
		{
	        LCRS.append("<", "LT", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		    c = createTokens(in_f, LCRS, c, line, check);
		    return c; 
		}
	}
	if(c == '>')
	{
	    prev_c = c;
	    in_f.get(c);
	    if(c == '=')
		{
	        LCRS.append(">=", "GT_EQUAL", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		}
		else
		{
	        LCRS.append(">", "GT", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		    c = createTokens(in_f, LCRS, c, line, check);
		    return c;
		}
	}
	if(c == '&')
	{
	    prev_c = c;
	    in_f.get(c);
	    if(c == '&')
		{
	        LCRS.append("&&", "BOOLEAN_AND", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		}
		else
		{
		    cerr << "BOOLEAN_AND error on line: " << line;
		    c = -8;
		    return c;
		}
	}
	if(c == '|')
	{
	    prev_c = c;
	    in_f.get(c);
	    if(c == '|')
		{
	        LCRS.append("||", "BOOLEAN_OR", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		}
		else
		{
		    cerr << "BOOLEAN_OR error on line: " << line;
		    c = -9;
		    return c;
		} 
	}
	if(c == '!')
	{
	    prev_c = c;
	    in_f.get(c);
	    if(c == '=')
		{
	        LCRS.append("!=", "BOOLEAN_NOT_EQUAL", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		}
		else
		{
	        LCRS.append("!", "BOOLEAN_NOT", line, check);
	        if(!check)
	        {
	            check = true;
	        }
		    c = createTokens(in_f, LCRS, c, line, check);
		    return c;
		} 
		
	}
	if(c == '\n')
	{
	    line++;
	    check = false;
	}
	return 0;
}

bool validator(SymbolTable ST, Symbol *s, string& err)
{
    Symbol *temp = ST.getHead();
    bool is_valid = true;
    
    while (temp != nullptr)
    {
        if(temp->scope == s->scope || temp->scope == "0")
        {
            if (temp->name == s->name)
            {
                is_valid = false;
                err = temp->scope;
                return is_valid;
            }
            else if (temp->next_parameter != nullptr)
            {
                Symbol *temp2 = temp->next_parameter;
                while (temp2 != nullptr)
                {
                    if (temp2->name == s->name)
                    {
                        is_valid = false;
                        err = temp->scope;
                        return is_valid;
                    }
                    temp2 = temp2->next_parameter;
                }
            }
        }
        temp = temp->next_symbol;
    }
    return is_valid;
}

Node* createSymbol(SymbolTable& ST, LCRS_Tree CST, Node *&temp, int& count, int& scope, bool is_param)
{
    bool is_valid;
    
    string name;
    string i_type;
    string d_type;
    string is_array;
    string array_size;
    
    if(temp->token_type == "IDENTIFIER")
    {
        if(temp->token == "procedure")
        {
            name = temp->sibling->token;
            i_type = temp->token;
            d_type = "NOT APPLICABLE";
            is_array = "no";
            array_size = "0";
            scope;
                
            Symbol* to_add = new Symbol(name, i_type, d_type, is_array, array_size, to_string(scope));
                
            string err;
            is_valid = validator(ST, to_add, err);
                
            if(!is_valid)
            {
                if(err == "0")
                {
                   cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined globaly" << endl; 
                }
                else
                {
                   cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined locally" << endl;
                }
                return nullptr;
            }
                
            ST.append(to_add, false);
            
            temp = temp->sibling->sibling->sibling;
                
            if(temp->token != "void")
            {
                temp = createSymbol(ST, CST, temp, count, scope, true);
            }
        }
        else if (temp->token == "function")
        {
            name = temp->sibling->sibling->token;
            i_type = temp->token;
            d_type = temp->sibling->token;
            is_array = "no";
            array_size = "0";
            scope;
            
            Symbol* to_add = new Symbol(name, i_type, d_type, is_array, array_size, to_string(scope));
            
            string err;
            is_valid = validator(ST, to_add, err);
                
            if(!is_valid)
            {
                if(err == "0")
                {
                   cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined globaly" << endl; 
                }
                else
                {
                   cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined locally" << endl;
                }
                return nullptr;
            }
            
            ST.append(to_add, false);
            
            if(temp->sibling->sibling->sibling->sibling->token != ")")
            {
                temp = createSymbol(ST, CST, temp->sibling->sibling->sibling->sibling, count, scope, true);
            }
        }
        else if(temp->token == "int" || temp->token == "char" || temp->token == "bool" || temp->token == "float")
        {
            if(is_param == false)
            {
                Symbol* to_add;
                
                name = temp->sibling->token;
                i_type = "datatype";
                d_type = temp->token;
                if(temp->sibling->sibling->token == "[")
                {
                    is_array = "yes";
                    array_size = temp->sibling->sibling->sibling->token;
                }
                else
                {
                    is_array = "no";
                    array_size = "0";
                }
                if(count != 0)
                {
                    to_add = new Symbol(name, i_type, d_type, is_array, array_size, to_string(scope));
                }
                else
                {
                    to_add = new Symbol(name, i_type, d_type, is_array, array_size, to_string(0));
                }
                
                string err;
                is_valid = validator(ST, to_add, err);
                if(!is_valid)
                {
                    if(err == "0")
                    {
                        cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined globaly" << endl; 
                    }
                    else
                    {
                        cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined locally" << endl;
                    }
                    return nullptr;
                }
            
                ST.append(to_add, false);
                
                temp = temp->sibling->sibling;
                while(temp->token == ",")
                {
                    temp = temp->sibling;
                    name = temp->token;
                    
                    if(count == 0)
                    {
                        to_add = new Symbol(name, i_type, d_type, is_array, array_size, to_string(0));
                    }
                    else
                    {
                        to_add = new Symbol(name, i_type, d_type, is_array, array_size, to_string(scope));
                    }
                    
                    is_valid = validator(ST, to_add, err);
                    if(!is_valid)
                    {
                        if(err == "0")
                        {
                            cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined globaly" << endl; 
                        }
                        else
                        {
                            cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined locally" << endl;
                        }
                        return nullptr;
                    }
                    
                    ST.append(to_add, false);
                    
                    temp = temp->sibling;
                }
            }
            else
            {
                name = temp->sibling->token;
                i_type = "datatype";
                d_type = temp->token;
                if(temp->sibling->sibling->token == "[")
                {
                    is_array = "yes";
                    array_size = temp->sibling->sibling->sibling->token;
                }
                else
                {
                    is_array = "no";
                    array_size = "0";
                }
                scope = scope;
                
                Symbol* to_add = new Symbol(name, i_type, d_type, is_array, array_size, to_string(scope));
            
                string err;
                is_valid = validator(ST, to_add, err);
                
                if(!is_valid)
                {
                    if(err == "0")
                    {
                        cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined globaly" << endl; 
                    }
                    else
                    {
                        cout << "Error on line " << temp->line_num << ": variable \"" << name << "\" is already defined locally" << endl;
                    }
                    return nullptr;
                }
            
                ST.append(to_add, true);
                
                if(is_array == "yes")
                {
                    temp = temp->sibling->sibling->sibling->sibling->sibling;
                }
                else
                {
                    temp = temp->sibling->sibling;
                }
                
                if(temp->token != ")")
                {
                    temp = createSymbol(ST, CST, temp->sibling, count, scope, true);
                }
            }
                
        }
    }
    else if(temp->token == "{")
    {
        count++; 
    }
    else if(temp->token == "}")
    {
        count--;
        if(count == 0)
        {
            scope+=1;
        }
    }
    return temp;
}
    

int main()
{
	string filename;

	cout << "Please Enter Filename:" << endl;
	cin >> filename;

	string outstring = filename;

	ifstream in_f(filename);

	if (!in_f.is_open())
	{
		cerr << "Error opening the input file!";
		return -1;
	}

	string outfile = outstring.erase(outstring.length()-2,outstring.length()-1) + "-comments_replaced_with_whitespace.c";

	ofstream out_f(outfile);

	if (!out_f.is_open())
	{
		std::cerr << "Error opening the output file." << std::endl;
		in_f.close();
		return -1;
	}


	char c = removeComments(in_f, out_f);

	if(c < 0)
	{
		return c;
	}

	in_f.close();
	out_f.close();
    
	ifstream in_f2(outfile);

	if (!in_f2.is_open())
	{
		cerr << "Error opening the 2nd input file!";
		return -7;
	}
	
	outstring.pop_back();
	
	string outstring2 = "output-" + outstring + "txt";

	int line_num = 1;
	
	LCRS_Tree LCRS;
	
	bool check = true;
	
	while (in_f2.get(c))
	{
	    c = createTokens(in_f2, LCRS, c, line_num, check);
    
        if(c < 0)
	    {
		    return c;
	    } 
	}
    
    SymbolTable ST;
    Node *temp = LCRS.getHead();
    Symbol *end = ST.getHead();
    int count = 0;
    int scope = 1;
    
    
    while(temp->sibling != nullptr || temp->child != nullptr)
    {
        temp = createSymbol(ST, LCRS, temp, count, scope, false);
        if (temp == nullptr)
        {
            return -11;
        }
        else
        {
            if(temp->sibling != nullptr)
            {
                temp = temp->sibling;
            }
            else if (temp->child != nullptr)
            {
                temp = temp->child;
            }
        }
    }
    
    cout << endl;
    
    Symbol* i = ST.getHead();
    while(i != nullptr)
    {
        cout << "IDENTIFIER_NAME: " << i->name << endl; 
        cout << "IDENTIFIER_TYPE: " << i->i_type << endl;
        cout << "DATATYPE: " << i->d_type << endl;
        cout << "DATATYPE_IS_ARRAY: " << i->is_array << endl;
        cout << "DATATYPE_ARRAY_SIZE: " << i->array_size <<endl;
        cout << "SCOPE: " << i->scope << endl;
        cout << endl;
        
        i = i->next_symbol;
    }
    i = ST.getHead();
    while(i != nullptr)
    {
        Symbol* j = i->next_parameter;
        while(j != nullptr)
        {
            cout << "PARAMETER LIST FOR: " << i->name << endl;
            cout << "IDENTIFIER_NAME: " << j->name << endl;
            cout << "DATATYPE: " << j->d_type << endl;
            cout << "DATATYPE_IS_ARRAY: " << j->is_array << endl;
            cout << "DATATYPE_ARRAY_SIZE: " << j->array_size << endl;
            cout << "SCOPE: " << j->scope << endl;
            cout << endl;
            
            j = j->next_parameter;
        }
        i = i->next_symbol;
    }
    
    
    in_f2.close();

	return 0;
}