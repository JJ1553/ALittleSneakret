#include "sketching.h"
#include "set"
#include "algorithm"
#include <cmath>
#include <vector>
#include <tuple>
#include <limits>
using namespace cs225;

std::vector<uint64_t> kminhash(std::vector<int> inList, unsigned k, hashFunction h) {
    std::set<uint64_t> uniqueHashVals;

    for (int item : inList) {
        uint64_t hashValue = h(item);
        uniqueHashVals.insert(hashValue);
    }

    std::vector<uint64_t> minHashVals(uniqueHashVals.begin(), uniqueHashVals.end());

    while (minHashVals.size() < k) {
        minHashVals.push_back(GLOBAL_MAX_INT);
    }
    while (minHashVals.size() > k) {
        
        minHashVals.erase(std::prev(minHashVals.end()));
    }

    return minHashVals;
}

std::vector<uint64_t> khash_minhash(std::vector<int> inList, std::vector<hashFunction> hv) {
    std::vector<uint64_t> minHashVals;

    for (const auto& hashFuncts : hv) {
        
        uint64_t minHash = GLOBAL_MAX_INT;

        for (int item : inList) {
            uint64_t hashValue = hashFuncts(item);

            minHash = std::min(minHash, hashValue);
        }
        minHashVals.push_back(minHash);
    }

    return minHashVals;
}

std::vector<uint64_t> kpartition_minhash(std::vector<int> inList, int part_bits, hashFunction h) {
    int numParts = (int)(std::pow(2, part_bits));

    std::vector<uint64_t> minHashVals(numParts, GLOBAL_MAX_INT);

    for (int item : inList) {
        uint64_t hashValue = h(item);

        int partitionIndex = (hashValue >> (64 - part_bits));

        minHashVals[partitionIndex] = std::min(minHashVals[partitionIndex], hashValue);
    }

    return minHashVals;
}

float minhash_jaccard(std::vector<uint64_t> mh1, std::vector<uint64_t> mh2) {
    std::set<uint64_t> set1(mh1.begin(), mh1.end());
    std::set<uint64_t> set2(mh2.begin(), mh2.end());
    set1.erase(GLOBAL_MAX_INT);
    set2.erase(GLOBAL_MAX_INT);
    
    size_t intersectionSize = 0;
    for (uint64_t element : set1) {
        if (set2.find(element) != set2.end() && element != GLOBAL_MAX_INT) {
            intersectionSize++;
        }
    }

    size_t unionSize = set1.size() + set2.size() - intersectionSize;

    float jacSim = static_cast<float>(intersectionSize) / unionSize;

    return jacSim;
}

int minhash_cardinality(std::vector<uint64_t> mh, unsigned k) {

    double kMinValue = mh[k - 1] / (double)(std::pow(2, 64)-1);


    int estCardinality = (int)(std::ceil((double)k / kMinValue)) -1;

 
    return estCardinality;
}

float exact_jaccard(std::vector<int> raw1, std::vector<int> raw2) {

    std::set<int> set1(raw1.begin(), raw1.end());
    std::set<int> set2(raw2.begin(), raw2.end());

    size_t intersectionSize = 0;
    for (int element : set1) {
        if (set2.find(element) != set2.end()) {
            intersectionSize++;
        }
    }

    size_t unionSize = set1.size() + set2.size() - intersectionSize;

    float jacSim = (float)(intersectionSize) / unionSize;

    return jacSim;
}

int exact_cardinality(std::vector<int> raw) {
    std::set<int> uniqueElements(raw.begin(), raw.end());

    return (int)(uniqueElements.size());
}

MM::MM(const cs225::PNG& input, unsigned numTiles, unsigned k, hashFunction h) {
    int tileWidth = std::ceil((double)(input.width()) / numTiles);
    int tileHeight = std::ceil((double)(input.height()) / numTiles);

    
    minHashMatrix_.resize(numTiles, std::vector<std::vector<uint64_t>>(numTiles));

    for (unsigned i = 0; i < numTiles; i++) {
        for (unsigned j = 0; j < numTiles; j++) {
            std::vector<int> pixelList;

            for (int x = i * tileWidth; x < std::min((int)((i + 1) * tileWidth), (int)(input.width())); x++) {
                for (int y = j * tileHeight; y < std::min((int)((j + 1) * tileHeight), (int)(input.height())); y++) {
                    int pixel = (int)(input.getPixel(x, y).l * 255.0f);
                    pixelList.push_back(pixel);
                }
            }

            std::vector<uint64_t> hashList = kminhash(pixelList, k, h);
            minHashMatrix_[i][j] = hashList;
        }
    }
}

std::vector<uint64_t> MM::getMinHash(unsigned width, unsigned height) const {
    return minHashMatrix_[width][height];
}

int MM::countMatchTiles(const MM & other, float threshold) const {
     int count = 0;

    
    for (unsigned i = 0; i < minHashMatrix_.size(); ++i) {
        for (unsigned j = 0; j < minHashMatrix_[i].size(); ++j) {
            const std::vector<uint64_t>& thisTile = minHashMatrix_[i][j];
            const std::vector<uint64_t>& otherTile = other.minHashMatrix_[i][j];

            
            float similarity = minhash_jaccard(thisTile, otherTile);

            
            if (similarity >= threshold) {
                count++;
            }
        }
    }

    return count;
}

std::vector<std::tuple<int, int, int>> build_minhash_graph(std::vector<std::string> flist, unsigned numTiles, unsigned k, hashFunction h, float threshold) {
    std::vector<std::tuple<int, int, int>> edges;
    std::map<std::string, int> files;

    std::vector<MM> mms;
    for (const std::string& filename : flist) {
        cs225::PNG png;
        png.readFromFile(filename);
        mms.emplace_back(png, numTiles, k, h);
        files[filename] = mms.size() - 1;
    }

    
    for (int i = 0; i < (int)flist.size(); ++i) {
        for (int j = i + 1; j < (int)flist.size(); ++j) {
            int count = mms[i].countMatchTiles(mms[j], threshold);
            edges.emplace_back(files[flist[i]], files[flist[j]], count);
        }
    }

    return edges;
}