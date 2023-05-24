#include <iostream>
#include <fstream>

namespace sjtu
{
template<typename T>
class FILE_SYSTEM
{
    std::fstream f;
    bool is;
public:
    FILE_SYSTEM(const char *s)
    {
        is=false;
        std::ifstream f1(s);
        if(!f1.good()) {std::ofstream f2(s);is=true;}
        f.open(s,std::ios::in|std::ios::out|std::ios::binary);
    }
    bool empty() {return is;}
    void set_file(const char *s)
    {
        is=false;
        std::ifstream f1(s);
        if(!f1.good()) {std::ofstream f2(s);is=true;}
        f.open(s,std::ios::in|std::ios::out|std::ios::binary);
    }
    T read(int pos)
    {
        T tmp;
        f.seekg(pos*sizeof(tmp));
        f.read(reinterpret_cast<char*>(&tmp),sizeof(tmp));
        return tmp;
    }
    void write(int pos,T tmp)
    {
        f.seekp(pos*sizeof(tmp));
        f.write(reinterpret_cast<char*>(&tmp),sizeof(tmp));
    }
    void flush() {f.flush();}
};
}