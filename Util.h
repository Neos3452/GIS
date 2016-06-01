//
// Created by Przemek Piórkowski on 29.05.2016.
//

#ifndef STRONGLYCONNECTEDCOMPONENTSFINDER_UTIL_H
#define STRONGLYCONNECTEDCOMPONENTSFINDER_UTIL_H

#include <fstream>

namespace util {
    struct SingleResult {
        unsigned long duration;
        unsigned long stackDiff;
        unsigned long edges;

        SingleResult() : duration{0}, stackDiff{0}, edges{0} { }

        SingleResult(unsigned long duration, unsigned long stackDiff, unsigned long edges) : duration{duration},
                                                                                             stackDiff{stackDiff},
                                                                                             edges{edges} { }
    };

    SingleResult computeMeanValue(const std::vector<SingleResult>& resultVector) {
        unsigned long long int durationSum = 0;
        unsigned long long int stackDiffSum = 0;
        unsigned long long int edgesSum = 0;
        auto n = resultVector.size();
        for (const auto& sr : resultVector) {
            durationSum += sr.duration;
            stackDiffSum += sr.stackDiff;
            edgesSum += sr.edges;
        }

        return SingleResult(durationSum / n, stackDiffSum / n, edgesSum / n);
    }

    void writeResultsToCSV(const std::vector<SingleResult>& result, const std::string& filename) {
        std::ofstream resultFile;
        resultFile.open(filename);
        resultFile << "time,mem,edges" << std::endl;
        for (const auto& r : result) {
            resultFile << r.duration <<
                    "," << r.stackDiff <<
                    "," << r.edges << std::endl;
        }
        resultFile.close();
    }
}

#endif //STRONGLYCONNECTEDCOMPONENTSFINDER_UTIL_H
