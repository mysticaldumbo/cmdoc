#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>

using namespace std;

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

void printUsage() {
    cout << "Usage: cmdoc <filename.cmdoc/cptf>" << endl;
}

string getFileExtension(const string& filename) {
    size_t dotIndex = filename.find_last_of(".");
    if (dotIndex != string::npos) {
        return filename.substr(dotIndex + 1);
    }
    return "";
}

#ifdef _WIN32
void clearScreen() {
    system("cls");
}

void turnEchoOff() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
}

void restoreTerminal() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode | ENABLE_ECHO_INPUT);
}
#else
void clearScreen() {
    system("clear");
}

void turnEchoOff() {
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    struct termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restoreTerminal() {
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    struct termios newt = oldt;
    newt.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}
#endif

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printUsage();
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "error: unable to open file " << filename << endl;
        return 1;
    }

    string fileExtension = getFileExtension(filename);
    transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

    if (fileExtension != "cmdoc" && fileExtension != "cptf") {
        cout << "error: unsupported file format. only .cmdoc and .cptf files are supported." << endl;
        return 1;
    }

    clearScreen();
    turnEchoOff();

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();

    #ifdef _WIN32
    system("pause > nul");
    #else
    cin.get();
    #endif

    restoreTerminal();
    return 0;
}