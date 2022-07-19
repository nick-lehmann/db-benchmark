#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "Filters/All.h"

std::string exec(const char *cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL) result += buffer;
    }
    return result;
}

std::string getColumnName(unsigned index) { return "c" + std::to_string(index); }

vector<std::string> getColumnNames(unsigned numberOfAttributes) {
    vector<std::string> names;
    for (unsigned i = 0; i < numberOfAttributes; i++) {
        names.push_back(getColumnName(i));
    }
    return names;
}

std::string createTableStatement(std::string tableName, vector<std::string> columnNames, std::string columnType) {
    std::ostringstream builder;
    builder << "CREATE TABLE " << tableName << " (";

    for (unsigned i = 0; i < columnNames.size(); i++) {
        builder << columnNames[i] << " " << columnType;
        if (i != columnNames.size() - 1) {
            builder << ", ";
        }
    }
    builder << ")";

    return builder.str();
}

std::string createTable(std::string dbPath, std::string createStatement) {
    std::ostringstream cmd;
    cmd << "sqlite3 " << dbPath << " '" << createStatement << "'";
    return exec(cmd.str().c_str());
}

std::string runQuery(std::string dbPath, std::string query) {
    std::ostringstream cmd;
    cmd << "sqlite3 " << dbPath << " '" << query << "'";
    return exec(cmd.str().c_str());
}

vector<string> splitIntoVector(const string &s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

template <typename t, unsigned size>
std::array<t, size> splitIntoArray(const string &s, char delim) {
    std::array<t, size> result;
    stringstream ss(s);
    string item;

    for (unsigned i = 0; i < size; i++) {
        getline(ss, item, delim);
        result[i] = std::stoi(item);
    }

    return result;
}

template <typename T, unsigned width>
using SqliteQueryResult = std::vector<std::array<T, width>>;

template <typename T, unsigned width>
std::vector<std::array<T, width>> parseQueryResult(std::string &output) {
    std::vector<std::string> lines = splitIntoVector(output, '\n');

    std::vector<std::array<T, width>> result;
    result.reserve(lines.size());

    for (const auto &line : lines | std::views::drop(1)) {
        auto parts = splitIntoArray<T, width>(line, '|');
        result.push_back(parts);
    }

    return result;
}

template <typename T, SIMD Variant>
std::string filterToSqlOperator(Filters::Filter<T, Variant> *filter) {
    if (dynamic_cast<Filters::LessThan<T, Variant> *>(filter) != nullptr) {
        return "<";
    } else if (dynamic_cast<Filters::LessEqual<T, Variant> *>(filter) != nullptr) {
        return "<=";
    } else if (dynamic_cast<Filters::Equal<T, Variant> *>(filter) != nullptr) {
        return "=";
    } else if (dynamic_cast<Filters::GreaterEqual<T, Variant> *>(filter) != nullptr) {
        return ">=";
    } else if (dynamic_cast<Filters::GreaterThan<T, Variant> *>(filter) != nullptr) {
        return ">";
    }

    return "";
}

std::string buildSelectClause(std::vector<uint64_t> &projection) {
    std::ostringstream clause;
    for (unsigned columnIndex = 0; columnIndex < projection.size(); columnIndex++) {
        clause << getColumnName(projection[columnIndex]);
        if (columnIndex < projection.size() - 1) {
            clause << ", ";
        }
    }
    return clause.str();
}

template <typename T, SIMD Variant>
std::string buildWhereClause(std::vector<Filters::Filter<uint64_t, Variant> *> &filters) {
    std::ostringstream clause;
    for (unsigned filterIndex = 0; filterIndex < filters.size(); filterIndex++) {
        clause << getColumnName(filters[filterIndex]->index) << filterToSqlOperator<T, Variant>(filters[filterIndex])
               << filters[filterIndex]->value;
        if (filterIndex < filters.size() - 1) {
            clause << " and ";
        }
    }
    return clause.str();
}

/// Build the SQL query to get the correct result for the test case.
/// - Selects only the columns given in `projection`.
/// - Filters the columns given in `filters`.
/// Many test cases operate on a different number of rows. Instead of recreating the table for the different sizes, we use a subquery that
/// limit the number of rows to which the projection and filters are applied.
///
/// # Structure of SQL query
/// ```sql
/// SELECT projection
/// FROM (
///   SELECT * FROM data LIMIT
/// )
/// WHERE filters;
/// ```
template <typename T, SIMD Variant>
std::string buildSQLQuery(std::vector<uint64_t> &projection, std::vector<Filters::Filter<T, SIMD::None> *> &filters, std::string tableName,
                          unsigned partition) {
    std::ostringstream stmt;
    stmt << "select " << buildSelectClause(projection) << " from ( select * from " << tableName << " limit " << partition << ") where "
         << buildWhereClause<T, Variant>(filters);
    return stmt.str();
}

// PSV = Pipe separated values
// We have to use the default import format of Sqlite3. Unfortunately, it is not possible to customize the format and import a file when
// supplying the commands to the sqlite3 cli directly from the shell.
void importPSV(std::string dbPath, std::string tableName, std::string filePath) {
    std::ostringstream cmd;
    cmd << "sqlite3 " << dbPath << " '"
        << ".import " << filePath << " " << tableName << "'";
    exec(cmd.str().c_str());
}
