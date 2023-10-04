#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Stack {
private:
    string data[1000];
    int currentIndex = -1;

public:
    void push(const string& value) {
        if (currentIndex < 999) {
            currentIndex++;
            data[currentIndex] = value;
        }
    }

    string top() {
        if (currentIndex >= 0) {
            return data[currentIndex];
        } else {
            return "";
        }
    }

    void pop() {
        if (currentIndex >= 0) {
            currentIndex--;
        }
    }

    bool empty() {
        return currentIndex == -1;
    }
};

bool isValidChar(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '=');
}

bool isFirstChValid(char c) {
    return (c >= 'A' && c <= 'Z');
}

bool isSecondChValid(char c) {
    return ((c >= '0' && c <= '9') || (c == '\0') || (c == '\n'));
}

int main() {
    system("chcp 65001");
    Stack loopStack;
    string inputFileName;
    string outputFileName;

    cout << "Введите название входного файла: ";
    cin >> inputFileName;
    cout << "Введите название выходного файла: ";
    cin >> outputFileName;
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile) {
        cerr << "Проблема с входным файлом" << endl;
        return 1;
    }

    if (!outputFile) {
        cerr << "Проблема с выходным файлом" << endl;
        return 1;
    }

    string line;
    string currentWord;
    int indentationLevel = 0;
    int counter = 0;
    bool forFound = false;

    while (getline(inputFile, line)) {
        currentWord = "";
        counter += 1;
        for (int i = 0; i < line.length() + 1; i++) {
            char c = line[i];

            if (isValidChar(c)) {
                currentWord += c;
            } else {
                if (forFound) {
                    string ID;
                    for (int j = 3; j < currentWord.length(); j++) {
                        ID += currentWord[j];
                    }

                    if ((ID.length() <= 2) && isFirstChValid(ID[0]) && isSecondChValid(ID[1])) {
                        loopStack.push(ID);
                        cout << "FOR ID: " << ID << endl;
                    } else {
                        cerr << "ID задан некорректно " << counter << endl;
                        return 1;
                    }
                }

                if (currentWord == "FOR") {
                    forFound = true;
                }

                if (currentWord == "NEXT") {
                    string ID;

                    if ((i < line.length() - 1) && isFirstChValid(line[i + 1])) {
                        ID += line[i + 1];

                        if ((i < line.length() - 1) && isSecondChValid(line[i + 2])) {
                            ID += line[i + 2];
                        } else {
                            cerr << "NEXT задан некорректно" << endl;
                            return 1;
                        }
                    } else {
                        cerr << "NEXT задан некорректно" << endl;
                        return 1;
                    }

                    indentationLevel -= 2;
                    cout << "найдено " << ID << endl;

                    if (!loopStack.empty()) {
                        string forID = loopStack.top();
                        cout << "stack " << forID << endl;
                        loopStack.pop();

                        if (ID == forID) {
                            cout << "ID совпали" << endl;
                        } else {
                            cerr << "ID не совпали" << endl;
                            return 1;
                        }
                    } else {
                        cerr << "ID FOR и NEXT не совпадают " << counter << endl;
                        return 1;
                    }
                }
                currentWord = "";
            }
        }

        for (int i = 0; i < indentationLevel; i++) {
            outputFile << ' ';
        }

        if (forFound) {
            indentationLevel += 2;
        }

        outputFile << line << endl;
        forFound = false;
    }

    if (!loopStack.empty()) {
        cerr << "В стеке остались данные" << endl;
        return 1;
    }

    inputFile.close();
    outputFile.close();

    cout << "Результат выведен в output.txt." << endl;

    return 0;
}
