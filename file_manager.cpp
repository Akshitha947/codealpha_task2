#include <iostream>
#include <string>
#include <cstdio>
#include <direct.h> // For _mkdir on Windows
#include <windows.h>
#include <limits> // For std::numeric_limits

// Function to list files in a directory
void listFiles(const std::string& path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        std::cout << "Error opening directory: " << path << ". Error code: " << GetLastError() << std::endl;
        return;
    }

    do {
        std::string fileName = findFileData.cFileName;
        if (fileName != "." && fileName != "..") {
            std::cout << fileName << std::endl;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// Function to create a directory
void createDirectory(const std::string& path) {
    if (_mkdir(path.c_str()) == 0) {
        std::cout << "Directory created: " << path << std::endl;
    } else {
        DWORD error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS) {
            std::cout << "Directory already exists: " << path << std::endl;
        } else {
            std::cout << "Failed to create directory: " << path << ". Error code: " << error << std::endl;
        }
    }
}

// Function to copy a file
void copyFile(const std::string& src, const std::string& dest) {
    if (GetFileAttributes(src.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::cout << "Source file does not exist: " << src << std::endl;
        return;
    }
    
    if (CopyFile(src.c_str(), dest.c_str(), FALSE)) {
        std::cout << "File copied from " << src << " to " << dest << std::endl;
    } else {
        DWORD error = GetLastError();
        std::cout << "Failed to copy file. Error code: " << error << std::endl;
    }
}

int main() {
    std::string command, path;

    while (true) {
        std::cout << "Enter command (list, mkdir, copy, exit): ";
        std::cin >> command;

        // Clear the newline character left in the input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (command == "list") {
            std::cout << "Enter directory path: ";
            std::getline(std::cin, path);
            listFiles(path);
        } else if (command == "mkdir") {
            std::cout << "Enter new directory path: ";
            std::getline(std::cin, path);
            createDirectory(path);
        } else if (command == "copy") {
            std::string src, dest;
            std::cout << "Enter source file path: ";
            std::getline(std::cin, src);
            std::cout << "Enter destination file path (including file name): ";
            std::getline(std::cin, dest);
            copyFile(src, dest);
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    return 0;
}
