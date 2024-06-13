#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include "zstr.hpp"

using namespace std;
int main(int argc, char *argv[]) {
  // Flush after every cout / std::cerr
  cout << unitbuf;
  cerr << unitbuf;

  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
//  cout << "Logs from your program will appear here!\n";

  // Uncomment this block to pass the first stage
  //
  if (argc < 2) {
    cerr << "No command provided.\n";
    return EXIT_FAILURE;
  }

  string command = argv[1];

  if (command == "init") {
    try {
      filesystem::create_directory(".git");
      filesystem::create_directory(".git/objects");
      filesystem::create_directory(".git/refs");

      ofstream headFile(".git/HEAD");
      if (headFile.is_open()) {
        headFile << "ref: refs/heads/main\n";
        headFile.close();
      } else {
        cerr << "Failed to create .git/HEAD file.\n";
        return EXIT_FAILURE;
      }

      cout << "Initialized git directory\n";
    } catch (const filesystem::filesystem_error &e) {
      cerr << e.what() << '\n';
      return EXIT_FAILURE;
    }
  }
  if (command == "cat-file") {
    if (argc < 4) {
      cerr << "Expect hash.\n";
      return EXIT_FAILURE;
    }

    if ((string)argv[2] != "-p") {
      cerr << "Expect hash.\n";
      return EXIT_FAILURE;
    }

    string hash = argv[3];

    string directory = hash.substr(0, 2);

    string filename = hash.substr(2);

    zstr::ifstream objectFile(".git/objects/" + directory + "/" + filename, std::ofstream::binary);

    if (!objectFile.is_open()) {
      cerr << "Error opening the file!" << endl;
      return EXIT_FAILURE;
    }

    string content{std::istreambuf_iterator<char>(objectFile), std::istreambuf_iterator<char>()};

    objectFile.close();

    cout << content << endl;

    string fileContent = content.substr(content.find("\0") + 1);

    cout << fileContent << std::flush;
  } else {
    cerr << "Unknown command " << command << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
