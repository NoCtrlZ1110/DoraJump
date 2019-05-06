//#include "stdafx.h"
#include "CommonFunction.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "Platform.h"
#include "iostream"
#include "SDL_mixer.h"
#include "PowerUp.h"
#include <ctime>
#include "ThreatObjects.h"
#include "Score.h"
#include <windows.h>


#undef main

// Một số từ ngữ dùng trong code
// PLAT: Tấm Ván
// REAL PLAT: Ván Thật (Màu Xanh)
// FAKE PLAT: Ván Giả  (Màu Nâu) sẽ bị rơi khi người dùng nhảy vào

BaseObject g_background;
int Fake_Num=0;
int number = Plat_Num;
bool game_over = false;
bool start = false;
int count_ =0;
bool is_quit = false;
BaseObject g_over;
PowerUp loxo;
ThreatObjects Mouse;

// Khởi Tạo SDL
bool InitData(){
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
    g_windows = SDL_CreateWindow("DoraJump_NoCtrlZ"
        ,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);

    if (g_windows == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_windows, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen,255,255,255,255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags)&& imgFlags))
                success = false;
        }
    }
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        return false;
    g_sound_jump = Mix_LoadWAV("Sound//jump.wav");
    g_sound_fake = Mix_LoadWAV("Sound//lomise.wav");
    g_sound_die = Mix_LoadWAV("Sound//game_over.wav");
    g_sound_feder = Mix_LoadWAV("Sound//feder.wav");
    g_sound_over = Mix_LoadWAV("Sound//over.wav");
    g_music = Mix_LoadMUS("Sound//music.mp3");

    return success;


}
// Load Background
bool LoadBackground()
{
    bool ret = g_background.LoadImg("img/background.png",g_screen);
    if (ret == false ) return false;
    return true;
}
// Giải phóng khi kết thúc chương trình
void close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_windows);
    g_windows = NULL;
    IMG_Quit();
    SDL_Quit();
}
// Kiểm Tra Map Để Plat Không bị Trùng
bool checkMap(Platform* plat_objects)
{
    Platform *plat1,*plat2;
    for (int i =0; i< number; i++)
        for (int j =i+1; j< number; j++)
        {
            plat1 = plat_objects + i;
            plat2 = plat_objects + j;
            if ((!(plat1->Drop||plat2->Drop))&&
                ((plat2->getX()>=plat1->getX()&& plat2->getX()<=plat1->getX()+width_platform_&&plat2->getY()>=plat1->getY()&&plat2->getY()<=plat1->getY()+height_platform_)||(
                plat2->getX()+width_platform_>= plat1->getX() && plat2->getX()<=plat1->getX()&& plat2->getY()+height_platform_>=plat1->getY()&& plat2->getY()+height_platform_<=plat1->getY()+height_platform_)
                ||(plat2->getX()>=plat1->getX()&& plat2->getX()<=plat1->getX()+width_platform_&& plat2->getY()+height_platform_>=plat1->getY()&& plat2->getY()+height_platform_<=plat1->getY()+height_platform_)
                ||(plat2->getX()+width_platform_>= plat1->getX() && plat2->getX()<=plat1->getX()&&plat2->getY()>=plat1->getY()&&plat2->getY()<=plat1->getY()+height_platform_)
                ))

                plat2->SetRect(rand()%(SCREEN_HEIGHT-height_platform_),-height_platform_-rand()%50);

        }
}

bool checkJump(Platform* plat_objects,MainObject &p_player)
{
    Platform* plat;
    checkMap(plat_objects); // Kiểm tra Map xem có ván nào bị trùng không.
    for (int tt =0; tt<number;tt++)
        {
             if (plat->object==1)
            {
                loxo.SetRect(plat->getX()+width_platform_/2-loxo_width/2,plat->getY()-loxo_height+6) ;
            }
            if (plat->object==2)
            {
                Mouse.SetRect(plat->getX()+width_platform_/2-width_monster_/2,plat->getY()-height_monster_+6);
            }
        plat=(plat_objects +tt); // Tăng con trỏ lên 1 đơn vị để trỏ vào phần tử tiếp theo
        if (plat->Drop) // Hiệu ứng Fake Plat rơi
            {
                plat->LoadImg("img\\Platform_Fake_Drop.png",g_screen);
                plat->SetRect(plat->getX(),plat->getY()+DROP_SPEED);;
            }
        // Nếu đi quá màn hình thì reset lại vị trí của Plat
        if (plat->getY()>SCREEN_HEIGHT)
           {
            if (tt==0&&rand()%2==1) plat->object=1; else if (tt==0) plat->object=0;
            if (tt==1&&rand()%3==1) plat->object=2; else if (tt==1) plat->object=0;
            plat->SetRect(rand()%(SCREEN_WIDTH-width_platform_),-height_platform_-rand()%10);
            if (plat->getPlat_type()==FAKE_PLAT) plat->SetRect(rand()%SCREEN_WIDTH,-height_platform_*2);
            // Hủy đi trạng thái rơi của Fake Plat
            plat->Drop=false;
            if (Fake_Num<=MAX_FAKE_PLAT&&rand()%20==1&&tt!=0&&tt!=1)
            {
                plat->setPlat_type(FAKE_PLAT);
                Fake_Num++;

            }
           }

        }


    // Nếu người chơi vượt qua một chiều cao nào đó thì dịch chuyển cả map đi xuống
    //std::cout << p_player.on_ground << "\n";
    if (p_player.getY()<=MAX_HEIGHT)
    {
        loxo.SetRect(loxo.getX(),loxo.getY()-p_player.getY_val());
        Mouse.SetRect(Mouse.getX(),Mouse.getY()-p_player.getY_val());
        for (int tt =0; tt<number;tt++)
        {
        plat=(plat_objects + tt);

        plat->SetRect(plat->getX(),plat->getY()-p_player.getY_val()); // Dịch chuyển map
        if(!plat->Drop)
        switch (plat->getPlat_type())
        {
            case REAL_PLAT:
            plat->LoadImg("img\\Platform.png",g_screen);
            break;
            case FAKE_PLAT:
            plat->LoadImg("img\\Platform_fake.png",g_screen);
            break;

        }
        // Điểm số của người chơi
        score_-=p_player.getY_val();
        score = score_/1000;
        std::cout <<"Score: "<< score<< "\n";
        // Đạt mức 500 điểm thì giảm đi 5 tấm ván
        if (abs(score-500)<10) number=Plat_Num-4;
        // Đạt 1000 điểm giảm tiếp đi 5 tấm nữa
        if (abs(score-1000)<10) number=Plat_Num-8;
        if (abs(score-1500)<10) number=Plat_Num-12;
        if (abs(score-2000)<10) number=Plat_Num-16;

        }
    }


        if (p_player.on_ground)//||p_player.getY()>=SCREEN_HEIGHT-20)
       {



        if (!game_over)
        {
            g_over.Render(g_screen,NULL);
            g_over.SetRect(g_over.getX(),g_over.getY()-10);
            for (int tt =0; tt<number;tt++)
            {
            plat=(plat_objects +tt);
            if(!plat->Drop)
            plat->SetRect(plat->getX(),plat->getY()-10);
            };

        }
        else g_over.Animate(g_screen,391,523,5);
        count_++;
        if (count_ == 1) g_over.LoadImg("img//game_over.png",g_screen);
        if (count_ ==10) Mix_PlayChannel(-1,g_sound_die,0);
        if (count_ == 100)
        {
            game_over = true;
            g_over.LoadImg("img//game_over_animate.png",g_screen);
        }

       }


    // Kiểm tra xem nhân vật có giẫm vào Plat không để còn cho nhảy
    for (int t = 0; t<number; t++)
        {
        plat=(plat_objects +t);
        if  ((p_player.getY_val()>0)&&((p_player.getX()>=plat->getX()&& p_player.getX()<=plat->getX()+width_platform_ &&abs( p_player.getY()+ 30 - plat->getY())<=5)||
            (p_player.getX()+p_player.getWidth()>=plat->getX()&&
             p_player.getX()+p_player.getWidth() <=plat->getX()+width_platform_
             && abs(p_player.getY()+ 30 - plat->getY())<=10)))
        {
            if (plat->getPlat_type()==REAL_PLAT)
            {
                if (plat->object==1) p_player.highhigh_jump=true;
                if (plat->object==2&&!p_player.on_ground)
                {
                    Mix_PlayChannel(-1,g_sound_over,0);
                    //SDL_Delay(800);
                    p_player.on_ground = true;
                    return true;

                }
                if (!p_player.on_ground) return true;
            }


            // Nếu là Real Plat thì cho nhân vật nhảy lên, còn nếu là Fake Plat thì không làm gì
            else {
                    if (plat->Drop == false) Mix_PlayChannel(-1,g_sound_fake,0);
                    plat->Drop = true;
            }

        }


    }
    return false;
}
void LoadMenu()
{
    Mix_PlayMusic(g_music, -1);
    BaseObject Menu;
    Menu.LoadImg("img//Menu_clip.png",g_screen);
    while (!start)
    {
        Menu.Animate(g_screen,450,650,10);
         while (SDL_PollEvent(&g_event)!=0)
        {
            if (g_event.type == SDL_KEYDOWN)
            {

               start= true;

            }
            if (g_event.type == SDL_QUIT)
                {
                    is_quit = true;
                    start= true;
                    break;
                }
        }
        SDL_RenderPresent(g_screen);

    }

}

void reTry(Platform* plat_objects,MainObject *p_player)
{
//    p_player->y_pos_=0;
//    p_player->x_pos_=SCREEN_WIDTH/2-p_player->getWidth()/2;

    p_player->x_pos_= (plat_objects)->getX()+10;
    p_player->y_pos_= (plat_objects)->getY()-200;

    p_player->y_val_=0;
    for (int t = 0; t<number; t++) // Tạo ra number tấm ván
        {
            Platform* plat = (plat_objects +t);
            if (plat)
            {
                bool ret = plat->LoadImg("img\\Platform.png",g_screen);
            }

        // Chọn ngẫu nhiên vị trí cho các tấm ván
        int rand_y = rand()%(SCREEN_HEIGHT);
        int rand_x = rand()%(SCREEN_WIDTH-width_platform_);
        plat->SetRect(rand_x,rand_y);
        plat->setPlat_type(1);
        plat->object = 0;

        }
    //plat_objects->object = 1;
    // Kiểm tra xem map có tấm ván nào bị trùng với nhau hay không
    checkMap(plat_objects);
    g_over.SetRect(30,SCREEN_HEIGHT+420);

    game_over = false;
    p_player->on_ground = false;
    count_ = 0;
    number = Plat_Num;
    score_ = 0;
    score = 0;
    start = false;
    LoadMenu();

}
int main(int argc, char* argv[])
{
    ImpTimer fps_timer;
    if (InitData()== false) // Khởi tạo môi trường SDL
    return  -1;

    LoadMenu();

    if (LoadBackground()== false) // Load Background
    return -2;

    // Create Platform ( Tạo ra các tấm ván )
    Platform* plat_objects = new Platform[Plat_Num];
    srand(time(0)); // Khởi tạo tính ngẫu nhiên
    for (int t = 0; t<number; t++) // Tạo ra number tấm ván
        {
            Platform* plat = (plat_objects +t);
            if (plat)
            {
                bool ret = plat->LoadImg("img\\Platform.png",g_screen);
            }

        // Chọn ngẫu nhiên vị trí cho các tấm ván
        int rand_y = rand()%(SCREEN_HEIGHT);
        int rand_x = rand()%(SCREEN_WIDTH-width_platform_);
        plat->SetRect(rand_x,rand_y);


        }
    //plat_objects->object = 1;
    // Kiểm tra xem map có tấm ván nào bị trùng với nhau hay không
    checkMap(plat_objects);
    g_over.SetRect(30,SCREEN_HEIGHT+420);

    Score p_score;
    p_score.SetRect(400,10);


    // Tạo ra nhân vật
    MainObject p_player;
    // Load ảnh cho nhân vật
    p_player.LoadImg("img//player_right.png",g_screen);

    // Set hiệu ứng chuyển động Animation cho nhân vật
    p_player.set_clips();
    p_player.x_pos_= plat_objects->getX()+5;
    p_player.y_pos_= plat_objects->getY()-200;

    Mouse.LoadImg("img//Mouse.png",g_screen);
    Mouse.SetRect(0,SCREEN_WIDTH*2);
    g_over.LoadImg("img//game_over.png",g_screen);

    loxo.LoadImg("img//loxo.png",g_screen);
    loxo.SetRect(0,SCREEN_WIDTH*2);


    BaseObject score_bar_;
    score_bar_.LoadImg("img//Score_bar.png",g_screen);

    // Vòng lặp chính duy trì chương trình
    while (!is_quit)
    {
        fps_timer.start();

        // Bắt sự kiện từ người dùng
        while (SDL_PollEvent(&g_event)!=0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            p_player.HandleInputAction(g_event,g_screen);
            if (game_over==true && g_event.type == SDL_KEYDOWN)
            {

                if (g_event.key.keysym.sym == SDLK_SPACE)
                {
                reTry(plat_objects,&p_player);
                }
            }
        }
        // Color Key
        SDL_SetRenderDrawColor(g_screen,255,255,255,255);
        // Xuất ảnh lên màn hình
        SDL_RenderClear(g_screen);
        g_background.Render(g_screen,NULL);
        // Xuất hình ảnh các tấm ván
        for (int tt =0; tt <number; tt++)
        {
            Platform* plat = (plat_objects + tt);
            if (plat)
            {
                plat->Render(g_screen,NULL);
            }
        }
//        if (game_over)
//            g_over.Render(g_screen,NULL);

        loxo.Render(g_screen,NULL);
        Mouse.Render(g_screen,NULL);
        // Điều khiển nhân vật
        p_player.DoPlayer(checkJump(plat_objects,p_player),g_sound_jump,g_sound_feder);//plat_objects);
        // Xuất hình ảnh nhân vật lên màn hình
        p_player.Show(g_screen);

        std::cout<<p_player.getY()<<"\n";


        score_bar_.Render(g_screen,NULL);
        p_score.Show(g_screen,score);

        SDL_RenderPresent(g_screen);

        // Hàm tính thời gian để tính FPS
        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND; // milisecond

        if (real_imp_time< time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >=0)
                SDL_Delay(delay_time);
        }
    }

    close ();
    return 0;
}

