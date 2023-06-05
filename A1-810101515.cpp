#include<cstdlib>
#include<iostream>
#include<vector>
#include<cstring>
#include<fstream>


using namespace std;


typedef struct Day Day;

struct Day{
    int day;
    int month;
    int year;
    vector<string> note;
    int num_of_notes;
    int num_of_chars;
    int positive_words;
};


void command_identifier(string, string, Day &, vector<Day> &, int &);
void start_day(string, Day &);
void show_day(string, vector<Day> );
void show_the_longest_day(vector<Day> );
void show_the_best_day(vector<Day>);
void print_partial_of_note(int, vector<Day>);


int main() {
    string line; 
    string command;
    Day temp_struct;
    vector<Day> every_single_day; //the vector that we keep all the information ofall the days in it
    int counter = 0, flag_command = 0; //flag_command :    1 -> show_day -/longest/best    0-> we wrote a note   -1 ->we started a day
    while(getline(cin, line)) { //getting every line it types
        char line_temp[line.length() + 1]; 
        strcpy(line_temp, line.c_str());
        command = strtok(line_temp, " "); //getting the first word to see what command or note it is
        command_identifier(command, line, temp_struct, every_single_day, flag_command);
        counter++;
        fflush(stdin);
    }
}

void command_identifier(string command, string line, Day &temp_struct, vector<Day> &every_single_day, int &flag_command) { //identifing the command
    if(command == "start_day") { // saving last days structure and preparing today's struct
        if(flag_command == 0) {
            every_single_day.push_back(temp_struct);
        }
        flag_command = -1;
        temp_struct.num_of_notes = 0;
        temp_struct.num_of_chars = 0;
        temp_struct.positive_words = 0;
        start_day(line, temp_struct);
    }
    else if(command == "show_day") { //the command is show_day and we are calling its function to print the day it asked
        if(flag_command == 0) {
            every_single_day.push_back(temp_struct);
        }
        flag_command = 1;
        show_day(line, every_single_day);
    }
    else if(command == "show_the_longest_day") {
        if(flag_command == 0) {
            every_single_day.push_back(temp_struct);
        }
        flag_command = 1;
        show_the_longest_day(every_single_day);
    }
    else if(command == "show_the_best_day") {
        if(flag_command == 0) {
            every_single_day.push_back(temp_struct);
        }
        flag_command = 1;
        show_the_best_day(every_single_day);
    }
    else { //it comes here when we want to write a note to our day
        if(flag_command == 1) { //removing the last incomplete struct
            every_single_day.pop_back();
        }
        flag_command = 0;
        (temp_struct.note).push_back(line);
        temp_struct.num_of_notes++; //increasing the number of notes writen by one
        temp_struct.num_of_chars += line.length(); 

        /*counting the positive words in every line that has been gave as a note*/
        string word_file; //the word we get from reading the positive-words file
        string filename = "positive-words.txt"; //the file name we have all the positive words
        char line_temp[line.length() + 1]; 
        char *word_line;
        word_line = (char*)malloc(sizeof(char) * line.length() + 1);
        strcpy(line_temp, line.c_str());
        word_line = strtok(line_temp, " ");
        while(word_line != NULL) { //the loop ends when we pass the last word in the line
            fstream file;
            file.open(filename.c_str());
            word_file.clear();
            while(file >> word_file) { //the loop ends when we pass the last word in the file
                if(word_file.compare(word_line) == 0) {
                    temp_struct.positive_words++;
                    break;
                }
            }
            file.close();
            word_line = strtok(NULL, " "); //getting the next word in the line
        }
    }
}

void start_day(string line, Day &temp_struct) {        
    temp_struct = {}; //empting the last struct

    /*gatting the date from line intto another seprate string*/
    string date;
    char temp_date[line.length() + 1];
    strcpy(temp_date, line.c_str());
    date = strtok(temp_date, " ");
    date = strtok(NULL, " ");

    /*puting the date as integer in that days structure*/
    char date_char[date.length()];
    strcpy(date_char, date.c_str());
    temp_struct.day = stoi(strtok(date_char, "/"));
    temp_struct.month = stoi(strtok(NULL, "/"));
    temp_struct.year = stoi(strtok(NULL, "/"));
}

void show_day(string line, vector<Day> every_single_day) {
    string date;
    int temp_day, temp_month, temp_year;
    char temp_date[line.length() + 1];

    /*seprating the date given into 3 integers that we will compare them to all the dates that we have stored in our diary until now*/
    strcpy(temp_date, line.c_str());
    date = strtok(temp_date, " ");
    date = strtok(NULL, " ");
    char date_char[date.length()];
    strcpy(date_char, date.c_str());
    temp_day = stoi(strtok(date_char, "/"));
    temp_month = stoi(strtok(NULL, "/"));
    temp_year = stoi(strtok(NULL, "/"));

    for(int i = 0; i < every_single_day.size(); i++) { // comparing the dates to find its equivelant
        if(every_single_day[i].year == temp_year && every_single_day[i].month == temp_month && every_single_day[i].day == temp_day) {
            for(int j = 0; j < (every_single_day[i].note).size(); j++) {
                cout << (every_single_day[i].note)[j] << endl;
            }
            break;
        }
    }
}

void show_the_longest_day(vector<Day> every_single_day) {
    int temp_high = 0, temp_low = 0, maxs_index = 1;
    for(int i = 1; i < every_single_day.size(); i++) { //trying to find the longest day's index in our vector(most chars)
        temp_low = every_single_day[i].num_of_chars;
        if(temp_low > temp_high) {
            temp_high = temp_low;
            maxs_index = i;
        }
    }
    print_partial_of_note(maxs_index, every_single_day);
}

void show_the_best_day(vector<Day> every_single_day) {
    int temp_high = 0, temp_low = 0, maxs_index = 1;
    for(int i = 1; i < every_single_day.size(); i++) { //trying to find the best day's index in our vector(most positive-woords)
        temp_low = every_single_day[i].positive_words;
        if(temp_low > temp_high) {
            temp_high = temp_low;
            maxs_index = i;
        }
    }
    print_partial_of_note(maxs_index, every_single_day);
}

void print_partial_of_note(int index, vector<Day> every_single_day) {
    cout << every_single_day[index].day << "/" << every_single_day[index].month << "/" << every_single_day[index].year << endl;
    if(every_single_day[index].num_of_chars > 20) { //printing a summury if we have more than 20 chars in our diary
        int counter_index = 0, counter_char = 0;
        for(int i = 0; i < 20; i++) {
            if(counter_char == (every_single_day[index].note[counter_index]).size()) { 
                cout << endl;
                counter_char = 0;
                counter_index++;
            }
            else {
                char ans = (every_single_day[index].note[counter_index])[counter_char];
                cout << ans; //printing until reaching the newline
                counter_char++;
            }
        } 
        cout << "..." << endl;
    }
    else { //printing the hole diary cuase its less than than 20 chars 
        for(int i = 0; i < (every_single_day[index].note).size(); i++) {
            cout << (every_single_day[index].note)[i] << endl;
        }
    }
}