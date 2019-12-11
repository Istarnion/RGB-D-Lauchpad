#ifndef UTILS_H_
#define UTILS_H_

// This is quite arbitrary, but kept low to save memory and startup time
#define MAX_SENSORS 4

static void
UpdateProjectionMatrix()
{
    int w, h;
    RendererGetSize(&w, &h);
    float aspect = (float)w / (float)h;

    RendererSetProjectionMatrix(PerspectiveMat4(aspect, 45.0f, 10.0f, 100000.0f));
}

static void
FPSCamera(InputState *input, Camera *cam)
{
    V3 camera_movement = MakeV3(0, 0, 0);
    camera_movement.x = (!!(bool)input->right) - (!!(bool)input->left);
    camera_movement.y = (!!(bool)input->up) - (!!(bool)input->down);
    camera_movement.z = (!!(bool)input->forward) - (!!(bool)input->back);

    if(MagnitudeSquaredV3(camera_movement) > 0)
    {
        camera_movement = ScaleV3(NormalizeV3(camera_movement), 100);
        MoveCamera(cam, camera_movement);
    }

    if(input->left_mouse_button)
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        RotateCamera(cam, -Input()->mouse_delta.x, Input()->mouse_delta.y);
    }
    else
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}

static float
Tween(float t)
{
    float result = 3*t*t - 2*t*t*t;
    return result;
}

static float
Lerp(float a, float b, float t)
{
    return a*(1.0f-t) + b*t;
}

#endif /* end of include guard: UTILS_H_ */

