#include <stdlib.h>
#include <functional>
#include "ValueInterface.h"

class Encoder
{
public:
    void setList(std::vector<Value> *_list)
    {
        list = _list;
        //Value frequencyValue(30.0, 200.0, 40.0, "TOM", 100, true);
        //values.push_back(frequencyValue);
    }

    float getValue()
    {
        return (*list)[selectedValueIndex].value;
    }

    const char *getParamName()
    {
        return (*list)[selectedValueIndex].nameTag;
    }

    bool setReading(bool A, bool B, bool C)
    {
        bool changed = false;
        if (C != lastClickState)
        {
            if (C)
            {
                changed = true;
                onClick();
            }
            lastClickState = C;
        }
        state = A;
        if (state != lastState)
        {
            if (B != state)
            {
                value++;
                if ((value & 1) && list != nullptr)
                    (*list)[selectedValueIndex].increment();
            }
            else
            {
                value--;
                if (value & 1 && list != nullptr)
                    (*list)[selectedValueIndex].decrement();
            }
            changed = true;
        }
        lastState = state;
        return changed;
    }

    std::vector<Value> *list;
    byte selectedValueIndex = 0;
    void onClick()
    {
        if (list != nullptr)
        {
            selectedValueIndex = (selectedValueIndex + 1) % list->size();
            // Serial.println((*list)[selectedValueIndex].value);
        }
    }
    int value = 0;
    bool lastClickState = false;
    bool lastState = false;
    bool state = false;
};

// class Encoder
// {
// public:
//     int getValue()
//     {
//         return value >> 1;
//     }

//     bool setReading(bool A, bool B, bool C)
//     {
//         bool changed = false;
//         if (C != lastClickState)
//         {
//             if (C)
//             {
//                 if (onClick != nullptr)
//                 {
//                     onClick();
//                 }
//             }
//             lastClickState = C;
//         }
//         state = A;
//         if (state != lastState)
//         {
//             if (B != state)
//             {
//                 value++;
//                 if (parameter != nullptr && (value & 1))
//                     parameter->increment();
//             }
//             else
//             {
//                 value--;
//                 if (parameter != nullptr && (value & 1))
//                     parameter->decrement();
//             }
//             changed = true;
//         }
//         lastState = state;
//         return changed;
//     }

//     void setParameterPointer(Value *p)
//     {
//         parameter = p;
//     }

// private:
//     int value = 0;
//     bool lastState = false;
//     bool state = false;
//     bool lastClickState = false;
//     Value *parameter = nullptr;
// };