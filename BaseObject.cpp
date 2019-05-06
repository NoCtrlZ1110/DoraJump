#include "CommonFunction.h"
#include "BaseObject.h"

BaseObject::BaseObject(){
    rect_.x=0;
    rect_.y=0;
    rect_.w=0;
    rect_.h=0;
    p_object_ = NULL;

}
BaseObject::~BaseObject()
{
    Free();

}
void BaseObject::Animate(SDL_Renderer* des,int width_,int height_,int number_)
{
    for (int i=0; i<number_; i++)
    {
        object_clip_[i].x = i*width_;
        object_clip_[i].y = 0;
        object_clip_[i].w = width_;
        object_clip_[i].h = height_;
    }
    if (num==number_) num = 0;
    SDL_Rect* current_clip = &object_clip_[num];
    num++;
    SDL_Rect renderQuad = {rect_.x, rect_.y, width_, height_};
    SDL_RenderCopy(des, p_object_,current_clip, &renderQuad);
    SDL_Delay(80);
}

bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    Free();
    SDL_Texture* new_texture = NULL;
    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if (load_surface!=NULL)
    {
        SDL_SetColorKey(load_surface,SDL_TRUE, SDL_MapRGB(load_surface->format,COLOR_KEY_R,COLOR_KEY_G,COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen,load_surface);
        if (new_texture != NULL)
        {
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
        }
    }
    SDL_FreeSurface(load_surface);
    p_object_ = new_texture;
    delete new_texture;
    delete load_surface;
    return p_object_ !=NULL;
}

void BaseObject::Render(SDL_Renderer* des,const SDL_Rect* clip ){
    SDL_Rect renderquad = {rect_.x,rect_.y,rect_.w,rect_.h};
    SDL_RenderCopy(des, p_object_, clip, &renderquad);


}
void BaseObject::Free(){
    if (p_object_!= NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }



};
