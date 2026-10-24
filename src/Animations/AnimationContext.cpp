#include "../../include/Animations/AnimationContext.h"

namespace Animations
{
    AnimationContext::AnimationContext(const std::string& data)
    {
        uint16_t index = 0;
        uint16_t count = 0;
        while (index < data.length())
        {
            if (data[index] == '}')
            {
                count++;
            }
            index++;
        }
        
        if (count > 0)
        {
            this->sequences.reserve(count);
            this->braceIndexes.reserve(count * 2);
        }

        this->sequences.reserve(count);
        this->Parse(data);
    }

    void AnimationContext::Parse(const std::string& data)
    {
        uint16_t index = 0;
        uint16_t left = 0;
        uint16_t right = 1;
        uint8_t length = static_cast<uint8_t>(data.length()) - 1;

        // while (right < length)
        // {
        //     if (data[right] == "{" || data[right] == "}")
        //     {
        //         this->braceIndexes.push_back(right);
        //     }
        //     right++;
        // }

        // if (this->braceIndexes.size() % 2 != 0)
        // {
        //     // handle unballanced braces
        //     return;
        // }

        // left = 1;
        // right = static_cast<uint16_t>(this->braceIndexes.size()) - 2;
        // while (left < right)
        // {
        //     std::string window = data.substr(left, (right - left) + 1);
        //     this->sequences.push_back(window);
        //     left++;
        //     right--;
        // }

        // if (this->sequences.size() >  0)
        // {
        //     this->pDefinitions.reserve(this->sequences.size());
        // }

        // for (uint16_t index = 0; index < this->sequences.size(); ++index)
        // {
        //     this->pDefinitions.push_back(Animations::Definitions::Percise(this->sequences[index]));
        // }
    }

    void SetType(const std::string& data)
    {
        // Assume type is the first number in the outer most { 45 [{}, {}, {}]}
        uint16_t left = 0;
        uint16_t right = 1;
        uint16_t length = static_cast<uint16_t>(data.length()) - 1;
        uint8_t asciiZero = 48;
        uint8_t asciiNine = 57;
        uint8_t curAsciiVal = 0;
        bool isFirstBrace = false;
        std::string num = "";

        // while (right < length)
        // {
        //     if (data[right] == '{')
        //     {
        //         if (!isFirstBrace)
        //         {
        //             isFirstBrace = true;
        //             right++;
        //             continue;
        //         }

        //         break;
        //     }

        //     if (data[right] == " " && num.length() > 0)
        //     {
        //         break;
        //     }

        //     curAsciiVal = static_cast<uint8_t>(data[right]);
        //     if (curAsciiVal >= asciiZero  && curAsciiVal <= asciiNine)
        //     {
        //         num += data[right];
        //     }

        //     right++;
        // }

        // if (num.length() > 0)
        // {
        //     this->type = static_cast<uint8_t>(num);
        // }
    }

    std::vector<Definitions::Percise> AnimationContext::GetPerciseDefinitions(void)
    {
        return this->pDefinitions;
    }

    AnimationContext::~AnimationContext()
    {}
}