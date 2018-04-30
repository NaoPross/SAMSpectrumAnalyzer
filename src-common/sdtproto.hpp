#ifndef SDTPROTO_HPP
#define SDTPROTO_HPP


namespace sdt
{

    #include "sdtproto.h"

    struct frame : public sdt_frame
    {
    public:
        frame()
        {
            static_assert(sizeof(frame) == sizeof(sdt_frame));

            header = 0;
            length = 0;
            samples = nullptr;
        }

        ~frame()
        {
            dealloc();
        }

        inline bool realloc()
        {
            dealloc();
            return alloc();
        }

        bool alloc()
        {
            if (samples != nullptr)
                return false;

            samples = new complex_uint16_t[length];
            return true;
        }

        bool dealloc()
        {
            if (samples == nullptr)
                return false;

            delete samples;
            samples = nullptr;
            return true;
        }
    };
};

#endif
