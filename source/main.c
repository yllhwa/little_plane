#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_W 10
#define MAP_H 5
#define BLACKHOLE_C 5
#define PI 3.1415926
#define PLANER 2
#define MUSICTIME
#define PLANE_V 8
#define PLANE_V2 15
#define SHOOT_V 20

#include "window.h"
#include "source.h"
const int FPS = 1000 / 40;
Uint32 _FPS_Timer, last_time;
//主函数
double shoot_all[100][3];
SDL_Texture *shoot1 = NULL;
SDL_Rect shoot;
double plane_posi[10][3];
SDL_Rect plane_out;

char mess[10];

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL; //初始化为NULL，后面可以检测是否创建成功
    SDL_Renderer *renderer = NULL;
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_Init(MIX_DEFAULT_FORMAT);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    TTF_Init();
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "info",
                             "\
第一次坎星战争中，小兔子国成功取得了无产阶级革命的伟大胜利\n\
然而外国资本主义政权担心共产主义对其剥削统治造成颠覆，对小兔子国进行了丧心病狂的打压\n\
今天，外国的飞机已经公然开进了小兔子国的领空...\n\n\
鼠标控制飞机飞行的方向\n\
鼠标左键发射导弹\n\
空格键开启/关闭加速\n\
p键暂停\n\
ESC键退出\n\n\
对您的飞机进行频繁的操作将使得其反应能力下降\n\n\
一定情况下帝国主义的飞机会在领空外徘徊\n\
但是帝国主义亡我之心不死！其有很大概率重返领空\n\n\
大量开火之前请考虑您守护的人民，我们追求精准打击！",
                             NULL);
    //产生窗口
    window = SDL_CreateWindow("little plane", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALLOW_HIGHDPI);
    //产生渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //SDL_RENDERER_ACCELERATED:使用硬件加速(即GPU加速),SDL_RENDERER_PRESENTVSYNC(使用显示器的刷新率来更新画面)
    //加载纹理
    SDL_Texture *plane1 = NULL, *map1 = NULL, *plane2 = NULL;
    plane1 = LoadImage(renderer, PLANE1);
    plane2 = LoadImage(renderer, PLANE2);
    map1 = LoadImage(renderer, MAP1);
    shoot1 = LoadImage(renderer, SHOOT);

    SDL_Texture *image11 = NULL;
    SDL_Color color = {0, 0, 0};
    image11 = RenderText(renderer, "TTF fonts are cool!", TESTTTF1,
                         color, 64);

    SDL_Rect plane;
    SDL_Rect plane2_rect;
    SDL_Rect map;
    SDL_Rect shoot;
    SDL_QueryTexture(plane1, NULL, NULL, &plane.w, &plane.h);
    SDL_QueryTexture(plane2, NULL, NULL, &plane_out.w, &plane_out.h);
    SDL_QueryTexture(plane2, NULL, NULL, &plane2_rect.w, &plane2_rect.h);
    SDL_QueryTexture(map1, NULL, NULL, &map.w, &map.h);
    SDL_QueryTexture(shoot1, NULL, NULL, &shoot.w, &shoot.h);
    //渲染纹理
    SDL_Point center_point = {plane.w / 2, plane.h / 2}; //旋转坐标是相对于图像而言
    double angle = 90;
    ApplySurface(0, 0, map1, renderer, NULL, 0, NULL);
    ApplySurface(SCREEN_WIDTH / 2 - plane.w / 2, SCREEN_HEIGHT / 2 - plane.h / 2, plane1, renderer, NULL, angle, &center_point);
    SDL_RenderPresent(renderer);

    Mix_Chunk *fashe = Mix_LoadWAV(FASHE);
    Mix_Chunk *win_s = Mix_LoadWAV(WIN);
    Mix_Chunk *dash = Mix_LoadWAV(DASH);
    Mix_Chunk *out_s = Mix_LoadWAV(OUT);

    //事件处理
    double map_x = 0,
           map_y = 0;
    double v = PLANE_V;
    SDL_Event my_event;
    int quit = 0;
    sound_play(BGM1, -1);
    int speedup = 0;

    plane_posi[0][0] = 8;

    srand((unsigned)time(NULL));
    for (int i = 1; i <= 8; i++)
    {
        plane_posi[i][0] = rand() % 180;
        plane_posi[i][1] = SCREEN_WIDTH * 3 / 4;
        plane_posi[i][2] = SCREEN_HEIGHT * 3 / 4;
    }
    while (SDL_PollEvent(&my_event))
    {
        continue;
    }
    int win = 0;
    while (!quit) //事件主循环
    {
        if (win)
        {
            Mix_PlayChannel(-1, win_s, 0);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                     "You Win",
                                     "\
从来就没有什么救世主，\n\
    也不靠神仙皇帝！\n\
        要创造人类的幸福，\n\
            全靠我们自己！\n\
                我们要夺回劳动果实，\n\
            让思想冲破牢笼！\n\
        快把那炉火烧得通红，\n\
    趁热打铁才能成功!",
                                     NULL);
            exit(0);
        }
        if (SDL_GetTicks() - _FPS_Timer < FPS)
        {
            SDL_Delay(FPS - SDL_GetTicks() + _FPS_Timer);
        }
        if (is_knock(map_x, map_y, map.w, map.h))
        {
            Mix_PlayChannel(-1, out_s, 0);
            map_x = -100;
            map_y = -100;
            while (SDL_PollEvent(&my_event))
            {
                continue;
            }
        }
        is_boom(map_x, map_y);
        //angle = get_angel(my_event.motion.x, my_event.motion.y);
        map_x -= v * sin(angle * PI / 180.0);
        map_y += v * cos(angle * PI / 180.0);
        mess[0] = (char)((int)plane_posi[0][0] + 48);
        mess[1] = '\0';
        image11 = RenderText(renderer, mess, TESTTTF1,
                             color, 64);
        SDL_RenderClear(renderer);
        win = ApplyMap(map_x, map_y, map1, renderer, map.w, map.h);
        ApplySurface(SCREEN_WIDTH / 2 - plane.w / 2, SCREEN_HEIGHT / 2 - plane.h / 2, plane1, renderer, NULL, angle, &center_point);
        ApplySurface(10, 10, image11, renderer, NULL, 0, NULL);

        if (plane_posi[0][0])
        {
            for (int i = 1; i <= (int)plane_posi[0][0]; i++)
            {
                plane_posi[i][1] += PLANE_V * sin(plane_posi[i][0] * PI / 180.0);
                plane_posi[i][2] -= PLANE_V * cos(plane_posi[i][0] * PI / 180.0);
                ApplySurface(map_x + plane_posi[i][1], map_y + plane_posi[i][2], plane2, renderer, NULL, plane_posi[i][0], NULL);
            }
        }
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&my_event)) //从队列里取出事件
        {
            if (win)
            {
                Mix_PlayChannel(-1, win_s, 0);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                         "You Win",
                                         "\
从来就没有什么救世主，\n\
    也不靠神仙皇帝！\n\
        要创造人类的幸福，\n\
            全靠我们自己！\n\
                我们要夺回劳动果实，\n\
            让思想冲破牢笼！\n\
        快把那炉火烧得通红，\n\
    趁热打铁才能成功!",
                                         NULL);

                exit(0);
            }
            if (is_knock(map_x, map_y, map.w, map.h))
            {
                Mix_PlayChannel(-1, out_s, 0);
                map_x = -100;
                map_y = -100;
                while (SDL_PollEvent(&my_event))
                {
                    continue;
                }
            }
            is_boom(map_x, map_y);
            angle = get_angel(my_event.motion.x, my_event.motion.y);
            map_x -= v * sin(angle * PI / 180.0);
            map_y += v * cos(angle * PI / 180.0);

            SDL_RenderClear(renderer);
            win = ApplyMap(map_x, map_y, map1, renderer, map.w, map.h);
            ApplySurface(SCREEN_WIDTH / 2 - plane.w / 2, SCREEN_HEIGHT / 2 - plane.h / 2, plane1, renderer, NULL, angle, &center_point);
            mess[0] = (char)((int)plane_posi[0][0] + 48);
            mess[1] = '\0';
            image11 = RenderText(renderer, mess, TESTTTF1,
                                 color, 64);
            ApplySurface(10, 10, image11, renderer, NULL, 0, NULL);

            if (plane_posi[0][0])
            {
                for (int i = 1; i <= (int)plane_posi[0][0]; i++)
                {
                    plane_posi[i][1] += PLANE_V * sin(plane_posi[i][0] * PI / 180.0);
                    plane_posi[i][2] -= PLANE_V * cos(plane_posi[i][0] * PI / 180.0);
                    ApplySurface(map_x + plane_posi[i][1], map_y + plane_posi[i][2], plane2, renderer, NULL, plane_posi[i][0], NULL);
                }
            }
            SDL_RenderPresent(renderer);
            switch (my_event.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN:
                switch (my_event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = 1;
                    break;
                case SDLK_SPACE:
                    if (speedup)
                    {
                        v = PLANE_V;
                        speedup = 0;
                    }
                    else
                    {
                        v = PLANE_V2;
                        Mix_PlayChannel(-1, dash, 0);
                        speedup = 1;
                    }
                    break;
                case SDLK_p:
                    while (SDL_WaitEvent(&my_event))
                    {
                        if (my_event.type == SDL_KEYDOWN && my_event.key.keysym.sym == SDLK_p)
                        {
                            break;
                        }
                        else
                        {
                            SDL_Delay(50);
                        }
                    }
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEMOTION:
                if (win)
                {
                    Mix_PlayChannel(-1, win_s, 0);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                             "You Win",
                                             "\
从来就没有什么救世主，\n\
    也不靠神仙皇帝！\n\
        要创造人类的幸福，\n\
            全靠我们自己！\n\
                我们要夺回劳动果实，\n\
            让思想冲破牢笼！\n\
        快把那炉火烧得通红，\n\
    趁热打铁才能成功!",
                                             NULL);

                    exit(0);
                }
                SDL_RenderClear(renderer);
                win = ApplyMap(map_x, map_y, map1, renderer, map.w, map.h);
                ApplySurface(SCREEN_WIDTH / 2 - plane.w / 2, SCREEN_HEIGHT / 2 - plane.h / 2, plane1, renderer, NULL, angle, &center_point);
                if (plane_posi[0][0])
                {
                    for (int i = 1; i <= (int)plane_posi[0][0]; i++)
                    {
                        plane_posi[i][1] += PLANE_V * sin(plane_posi[i][0] * PI / 180.0);
                        plane_posi[i][2] -= PLANE_V * cos(plane_posi[i][0] * PI / 180.0);
                        ApplySurface(map_x + plane_posi[i][1], map_y + plane_posi[i][2], plane2, renderer, NULL, plane_posi[i][0], NULL);
                    }
                }
                mess[0] = (char)((int)plane_posi[0][0] + 48);
                mess[1] = '\0';
                image11 = RenderText(renderer, mess, TESTTTF1,
                                     color, 64);
                ApplySurface(10, 10, image11, renderer, NULL, 0, NULL);

                SDL_RenderPresent(renderer);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (my_event.button.button == SDL_BUTTON_LEFT)
                {
                    int posi = (int)shoot_all[0][0] + 1;
                    shoot_all[posi][0] = get_angel(my_event.motion.x, my_event.motion.y);
                    shoot_all[posi][1] = SCREEN_WIDTH / 2 - map_x;
                    shoot_all[posi][2] = SCREEN_HEIGHT / 2 - map_y;
                    shoot_all[0][0] += 1;
                    Mix_PlayChannel(-1, fashe, 0);
                }
                else if (my_event.button.button == SDL_BUTTON_RIGHT)
                {
                }
                break;
            default:
                break;
            }
        }
        _FPS_Timer = SDL_GetTicks();
        //SDL_Delay(5);
    }
    //退出之前一定要释放掉窗口
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}