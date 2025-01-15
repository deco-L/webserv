#ifndef MEME_HPP
#define MEME_HPP

#include <map>
#include <string>

class MIME {
private:
  static const std::map<std::string, std::string> _types;
  static std::map<std::string, std::string> _initTypes(void);

public:
  static std::string getMIMEType(const std::string& extension);
};

#endif
