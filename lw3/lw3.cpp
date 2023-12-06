/*
 Общими требованиями к лабораторной работе являются:
   1) вводить исходное дерево из файла в понятной для
пользователя форме, а не с клавиатуры;
   2) по требованию пользователя показывать дерево на экране;
   3) обрабатывать дерево в оперативной памяти, а не путем
многократного обращения к файлу;
   4) обеспечить возможность многократных запросов без
повторного запуска программы.

26. Информация о файлах на жестких дисках компьютера
записана с помощью дерева.  Обеспечить выполнение следующих
операций:
1) загрузку дерева в память из файла;
2) обход дерева папок в режиме диалога (раскрытие папок, подъем на уровень и т. п.);
3) корректировку дерева при создании новых папок и файлов,
их переименовании, копировании, переносе и удалении.
4) сохранение дерева в файле (14).

Тимофеев Захар ПС-21 Clion 2023
*/

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <conio.h>
#include <stack>

using namespace std;

class TreeNode {
private:
    string name;
    vector<TreeNode *> children;
    TreeNode *parent;

public:
    TreeNode(string name, bool isFile = false)
            : name(std::move(name)), isFile(isFile), parent(nullptr) {}

    void addChild(TreeNode *childNode) {
        if (!isFile) {
            children.push_back(childNode);
            childNode->setParent(this);
        }
    }

    TreeNode *getParent() const {
        return parent;
    }

    void setParent(TreeNode *parentNode) {
        parent = parentNode;
    }

    string getName() const {
        return name;
    }

    bool getIsFile() const {
        return isFile;
    }

    vector<TreeNode *> &getChildren() {
        return children;
    }

    void setName(const string &newName) {
        name = newName;
    }

    string toString(int level = 0) const {
        string ret = string(level, '\t') + name + (isFile ? " (file)\n" : "\n");
        if (!isFile) {
            for (const auto &child: children) {
                ret += child->toString(level + 1);
            }
        }
        return ret;
    }

    bool isFile;
};

class FileSystem {
private:
    TreeNode *root;
    TreeNode *currentNode;
    TreeNode *copyBuffer;

    TreeNode *findNode(TreeNode *node, const string &name) {
        if (node->getName() == name) {
            return node;
        }
        if (!node->getIsFile()) {
            for (const auto &child: node->getChildren()) {
                TreeNode *result = findNode(child, name);
                if (result) {
                    return result;
                }
            }
        }
        return nullptr;
    }

public:
    FileSystem(TreeNode *rootNode) : root(rootNode), currentNode(rootNode), copyBuffer(nullptr) {}

    void goToParent() {
        if (currentNode->getParent()) {
            currentNode = currentNode->getParent();
        }
    }

    string getCurrentNodeName() const {
        return currentNode->getName();
    }

    TreeNode *getRootNode() const {
        return root;
    }

    vector<string> listContents() const {
        vector<string> contents;
        for (const auto &child: currentNode->getChildren()) {
            contents.push_back(child->getName() + (child->getIsFile() ? " (file)" : ""));
        }
        return contents;
    }

    bool changeDirectory(const string &folderName) {
        for (const auto &child: currentNode->getChildren()) {
            if (child->getName() == folderName && !child->getIsFile()) {
                currentNode = child;
                return true;
            }
        }
        return false;
    }

    void create(const string &name, bool isFile = false) {
        TreeNode *newNode = new TreeNode(name, isFile);
        currentNode->addChild(newNode);
    }

    bool rename(const string &oldName, const string &newName) {
        for (const auto &child: currentNode->getChildren()) {
            if (child->getName() == oldName) {
                child->setName(newName);
                return true;
            }
        }
        return false;
    }

    bool remove(const string &name) {
        auto &children = currentNode->getChildren();
        for (size_t i = 0; i < children.size(); ++i) {
            if (children[i]->getName() == name) {
                delete children[i];
                children.erase(children.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool move(const string &name, const string &destination) {
        TreeNode *nodeToMove = nullptr;
        auto &children = currentNode->getChildren();
        for (size_t i = 0; i < children.size(); ++i) {
            if (children[i]->getName() == name) {
                nodeToMove = children[i];
                children.erase(children.begin() + i);
                break;
            }
        }

        if (!nodeToMove) {
            return false;
        }

        TreeNode *destinationNode = findNode(root, destination);
        if (destinationNode && !destinationNode->getIsFile()) {
            destinationNode->addChild(nodeToMove);
            return true;
        }

        return false;
    }
    void copy(const string &name) {
        copyBuffer = findNode(currentNode, name);
        if (copyBuffer) {
            cout << "Copied " << name << endl;
        } else {
            cout << "Item not found!" << endl;
            system("pause");
            system("cls");
        }
    }

    void paste() {
        if (copyBuffer) {
            TreeNode *newNode = new TreeNode(*copyBuffer); // Deep copy the node
            currentNode->addChild(newNode);
            cout << "Pasted " << copyBuffer->getName() << endl;
        } else {
            cout << "Nothing to paste!" << endl;
            system("pause");
            system("cls");
        }
    }

};

void saveTree(TreeNode* node, const string& filename, ofstream& out, int level = 0) {
    if (!node) return;

    for (int i = 0; i < level; i++) {
        out << '\t';
    }

    out << node->getName() << (node->getIsFile() ? " (file)" : "") << endl;

    for (auto child : node->getChildren()) {
        saveTree(child, filename, out, level + 1);
    }
}

void saveTreeToFile(TreeNode* node, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cout << "Failed to open file for saving!" << endl;
        return;
    }

    saveTree(node, filename, out);
    out.close();
}

TreeNode* loadTree(const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cout << "Failed to open file for loading!" << endl;
        return nullptr;
    }

    string line;
    vector<TreeNode*> stack;
    while (getline(in, line)) {
        int level = 0;
        while (level < line.size() && line[level] == '\t') {
            level++;
        }

        line = line.substr(level);
        bool isFile = false;
        if (line.find(" (file)") != string::npos) {
            isFile = true;
            line = line.substr(0, line.size() - 7);
        }

        TreeNode* node = new TreeNode(line, isFile);

        if (level == stack.size()) {
            if (!stack.empty()) {
                stack.back()->addChild(node);
            }
        } else {
            while (level < stack.size()) {
                stack.pop_back();
            }
            stack.back()->addChild(node);
        }

        stack.push_back(node);
    }

    in.close();

    return stack.empty() ? nullptr : stack.front();
}


void handleChoice(int choice, FileSystem &fs, const vector<string> &contents, int choosed) {
    string name, oldName, newName, destination, filename, currentName;
    bool isFile;
    size_t found;

    if (choice <= contents.size()) {
        name = contents[choice - 1];

        if (name == "...") {
            fs.goToParent();
            system("cls");
            return;
        } else if (fs.changeDirectory(name)) {
            system("cls");
            return;
        }
        cout << "Вы выбрали: " << name << endl;
        system("pause");
        return;
    } else {
        choice -= contents.size();
    }

    switch (choice) {
        default:
            cout << "Неверный выбор!" << endl;

        case 1:
            cout << "Введите имя: ";
            cin >> name;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Это файл? (y/n):: ";
            cin >> oldName;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            isFile = oldName == "y";
            fs.create(name, isFile);
            system("cls");
            break;

        case 2:
            oldName = contents[choosed-1];
            cout << "Вы хотите переименовать: " << oldName << endl;
            cout << "Введите новое имя: ";
            cin >> newName;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (!fs.rename(oldName, newName)) {
                cout << "Переименование не удалось!" << endl;
                system("pause");
            }
            system("cls");
            break;

        case 3: {
            name = contents[choosed-1];
            found = name.find("(file)");
            if (found != std::string::npos) {
                name.erase(found - 1, 7); // 6 is the length of "{file}"
            }
            cout << "Вы хотите переместить: " << name << endl;
            cout << "Введите имя папки назначения: ";
            cin >> destination;
            if (!fs.move(name, destination)) {
                cout << "Перемещение не удалось!" << endl;
                system("pause");
            }
            system("cls");
            break;
        }

        case 4:
            name = contents[choosed - 1];
            cout << "Вы хотите удалить: " << name << endl;
            char confirmation;
            cout << "Вы уверены? (y/n): ";
            cin >> confirmation;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (confirmation == 'y' || confirmation == 'Y') {
                if (!fs.remove(name)) {
                    cout << "Удаление не удалось!" << endl;
                    system("pause");
                }
            } else {
                cout << "Удаление отменено." << endl;
                system("pause");
            }
            system("cls");
            break;

        case 5:
            cout << "Сохранен в файл: " << name << endl;
            name = contents[choosed-1];
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            saveTreeToFile(fs.getRootNode(), name);

            break;

        case 6: // Copy
            name = contents[choosed-1];
            found = name.find("(file)");
            if (found != std::string::npos) {
                name.erase(found - 1, 7); // 6 is the length of "{file}"
            }
            fs.copy(name);
            cout << "Файл скопирован " << name << endl;
            system("pause");
            break;

        case 7: // Paste
            fs.paste();
            cout << "Файл вставлен" << endl;
            system("pause");
            break;

        case 8:
            cout << "Введите имя папки назначения: ";
            cin >> name;
            //name = contents[choosed-1];
            TreeNode *loadedTree = loadTree(name);
            if (loadedTree) {
                fs = FileSystem(loadedTree);
                cout << "Файловая система успешно загружена!" << endl;
                system("pause");
            } else {
                cout << "Не удалось загрузить файловую систему!" << endl;
                system("pause");
            }
            system("cls");
            break;
    }
}

void run_filesystem_ui() {
    FileSystem fs(new TreeNode("Root"));
    system("cls");

    const vector<string> fixedChoices = {
            "1 Создать новую папку/файл",
            "2 Переименовать папку/файл",
            "3 Переместить папку/файл",
            "4 Удалить папку/файл",
            "5 Сохранить в файл",
            "6 Копировать папку/файл",
            "7 Вставить скопированную папку/файл",
            "8 Загрузить из файла"
    };

    int choice = 1;

    while (true) {
        system("cls");

        cout << "Текущая директория: " << fs.getCurrentNodeName() << endl;
        cout << "В этой директории содержится:" << endl;

        vector<string> contents = fs.listContents();
        contents.insert(contents.begin(), "...");
        int totalChoices = contents.size();

        for (int i = 1; i <= contents.size(); ++i) {
            if (i == choice) cout << "> ";
            else cout << "  ";
            cout << contents[i - 1] << endl;
        }

        cout << "\nВарианты действий:" << endl;

        for (int i = 1; i <= fixedChoices.size(); ++i) {
            cout << (fixedChoices[i - 1]) << endl;
        }

        int keyPressed = getch();
        switch (keyPressed) {
            case 72:
                choice = (choice <= 1) ? totalChoices : choice - 1;
                break;
            case 80:
                choice = (choice >= totalChoices) ? 1 : choice + 1;
                break;
            case 13:
                handleChoice(choice, fs, contents, choice);
                break;

        }
        if (keyPressed >= '1' && keyPressed <= '8') {
            handleChoice(keyPressed - '0' + contents.size(), fs, contents, choice);
        }
    }
}

int main() {
    system("chcp 65001");
    run_filesystem_ui();
    return 0;
}

