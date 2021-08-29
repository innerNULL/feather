/// file: BiDict.cpp

#include <iostream>
#include <spdlog/spdlog.h>

#include "feather/BiDict.h"


namespace feather {


BiDict::BiDict(const std::vector<std::string>& schema, 
    const std::string& name) {
  this->name_ = name;
  this->schema_ = schema;

  int32_t col_index = 0;
  for (const std::string& key1 : schema) {
    this->col_schema_[key1] = col_index++;
    for (const std::string& key2 : schema) {
      if (key1 != key2) {
        std::string dict_name = key1 + "2" + key2;
        this->dicts_[dict_name] = {};
      }
    }
  }
}


int32_t BiDict::Register(
    const std::vector<std::string>& record) {
  int32_t field_num = this->schema_.size();
  if (field_num != record.size()) {
    throw "The element number of record should be same with \
        schema field number.";
  }
  for (int32_t index1 = 0; index1 < field_num; ++index1) {
    for (int32_t index2 = 0; index2 < field_num; ++index2) {
      if (index1 == index2) { continue; }
      std::string dict = this->Indexs2DictName(index1, index2);
      /// TODO@202108281125: Check key existence first.
      this->dicts_[dict][record[index1]].emplace_back(record[index2]);
    }
  }
  return 0;
}


std::string BiDict::Indexs2DictName(
    const int32_t index1, const int32_t index2) {
  if (this->schema_.size() == 0 || this->dicts_.size() == 0) {
    throw "No schema or dict has been registered/initialized.";
  }
  return this->schema_[index1] + "2" + this->schema_[index2];
}


std::vector<std::string> BiDict::Map(
    const std::string from, const std::string to, 
    const std::string& key) {
  std::vector<std::string> value;
  std::string using_dict = from + "2" + to;
  if (this->dicts_.find(using_dict) == this->dicts_.end()) {
    throw ("No dict can mapping from " + from + " to " + to);
  } else if (
      this->dicts_[using_dict].find(key) == this->dicts_[using_dict].end()
  ) {
    spdlog::error("In inner dict '" + using_dict + "', no key " + key);
  } else {
    value = this->dicts_[using_dict][key];
  }
  return value;
} 


/// TODO@202108281800
std::string BiDict::Persistence(const std::string& path) {
  std::string out_path;
  std::vector<std::string> record_vec;
  std::unordered_map<std::string, bool> records;
  if (path.size() == 0) { out_path = ("./" + this->name_ + ".txt"); }

  for (auto dict_iter = this->dicts_.begin(); 
      dict_iter != this->dicts_.end(); ++dict_iter) {
    for (auto record_iter = dict_iter->second.begin(); 
        record_iter !=  dict_iter->second.end(); ++record_iter) {
      record_vec.clear();
      std::string dict_name = dict_iter->first;
      std::string key = record_iter->first;
      std::vector<std::string> value = record_iter->second;
      printf("dbg: %s: key=%s, val=%s\n", 
          dict_name.c_str(), key.c_str(), value[0].c_str()); 
    }
  }
  return out_path;
}


} // namespace feather
