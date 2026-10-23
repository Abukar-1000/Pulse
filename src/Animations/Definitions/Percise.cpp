#include "../../../include/Animations/Definitions/Percise.h"

namespace Animations::Definitions
{
    Percise::Percise()
    :   seq(0),
        frequency(0.0f),
        state(0),
        start(0),
        end(0),
        id(0)
    {}

    Percise::Percise(const std::string& data)
    {
        this->Parse(data);
    }

    void Percise::Parse(const std::string& data)
    {
        // Implement parsing logic here
        uint8_t count = 0;
        uint16_t left = 0;
        uint16_t right = 1;
        uint8_t length = static_cast<uint8_t>(data.length()) - 1;

        while (right < length && count < 6)
        {
            while (right < length && data[right] != ',')
            {
                right++;
            }

            std::string token = data.substr(left, right - left);
            if (count == 0)
            {
                this->seq = static_cast<uint8_t>(std::stoi(token));
            }
            else if (count == 1)
            {
                this->frequency = std::stof(token);
            }
            else if (count == 2)
            {
                this->state = static_cast<uint16_t>(std::stoi(token));
            }
            else if (count == 3)
            {
                this->start = static_cast<uint16_t>(std::stoi(token));
            }
            else if (count == 4)
            {
                this->end = static_cast<uint16_t>(std::stoi(token));
            }
            else if (count == 5)
            {
                this->id = static_cast<uint8_t>(std::stoi(token));
            }

            count++;
            left = right + 1;
            right = left;
        }
    }

    Percise::~Percise()
    {}
}