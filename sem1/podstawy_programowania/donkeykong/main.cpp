#include <math.h>
#include <stdio.h>
#include <time.h>

extern "C"
{
#include "./SDL2-2.0.10/include/SDL.h"
#include "./SDL2-2.0.10/include/SDL_main.h"
}

#define _USE_MATH_DEFINES
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define GRAVITY 9.81
// #define GRAVITY 0
#define SAVEOFFSET 70
#define MAXFRAMERATE 60

struct coords
{
    int x;
    int y;
};

struct tempText
{
    int x;
    int y;
    int size;
    const char *text;
    SDL_Color color;
    double timeEnd;
};

struct defaultData
{
    int x;
    int y;
    int width;
    int height;
};

template <typename TYPE>
class Vector
{
private:
    TYPE *elements;
    int capacity;
    int count;

public:
    Vector()
    {
        capacity = 10;
        count = 0;
        elements = new TYPE[capacity];
    }

    ~Vector()
    {
        delete[] elements;
    }

    void push_back(TYPE value)
    {
        if (count == capacity)
        {
            TYPE *newElements = new TYPE[capacity * 2];
            for (int i = 0; i < count; i++)
            {
                newElements[i] = elements[i];
            }
            delete[] elements;
            elements = newElements;
            capacity *= 2;
        }
        elements[count] = value;
        count++;
    }

    TYPE *begin()
    {
        return elements;
    }

    TYPE *end()
    {
        return elements + count;
    }

    void clear()
    {
        delete[] elements;
        capacity = 10;
        count = 0;
        elements = new TYPE[capacity]();
    }

    void erase(int index)
    {
        for (int i = index; i < count - 1; i++)
        {
            elements[i] = elements[i + 1];
        }
        count--;
    }

    int size()
    {
        return count;
    }

    TYPE &back()
    {
        return elements[count - 1];
    }

    TYPE &operator[](int index)
    {
        return elements[index];
    }

    const TYPE &operator[](int index) const
    {
        return elements[index];
    }
};

int strlen(const char *str)
{
    int i = 0;
    while (str[i] != '\0')
        i++;
    return i;
}

void DrawString(SDL_Surface *screen, int x, int y, int size, const char *text,
                SDL_Surface *charset, SDL_Color color = {255, 255, 255, 255})
{
    int px, py, c;
    SDL_Rect s, d;
    s.w = 8;
    s.h = 8;
    d.w = size;
    d.h = size;
    while (*text)
    {
        c = *text & 255;
        px = (c % 16) * 8;
        py = (c / 16) * 8;
        s.x = px;
        s.y = py;
        d.x = x;
        d.y = y;
        SDL_Surface *temp = SDL_ConvertSurfaceFormat(charset, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_SetSurfaceColorMod(temp, color.r, color.g, color.b);
        SDL_SetSurfaceAlphaMod(temp, color.a);

        SDL_BlitScaled(temp, &s, screen, &d);

        SDL_SetSurfaceColorMod(temp, 255, 255, 255);
        SDL_SetSurfaceAlphaMod(temp, 255);
        x += size;
        text++;
        SDL_FreeSurface(temp);
    };
};

void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y, int width = 0, int height = 0)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (width == 0)
    {
        dest.w = sprite->w;
    }
    else
    {
        dest.w = width;
    }
    if (height == 0)
    {
        dest.h = sprite->h;
    }
    else
    {
        dest.h = height;
    }
    SDL_BlitScaled(sprite, NULL, screen, &dest);
};

void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32 *)p = color;
};

void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color)
{
    for (int i = 0; i < l; i++)
    {
        DrawPixel(screen, x, y, color);
        x += dx;
        y += dy;
    };
};

void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor)
{
    int i;
    DrawLine(screen, x, y, k, 0, 1, outlineColor);
    DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
    DrawLine(screen, x, y, l, 1, 0, outlineColor);
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
    for (i = y + 1; i < y + k - 1; i++)
        DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

int init(SDL_Window **window, SDL_Renderer **renderer, SDL_Surface **screen, SDL_Texture **scrtex, SDL_Surface **charset)
{
    printf("init\n");
    int rc;
    srand(time(0));
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
                                     window, renderer);
    // rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
    //                                  window, renderer);
    if (rc != 0)
    {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    };

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(*window, "Matsvei Kasparovich 201627");

    *screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                   0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    *scrtex = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_ShowCursor(SDL_DISABLE);

    *charset = SDL_LoadBMP("./textures/cs8x8.bmp");
    if (*charset == NULL)
    {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(*screen);
        SDL_DestroyTexture(*scrtex);
        SDL_DestroyWindow(*window);
        SDL_DestroyRenderer(*renderer);
        SDL_Quit();
        return 1;
    };
    SDL_SetColorKey(*charset, true, 0x000000);
    return 0;
}

int loadTexture(SDL_Surface **surface, SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **scrtex, const char *path)
{
    *surface = SDL_LoadBMP(path);
    if (*surface == NULL)
    {
        printf("SDL_LoadBMP(%s) error: %s\n", path, SDL_GetError());
        SDL_FreeSurface(*surface);
        SDL_DestroyTexture(*scrtex);
        SDL_DestroyWindow(*window);
        SDL_DestroyRenderer(*renderer);
        SDL_Quit();
        return 1;
    };
    return 0;
}

class Animation
{
public:
    SDL_Surface *texture;
    int width, height, currentFrame = 0, state = 0;
    double frameTime, lastFrameTime = 0;
    int *animationFrames;

    Animation(SDL_Surface *texture, int width, int height, double frameTime, int *animationFrames)
    {
        this->texture = texture;
        this->width = width;
        this->height = height;
        this->frameTime = frameTime;
        this->animationFrames = animationFrames;
    }

    void updateFrame(double delta)
    {
        lastFrameTime += delta;
        currentFrame = (int)(lastFrameTime / frameTime) % animationFrames[state];
    }

    void draw(SDL_Surface *screen, int x, int y, int entWidth, int entHeight)
    {
        SDL_Rect s, d;
        s.x = currentFrame * width;
        s.y = state * height;
        s.w = width;
        s.h = height;
        d.x = x;
        d.y = y;
        d.w = entWidth;
        d.h = entHeight;
        SDL_BlitScaled(texture, &s, screen, &d);
    }
};
// types:
// 0 - platform
// 1 - ledder
class GameObject
{
public:
    int x, y, width, height, type, isDestoyed = 0;
    SDL_Surface *texture;
    GameObject() {}
    GameObject(int x, int y, int width, int height, SDL_Surface *texture)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->texture = texture;
    }
};

class GameEntity
{
public:
    int x;
    int y;
    int width;
    int height;
    int state = 0;
    float xSpeed = 0;
    float ySpeed = 0;
    float jumpHeight = 0.4;
    float speed = 50;
    bool isOnGround = false;
    bool isOnLedder = false;
    bool isUp = false;
    bool isDown = false;
    bool isLooted = false;
    char type; // m - mario, p - policeman, b - boss, s - princess
    Animation *animation;

    GameEntity() {}
    GameEntity(int x, int y, int width, int height, Animation *animation, char type)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->animation = animation;
        this->type = type;
    }

    void updateData(double delta)
    {
        if (!isOnGround)
        {
            y += (ySpeed <= 0) ? ySpeed * speed * delta : ceil(ySpeed * speed * delta);
            if (isUp)
                if (isOnLedder)
                    y--;
                else
                    isUp = false;
            else if (isDown)
                if (isOnLedder)
                    y++;
                else
                    isDown = false;

            else if (type != 'b' && type != 's')
                ySpeed += GRAVITY * delta;
        }
        else
        {
            isUp = false;
            isDown = false;
        }
        x += (xSpeed <= 0) ? xSpeed * speed * delta : ceil(xSpeed * speed * delta);
        if (type == 'p' || type == 'b' || type == 's')
            return;
        if (y > SCREEN_HEIGHT - SAVEOFFSET)
        {
            y = SCREEN_HEIGHT - SAVEOFFSET;
            ySpeed = 0;
            isOnGround = true;
        }
        if (y < 0 + SAVEOFFSET)
        {
            y = 0 + SAVEOFFSET;
            ySpeed = 0;
        }
        if (x < 0 + SAVEOFFSET)
        {
            x = 0 + SAVEOFFSET;
            xSpeed = 0;
        }
        if (x > SCREEN_WIDTH - SAVEOFFSET - width)
        {
            xSpeed = 0;
            x = SCREEN_WIDTH - SAVEOFFSET - width;
        }
    }

    void updateFrame(SDL_Surface *screen, double delta)
    {
        animation->state = state;
        animation->updateFrame(delta);
        animation->draw(screen, x, y, width, height);
    }
    void setTexture(SDL_Surface *texture)
    {
        this->animation->texture = texture;
    }

    void printData()
    {
        printf("x: %d, y: %d\n", x, y);
        printf("xSpeed: %lf, ySpeed: %lf\n", xSpeed, ySpeed);
    }

    coords getPos()
    {
        return {x, y};
    }

    void jump()
    {
        if (isOnGround)
        {
            ySpeed = -sqrt(2 * GRAVITY * jumpHeight);
            isOnGround = false;
        }
    }

    void down()
    {
        if (isOnGround)
        {
            y += 10;
        }
    }
};

bool checkAABB(GameEntity &entity, GameObject &object)
{
    if (entity.x + entity.width >= object.x && entity.x <= object.x + object.width &&
        entity.y + entity.height >= object.y && entity.y <= object.y + object.height)
    {
        return true;
    }
    return false;
}

// bool checkAABBBCoords(defaultData &entity, defaultData &object)
// {
//     if (entity.x + entity.width >= object.x && entity.x <= object.x + object.width &&
//         entity.y + entity.height >= object.y && entity.y <= object.y + object.height)
//     {
//         return true;
//     }
//     return false;
// }

int AABBWithDepth(defaultData &entity, defaultData &object)
{
    if (entity.x + entity.width >= object.x && entity.x <= object.x + object.width &&
        entity.y + entity.height >= object.y && entity.y <= object.y + object.height)
    {
        float overlapX = std::min(entity.x + entity.width, object.x + object.width) - std::max(entity.x, object.x);
        float overlapY = std::min(entity.y + entity.height, object.y + object.height) - std::max(entity.y, object.y);
        return overlapX < overlapY ? overlapX : overlapY;
    }
    return 0;
}

float checkCollision(GameEntity &entity, GameObject &object)
{
    float penetrationDepth = 0.0;

    if (entity.x + entity.width >= object.x && entity.x <= object.x + object.width &&
        entity.y + entity.height >= object.y && entity.y + entity.height - 2 <= object.y + 4)
    {
        float overlapX = std::min(entity.x + entity.width, object.x + object.width) - std::max(entity.x, object.x);
        float overlapY = std::min(entity.y + entity.height, object.y + object.height) - std::max(entity.y, object.y);

        if (overlapX < overlapY)
        {
            penetrationDepth = overlapX;
            if (entity.xSpeed >= 0)
            {
                entity.x -= overlapX;
                if (entity.type == 'p')
                    entity.xSpeed = -entity.xSpeed;
            }
            else
            {
                entity.x += overlapX;
                if (entity.type == 'p')
                    entity.xSpeed = -entity.xSpeed;
            }
        }
        else
        {
            penetrationDepth = overlapY;
            if (entity.ySpeed >= 0)
            {
                entity.y -= overlapY;
                entity.isOnGround = true;
            }
            else
            {
                entity.y += overlapY;
            }
        }
    }
    return penetrationDepth;
}

void addPlatforms(Vector<GameObject> &platforms, SDL_Surface *platformSurface, int xStart, int yStart, int count, bool isStraight = 0, bool isUp = 1)
{
    int bundle = 2;
    int size = 24;
    if (isStraight)
    {
        for (int c = 0; c < count; c++)
        {
            platforms.push_back(GameObject(xStart + size * c, yStart, size, size, platformSurface));
        }
    }
    else
    {
        for (int c = 0; c < count; c++)
        {
            for (int b = 0; b < bundle; b++)
            {
                if (isUp)
                {
                    platforms.push_back(GameObject(xStart + size * b + c * size * bundle, yStart - 2 * c, size, size, platformSurface));
                }
                else
                {
                    platforms.push_back(GameObject(xStart + size * b + c * size * bundle, yStart + 2 * c, size, size, platformSurface));
                }
            }
        }
    }
}

int randint(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void leddersConstructor(Vector<GameObject> &ledders, Vector<GameObject> &platforms, defaultData coords, SDL_Surface *ledderSurface)
{
    ledders.push_back(GameObject(coords.x, coords.y, 24, 24, ledderSurface));
    int toY = SCREEN_HEIGHT - SAVEOFFSET;
    for (const GameObject &platform2 : platforms)
    {
        if (platform2.x == ledders.back().x && platform2.y > ledders.back().y)
        {
            if (platform2.y < toY)
                toY = platform2.y;
        }
    }
    toY -= 24;
    while (ledders.back().y < toY)
    {
        ledders.push_back(GameObject(ledders.back().x, ledders.back().y + 24, 24, 24, ledderSurface));
    }
}

void level1(Vector<GameObject> &platforms, SDL_Surface *platformSurface, Vector<GameObject> &ledders, SDL_Surface *ledderSurface, Vector<GameObject> &items, SDL_Surface *itemSurface)
{
    addPlatforms(platforms, platformSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET, 18, 1);
    addPlatforms(platforms, platformSurface, SAVEOFFSET + 18 * 24, SCREEN_HEIGHT - SAVEOFFSET, 9);
    addPlatforms(platforms, platformSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET - 24 * 4, 18, false, false);
    addPlatforms(platforms, platformSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET - 24 * 8, 18);
    for (int i = 0; i < 18; i += 2)
        addPlatforms(platforms, platformSurface, SAVEOFFSET + i * 24 * 2, SCREEN_HEIGHT - SAVEOFFSET - 24 * 12, 2, 1);
    addPlatforms(platforms, platformSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET - 24 * 16, 36, 1);

    int randIndexesLedders[8] = {randint(45, 71), randint(72, 90), randint(117, 125), randint(144, 161), randint(45, 71), randint(72, 90), randint(117, 125), randint(144, 161)};
    printf("sizeof(platforms) = %d\n", platforms.size());

    for (int i = 0; i < platforms.size(); i++)
    {
        GameObject &platform = platforms[i];
        if ((rand() % 25 == 0) && i > 10)
        {
            if (items.size() != 0)
            {
                bool isOk = true;
                for (const GameObject &item : items)
                {
                    int x = platform.x - item.x;
                    int y = platform.y - 24 - item.y;
                    float dest = sqrt(x * x + y * y);
                    if (dest < 150)
                        isOk = false;
                }
                if (isOk)
                    items.push_back(GameObject(platform.x, platform.y - 24, 24, 24, itemSurface));
            }
            else
                items.push_back(GameObject(platform.x, platform.y - 24, 24, 24, itemSurface));
        }
        for (int index : randIndexesLedders)
        {
            if (i == index)
            {
                defaultData coords = {platform.x, platform.y, 24, 24};
                leddersConstructor(ledders, platforms, coords, ledderSurface);
            }
        }
    }
    defaultData coords = {platforms[platforms.size() - 33].x, platforms[platforms.size() - 33].y - 24 * 8, 24, 24};
    leddersConstructor(ledders, platforms, coords, ledderSurface);
}
void level2(Vector<GameObject> &platforms, SDL_Surface *platformSurface, Vector<GameObject> &ledders, SDL_Surface *ledderSurface, Vector<GameObject> &items, SDL_Surface *itemSurface)
{
    addPlatforms(platforms, platformSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET, 36, 1);
    for (int i = 1; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
            addPlatforms(platforms, platformSurface, SAVEOFFSET + j * 24 * 8, SCREEN_HEIGHT - SAVEOFFSET - 24 * 3 * i, 4, 1);
    }
    for (int j = 0; j < 4; j++)
        addPlatforms(platforms, platformSurface, SAVEOFFSET + j * 24 * 8, SCREEN_HEIGHT - SAVEOFFSET - 24 * 16, 6, 1);

    int randIndexesLedders[5] = {randint(48, 55), randint(56, 63), randint(88, 95), randint(96, 103), 136};
    printf("sizeof(platforms) = %d\n", platforms.size());

    for (int i = 0; i < platforms.size(); i++)
    {
        GameObject &platform = platforms[i];
        if ((rand() % 25 == 0) && i > 10)
        {
            if (items.size() != 0)
            {
                bool isOk = true;
                for (const GameObject &item : items)
                {
                    int x = platform.x - item.x;
                    int y = platform.y - 24 - item.y;
                    float dest = sqrt(x * x + y * y);
                    if (dest < 150)
                        isOk = false;
                }
                if (isOk)
                    items.push_back(GameObject(platform.x, platform.y - 24, 24, 24, itemSurface));
            }
            else
                items.push_back(GameObject(platform.x, platform.y - 24, 24, 24, itemSurface));
        }
        for (int index : randIndexesLedders)
        {
            if (i == index)
            {
                defaultData coords = {platform.x, platform.y, 24, 24};
                leddersConstructor(ledders, platforms, coords, ledderSurface);
            }
        }
    }
    defaultData coords = {platforms[platforms.size() - 22].x, platforms[platforms.size() - 22].y - 24 * 8, 24, 24};
    leddersConstructor(ledders, platforms, coords, ledderSurface);
}

double eval(const char *formula, double x)
{
    double result = 0;
    int i = 0;
    while (formula[i] != '\0')
    {
        switch (formula[i])
        {
        case 'x':
            result += x;
            break;
        case '^':
            result = pow(result, formula[++i] - '0');
            break;
        case '-':
            result -= formula[++i] - '0';
            break;
        case '+':
            result += formula[++i] - '0';
            break;
        case '*':
            result *= formula[++i] - '0';
            break;
        case '/':
            result /= formula[++i] - '0';
            break;
        case '%':
            result = (int)result % (formula[++i] - '0');
            break;
        case 's':
            result = sin(result);
            break;
        case 'c':
            result = cos(result);
            break;
        case 'r':
            result = sqrt(result);
            break;
        case 'a':
            result = fabs(result);
            break;
        case 'f':
            result = floor(result);
            break;
        case 'p':
            result = pow(result, formula[++i] - '0');
            break;
        case 'n':
            result = -result;
            break;
        case ' ':
            break; // do nothing
        default:
            printf("Unknown symbol: %c\n", formula[i]);
        }
        i++;
    }
    return result;
}

void addPlatformsByFormula(Vector<GameObject> &platforms, SDL_Surface *platformSurface, Vector<GameObject> &ledders, SDL_Surface *ledderSurface, int xStart, int yStart, int lenght, const char *formula, bool isLedder = true)
{
    int size = 24;
    for (int i = 0; i < lenght; i++)
    {
        double x = xStart + size * i;
        double y = yStart - eval(formula, i);
        platforms.push_back(GameObject(x, y, size, size, platformSurface));
        if (y > SCREEN_HEIGHT - SAVEOFFSET - 24)
            isLedder = false;
    }
    if (isLedder)
    {
        int ledderIndex = randint(0, lenght - 1);
        defaultData coords = {platforms[platforms.size() - lenght + ledderIndex].x, platforms[platforms.size() - lenght + ledderIndex].y, 24, 24};
        leddersConstructor(ledders, platforms, coords, ledderSurface);
    }
}

void level3(Vector<GameObject> &platforms, SDL_Surface *platformSurface, Vector<GameObject> &ledders, SDL_Surface *ledderSurface, Vector<GameObject> &items, SDL_Surface *itemSurface)
{
    // addPlatforms(platforms, platformSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET, 36, 1);
    addPlatforms(platforms, platformSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET - 24 * 16, 36, 1);

    addPlatformsByFormula(platforms, platformSurface, ledders, ledderSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET, 36, "x s *3 ");
    addPlatformsByFormula(platforms, platformSurface, ledders, ledderSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET - 24 * 6, 18, "x*2 n");
    addPlatformsByFormula(platforms, platformSurface, ledders, ledderSurface, SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET - 24 * 8, 7, "x s *6 *4");
    addPlatformsByFormula(platforms, platformSurface, ledders, ledderSurface, SAVEOFFSET + 24 * 6, SCREEN_HEIGHT - SAVEOFFSET - 24 * 10, 7, "");
    addPlatformsByFormula(platforms, platformSurface, ledders, ledderSurface, SAVEOFFSET + 24 * 17, SCREEN_HEIGHT - SAVEOFFSET - 24 * 8, 6, "x r *4", false);
    addPlatformsByFormula(platforms, platformSurface, ledders, ledderSurface, SAVEOFFSET + 24 * 24, SCREEN_HEIGHT - SAVEOFFSET - 24 * 9, 7, "", false);
    defaultData coords = {SAVEOFFSET + 24 * 30, SCREEN_HEIGHT - SAVEOFFSET - 24 * 16, 24, 24};
    leddersConstructor(ledders, platforms, coords, ledderSurface);
}

void resetData(GameEntity &mario, Vector<GameObject> &platforms, Vector<GameObject> &ledders, Vector<GameObject> &items, Vector<tempText> &tempTexts, Vector<GameEntity> &policemen, double &lastSpawnTime, double &spawnInterval, GameEntity &boss, GameEntity &princess)
{
    // new game
    mario.x = SAVEOFFSET;
    mario.y = SCREEN_HEIGHT - SAVEOFFSET - 40 - 8;
    mario.xSpeed = 0;
    mario.ySpeed = 0;
    mario.isOnGround = false;
    mario.isOnLedder = false;
    mario.isUp = false;
    mario.isDown = false;
    mario.state = 0;
    mario.animation->state = 0;

    boss.x = 70;
    boss.y = 98;
    boss.xSpeed = 0;
    boss.ySpeed = 0;

    princess.x = 166;
    princess.y = 98;
    princess.xSpeed = 0;
    princess.ySpeed = 0;

    spawnInterval = 3;
    platforms.clear();
    ledders.clear();
    items.clear();
    tempTexts.clear();
    policemen.clear();
}

double uniform(double min, double max)
{
    return (double)rand() / RAND_MAX * (max - min) + min;
}

#ifdef __cplusplus
extern "C"
#endif
    int
    main(int argc, char **argv)
{
    int t1, t2, quit, frames, score, scoreDelta, bestScore, level, cutscene = 0;
    double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
    coords position;
    SDL_Event event;
    SDL_Surface *screen, *charset;
    SDL_Surface *marioSurface;
    SDL_Surface *platformSurface;
    SDL_Surface *ledderSurface;
    SDL_Surface *itemSurface;
    SDL_Surface *policemanSurface;
    SDL_Surface *policeSpawnerSurface;
    SDL_Surface *bossSurface;
    SDL_Surface *princessSurface;
    SDL_Texture *scrtex;
    SDL_Window *window;
    SDL_Renderer *renderer;

    Vector<GameObject> platforms;
    Vector<GameObject> ledders;
    Vector<GameObject> items;

    Vector<tempText> tempTexts;

    Vector<GameEntity> policemen;

    if (init(&window, &renderer, &screen, &scrtex, &charset))
    {
        return 1;
    }

    if (loadTexture(&marioSurface, &window, &renderer, &scrtex, "./textures/anim_jan.bmp"))
    {
        return 1;
    }

    if (loadTexture(&platformSurface, &window, &renderer, &scrtex, "./textures/platform.bmp"))
    {
        return 1;
    }
    if (loadTexture(&ledderSurface, &window, &renderer, &scrtex, "./textures/ledder.bmp"))
    {
        return 1;
    }
    if (loadTexture(&itemSurface, &window, &renderer, &scrtex, "./textures/item1.bmp"))
    {
        return 1;
    }
    if (loadTexture(&policemanSurface, &window, &renderer, &scrtex, "./textures/anim_policeman.bmp"))
    {
        return 1;
    }
    if (loadTexture(&policeSpawnerSurface, &window, &renderer, &scrtex, "./textures/policeSpawner.bmp"))
    {
        return 1;
    }
    if (loadTexture(&bossSurface, &window, &renderer, &scrtex, "./textures/anim_boss.bmp"))
    {
        return 1;
    }
    if (loadTexture(&princessSurface, &window, &renderer, &scrtex, "./textures/princess.bmp"))
    {
        return 1;
    }

    level1(platforms, platformSurface, ledders, ledderSurface, items, itemSurface);

    char text[128];
    int background = SDL_MapRGB(screen->format, 0x20, 0x27, 0x2F); // #20272F
    t1 = SDL_GetTicks();

    // outside data
    frames = 0;
    fpsTimer = 0;
    fps = 0;
    quit = 0;
    worldTime = 0;

    // game data
    score = 0;
    scoreDelta = 0;
    bestScore = 12345;
    level = 1;
    double lastSpawnTime = 0;
    double spawnInterval = 3;
    double lastMotionTime = 0;
    GameEntity mario(SAVEOFFSET, SCREEN_HEIGHT - SAVEOFFSET - 40 - 8, 40, 40, new Animation(marioSurface, 64, 64, 0.4, new int[5]{1, 3, 3, 1, 2}), 'm');
    GameEntity boss(70, 98, 48, 48, new Animation(bossSurface, 64, 64, 0.4, new int[2]{1, 3}), 'b');
    GameObject policeSpawner(platforms[platforms.size() - 35].x, platforms[platforms.size() - 36].y - 48, 48, 48, policeSpawnerSurface);
    GameEntity princess(166, 98, 48, 48, new Animation(princessSurface, 64, 64, 1, new int[1]{1}), 's');
    printf("boss.x = %d, boss.y = %d\n", boss.x, boss.y);
    printf("princess.x = %d, princess.y = %d\n", princess.x, princess.y);
    while (!quit)
    {
        SDL_FillRect(screen, NULL, background);
        t2 = SDL_GetTicks();
        delta = (t2 - t1) * 0.001;
        t1 = t2;

        worldTime += delta;
        if (scoreDelta > 0)
        {
            score += 5;
            scoreDelta -= 5;
        }
        else if (scoreDelta < 0)
        {
            score -= 5;
            scoreDelta += 5;
        }

        defaultData marioData = {mario.x, mario.y, mario.width, mario.height};
        defaultData winData = {SAVEOFFSET, SAVEOFFSET, 192, 76};

        // DrawRectangle(screen, winData.x, winData.y, winData.width, winData.height, (255, 0, 0, 255), (0, 0, 0, 255));
        if (AABBWithDepth(marioData, winData))
        {
            if (level == 1)
            {
                cutscene = 1;
                score += 200;
            }
            else if (level == 2)
            {
                cutscene = 2;
                score += 500;
            }
            else if (level == 3)
            {
                cutscene = 3;
                score += 1000;
            }
        }

        if (cutscene == 1 || cutscene == 2)
        {
            boss.state = 0;
            if (lastMotionTime < .6)
            {
                boss.xSpeed = 2;
                boss.updateData(delta);
            }
            else if (lastMotionTime < .8)
            {
                princess.xSpeed = -2;
                princess.updateData(delta);
                boss.xSpeed = -2;
                boss.updateData(delta);
            }
            else if (lastMotionTime < 2.5)
            {
                princess.xSpeed = 0;
                princess.ySpeed = -2;
                princess.updateData(delta);
                boss.xSpeed = 0;
                boss.ySpeed = -2;
                boss.updateData(delta);
            }
            else
            {
                cutscene = 0;
                lastMotionTime = -delta;
                resetData(mario, platforms, ledders, items, tempTexts, policemen, lastSpawnTime, spawnInterval, boss, princess);
                if (level == 1)
                {
                    level = 2;
                    level2(platforms, platformSurface, ledders, ledderSurface, items, itemSurface);
                }
                else
                {
                    level = 3;
                    level3(platforms, platformSurface, ledders, ledderSurface, items, itemSurface);
                }
            }
            lastMotionTime += delta;
        }
        else if (cutscene == 3)
        {
            boss.state = 0;
            if (lastMotionTime < .6)
            {
                boss.xSpeed = -2;
                boss.updateData(delta);
            }
            else if (lastMotionTime < 1)
            {
                boss.xSpeed = 0;
                boss.ySpeed += 2 * delta * 100;
                boss.updateData(delta);
            }
            else
            {
                cutscene = 0;
                lastMotionTime = -delta;
                resetData(mario, platforms, ledders, items, tempTexts, policemen, lastSpawnTime, spawnInterval, boss, princess);
                level = 1;
                score = 0;
                scoreDelta = 0;
                worldTime = 0;
                cutscene = 0;
                lastMotionTime = 0;
                lastSpawnTime = 0;
                level1(platforms, platformSurface, ledders, ledderSurface, items, itemSurface);
            }
            lastMotionTime += delta;
        }

        if (worldTime - lastSpawnTime >= spawnInterval && !cutscene)
        {
            boss.state = 1;
            policemen.push_back(GameEntity(policeSpawner.x + 24, policeSpawner.y + 23, 24, 24, new Animation(policemanSurface, 64, 64, 0.4, new int[2]{4, 4}), 'p'));
            policemen.back().xSpeed = uniform(1, 3);
            lastSpawnTime = worldTime;
            spawnInterval = uniform(0, 5);
            boss.animation->frameTime = spawnInterval / 3 * 2;
            boss.animation->lastFrameTime = 0;
        }

        for (GameEntity &policeman : policemen)
        {
            if (policeman.x < 0 - policeman.width || policeman.x > SCREEN_WIDTH || policeman.y < 0 - policeman.height || policeman.y > SCREEN_HEIGHT)
            {
                policemen.erase(&policeman - policemen.begin());
                continue;
            }
            policeman.isOnGround = false;
            for (GameObject &platform : platforms)
            {
                float penetrationDepth = checkCollision(policeman, platform);
                if (penetrationDepth != 0)
                {
                    policeman.y -= penetrationDepth;
                    policeman.ySpeed = 0;
                    break;
                }
            }
            for (GameObject &ledders : ledders)
            {
                if (checkAABB(policeman, ledders) && rand() % 70 == 0 && policeman.y < SCREEN_HEIGHT - SAVEOFFSET - 40 - 8 * 3)
                {
                    policeman.isDown = true;
                    policeman.isOnLedder = true;
                    policeman.down();
                    break;
                }
                else
                {
                    policeman.isOnLedder = false;
                    policeman.isDown = false;
                }
            }

            // DrawSurface(screen, policemanSurface, policeman.x, policeman.y, policeman.height, policeman.width);
            if (policeman.xSpeed < 0)
            {
                policeman.state = 1;
                policeman.updateFrame(screen, delta);
            }
            else
            {
                policeman.state = 0;
                policeman.updateFrame(screen, delta);
            }
            policeman.updateData(delta);

            defaultData marioData = {mario.x, mario.y, mario.width, mario.height};
            defaultData policemanData = {policeman.x, policeman.y - mario.height, policeman.width, policeman.height + mario.height};
            int penetrationDepth = AABBWithDepth(marioData, policemanData);
            if (penetrationDepth)
            {
                if (mario.y + mario.height < policeman.y + policeman.height / 2)
                {
                    if (!policeman.isLooted)
                    {
                        policeman.isLooted = true;
                        int randScore = randint(1, 20) * 50;
                        scoreDelta += randScore;
                        char *t = new char[50];
                        sprintf(t, "+%d", randScore);
                        tempTexts.push_back(tempText{policeman.x, policeman.y - 10, 10, t, {0, 255, 0, 40}, worldTime + 1.3});
                    }
                }
                else if (penetrationDepth > 10 && !cutscene)
                {
                    resetData(mario, platforms, ledders, items, tempTexts, policemen, lastSpawnTime, spawnInterval, boss, princess);
                    level = 1;
                    score = 0;
                    scoreDelta = 0;
                    worldTime = 0;
                    cutscene = 0;
                    lastMotionTime = 0;
                    lastSpawnTime = 0;
                    level1(platforms, platformSurface, ledders, ledderSurface, items, itemSurface);
                }
            }
        }

        for (const GameObject &platform : platforms)
        {
            DrawSurface(screen, platform.texture, platform.x, platform.y, platform.width, platform.height);
        }

        for (const GameObject &ledder : ledders)
        {
            DrawSurface(screen, ledder.texture, ledder.x, ledder.y, 24, 24);
        }

        for (GameObject &item : items)
        {
            if (item.isDestoyed)
            {
                items.erase(&item - items.begin());
                continue;
            }
            if (checkAABB(mario, item))
            {
                item.isDestoyed = true;
                int randScore = randint(1, 20) * 50;
                scoreDelta += randScore;
                char *t = new char[50];
                sprintf(t, "+%d", randScore);
                tempTexts.push_back(tempText{item.x, item.y - 10, 10, t, {0, 255, 0, 40}, worldTime + 1.3});
            }

            DrawSurface(screen, item.texture, item.x, item.y, 24, 24);
        }

        for (int i = 0; i < tempTexts.size(); i++)
        {
            if (tempTexts[i].timeEnd < worldTime)
            {

                tempTexts.erase(i);
                i--;
                continue;
            }
            DrawString(screen, tempTexts[i].x, tempTexts[i].y, tempTexts[i].size, tempTexts[i].text, charset, tempTexts[i].color);
            tempTexts[i].color.a = (tempTexts[i].timeEnd - worldTime) * 255 / 1.3;
        }

        mario.isOnGround = false;

        for (GameObject &platform : platforms)
        {
            float penetrationDepth = checkCollision(mario, platform);
            if (penetrationDepth != 0)
            {
                mario.y -= penetrationDepth;
                mario.ySpeed = 0;
                break;
            }
        }
        mario.updateData(delta);
        if (!mario.isOnGround)
            mario.state = 3;
        else if (mario.xSpeed == 0)
            mario.state = 0;
        else if (mario.xSpeed < 0)
        {
            mario.state = 2;
        }
        else
        {
            mario.state = 1;
        }

        if (mario.isOnLedder && (mario.isUp || mario.isDown))
            mario.state = 4;
        mario.updateFrame(screen, delta);

        DrawSurface(screen, policeSpawnerSurface, policeSpawner.x, policeSpawner.y, policeSpawner.height, policeSpawner.width);
        boss.updateFrame(screen, delta);
        DrawSurface(screen, princessSurface, princess.x, princess.y, princess.height, princess.width);

        for (GameObject &ledder : ledders)
        {
            if (checkAABB(mario, ledder))
            {
                mario.isOnLedder = true;
                break;
            }
            else
            {
                mario.isOnLedder = false;
            }
        }

        fpsTimer += delta;
        if (fpsTimer > 0.5)
        {
            fps = frames * 2;
            frames = 0;
            fpsTimer -= 0.5;
        };

        if (!cutscene)
        {
            DrawString(screen, 90, 10, 20, "SCORE", charset);
            sprintf(text, "%06d", score);
            DrawString(screen, 80, 40, 20, text, charset);

            sprintf(text, "HIGH SCORE");
            DrawString(screen, screen->w / 2 - strlen(text) * 20 / 2, 10, 20, text, charset, {255, 0, 0, 255}); // #FF0000
            sprintf(text, "%06d", bestScore);
            DrawString(screen, screen->w / 2 - strlen(text) * 20 / 2, 40, 20, text, charset);
            sprintf(text, "FPS: %f", fps);
            DrawString(screen, screen->w / 2 - strlen(text) * 20 / 2, 60, 10, text, charset);

            sprintf(text, "L=%d", level);
            DrawString(screen, screen->w - 160, 70, 20, text, charset, {0, 0, 255, 255}); // #0000FF
            sprintf(text, "TIME: %.1lf", worldTime);
            DrawString(screen, screen->w - 160, 50, 10, text, charset);

            sprintf(text, "onGround=%d", mario.isOnGround);
            DrawString(screen, screen->w - 160, 30, 10, text, charset);

            sprintf(text, "onLedder=%d", mario.isOnLedder);
            DrawString(screen, screen->w - 160, 40, 10, text, charset);
        }

        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    quit = 1;
                else if (event.key.keysym.sym == SDLK_n)
                {
                    resetData(mario, platforms, ledders, items, tempTexts, policemen, lastSpawnTime, spawnInterval, boss, princess);
                    level = 1;
                    score = 0;
                    scoreDelta = 0;
                    worldTime = 0;
                    cutscene = 0;
                    lastMotionTime = 0;
                    lastSpawnTime = 0;
                    level1(platforms, platformSurface, ledders, ledderSurface, items, itemSurface);
                    level = 1;
                }
                else if (event.key.keysym.sym == SDLK_1)
                {
                    resetData(mario, platforms, ledders, items, tempTexts, policemen, lastSpawnTime, spawnInterval, boss, princess);
                    level = 1;
                    score = 0;
                    scoreDelta = 0;
                    worldTime = 0;
                    cutscene = 0;
                    lastMotionTime = 0;
                    lastSpawnTime = 0;
                    level1(platforms, platformSurface, ledders, ledderSurface, items, itemSurface);
                    level = 1;
                }
                else if (event.key.keysym.sym == SDLK_2)
                {
                    resetData(mario, platforms, ledders, items, tempTexts, policemen, lastSpawnTime, spawnInterval, boss, princess);
                    level2(platforms, platformSurface, ledders, ledderSurface, items, itemSurface);
                    level = 2;
                }
                else if (event.key.keysym.sym == SDLK_3)
                {
                    resetData(mario, platforms, ledders, items, tempTexts, policemen, lastSpawnTime, spawnInterval, boss, princess);
                    level3(platforms, platformSurface, ledders, ledderSurface, items, itemSurface);
                    level = 3;
                }
                else if (event.key.keysym.sym == SDLK_LEFT && !cutscene)
                {
                    mario.xSpeed = -3;
                    mario.state = 2;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT && !cutscene)
                {
                    mario.xSpeed = 3;
                    mario.state = 1;
                }
                else if (event.key.keysym.sym == SDLK_SPACE && !cutscene)
                {
                    if (mario.isOnLedder && mario.isOnGround && !cutscene)
                        mario.isUp = true;
                    mario.jump();
                    mario.state = 3;
                }
                else if (event.key.keysym.sym == SDLK_DOWN && !cutscene)
                    if (mario.isOnLedder && mario.isOnGround && mario.y < SCREEN_HEIGHT - SAVEOFFSET - 40 - 8 * 3)
                    {
                        mario.isDown = true;
                        mario.isUp = false;
                        mario.down();
                    }
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
                {
                    mario.xSpeed = 0;
                    mario.state = 0;
                    mario.updateFrame(screen, delta);
                }
                else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
                {
                    mario.state = 0;
                }
                break;
            case SDL_QUIT:
                quit = 1;
                break;
            };
        };

        frames++;
    };

    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(marioSurface);
    SDL_FreeSurface(platformSurface);
    SDL_FreeSurface(ledderSurface);
    SDL_FreeSurface(itemSurface);
    SDL_FreeSurface(policemanSurface);
    SDL_FreeSurface(policeSpawnerSurface);

    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
};
