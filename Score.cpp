#include "Score.h"
#include <string.h>
#include <sstream>
#include <iostream>
Score::Score()
{
        for (int i=0; i<10; i++)
    {
        score_clip_[i].x = i*score_width;
        score_clip_[i].y = 0;
        score_clip_[i].w = score_width;
        score_clip_[i].h = score_height;
    }

}
Score::~Score()
{

}
//
//// Xuất hình ảnh nhân vật lên màn hình
void Score::Show(SDL_Renderer* des,int score_)
{
    LoadImg("img//Score.png",des);
    if (score_<10)
    {
    SDL_Rect* current_clip = &score_clip_[score_];
    SDL_Rect renderQuad = {rect_.x, rect_.y, score_width, score_height};
    SDL_RenderCopy(des, p_object_,current_clip, &renderQuad);
    }
    else
    {
        std::stringstream ss;
        ss << score_;
        std::string str = ss.str();
        int n = str.length();
        for (int i=n-1;i>=0;i--)
        {
            char temp = str[i];
            SDL_Rect* current_clip = &score_clip_[temp-48];
            SDL_Rect renderQuad = {rect_.x-score_width*(n-i-1), rect_.y, score_width, score_height};
            SDL_RenderCopy(des, p_object_,current_clip, &renderQuad);

        }
    }

}
