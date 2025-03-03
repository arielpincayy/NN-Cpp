#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <memory>

using Data = std::vector<std::vector<int>>;

std::unique_ptr<Data> readFile(const std::string& filename) {
    std::unique_ptr<Data> data = std::make_unique<Data>();

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream sstream(line);
        std::string cell;
        std::vector<int> row;

        while (std::getline(sstream, cell, ',')) {
            row.push_back(std::stoi(cell));
        }

        data->push_back(row); 
    }

    file.close(); 
    return data;
}

int main() {
    try {
        std::unique_ptr<Data> data = readFile("files/dataset_encoded.csv");

        for (const auto& row : *data) {
            for (const auto& cell : row) {
                std::cout << cell << ' ';
            }
            std::cout << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "Se produjo un error: " << e.what() << std::endl;
    }

    return 0;
}
