#include "terminal.hpp"
#include "filesys.hpp"

void Terminal::run(std::istream& istream, std::ostream& ostream) {
    if (!initialized_)throw std::logic_error("Terminal not initialized");
    std::string line; line.reserve(128);

    ostream << "Terminal is runned\n";
    ostream << " << ";
    while (std::getline(istream, line)) {
        if (line.empty()) {ostream << "<< "; continue;}
        if (line == "exit") {ostream << "End of session" << std::endl;
            return;
        }

        try {
            if (std::string result = commandManager_.executeCommand(*fileSystem_, line); !result.empty())ostream << result << std::endl;
        } catch (std::exception& e) {
            ostream << e.what() << "\n";
        }
        ostream << "<< ";
    }
}

void Terminal::initialize() {
    if (initialized_) throw std::logic_error("Terminal is initialized");

    fileSystem_ = std::make_shared<FileSystem>();

    initialized_ = true;
}

std::string Terminal::send(const std::string& line) {

    std::cout << "SENDED: " << line << std::endl;

    if (!initialized_)
        throw std::logic_error("Terminal not initialized");

    if (line.empty())
        return {};


    if (line == "exit")
        throw std::logic_error("exit"); // при таком соо тупо завершить работу программы

    try {
        std::string result = commandManager_.executeCommand(*fileSystem_, line);
        if (!result.empty())
            return result;
    } catch (std::exception& e) {
        return std::string{e.what()};
    } return "";
}

std::string& Terminal::readFileToString(const std::string& filename) const {
    auto curd = fileSystem_->getCurrentDirectory();
    if (!curd)
        throw std::runtime_error("Cant get current directory.");

    auto file = curd->findByName(filename);
    if (!file)
        throw std::invalid_argument("No such file");

    auto reg = std::dynamic_pointer_cast<RegularFile>(file);
    if (!reg)
        throw std::invalid_argument("File is not regular");

    return reg->getData();
}
