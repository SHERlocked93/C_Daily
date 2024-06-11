#include <fstream>
#include <iostream>
#include <sstream>

unsigned char arr[12]{1, 1, 2, 2, 3, 3, 4, 4, 1, 1, 1, 1};
inline void CopyFromBuff(unsigned char* Dest, unsigned char* Sour, int Len,
                         int& Index)
{
    memcpy(Dest, Sour + Index, Len);
    Index += Len;
}

struct bodyBase {
    static const int len = 8;
};

template <typename BodyType>
struct derivedBase {
    BodyType body;

    virtual void getInfo()
    {
        int i = 0;

        CopyFromBuff(reinterpret_cast<unsigned char*>(&body), arr, BodyType::len, i);
    }
};

struct aBody : bodyBase {
    int i = 0;
    int j = 0;
    static const int len = 8;
};

struct bBody : bodyBase {
    int m = 0;
    int n = 0;
    int k = 0;
    static const int len = 12;
};

struct a : derivedBase<aBody> {};

struct b : derivedBase<bBody> {
};

int main()
{
    a aa{};
    aa.getInfo();

    b bb{};
    bb.getInfo();
    return 0;
}
