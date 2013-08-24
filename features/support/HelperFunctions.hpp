#ifndef _FEATURES_HELPER_FUNCTIONS
#define _FEATURES_HELPER_FUNCTIONS

#include <cucumber-cpp/internal/Table.hpp>
#include <fstream>

template <typename T>
void writeToFile(const char *filename, T content) {
  using namespace::std;
  ofstream file;
  file.open(filename, ios::out | ios::trunc);
  file << content;
  file.close();
}

void logCycleEvent(const char *name, const char *log, const char *separator) {
  using namespace::std;
  ofstream file;
  file.open(log, ios::out | ios::app);
  file << separator << name;
  file.close();
}

void logTableAsHashes(const cucumber::internal::Table & table, const char *log) {
    using namespace::std;
    typedef cucumber::internal::Table::hashes_type hashes_type;
    typedef cucumber::internal::Table::hash_row_type hash_row_type;

    ofstream file;
    file.open(log, ios::out | ios::trunc);

    file << "[";
    const hashes_type & hashes = table.hashes();
    for (hashes_type::const_iterator tit = hashes.begin(); tit != hashes.end(); ++tit) {
        if (tit != hashes.begin()) {
            file << ",";
        }
        file << "{";
        for (hash_row_type::const_iterator rit = tit->begin(); rit != tit->end(); ++rit) {
            if (rit != tit->begin()) {
                file << ",";
            }
            file << "\"" << rit->first << "\":\"" << rit->second << "\"";
        }
    file << "}";
    }
    file << "]";
    file.close();
}

#endif /* _FEATURES_HELPER_FUNCTIONS */

