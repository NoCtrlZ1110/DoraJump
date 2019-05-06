#include "MainObject.h"
#include "Platform.h"
#include <iostream>
#define BONUS 7
static bool temp=false;
static bool press=false;
static float bonus=0;
MainObject::MainObject()
{
    // Các giá trị ban đầu khi khởi tạo đối tượng mới
    frame_=0;
    // Tọa Độ X
    x_pos_=200;
    y_pos_=200;
    // Vận tốc
    x_val_=0;
    y_val_=0;
    // Chiều rộng của nhân vật
    width_frame_ =0;
    // Chiều cao của nhân vật
    height_frame_ =0;
    // Trạng thái
    status_ = -1;
    input_type_.left_=0;
    input_type_.right_=0;
    input_type_.jump_=0;
    input_type_.down_=0;
    input_type_.up_=0;
    on_ground = false;
    high_jump = false;
    highhigh_jump = false;




}
MainObject::~MainObject()
{

}

// Lấy dữ liệu hình ảnh từ file vào chương trình
bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path,screen);
    if (ret == true)
    {
        width_frame_ = rect_.w/FrameNum;
        height_frame_ = rect_.h;

    }
    return ret;
}
// Tạo hiệu ứng Animation
void MainObject::set_clips()
{
    if (width_frame_>0 && height_frame_>0)
    {
        for (int i=0; i<FrameNum; i++)
        {
            frame_clip_[i].x = i*width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}
// Xuất hình ảnh nhân vật lên màn hình
void MainObject::Show(SDL_Renderer* des)
{
    // Hướng về bên trái thì load ảnh nhân vật bên trái
    if (temp)
    {
    }
    else
        {
            if (status_ == WALK_LEFT)
        {
            LoadImg("img//jump_left.png",des);
        }
        else
        // Load ảnh bên phải
        {
            LoadImg("img//jump_right.png",des);
        }

        }
    if (input_type_.left_==1 || input_type_.right_ == 1)
    {
        frame_++;
    }
    else
    {
        frame_=9;
    }
    if (frame_>=FrameNum) frame_ =0;
    rect_.x = x_pos_;
    rect_.y = y_pos_;

    SDL_Rect* current_clip = &frame_clip_[frame_];
    SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};
    SDL_RenderCopy(des, p_object_,current_clip, &renderQuad);

}
// Hàm điều khiển trạng thái nhân vật
void MainObject::HandleInputAction(SDL_Event events,  SDL_Renderer* screen)
{

    // Khi nhấn nút xuống
    if (events.type == SDL_KEYDOWN)
    {

        switch (events.key.keysym.sym)
        {

        case SDLK_RIGHT:
            {
                press = true;
                status_ = WALK_RIGHT;
                input_type_.right_ = 1;
                input_type_.left_ = 0;
                x_val_+=5;
            }
            break;
        case SDLK_LEFT:
            {
                press = true;
                status_ = WALK_LEFT;
                input_type_.left_ = 1;
                input_type_.right_ = 0;
                x_val_-=5;
        }
            break;
        case SDLK_DOWN:
            {
                high_jump = true;
            }

        default:
            break;
        }
    }
    // Khi thả nút ra
    else if (events.type == SDL_KEYUP)
    {
        press= false;
        switch (events.key.keysym.sym)
        {
        case SDLK_RIGHT:
            {
                input_type_.right_ = 0;

            }
            break;
        case SDLK_LEFT:
            {
                input_type_.left_ = 0;

            }
            break;
        case SDLK_DOWN:
            {
                high_jump = false;

            }
            break;

        default:
            break;
        }


    }
}
// Hàm Di Chuyển Nhân Vật
void MainObject::DoPlayer(bool check, Mix_Chunk* jumpsound, Mix_Chunk* highhighjumpsound)
{
    temp = check;
    if //(on_ground||
        (check)
    {
        if (highhigh_jump)
        {
            y_val_-=50;
            Mix_PlayChannel(-1,highhighjumpsound,0);
            highhigh_jump = false;
        }
        else
        if (!high_jump)
        {
            y_val_ = -JUMP_SPEED;
            Mix_PlayChannel(-1,jumpsound,0);
        }
        else
        {
            y_val_ = -JUMP_SPEED - 5;
            Mix_PlayChannel(-1,jumpsound,0);
        }
    }

    else {

        y_val_ += 0.8;

    };
    if (!press)
    {
        if (x_val_>=0)
            {
                x_val_-=0.2;
                if (x_val_<0) x_val_ = 0;
            }
            else
                {
                    x_val_ +=0.2;
                    if (x_val_ >0) x_val_ = 0;

                }
    }
    if (x_val_>7) x_val_=7;
    if (x_val_<-7) x_val_=-7;

//    // Giới hạn vận tốc của y
    if (y_val_ >= MAX_FALL_SPEED)
    {
        y_val_ = MAX_FALL_SPEED;
    }
    if (input_type_.left_ == 1)
    {
        x_val_ -= (PLAYER_SPEED);//+bonus);

    }
    else if (input_type_.right_ == 1)
    {
        x_val_ +=(PLAYER_SPEED);//+bonus);

    };

    x_pos_ += (x_val_);
    y_pos_ += y_val_;
    // Chỉ cho nhân vật nhảy đến một độ cao nào đó.
    if (y_pos_<MAX_HEIGHT) y_pos_ = MAX_HEIGHT;

    // Cho nhân vật đi xuyên màn hình
    if (x_pos_ + width_frame_ <0)
    {
        x_pos_ = SCREEN_WIDTH ;//- width_frame_;
    }
    else if (x_pos_ >= SCREEN_WIDTH)
        {
            x_pos_ = - width_frame_;
        };
    if (y_pos_+ height_frame_>=SCREEN_HEIGHT+height_platform_)
        {
            if (y_pos_ >= SCREEN_HEIGHT + height_frame_+20)
            y_pos_ = SCREEN_HEIGHT + height_frame_+20;
            on_ground = true;
        }
        //else on_ground = false;

}




