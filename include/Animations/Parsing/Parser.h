#include Parser_h
#define Parser_h
#include <string>
#include <vector>
#include "pico/stdlib.h"

namespace Animations::Parsing
{
    class Parser
    {
        private:
            std::vector<std::string> tokens;
            bool Parse(const std::string& data);
            void Tokenize(const std::string& data);
        public:
            ~Parser();
            Parser();
            Parser(const std::string& data);
            bool Parse(void);
    };
}

#endif