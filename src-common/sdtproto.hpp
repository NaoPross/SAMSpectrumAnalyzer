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
            header = 0;
            length = 0;
            samples = nullptr;
        }

        ~frame()
        {

            if (_alloc)
                delete samples;
        }

        bool alloc()
        {
            if (_alloc)
                return false;

            samples = new complex_uint16_t[length];
            _alloc = true;

            return true;
        }

    private:
        bool _alloc = false;
    };
};

#endif
