#include <iostream>
#include <cstdint>

class Cat{
public:
    Cat(){
        std::cout << "meow" << "\n";
    }
    ~Cat(){
        std::cout << "bye" << "\n";
    }
private:
    int m_age;
};

int main(){
    // C style heap Cat
    Cat* cat_ptr = (Cat *)malloc(sizeof(Cat));
    free(cat_ptr);

    // C style heap Cats(Cat array);
    Cat* catap = (Cat *)malloc(sizeof(Cat)*5);
    free(catap);

    // C style heap int
    int32_t* ip = (int32_t *) malloc(sizeof(int32_t));
    *ip = 100;
    free(ip);

    // C style heap array
    int32_t* iap = (int32_t *)malloc(sizeof(int32_t)*5);
    iap[0] = 100;
    free(iap);
}