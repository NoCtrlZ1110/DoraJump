#ifndef THREATOBJECTS_H_INCLUDED
#define THREATOBJECTS_H_INCLUDED
#include "CommonFunction.h"
#include "BaseObject.h"
const int width_monster_ = 44 ;
const int height_monster_ = 50 ;

class ThreatObjects : public BaseObject
{
public:
    ThreatObjects();
    ~ThreatObjects();
    void LoadMonster(int type);
    int type;
private:

};



#endif // THREATOBJECTS_H_INCLUDED
