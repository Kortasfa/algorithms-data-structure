//В текстовом файле задана последовательность целых чисел.
//Отсортировать файл методом естественного слияния с 4 лентами.
//Удалить повторное вхождение чисел, не используя дополнительной
//памяти (9)
//
//Тимофеев Захар Clion

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void merge(std::vector<int> &from1, std::vector<int> &from2, std::vector<int> &to1, std::vector<int> &to2) {
    size_t i1 = 0, i2 = 0;
    bool switchTo = false;

    while (i1 < from1.size() || i2 < from2.size()) {
        std::vector<int> &to = switchTo ? to2 : to1;
        while (true) {
            if (i1 < from1.size() && (i2 >= from2.size() || from1[i1] <= from2[i2])) {
                if (to.empty() || to.back() != from1[i1]) {
                    to.push_back(from1[i1]);
                }
                i1++;
            } else if (i2 < from2.size()) {
                if (to.empty() || to.back() != from2[i2]) {
                    to.push_back(from2[i2]);
                }
                i2++;
            } else {
                break;
            }
            if ((i1 < from1.size() && from1[i1] < to.back()) || (i2 < from2.size() && from2[i2] < to.back())) {
                break;
            }
        }
        switchTo = !switchTo;
    }
}

void sortFiles(std::vector<int> &f1, std::vector<int> &f2, std::vector<int> &f3, std::vector<int> &f4) {
    while (true) {
        f3.clear();
        f4.clear();

        // Разделение и распределение
        merge(f1, f2, f3, f4);

        if (f4.empty()) {
            f1 = f3;
            break;
        }

        f1.clear();
        f2.clear();

        // Слияние
        merge(f3, f4, f1, f2);
    }
}

int main() {
    vector<int> f1, f2, f3, f4;
    string line;
    int temp;

    ifstream in("input.txt");
    ofstream out;
    if (in.is_open()) {
        while (in >> temp) {
            if (f1.size() == f2.size()) {
                f1.push_back(temp);
            } else {
                f2.push_back(temp);
            }
        }
    }
    in.close();

    sortFiles(f1, f2, f3, f4);
    for (int i: f1) {
        cout << i << " ";
    }
}
