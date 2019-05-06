#ifndef BASEOBJECT_H_INCLUDED
#define BASEOBJECT_H_INCLUDED
#include "CommonFunction.h"


class BaseObject{
public:
        BaseObject();
        ~BaseObject();
        void Show(SDL_Surface* des);
        virtual bool LoadImg(std::string path, SDL_Renderer* screen);
        void SetRect(const int& x,const int& y){
            rect_.x = x;
            rect_.y = y;
        }
        SDL_Rect GetRect()const{return rect_; };
        int getX() const {return rect_.x;};
        int getY() const {return rect_.y;};
        SDL_Texture* GetObject() {return p_object_;};
        void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
        void Free();
        void Animate(SDL_Renderer* des,int width_,int height_,int number_);


protected:
        SDL_Rect object_clip_[20];
        SDL_Rect rect_;
        SDL_Texture* p_object_;
        int num = 0;





};


#endif // BASEOBJECT_H_INCLUDED
