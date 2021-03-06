#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>

constexpr auto messageLength = 1023;
constexpr auto bitfieldLength = 10;

std::vector<int> readFile(const std::string &fileName) {
    std::ifstream file;
    std::string number;
    std::vector<int> result;
    file.open(fileName);

    if (!file.is_open()) {
        return std::vector<int>{};
    }

    while (file >> number) {
        result.push_back(std::stoi(number));
        number.clear();
    }

    file.close();
    return result;
}

std::vector<bool> episodeOne() {
    auto bitfield = std::vector<bool>{};
    auto episode = std::vector<bool>{};
    for (auto i = 0; i < bitfieldLength; i++) {
        bitfield.push_back(true);
    }
    for (auto i = 0; i < messageLength; i++) {
        episode.push_back(bitfield.back());
        bool rotateBit = bitfield.at(2) ^bitfield.at(9);
        std::shift_right(bitfield.begin(), bitfield.end(), 1);
        bitfield.front() = rotateBit;
    }
    return episode;
}

std::vector<bool> episodeTwo(int a, int b) {
    auto bitfield = std::vector<bool>{};
    auto episode = std::vector<bool>{};
    for (auto i = 0; i < bitfieldLength; i++) {
        bitfield.push_back(true);
    }
    for (auto i = 0; i < messageLength; i++) {
        episode.push_back(bitfield.at(a) ^bitfield.at(b));
        auto rotateBit =
                bitfield.at(1) ^bitfield.at(2) ^bitfield.at(5) ^bitfield.at(7) ^bitfield.at(8) ^bitfield.at(9);
        std::shift_right(bitfield.begin(), bitfield.end(), 1);
        bitfield.front() = rotateBit;
    }
    return episode;
}

std::vector<std::vector<bool>> chipCodes() {
    std::vector<std::vector<int>> combinations = {{1,5},{2,6},{3,7},{4,8},{0,8},{1,9},{0,7},{1,8},{2,9},{1,2},{2,3},{4,5},{5,6},{6,7},{7,8},{8,9},{0,3},{1,4},{2,5},{3,6},{4,7},{5,8},{0,2},{3,5}};
    std::vector<std::vector<bool>> chipSequences{};
    auto firstEpisode = episodeOne();

    for(auto combination : combinations) {
        auto secondEpisode = episodeTwo(combination.front(), combination.back());
        std::transform(secondEpisode.begin(), secondEpisode.end(), firstEpisode.begin(), secondEpisode.begin(),
                       [](auto first, auto second) {
                           return first ^ second;
                       });
        chipSequences.push_back(secondEpisode);
    }
    return chipSequences;
}

void print(int id, int delta, int crossProduct) {
    if(200 < crossProduct && crossProduct < 800) {
        printf("Satellite %2d has sent bit 1 (delta = %3d)\n",id, delta);
    } else if(-200 > crossProduct && crossProduct > -800) {
        printf("Satellite %2d has sent bit 0 (delta = %3d)\n",id, delta);
    }
}


int main(int argc, char **argv) {

    clock_t start, stop;
    start = clock();

    if (argc < 2) {
        perror("File parameter is missing!\n");
        return -1;
    }
    const auto data = readFile(argv[1]);
    if (data.empty()) {
        perror("File is empty or does not exists!\n");
        return -1;
    }
    const auto chipSequences = chipCodes();
    auto id = 1;
    for (auto chipSequence : chipSequences) {
        for(auto delta = 0; delta < messageLength; delta++) {
            auto crossProduct = 0;
            for(auto number = 0; number < messageLength; number++) {
                crossProduct += chipSequence.at(number) * data.at(number);
            }
            print(id,delta,crossProduct);
            std::rotate(chipSequence.rbegin(),chipSequence.rbegin()+1, chipSequence.rend());
        }
        id++;
    }

    stop = clock();
    printf("\n%.0f Milliseconds\n", ((double)(stop - start) / CLOCKS_PER_SEC) * 1000);

    return 0;
}
