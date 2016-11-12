#ifndef TOOLS_H
#define TOOLS_H
#include <string>
#include <vector>
#include <set>

class Tools
{
    public:
        Tools() = delete;
        static float floatArrayAVG(const float tab[], const std::size_t taille);
        static int getMaxIndex(const int tab[], const std::size_t taille);
        static int getVectorIndex(const std::vector<std::string> vec, const std::string& value);
        template<typename T>
        static std::vector<T> setToVector(std::set<T> setCollection)
        {
            return std::vector<T>(setCollection.begin(), setCollection.end());
        }

};

#endif // TOOLS_H
