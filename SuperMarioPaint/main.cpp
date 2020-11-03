/*
    Joseph Hesketh Prichard
    JEH190000
    10/11/20
*/
#include <iostream>
#include <fstream>
#include <ostream>

using namespace std;

const int pen_command = 0;
const int direction_command = 1;
const int distance_command = 2;
const int bold_command = 3;
const int output_command = 4;
const char text = '*';
const char bold = '#';
const char bold_symbol = 'B';
const char output_symbol= 'P';
const char pen_down = 2;
const char pen_up = 1;
const string right_direction = "E";
const string left_direction = "W";
const string up_direction = "N";
const string down_direction = "S";
const int height = 50;
const int width = 50;

struct Node {
    char c = ' ';
    Node* up_ptr = nullptr;
    Node* down_ptr = nullptr;
    Node* left_ptr = nullptr;
    Node* right_ptr = nullptr;
};

void createGrid(Node* &head);

void printGrid(Node*, ostream &out, Node*);

void deleteGrid(Node*);

void createHorizontalLink(Node*,Node*);

void createVerticalLink(Node*,Node*);

void goLeft(Node* &curr, int);

void goDown(Node* &curr, int);

void goUp(Node* &curr, int);

void goRight(Node* &curr, int);

void drawLeft(Node* &curr, int, char);

void drawDown(Node* &curr, int, char);

void drawUp(Node* &curr, int, char);

void drawRight(Node* &curr, int, char);

void drawCommand(int, const string&, Node* &curr, int, char);

void parseCommand(string, int &pen, string &direction,
                  int &distance, char &to_print, bool &ignore_next, bool &output);

void parseCommandComponent(string component, int, int &pen, string &direction,
                           int &distance, char &to_print, bool &ignore_next, bool &output);

int main()
{
    //receive the file name of the input file and open it
    ifstream input;
    string file_name;
    cout << "Enter an input file: ";
    cin >> file_name;
    cout << endl;
    input.open(file_name, ios::in);
    if(!input) { //if the input does not open output error message and return from main
        printf("%s","Invalid file name.");
        return 0;
    }

    //create grid
    //pointer to first element and current element
    Node* head = nullptr;
    createGrid(head);
    Node* curr = head;

    //command components
    int pen = pen_down;
    string direction = "E";
    int distance = 0;
    char to_print = text;
    bool output = false;

    string line;

    bool ignore = false;
    //look through each line in input commands file
    while(getline(input,line)) {
        //parse the command line and assign values to command components (pass by reference)
        parseCommand(line, pen, direction, distance, to_print, ignore, output);
        //draw based on command if the command is not to be ignored
        if(!ignore) {
            //draw and pass commands components
            drawCommand(pen, direction, curr, distance, to_print);
            //output if command should be printed
            if(output) {
                printGrid(head, cout, head);
                cout << endl;
                cout << endl;
            }
        }
        //reset to parse new command
        to_print = text;
        output = false;
        ignore = false;
    }

    ofstream paint;
    paint.open("paint.txt", ios::trunc);
    if(!paint) {
        cout << "Invalid output file";
        return 0;
    }
    printGrid(head, paint, head);

    paint.close();

    //close input file and delete grid
    input.close();
    deleteGrid(head);
    return 0;
}

//creates a grid and assigns head to 0,0
void createGrid(Node* &head) {
    Node* up = nullptr; //pointer to linked list above
    Node* left = new Node(); //pointer to left element in a linked list
    Node* first = left; //the first node in our new linked list
    head = left;
    for(int i = 0; i < height; i++) {
        //create a doubly linked list of right and left nodes
        for(int j = 1; j < width; j++) {
            //create a right node
            Node* right = new Node();
            //link left,right then up,down
            createHorizontalLink(left,right);
            createVerticalLink(up,left);
            //move to the right
            left = right;
            if(up != nullptr) {
                up = up->right_ptr;
            }
        }
        //link vertically then go to the next row
        createVerticalLink(up,left);
        up = first;
        left = new Node();
        first = left;
    }
}

//creates a link between a left and a right node
void createHorizontalLink(Node* left, Node* right) {
    if(left != nullptr) {
        left->right_ptr = right;
    }
    if(right != nullptr) {
        right->left_ptr = left;
    }
}

//creates a link between an up and a down node
void createVerticalLink(Node* up, Node* down) {
    if(up != nullptr) {
        up->down_ptr = down;
    }
    if(down != nullptr) {
        down->up_ptr = up;
    }
}

//prints a node in out stream and moves the pointer to the next element in the grid
//starts at the current node
void printGrid(Node* curr, ostream &out, Node* anchor) {
    //if the next node is null (end of list) go to the next list
    out << curr->c;
    if(curr->right_ptr == nullptr) {
        //terminate if there is no next list
        if(curr->down_ptr == nullptr) {
            return;
        }
        //go to the next row
        curr = anchor;
        goDown(curr, 1);
        //print a new line and then the next character
        out << endl;
        out << curr->c;
        //print node and go to right
        printGrid(curr->right_ptr, out, curr);
    } else {
        //print node and go to right
        printGrid(curr->right_ptr, out, anchor);
    }

}

//deletes a node and moves the pointer to the next element in the grid
//starts the deletion at the current node
void deleteGrid(Node* curr) {
    //if the next node is null (end of list) go to the next list
    if(curr->right_ptr == nullptr) {
        Node* to_delete = curr;
        //terminate if there is no next list
        if(curr->down_ptr == nullptr) {
            return;
        }
        goDown(curr, 1);
        goLeft(curr, width-1);
        delete to_delete;
    }
    //delete node and go to right
    Node* next = curr->right_ptr;
    delete curr;
    deleteGrid(next);
}

//recursive function to move the pointer down until the amount is 0
void goRight(Node* &curr, int amount) {
    if(amount == 0) {
        return;
    }
    curr = curr->right_ptr;
    goRight(curr,amount-1);
}

//recursive function to go move the pointer down until the amount is 0
void goDown(Node* &curr, int amount) {
    if(amount == 0) {
        return;
    }
    curr = curr->down_ptr;
    goDown(curr,amount-1);
}

//recursive function to move the pointer left until the amount is 0
void goLeft(Node* &curr, int amount) {
    if(amount == 0) {
        return;
    }
    curr = curr->left_ptr;
    goLeft(curr,amount-1);
}

//recursive function to go move the pointer up until the amount is 0
void goUp(Node* &curr, int amount) {
    if(amount == 0) {
        return;
    }
    curr = curr->up_ptr;
    goUp(curr,amount-1);
}

//recursive function to move the pointer down until the amount is 0
void drawDown(Node* &curr, int amount, char to_print) {
    //decrement until amount is 0
    if(amount == 0) {
        return;
    }
    curr = curr->down_ptr;
    //overwrite character if its not bold
    if(curr->c != bold) {
        curr->c = to_print;
    }
    drawDown(curr,amount-1,to_print);
}

//recursive function to move the pointer left until the amount is 0
void drawLeft(Node* &curr, int amount, char to_print) {
    //decrement until amount is 0
    if(amount == 0) {
        return;
    }
    curr = curr->left_ptr;
    //overwrite character if its not bold
    if(curr->c != bold) {
        curr->c = to_print;
    }
    drawLeft(curr,amount-1,to_print);
}

//recursive function to go move the pointer up until the amount is 0
void drawUp(Node* &curr, int amount, char to_print) {
    //decrement until amount is 0
    if(amount == 0) {
        return;
    }
    curr = curr->up_ptr;
    //overwrite character if its not bold
    if(curr->c != bold) {
        curr->c = to_print;
    }
    drawUp(curr,amount-1,to_print);
}

//recursive function to move the pointer down until the amount is 0
void drawRight(Node* &curr, int amount, char to_print) {
    //decrement until amount is 0
    if(amount == 0) {
        return;
    }
    curr = curr->right_ptr;
    //overwrite character if its not bold
    if(curr->c != bold) {
        curr->c = to_print;
    }
    drawRight(curr,amount-1,to_print);
}

bool hasSpace(string str) {
    //loop through string char by char
    for(unsigned int i = 0; i < str.size(); i++) {
        if(str[i] == ' ') { //if string has any space return true (it has at least one space)
            return true;
        }
    }
    return false;
}

//parse command and send it to variables
void parseCommand(string command, int &pen, string &direction,
                  int &distance, char &to_print, bool &ignore_next, bool &output) {
    //ignore the command if it has any spaces
    //last character is a comma
    //command is an empty string (cannot be checked)
    if(hasSpace(command) || command[command.length()-1] == ',' || command.empty()) {
        ignore_next = true;
        return;
    }
    //expect the pen command first
    int command_num = pen_command;
    //loop until command string is empty
    while(command.length() > 0) {
        //get index location of comma
        string component;
        long long int index = command.find(',');
        if (index != -1) {
            component = command.substr(0,index);   //copy component from command
            command = command.substr(index+1);   //break off component from command
            //assign component to proper variable and validate
            parseCommandComponent(component, command_num, pen, direction, distance, to_print, ignore_next, output);
        } else {
            //assign component (remainder of the command) to proper variable and validate
            parseCommandComponent(command, command_num, pen, direction, distance, to_print, ignore_next, output);
            //empty command to terminate loop
            command = "";
        }
        command_num++;
    }
}

//determine the active command component and parse it from a string to assign it to a variable
void parseCommandComponent(string component, int command_num, int &pen, string &direction,
                           int &distance, char &to_print, bool &ignore_next, bool &output) {
    try {
        //parse the command string
        //assign to a different variable depending on current command component
        //variables store the current command components
        if(command_num == pen_command) {
            //convert to pen number
            pen = stoi(component);
        } else if(command_num == direction_command) {
            //if its more than one character ignore
            if(component.length() > 1) {
                ignore_next = true;
            }
            //convert to direction char
            direction = static_cast<char>(component[0]);
        } else if(command_num == distance_command) {
            //convert to distance number
            distance = stoi(component);
            //if its smaller than 1 do not execute
            if(distance < 1) {
                ignore_next = true;
            }
        } else if(command_num == bold_command) {
            //if its more than one character ignore
            if(component.length() > 1) {
                ignore_next = true;
            }
            if(component[0] == bold_symbol) {
                //print bold next time
                if(pen == pen_up) {
                    ignore_next = true;
                } else {
                    to_print = bold;
                }
            } else if(component[0] == output_symbol) {
                //output to console next time
                output = true;
            } else {
                ignore_next = true;
            }
        } else if(command_num == output_command) {
            //if its more than one character ignore
            if(component.length() > 1) {
                ignore_next = true;
            }
            if(component[0] == output_symbol) {
                //output to console next time
                output = true;
            } else {
                ignore_next = true;
            }
        }
        //ignore the command if it has any spaces
        if(hasSpace(component)) {
            ignore_next = true;
        }
    } catch(...) { //ignore if there are any errors
        ignore_next = true;
    }
}

//checks if a given direction is within bounds
bool inBoundsRight(Node* curr, int distance) {
    //loop until distance and if curr is ever null distance is out of bounds
    for(int i = 1; i <= distance; i++) {
        curr = curr->right_ptr;
        //if null out of bounds
        if(curr == nullptr) {
            return false;
        }
    }
    return true;
}

bool inBoundsLeft(Node* curr, int distance) {
    //loop until distance and if curr is ever null distance is out of bounds
    for(int i = 1; i <= distance; i++) {
        curr = curr->left_ptr;
        //if null out of bounds
        if(curr == nullptr) {
            return false;
        }
    }
    return true;
}

bool inBoundsUp(Node* curr, int distance) {
    //loop until distance and if curr is ever null distance is out of bounds
    for(int i = 1; i <= distance; i++) {
        curr = curr->up_ptr;
        //if null out of bounds
        if(curr == nullptr) {
            return false;
        }
    }
    return true;
}

bool inBoundsDown(Node* curr, int distance) {
    //loop until distance and if curr is ever null distance is out of bounds
    for(int i = 1; i <= distance; i++) {
        curr = curr->down_ptr;
        //if null out of bounds
        if(curr == nullptr) {
            return false;
        }
    }
    return true;
}

//draws a command to canvas based off command info
void drawCommand(int pen, const string& direction, Node* &curr, int distance, char to_print) {
    if(pen == pen_down) { //pen down
        if(direction == right_direction) {
            //if within the bounds going right, draw right
            if(inBoundsRight(curr, distance)) {
                drawRight(curr,distance,to_print);
            }
        } else if(direction == left_direction) {
            //if within the bounds going left, draw left
            if(inBoundsLeft(curr, distance)) {
                drawLeft(curr,distance,to_print);
            }
        } else if(direction == down_direction) {
            //if within the bounds going down, draw down
            if(inBoundsDown(curr, distance)) {
                drawDown(curr,distance,to_print);
            }
        } else if(direction == up_direction) {
            //if within the bounds going up, draw up
            if(inBoundsUp(curr, distance)) {
                drawUp(curr,distance,to_print);
            }
        }
    } else if(pen == pen_up) { //pen up
        if(direction == ::right_direction) {
            //if within the bounds going right, go right
            if(inBoundsRight(curr, distance)) {
                goRight(curr,distance);
            }
        } else if(direction == left_direction) {
            //if within the bounds going left, go left
            if(inBoundsLeft(curr, distance)) {
                goLeft(curr,distance);
            }
        } else if(direction == down_direction) {
            //if within the bounds going down, go down
            if(inBoundsDown(curr, distance)) {
                goDown(curr,distance);
            }
        } else if(direction == up_direction) {
            //if within the bounds going up, go up
            if(inBoundsUp(curr, distance)) {
                goUp(curr,distance);
            }
        }
    }
}
