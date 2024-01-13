//В текстовом файле задана последовательность целых чисел.
//Отсортировать файл методом естественного слияния с 4 лентами.
//Удалить повторное вхождение чисел, не используя дополнительной
//памяти (9)
//
//Тимофеев Захар Clion

#include <fstream>
#include <iostream>
#include <limits>
#include <string>

void splitFile(const std::string& inputFileName, const std::string& file1, const std::string& file2) {
    std::ifstream inputFile(inputFileName);
    std::ofstream outFile1(file1), outFile2(file2);

    int number, lastNumber = std::numeric_limits<int>::min();
    bool writeToFirst = true;

    while (inputFile >> number) {
        if (number < lastNumber) {
            writeToFirst = !writeToFirst;
        }
        if (writeToFirst) {
            outFile1 << number << " ";
        } else {
            outFile2 << number << " ";
        }
        lastNumber = number;
    }

    inputFile.close();
    outFile1.close();
    outFile2.close();
}

void mergeFiles(const std::string& inFile1, const std::string& inFile2,
                const std::string& outFile1, const std::string& outFile2) {
    std::ifstream input1(inFile1), input2(inFile2);
    std::ofstream output1(outFile1), output2(outFile2);

    int num1, num2, lastWritten = std::numeric_limits<int>::min();
    bool hasNum1 = static_cast<bool>(input1 >> num1);
    bool hasNum2 = static_cast<bool>(input2 >> num2);
    bool writeToFirst = true;

    while (hasNum1 || hasNum2) {
        int current;
        if (!hasNum1) {
            current = num2;
            hasNum2 = static_cast<bool>(input2 >> num2);
        } else if (!hasNum2 || num1 < num2) {
            current = num1;
            hasNum1 = static_cast<bool>(input1 >> num1);
        } else {
            current = num2;
            hasNum2 = static_cast<bool>(input2 >> num2);
        }

        if (current != lastWritten) {
            if (writeToFirst) {
                output1 << current << ' ';
            } else {
                output2 << current << ' ';
            }
            lastWritten = current;
            writeToFirst = !writeToFirst;
        }
    }

    input1.close();
    input2.close();
    output1.close();
    output2.close();
}

bool isSortedFile(const std::string& fileName) {
    std::ifstream file(fileName);
    int prev, curr;
    if (!(file >> prev)) {
        return true;
    }

    while (file >> curr) {
        if (curr < prev) {
            return false;
        }
        prev = curr;
    }
    return true;
}

void naturalMergeSort(const std::string& inputFileName, const std::string& sortedFileName) {
    std::string file1 = "temp1.txt";
    std::string file2 = "temp2.txt";
    std::string file3 = "temp3.txt";
    std::string file4 = "temp4.txt";

    splitFile(inputFileName, file1, file2);

    while (!isSortedFile(file1) || !isSortedFile(file2)) {
        mergeFiles(file1, file2, file3, file4);
        std::swap(file1, file3);
        std::swap(file2, file4);
    }

    std::string sortedFile = isSortedFile(file1) ? file1 : file2;
    std::ifstream sortedInput(sortedFile);
    std::ofstream sortedOutput(sortedFileName);
    sortedOutput << sortedInput.rdbuf();

    sortedInput.close();
    sortedOutput.close();
}


int main() {
    std::string inputFileName = "input.txt";
    std::string sortedFileName = "sorted.txt";

    naturalMergeSort(inputFileName, sortedFileName);

    return 0;
}
