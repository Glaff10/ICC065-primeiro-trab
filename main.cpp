#include <algorithm>
#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <string.h>

using namespace std;

// Comando "cat"
void read_file(string path, string filename, int lines_to_read = -1, bool descending = false) {
    vector<string> lines;

    string filepath = path + filename;
    ifstream source(filepath);
    string line;

    while(getline(source, line)) {
        lines.push_back(line);
    }

    source.close();

    if(descending) {
        reverse(lines.begin(), lines.end());
    }

    int i = 0;
    for(vector<string>::iterator it = lines.begin(); it != lines.end(); ++it, ++i) {
        if(i == lines_to_read) break;
        cout << *it << endl;
    }
}

// Comando "more"

// Comando "head"
void head(string path, string filename, int lines_to_read) {
    read_file(path, filename, lines_to_read);
}

// Comando "tail"
void tail(string path, string filename, int lines_to_read) {
    read_file(path, filename, lines_to_read, true);
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
        result += buffer.data();
    }
    cout << result;
}

int main() {
    cout << "Using cat..." << endl;
    read_file("/home/gasmartin/dev/pita/", "teste.txt");
    cout << endl;

    cout << "Using head..." << endl;
    head("/home/gasmartin/dev/pita/", "teste.txt", 10);
    cout << endl;

    cout << "Using tail..." << endl;
    tail("/home/gasmartin/dev/pita/", "teste.txt", 5);
    cout << endl;

    cout << "Using run_program..." << endl;
    run_program("./program.sh");
    cout << endl;

    return 0;
}
