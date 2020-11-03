/*
    Joseph Hesketh Prichard
    JEH190000
*/
#include <iostream>
#include <fstream>
#include <memory>
#include <iomanip>

using namespace std;

const int cols = 4;
const int min_row = 1;

int number_lines(string);

void init_streams(float**,string);

void init_stream(float*,string);

void parse_coefficient(string,float &x,float &y,float &z,float);

void parse_sum(string,float &sum);

void init_stream(float*,float,float,float,float);

void output_streams(float**,int);

void switch_streams(float**,int,int);

void multiply_stream(float**,int,float);

void add_scalar_multiple(float**,int,float,int);

bool is_reduced(float**,int);

bool is_row_zero(float*);

bool is_row_reduced(float*,int &one_pos);

void recieve_user_input(float**,int);

void recieve_switch_input(float**,int);

void recieve_multiply_input(float**,int);

void recieve_add_input(float**,int);

bool check_solved(float**,int);

void output_solution(float**,int);

void output_term(float*,int,float*);

void delete_streams(float**,int);

void loop_input_bounds(int &val, string, int);

bool row_valid_mod(float**,int row);

void loop_input_mult(float &val, string);

int main()
{
    string file_name = "equations.txt";
    cin >> file_name;

    //test file name
    fstream temp_file;
    temp_file.open(file_name);
    if(!temp_file) {
        return 0;
    }
    temp_file.close();

    //declare and initialize streams based off file name
    int rows = number_lines(file_name);
    float** streams = new float*[rows];
    init_streams(streams,file_name);

    output_streams(streams,rows);
    if(is_reduced(streams,rows)) { //check if a matrix is already solved
        output_solution(streams,rows);
        return 0;
    }

    bool isSolved = false;
    while(!isSolved) {
        int selected;
        cout << "Enter an operation to take: ";
        cin >> selected;
        cout << selected << endl;
        switch (selected) { //determine which operation to take
        case 1:
            recieve_switch_input(streams,rows);
            isSolved = check_solved(streams,rows);
            break;
        case 2:
            recieve_multiply_input(streams,rows);
            isSolved = check_solved(streams,rows);
            break;
        case 3:
            recieve_add_input(streams,rows);
            isSolved = check_solved(streams,rows);
            break;
        case 4:
            cout << "Quit. Thank you for playing CrossTheStreams." << endl;
            output_streams(streams,rows);
            isSolved = true;
            break;
        }
    }

    delete_streams(streams,rows); //delete allocated streams

    return 0;
}

//delete all memory stored at double pointer
void delete_streams(float** streams, int rows) {
    float** pos = streams;
    for(int i = 0; i < rows; i++) {
        delete[] *(streams); //delete memory stored at each single pointer
        streams++;
    }
    streams = pos;
    delete[] streams; //delete memory stored at double pointer
}

//initialize all streams
void init_streams(float** streams, string file_name) {
    ifstream file;
    file.open(file_name, ios::binary);
    float** pos = streams; //anchor
    string line = "";
    //loop through line by line
    while(getline(file,line)) {
        //a stream array for each line
        float* stream = new float[cols];
        init_stream(stream,line); //initialize the stream
        *streams = stream; //assign a pointer to the stream
        streams++;
    }
    streams = pos;
    file.close();
}

//parse the string equation into the stream array
void init_stream(float* stream, string equation) {
    float x = 0;
    float y = 0;
    float z = 0;
    float sum = 0;
    float multiplier = 1;
    bool atEquals = false; //stores if we are at the = sign yet
    //loop until equation is empty
    while(equation.length() > 0) {
        string coefficient;
        long long int index = equation.find_first_of("+-="); //find the first index with a + = or -
        if(index != -1 && !atEquals) { //continue if we're not at = sign yet
            char c = equation.at(index); //store character at index
            coefficient = equation.substr(0,index); //copy coefficient from equation
            equation = equation.substr(index+1); //break off coefficient from equation
            parse_coefficient(coefficient,x,y,z,multiplier); //parse coefficient into variable
            if(c == '-') {
                multiplier = -1; //next coefficient should be negative
            } else {
                if(c == '=') {
                    atEquals = true; //reached =
                }
                multiplier = 1;
            }
        } else { //parse the rest of equation into sum
            parse_sum(equation,sum);
            equation="";
        }
    }
    init_stream(stream,x,y,z,sum); //initialize stream with parsed values
}

//initialize a stream with given x y z and sum values
void init_stream(float* stream, float x, float y, float z, float sum) {
    float *pos = stream; //anchor
    *stream = x;
    stream++;
    *stream = y;
    stream++;
    *stream = z;
    stream++;
    *stream = sum;
    stream = pos;
}

void parse_coefficient(string coefficient, float &x, float &y, float &z, float multiplier) {
    //get indexes for x y and z
    long long int x_index = coefficient.find('x');
    long long int y_index = coefficient.find('y');
    long long int z_index = coefficient.find('z');
    //determine if x y or z was found
    //cutoff until letter
    //parse value or assign 1 if no value
    if(x_index != -1) {
        coefficient = coefficient.substr(0,x_index);
        x = coefficient != "" ? stof(coefficient) : 1;
        x *= multiplier;
    } else if(y_index != -1) {
        coefficient = coefficient.substr(0,y_index);
        y = coefficient != "" ? stof(coefficient) : 1;
        y *= multiplier;
    } else if(z_index != -1) {
        coefficient = coefficient.substr(0,z_index);
        z = coefficient != "" ? stof(coefficient) : 1;
        z *= multiplier;
    }
}

//parse sum into coefficient
void parse_sum(string coefficient, float &sum) {
    sum = stof(coefficient);
}

//output streams to console
void output_streams(float** streams, int rows) {
    float** pos = streams; //anchor
    for(int i = 0; i < rows; i++) {
        float* stream = *streams;
        float* pos = stream; //anchor
        for(int j = 0; j < cols; j++) {
            float val = *stream;
            if (val == 0) val = 0;
            cout << setprecision(2) << fixed << setw(6) << val << " "; //output with a precision of 2
            stream++;
        }
        stream = pos;
        cout << endl;
        streams++;
    }
    cout << endl;
    streams = pos;
}

//calculate the number of lines in the file
//finds the number of streams
int number_lines(string file_name) {
    ifstream file;
    file.open(file_name, ios::binary);
    int rows = 0;
    string line = "";
    while(getline(file,line)) {
        //one stream fore each line
        if(line != "") {
            rows++;
        }
    }
    file.close();
    return rows;
}


void switch_streams(float** streams, int row1, int row2) {
    float** pos = streams; //anchor
    for(int i = 0; i < row1-1; i++) { //loop until first row
        streams++;
    }
    float* stream1 = *streams; //dereference first row
    streams = pos;
    for(int i = 0; i < row2-1; i++) { //loop until second row
        streams++;
    }
    float* stream2 = *streams; //dereference second row
    *streams = stream1; //assign to position
    streams = pos;
    for(int i = 0; i < row1-1; i++) { //loop until first row
        streams++;
    }
    *streams = stream2;
    streams = pos;
}

void multiply_stream(float** streams, int row, float scalar) {
    float** pos = streams; //anchor
    for(int i = 0; i < row-1; i++) { //loop until row
        streams++;
    }
    float* stream = *streams; //dereference stream
    for(int i = 0; i < cols; i++) { //loop through each element
        *stream *= scalar; //multiply each element by scalar
        stream++;
    }
    streams = pos;
}

//add scalar multiple to streams
void add_scalar_multiple(float** streams, int row_mult, float scalar, int row_mod) {
    float** pos = streams; //anchor
    for(int i = 0; i < row_mult-1; i++) { //loop until row to mult
        streams++;
    }
    float* stream_mult = *streams; //dereference stream to multiply to
    streams = pos;
    for(int i = 0; i < row_mod-1; i++) { //loop until row to modify
        streams++;
    }
    float* stream_modified = *streams; //deference stream to modify
    for(int i = 0; i < cols; i++) { //loop through each value in both rows
        //add value of element in stream multiply to stream modified times scalar
        *stream_modified += scalar*(*stream_mult);
        stream_modified++;
        stream_mult++;
    }
    streams = pos;
}

//check if matrix is reduced by looking at each row of the matrix
bool is_reduced(float** streams, int rows) {
    float** pos = streams; //anchor
    int one_pos = -1;
    for(int i = 0; i < rows; i++) {
        float* stream = *streams;
        if(is_row_zero(stream) && i != rows-1) {
            //if row is all 0 and is not the last row matrix is not reduced
            return false;
        } else if(!is_row_reduced(stream,one_pos)) {
            //if a row is not reduced relative to other rows checked so far
            return false;
        }
        streams++;
    }
    streams = pos;
    return true;
}

//determines if a row is reduced, relative to other rows in the matrix so far
bool is_row_reduced(float* stream, int &one_pos) {
    float* pos = stream; //anchor
    bool reached_one = false; //determines if we have reached 1 value yet
    for(int i = 0; i < cols-1; i++) {
        float val = *stream; //value at pointer
        //if value is 1 and we haven't reached 1
        if(val == 1 && !reached_one) {
            reached_one = true;
            /*if the position is not to the right relative to
            previous position, matrix is not reduced*/
            if(i <= one_pos) {
                stream = pos;
                return false;
            }
            one_pos = i; //assign new position the 1 was found
        } else if(val != 0) { //if any value besides 0 is found matrix is not reduced
            stream = pos;
            return false;
        }
        stream++;
    }
    stream = pos;
    return true;
}

//determines if a row is all zeros
bool is_row_zero(float* stream) {
    float* pos = stream;
    //loop through each column, return false if a non zero is found
    for(int i = 0; i < cols; i++) {
        float val = *stream;
        if(val != 0) {
            return false;
        }
        stream++;
    }
    stream = pos;
    return true;
}

//return true if matrix is reduced, otherwise return false
bool check_solved(float** streams, int rows) {
    if(is_reduced(streams, rows)) { //if it is reduced matrix is solved
        cout << "Matrix is in reduced row echelon form." << endl;
        output_solution(streams,rows);
        return true;
    } else {
        return false;
    }
}

//output the solution
void output_solution(float** streams, int rows) {
    float** pos = streams; //anchor
    for(int i = 0; i < rows; i++) {
        float* stream = *streams;
        float* pos = stream; //anchor
        for(int j = 0; j < cols-1; j++) {
            if(*stream == 1) { //found a term
                output_term(stream,i,pos); //output each term individually
                break;
            }
            stream++;
        }
        streams++;
        stream = pos;
    }
    streams = pos;
}

//output a term to console
void output_term(float* stream, int num, float* pos) {
    stream = pos; //anchor
    for(int i = 0; i < cols-1; i++) {
        stream++; //navigate to solution position
    }
    float sol = *stream; //get solution
    if (sol == 0) sol = 0;
    stream = pos;
    switch(num){ //determine which term and then output
    case 0:
        cout << setprecision(2) << fixed << "x = " << sol << endl;
        break;
    case 1:
        cout << setprecision(2) << fixed << "y = " << sol << endl;
        break;
    case 2:
        cout << setprecision(2) << fixed << "z = " << sol << endl;
        break;
    }
}

//inclusive check
bool within_bounds(int val, int min_val, int max_val) {
    return val >= min_val && val <= max_val;
}

//checks if a value is 0
bool is_zero(float val) {
    return val == 0;
}

//receive input to switch
void recieve_switch_input(float** streams, int rows) {
    int row1 = 0;
    int row2 = 0;
    //assign input to variables
    loop_input_bounds(row1, "Enter the first row to switch: ", rows);
    loop_input_bounds(row2, "Enter the second row to switch: ", rows);
    //do switch on matrix
    switch_streams(streams,row1,row2);
    output_streams(streams,rows);
}

//receive input to multiply
void recieve_multiply_input(float** streams, int rows) {
    int row = 0;
    float mult = 0;
    //assign input to variables
    loop_input_bounds(row, "Enter the row to multiply: ", rows);
    loop_input_mult(mult, "Enter the scalar multiplier: ");
    //check if row can be modified, a solved row cannot be modified
    if(!row_valid_mod(streams, row)) {
        cout << "A solved row cannot be modified" << endl;
        return;
    }
    //do multiply on matrix
    multiply_stream(streams,row,mult);
    output_streams(streams,rows);
}

//receive input to add
void recieve_add_input(float** streams, int rows) {
    int row_mult = 0;
    float mult = 0;
    int row_mod = 0;
    //assign input to variables
    loop_input_bounds(row_mult, "Enter the row for multiplication: ", rows);
    loop_input_mult(mult, "Enter the multiplier: ");
    loop_input_bounds(row_mod, "Enter the row to be modified by adding multiple: ", rows);
    //check if row can be modified, a solved row cannot be modified
    if(!row_valid_mod(streams, row_mod)) {
        cout << "A solved row cannot be modified" << endl;
        return;
    }
    //do add and multiply on matrix
    add_scalar_multiple(streams,row_mult,mult,row_mod);
    output_streams(streams,rows);
}

//receives values a row
//value is valid if it is within bounds
//loops until input is valid
void loop_input_bounds(int &variable, string prompt, int rows) {
    bool valid = false;
    //loop and prompt until input is within bounds of 0 and #rows
    while(!valid) {
        cout << prompt;
        cin >> variable;
        cout << variable << endl;
        valid = within_bounds(variable,min_row,rows);
    }
}

//receives values for a multiplier
//any value is valid besides 0
//loop until input is valid
void loop_input_mult(float &variable, string prompt) {
    bool valid = false;
    //loop and prompt until input is valid (not 0)
    while(!valid) {
        cout << prompt;
        cin >> variable;
        cout << variable << endl;
        valid = !is_zero(variable);
    }
}

//checks if a row is valid for modificaton
bool row_valid_mod(float** streams, int row) {
    float** pos = streams; //anchor
    //loop until row
    for(int i = 0; i < row-1; i++) {
        streams++;
    }
    //check if row is reduced, it can be modified if it is not reduced
    int prev = -1;
    bool is_valid = !is_row_reduced((*streams),prev);
    streams = pos;
    return is_valid;
}

