#ifndef TYPES_H
#define TYPES_H

enum error
{
    NO,
    COORDS_OUTSIDE_CANVAS,
    ZERO_DIVIDE
};


typedef struct size_int_st
{
    int width;
    int height;

public:
    size_int_st() {}
    size_int_st(int width, int height) : size_int_st()
    {
        this->height = height;
        this->width = width;
    }

} size_int;

typedef struct
{
    double x;
    double y;
    double z;
}point_t;

typedef struct
{
    double x, y, z;
} vector_t;

typedef struct tilt_st
{
    tilt_st() {}
    tilt_st(double pitch, double roll, double yaw)
    {
        this->pitch = pitch;
        this->roll = roll;
        this->yaw = yaw;
    }
    double pitch, roll, yaw; //тангаж, крен, рыскание
} tilt_t; //наклон

//Область видимости камеры
typedef struct fov_st
{
    fov_st() {}
    fov_st(double d, double height, double width)
    {
        this->d = d;
        this->height = height;
        this->width = width;
    }
    double d; //расстояние от камеры до окна просмотра
    double height;
    double width;
} fov_t;

typedef struct rgb_st
{
public:
    rgb_st() {}
    rgb_st(int r, int g, int b)
    {       
        cut_number(&r);
        cut_number(&g);
        cut_number(&b);

        this->r = r;
        this->g = g;
        this->b = b;
    }

    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;

private:
    void cut_number(int *n)
    {
        if (*n < 0)
            *n = 0;
        else if (*n > 255)
            *n = 255;
    }

}rgb_t;

typedef struct borders_st
{
    int left;
    int right;
    int top;
    int bottom;

public:
    borders_st() {}
    borders_st(int left, int right, int top, int bottom) : borders_st()
    {
        this->left = left;
        this->right = right;
        this->top = top;
        this->bottom = bottom;
    }

} borders;

#endif // TYPES_H
