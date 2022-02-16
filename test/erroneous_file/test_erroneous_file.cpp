
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"
#include "TestConfig.h"

#include "structure/LanguageString.h"

#include "IemlParser.h"
#include "ParserJsonSerializer.h"


using namespace ieml::parser;
namespace fs = std::filesystem;


TEST(ieml_erroneous_file_test_case, erroneous_file) {
    for (const auto& file : fs::directory_iterator(TEST_IEML_ERRONEOUS_FILES_FOLDER)) {
        std::ifstream exampleFile(file.path());
        std::string exampleString, line;
        while (std::getline(exampleFile, line)) {
            exampleString = exampleString + "\n" + line;
        }
        exampleFile.close();

        IEMLParser parser(exampleString);
        parser.parse();
    }
}