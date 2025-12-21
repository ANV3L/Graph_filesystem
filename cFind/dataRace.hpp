#pragma once

#include <string>
#include <vector>

size_t replace_stage_st(std::string& s, const std::string& from, const std::string& to);

size_t replace_stage_mt(std::string& s, const std::string& from, const std::string& to,
                              size_t threads_quantity = 4);

size_t replace(std::string& line, const std::vector<std::string>& v, int th = 4);

size_t replaceOneThread(std::string& line, const std::vector<std::string>& v);