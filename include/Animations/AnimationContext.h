#ifndef AnimationContext_h
#define AnimationContext_h
#include <vector>
#include "pico/stdlib.h"
#include "./Definitions/Percise.h"

namespace Animations
{
    class AnimationContext
    {
        private:
            uint8_t type;
            std::vector<uint16_t> braceIndexes;
            std::vector<std::string> sequences;
            std::vector<Definitions::Percise> pDefinitions;
            void Parse(const std::string& data);
            void SetType(const std::string& data);
        public:
            ~AnimationContext();
            AnimationContext(const std::string& data);
            std::vector<Definitions::Percise> GetPerciseDefinitions(void);
    };
}

#endif