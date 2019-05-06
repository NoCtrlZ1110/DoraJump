#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED
#include "CommonFunction.h"
#include "BaseObject.h"
const int width_platform_ = 77 ;
const int height_platform_ = 24 ;
const int Plat_Num = 15;
const int REAL_PLAT = 1;
const int FAKE_PLAT = 0;
const int MAX_FAKE_PLAT = 7;
const int DROP_SPEED = 10 ;
class Platform: public BaseObject
{
public:
    Platform();
    ~Platform();
    //void setPlat();
    void renderPlat(Platform* plat_objects);

    int getPlat_type() { return plat_type;}
    void setPlat_type(int plat_type_);
    int drop_val_;
    bool Drop ;
    int object;




private:
    int plat_type;
};

#endif // PLATFORM_H_INCLUDED
