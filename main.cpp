#include <algorithm>
#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int get_terminal_lines() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    return w.ws_row;
}

vector<string> read_file(string path, string filename) {
    string result;
    vector<string> lines;

    string filepath = path + filename;
    ifstream source(filepath);
    string line;

    while(getline(source, line)) {
        lines.push_back(line);
    }

    source.close();

    return lines;
}

int show_lines(string path, string filename, int start = 0, int end = -1, bool descending = false) {
    vector<string> lines = read_file(path, filename);

    if (start >= lines.size()) {
        return 0;
    }

    if(descending) {
        reverse(lines.begin(), lines.end());
    }

    int i = start;
    if(end >= lines.size()) {
        end = lines.size();
    }
    
    for(vector<string>::iterator it = lines.begin() + start; it != lines.end(); ++it, ++i) {
        if(i == end) break;
        cout << *it;
        if(i != end - 1) {
            cout << endl;
        }
    }

    return 1;
}

void cat(string path, string filename) {
    show_lines(path, filename);
}

// Comando "more"
void more(string path, string filename, int lines_per_page = -1) {
    if(lines_per_page == -1) {
        lines_per_page = get_terminal_lines();
    }

    int start = 0;
    int end = lines_per_page;

    int current_page = 1;

    while(show_lines(path, filename, start, end)) {
        cin.ignore();

        start = lines_per_page * current_page;
        end = start + lines_per_page;
        current_page++;
    }
}

// Comando "head"
void head(string path, string filename, int lines_to_read) {
    show_lines(path, filename, 0, lines_to_read, false);
}

// Comando "tail"
void tail(string path, string filename, int lines_to_read) {
    show_lines(path, filename, 0, lines_to_read, true);
}

// Chamando um programa e esperando sua execução
void run_program(string program_path) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(program_path.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        cout << buffer.data();
    }
}

int main() {
    string path = "/home/gasmartin/dev/pita/";
    string filename = "teste.txt";

    cout << "Using cat..." << endl;
    cat(path, filename);
    cout << endl;

    cout << "Using head..." << endl;
    head(path, filename, 10);
    cout << endl;

    cout << "Using tail..." << endl;
    tail(path, filename, 5);
    cout << endl;

    cout << "Using more..." << endl;
    more(path, filename);
    cout << endl;

    cout << "Using run_program..." << endl;
    run_program("./program.sh");
    cout << endl;

    return 0;
}
