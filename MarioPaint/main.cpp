/*
    Joseph Hesketh Prichard
    JEH190000
*/

#include <iostream>
#include <fstream>
#include <cctype>
#include <string.h>
#include <cmath>

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
const int canvas_size = 50;
const char separator = ',';
const char terminator = '\n';
const char pen_down = 2;
const char pen_up = 1;
const string right = "E";
const string left = "W";
const string up = "N";
const string down = "S";
const string file_canvas = "paint.txt";

bool inBoundsRight(fstream &paint, int);

bool inBoundsLeft(fstream &paint, int);

bool inBoundsUp(fstream &paint, int);

bool inBoundsDown(fstream &paint, int);

void drawRight(fstream &paint, int, char);

void drawLeft(fstream &paint, int, char);

void drawUp(fstream &paint, int, char);

void drawDown(fstream &paint, int, char);

void seekRight(fstream &paint, int);

void seekLeft(fstream &paint, int);

void seekUp(fstream &paint, int);

void seekDown(fstream &paint, int);

void drawCommand(int, string, fstream &paint, int, char);

void printFile(string);

void parseCommand(string, int &pen, string &direction,
                        int &distance, char &to_print, bool &ignore_next, bool &output);

void parseCommandComponent(string component, int, int &pen, string &direction,
                        int &distance, char &toprint, bool &ignoreNext, bool &output);

int main(){
    /////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////          DO  NOT CHANGE CODE BELOW THIS               //////////////
    /////////////////////////////////////////////////////////////////////////////////////////

    ifstream infile("paint_base.txt");
    ofstream outfile("paint.txt", ios::binary);
    string c;
    if (infile)
        while (getline(infile, c))
            outfile << c << "\n";

    infile.close();
    outfile.close();

    /////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////          DO  NOT CHANGE CODE ABOVE THIS               //////////////
    /////////////////////////////////////////////////////////////////////////////////////////

    //open the canvas to draw to
    fstream paint;
    paint.open(file_canvas, ios::in | ios::out | ios::ate | ios::binary);
    paint.seekp(0);

    //receive the file name of the input file and open it
    ifstream input;
    string file_name;
    cin >> file_name;
    input.open(file_name, ios::in);
    if(!input) { //if the input does not open output error message and return from main
        printf("%s","Invalid file name.");
        return 0;
    }

    //command components
    int pen = pen_down;
    string direction = "E";
    int distance = 0;
    char to_print = text;
    bool output = false;

    string line = "";

    bool ignore = false;
    //look through each line in input commands file
    while(getline(input,line)) {
        //parse the command line and assign values to command components (pass by reference)
        parseCommand(line, pen, direction, distance, to_print, ignore, output);
        //draw based on command if the command is not to be ignored
        if(!ignore) {
            //draw and pass commands components
            drawCommand(pen, direction, paint, distance, to_print);
            //output if command should be printed
            if(output) {
                printFile(file_canvas);
            }
        }
        //reset to parse new command
        to_print = text;
        output = false;
        ignore = false;
    }

    //close file and return from main
    paint.close();
    input.close();
    return 0;
}

bool isAllSpace(string str) {
    //loop through string char by char
    for(unsigned int i = 0; i < str.size(); i++) {
        if(str[i] != ' ') { //if string has any non space return false (it is not all space)
            return false;
        }
    }
    return true;
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

//write a file to console
void printFile(string file_name) {
    ifstream file;
    file.open(file_name, ios::binary); //open a new file stream
    int line_counter = 0;
    int first_line = -1; //the first line that contains characters
    int last_line = -1; //the second line that contains characters
    bool expecting_first = true; //whether or not we're at the first line yet
    string line;
    //loop through each line
    while(getline(file,line)) {
        if(!isAllSpace(line)) {
            //if line is all space and we're expecting the first line
            if(expecting_first) {
                first_line = last_line = line_counter;
                expecting_first = false;
            } else { //if line is all space and we're expecting the last line
                last_line = line_counter;
            }
        }
        line_counter++;
    }
    file.close(); //close when finished
    file.open(file_name, ios::binary); //open file stream again
    line_counter = 0;
    //loop through each line
    while(getline(file,line)) {
        //if the line is between the first and the last line print it
        if(line_counter >= first_line && line_counter <= last_line) {
            cout << line << endl;
        }
        line_counter++;
    }
    file.close(); //close when finished
}

//checks if a given direction is within bounds
bool inBoundsRight(fstream &paint, int distance) {
    //if the new column position after drawing exceeds canvas size
    return ((paint.tellp() % (canvas_size+1)) + distance) < canvas_size;
}

bool inBoundsLeft(fstream &paint, int distance) {
    //if the new column position after drawing position deceed past 0
    return ((paint.tellp() % (canvas_size+1)) - distance) >= 0;
}

bool inBoundsUp(fstream &paint, int distance) {
    //if the new row position after drawing deceed past 0
    return (floor(paint.tellp() / (canvas_size+1)) - distance) >= 0;
}

bool inBoundsDown(fstream &paint, int distance) {
    //if the new row position after drawing exceeds past 0
    return (floor(paint.tellp() / (canvas_size+1)) + distance) < canvas_size;
}

//draw in right direction on canvas
void drawRight(fstream &paint, int distance, char toprint) {
    long long int pos = paint.tellp();
    //loop through each character for distance
    for(int i = 1; i <= distance; i++) {
        //calculate new position (1 forward every iteration)
        long long int loc = pos+i;
        paint.seekp(loc);
        //if position does not have a bold print a character
        if(paint.peek() != bold) {
            paint << toprint;
        }
        paint.seekp(loc);
    }
}

//draw in left direction on canvas
void drawLeft(fstream &paint, int distance, char toprint) {
    long long int pos = paint.tellp();
    //loop through each character position for distance
    for(int i = 1; i <= distance; i++) {
        //calculate new position (1 backwards every iteration)
        long long int loc = pos-i;
        paint.seekp(loc);
        //if position does not have a bold print a character
        if(paint.peek() != bold) {
            paint << toprint;
        }
        paint.seekp(loc);
    }
}

//draw in up direction on canvas
void drawUp(fstream &paint, int distance, char toprint) {
    long long int pos = paint.tellp();
    //loop through each character position for distance
    for(int i = 1; i <= distance; i++) {
        //calculate new position (51 backwards every iteration)
        long long int loc = (pos-(canvas_size+1)*i);
        paint.seekp(loc);
        //if position does not have a bold print a character
        if(paint.peek() != bold) {
            paint << toprint;
        }
        paint.seekp(loc);
    }
}

//draw in down direction on canvas
void drawDown(fstream &paint, int distance, char toprint) {
    long long int pos = paint.tellp();
    //loop through each character position for distance
    for(int i = 1; i <= distance; i++) {
        //calculate new position (51 forward every iteration)
        long long int loc = (pos+(canvas_size+1)*i);
        paint.seekp(loc);
        //if position does not have a bold print a character
        if(paint.peek() != bold) {
            paint << toprint;
        }
        paint.seekp(loc);
    }
}

//seek right direction (move pen but do not draw on canvas)
void seekRight(fstream &paint, int distance) {
    long long int pos = paint.tellp();
    paint.seekp(pos+distance);
}

//seek left direction (move pen but do not draw on canvas)
void seekLeft(fstream &paint, int distance) {
    long long int pos = paint.tellp();
    paint.seekp(pos-distance);
}

//seek down direction (move pen but do not draw on canvas)
void seekDown(fstream &paint, int distance) {
    long long int pos = paint.tellp();
    paint.seekp((pos+(canvas_size+1)*distance));
}

//seek up direction (move pen but do not draw on canvas)
void seekUp(fstream &paint, int distance) {
    long long int pos = paint.tellp();
    paint.seekp((pos-(canvas_size+1)*distance));
}

//parse command and send it to variables
void parseCommand(string command, int &pen, string &direction,
                        int &distance, char &to_print, bool &ignore_next, bool &output) {
    //ignore the command if it has any spaces
    //last character is a comma
    //command is an empty string (cannot be checked)
    if(hasSpace(command) || command[command.length()-1] == ',' || command == "") {
        ignore_next = true;
        return;
    }
    //expect the pen command first
    int command_num = pen_command;
    //loop until command string is empty
    while(command.length() > 0) {
        //get index location of comma
        string component;
        long int index = command.find(',');
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

//draws a command to canvas based off command info
void drawCommand(int pen, string direction, fstream &paint, int distance, char to_print) {
    if(pen == pen_down) { //pen down
        if(direction.compare(::right) == 0) {
            //if within the bounds going right, draw right
            if(inBoundsRight(paint, distance)) {
                drawRight(paint,distance,to_print);
            }
        } else if(direction.compare(::left) == 0) {
            //if within the bounds going left, draw left
            if(inBoundsLeft(paint, distance)) {
                drawLeft(paint,distance,to_print);
            }
        } else if(direction.compare(::down) == 0) {
            //if within the bounds going down, draw down
            if(inBoundsDown(paint, distance)) {
                drawDown(paint,distance,to_print);
            }
        } else if(direction.compare(::up) == 0) {
            //if within the bounds going up, draw up
            if(inBoundsUp(paint, distance)) {
                drawUp(paint,distance,to_print);
            }
        }
    } else if(pen == pen_up) { //pen up
        if(direction.compare(::right) == 0) {
            //if within the bounds going right, seek right
            if(inBoundsRight(paint, distance)) {
                seekRight(paint,distance);
            }
        } else if(direction.compare(::left) == 0) {
            //if within the bounds going left, seek left
            if(inBoundsLeft(paint, distance)) {
                seekLeft(paint,distance);
            }
        } else if(direction.compare(::down) == 0) {
            //if within the bounds going down, seek down
            if(inBoundsDown(paint, distance)) {
                seekDown(paint,distance);
            }
        } else if(direction.compare(::up) == 0) {
            //if within the bounds going up, seek up
            if(inBoundsUp(paint, distance)) {
                seekUp(paint,distance);
            }
        }
    }
}

