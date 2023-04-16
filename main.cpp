#include <iostream>
#include <fstream>
#include <direct.h>
#include <filesystem>
#include <string>
#include <vector>
#include <Windows.h>

using std::string;

void createFilesInDirectory(const char *path, long numberOfFiles) {
    using std::to_string, std::ofstream;

    _mkdir(path);
    _chdir(path);

    long i{0u};
    while(i < numberOfFiles) {
        i++;
        string filename = "file" + to_string(i) + ".txt";
        ofstream file(filename);
    }
}

int isIncludingNumber(std::string fileName) {
    using std::isdigit, std::stoi;

    string numberString{""};
    for(char c : fileName) if(isdigit(c)) numberString += c;

    if(!numberString.empty()) return stoi(numberString);
    else return -1;

}

void printFileNames(const char *path) {
    using std::cout, std::endl;

    for (const auto file : std::filesystem::directory_iterator(path)) {
        string filenameStr = file.path().filename().string();
        int number = isIncludingNumber(filenameStr);

        if(number % 2 == 0) {
            try {
                std::filesystem::remove(file.path());
            }
            catch (std::filesystem::filesystem_error &e) {
                std::cerr << e.what() << endl;
                return;
            }
        }
    }
}

std::vector<std::string> getDrives() {
    using std::istringstream, std::getline, std::vector;

    vector<string> drives;
    char buffer[MAX_PATH];
    DWORD length = GetLogicalDriveStringsA(MAX_PATH, buffer);

    if (length == 0) return drives;

    string drivesStr(buffer, length);
    istringstream ss(drivesStr);
    string drive;

    while (getline(ss, drive, '\0')) {
        if (drive.size() > 0) {
            drives.push_back(drive);
        }
    }

    return drives;
}

std::string createInstallationPath() {
    using std::cout, std::endl, std::vector;

    vector<std::string> drives = getDrives();
    string firstDrive = "";
    bool gDriveFound = false;

     for (auto drive : drives) {
        int driveType = GetDriveType(drive.c_str());
        if (driveType != DRIVE_NO_ROOT_DIR) {
            if (drive == "G:\\") {
                gDriveFound = true;
                firstDrive = drive;
                break;
            } else if (firstDrive == "") {
                firstDrive = drive;
            }
        }
    }
    if(gDriveFound) return "G:\\";
    else return firstDrive;
}

int main() {
    using std::cout, std::endl;

    string drive = createInstallationPath();
    string pathToString = drive + "Programs/C++/process";
    const char *path = pathToString.c_str();

    if(!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }

    createFilesInDirectory(path, 20000);
    printFileNames(path);

    return 0;
}