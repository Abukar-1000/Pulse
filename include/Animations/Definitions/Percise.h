#ifndef Percise_h
#define Percise_h
#include <string>
#include "pico/stdlib.h"

namespace Animations::Definitions
{
    class Percise
    {
        private:
            void Parse(const std::string&);
        public:
            uint8_t  seq;
            float     frequency;
            uint16_t  state;
            uint16_t  start;
            uint16_t  end;
            uint8_t   id;
            ~Percise();
            Percise();
            Percise(const std::string&);
    };
}

#endif