#ifndef WINDOW_H
#define WINDOW_H
#include "source.h"
//加载图像到纹理
extern double shoot_all[100][3];
extern SDL_Texture *shoot1;
extern SDL_Rect shoot;
extern double plane_posi[10][3];
extern SDL_Rect plane_out;

SDL_Texture *LoadImage(SDL_Renderer *renderer, char *file)
{
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, file);
    if (texture == NULL)
    {
        printf("%s img failed to load\n", file);
        exit(0);
    }
    return texture;
}

//渲染纹理
void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend, SDL_Rect *clip, double angle, SDL_Point *center)
{
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    if (clip != NULL)
    {
        pos.w = clip->w;
        pos.h = clip->h;
    }
    else
    {
        SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
    }
    SDL_RenderCopyEx(rend, tex, clip, &pos, angle, center, SDL_FLIP_NONE); //第三个参数是裁剪，第四个参数是坐标
}

//渲染地图
int ApplyMap(int map_x, int map_y, SDL_Texture *map, SDL_Renderer *renderer, int w, int h)
{
    for (int i = 0; i < MAP_H; i++)
    {
        for (int j = 0; j < MAP_W; j++)
        {
            ApplySurface(map_x + j * w, map_y + i * h, map, renderer, NULL, 0, NULL);
        }
    }
    if ((int)shoot_all[0][0])
    {
        SDL_Point point = {shoot.w / 2, shoot.h / 2};
        int all = (int)shoot_all[0][0];
        for (int i = 1; i <= all; i++)
        {
            ApplySurface(map_x + shoot_all[i][1], map_y + shoot_all[i][2], shoot1, renderer, NULL, shoot_all[i][0], &point);
            shoot_all[i][1] += SHOOT_V * sin(shoot_all[i][0] * PI / 180.0);
            shoot_all[i][2] -= SHOOT_V * cos(shoot_all[i][0] * PI / 180.0);
            if (shoot_all[i][1] <= 0 || shoot_all[i][1] >= MAP_W * w || shoot_all[i][2] <= 0 || shoot_all[i][2] >= MAP_H * h)
            {
                for (int j = i; j <= all; j++)
                {
                    shoot_all[j][0] = shoot_all[j + 1][0];
                    shoot_all[j][1] = shoot_all[j + 1][1];
                    shoot_all[j][2] = shoot_all[j + 1][2];
                }
                shoot_all[0][0] -= 1;
                all--;
                Mix_Chunk *boombreak = Mix_LoadWAV(BOOMBREAK);
                Mix_PlayChannel(-1, boombreak, 0);
            }
        }
    }
    //ai模块
    if (plane_posi[0][0])
    {
        for (int i = 1; i <= plane_posi[0][0]; i++)
        {
            if (plane_posi[i][1] - plane_out.w * 2 / 3 <= PLANER)
            {
                plane_posi[i][0] = ((int)plane_posi[i][0] + 180) % 360 - rand() % 10;
            }
            else if (plane_posi[i][1] + plane_out.w * 2 / 3 - PLANER >= MAP_W * w)
            {
                plane_posi[i][0] = ((int)plane_posi[i][0] + 180) % 360 + 180 - rand() % 10;
            }
            else if (plane_posi[i][2] - plane_out.h * 2 / 3 <= PLANER)
            {
                plane_posi[i][0] = ((int)plane_posi[i][0] + 180) % 360 - rand() % 10;
            }
            else if (plane_posi[i][2] + plane_out.h * 2 / 3 - PLANER >= MAP_H * h)
            {
                plane_posi[i][0] = ((int)plane_posi[i][0] + 180) % 360 + 180 - rand() % 10;
            }

            if (plane_posi[i][1] <= -500 || plane_posi[i][1] >= MAP_W * w + 500 || plane_posi[i][2] <= -500 || plane_posi[i][2] >= MAP_H * h + 500)
            {
                plane_posi[i][1] = 300;
                plane_posi[i][2] = 300;
            }
        }
    }
    else
    {
        return 1;
    }
    return 0;
}

//渲染文字
SDL_Texture *RenderText(SDL_Renderer *renderer, char *message, char *fontFile,
                        SDL_Color color, int fontSize)
{
    //打开字体文件
    TTF_Font *font = NULL;
    font = TTF_OpenFont(fontFile, fontSize);
    if (font == NULL)
    {
        printf("%s ttf failed to load\n", fontFile);
        exit(0);
    }
    //渲染文字
    SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    //清除不必要的组件
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

//获取相对于y轴的角度
double get_angel(int x, int y)
{
    double center_x = SCREEN_WIDTH / 2;
    double center_y = SCREEN_HEIGHT / 2;
    double angel = (double)atan2f((float)x - center_x, center_y - (float)y) * 180.0 / 3.1415;
    return angel;
}

//碰撞判断(使用圆碰撞模型)
int is_knock(double map_x, int map_y, int w, int h)
{
    if (SCREEN_WIDTH / 2 - map_x <= PLANER || map_x + MAP_W * w - SCREEN_WIDTH / 2 <= PLANER || SCREEN_HEIGHT / 2 - map_y <= PLANER || map_y + MAP_H * h - SCREEN_HEIGHT / 2 <= PLANER)
    {
        return 1;
    }
    return 0;
}

typedef struct
{
    double x;
    double y;
} point;

bool IsIntersection(point a, point b, point c, point d)
{
    //AB = A + r(B-A), r 在[0,1]
    //CD = C + t(D-C),s 在[0,1]
    double r, s;
    double deno = (b.x - a.x) * (d.y - c.y) - (b.y - a.y) * (d.x - c.x);
    double mem1 = (a.y - c.y) * (d.x - c.x) - (a.x - c.x) * (d.y - c.y);
    double mem2 = (a.y - c.y) * (b.x - a.x) - (a.x - c.x) * (b.y - a.y);
    r = mem1 / deno;
    s = mem2 / deno;
    if (r > 1 || r < 0)
        return false;
    if (s > 1 || s < 0)
        return false;

    return true;
}

int is_boom(double map_x, double map_y)
{
    int all = (int)shoot_all[0][0];
    int flag = 0;
    if ((int)plane_posi[0][0])
    {
        for (int i = 1; i <= all; i++)
        {
            point a, b, c, d;
            //第i个导弹
            a.x = (double)shoot_all[i][1];
            a.y = (double)shoot_all[i][2];
            b.x = (double)(shoot_all[i][1] + SHOOT_V * sin(shoot_all[i][0] * PI / 180.0));
            b.y = (double)(shoot_all[i][2] + SHOOT_V * cos(shoot_all[i][0] * PI / 180.0));
            for (int j = 1; j <= (int)plane_posi[0][0]; j++)
            {
                //第j个飞机
                c.x = (double)plane_posi[j][1] + plane_out.w / 2;
                c.y = (double)plane_posi[j][2] + plane_out.h / 2;
                d.x = (double)(plane_posi[j][1] + PLANE_V * sin(plane_posi[j][0] * PI / 180.0) + plane_out.w / 2);
                d.y = (double)(plane_posi[j][2] + PLANE_V * cos(plane_posi[j][0] * PI / 180.0) + plane_out.h / 2);
                //if (IsIntersection(a, b, c, d))
                if ((a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y) <= 30 * 30)
                {
                    for (int j = i; j <= all; j++)
                    {
                        shoot_all[j][0] = shoot_all[j + 1][0];
                        shoot_all[j][1] = shoot_all[j + 1][1];
                        shoot_all[j][2] = shoot_all[j + 1][2];
                    }
                    shoot_all[0][0] -= 1;
                    all--;
                    Mix_Chunk *boombreak = Mix_LoadWAV(BOOMBREAK);
                    Mix_PlayChannel(-1, boombreak, 0);

                    for (int k = j; k <= (int)plane_posi[0][0]; k++)
                    {
                        plane_posi[k][0] = plane_posi[k + 1][0];
                        plane_posi[k][1] = plane_posi[k + 1][1];
                        plane_posi[k][2] = plane_posi[k + 1][2];
                    }
                    //plane_posi[0][0] -= 1;
                    plane_posi[0][0] -= 1;
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
            {
                break;
            }
        }
    }
}

void sound_play(char *filename, int times)
{
    Mix_Music *music = Mix_LoadMUS(filename);
    Mix_PlayMusic(music, times);
}
#endif