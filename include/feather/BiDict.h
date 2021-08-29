/// file: BiDict.h
/// Bidirectional dictionary class.


#ifndef FEATHER_BIDICT_H_
#define FEATHER_BIDICT_H_


#include <string>
#include <vector>
#include <unordered_map>


namespace feather {


class BiDict {
 public:
  BiDict(const std::vector<std::string>& schema, 
      const std::string& name="default");
  
  int32_t Register(const std::vector<std::string>& record);

  std::string Persistence(const std::string& path="");

  std::vector<std::string> Map(
      const std::string from, const std::string to, 
      const std::string& key);

 protected:
  std::string Indexs2DictName(
      const int32_t index1, const int32_t index2);

 private:
  std::string name_;
  std::vector<std::string> schema_;
  std::unordered_map<std::string, int32_t> col_schema_;
  std::unordered_map< 
      std::string, 
      std::unordered_map< std::string, std::vector<std::string> > 
  > dicts_;
};


} // namespace feather


#endif
