#ifndef MAGIC_MOTION_H_
#define MAGIC_MOTION_H_

#include "magic_math.h"
#include "frustum.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// NOTE(istarnion): We use decimeters (dm) as the base unit in this system.
// 1 unit = 1 dm
// 1 m    = 10 dm
// 1 inch = 0.254 dm
// 1 foot = 3.048 dm

// This is quite arbitrary, but kept low to save memory and startup time
#define MAX_SENSORS 4

// A voxel is a cube of size VOXEL_SIZE in the voxel grid
#define VOXEL_SIZE 0.5f

// Dimensions for the voxel space
#define NUM_VOXELS_X 100
#define NUM_VOXELS_Y 100
#define NUM_VOXELS_Z 100

#define BOUNDING_BOX_X (NUM_VOXELS_X * VOXEL_SIZE)
#define BOUNDING_BOX_Y (NUM_VOXELS_Y * VOXEL_SIZE)
#define BOUNDING_BOX_Z (NUM_VOXELS_Z * VOXEL_SIZE)

#define NUM_VOXELS (NUM_VOXELS_X*NUM_VOXELS_Y*NUM_VOXELS_Z)

typedef enum
{
    TAG_CAMERA_0,
    TAG_CAMERA_1,
    TAG_CAMERA_2,
    TAG_CAMERA_3
} MagicMotionTag;

typedef union
{
    uint32_t color;
    struct
    {
        uint8_t _padding, r, g, b;
    };
} Color;

typedef struct
{
    uint32_t point_count; // How many points are in this voxel
    Color color; // The average color of the points in this voxel
} Voxel;

void MagicMotion_Initialize(void);
void MagicMotion_Finalize(void);

unsigned int MagicMotion_GetNumCameras(void);
const char *MagicMotion_GetCameraName(unsigned int camera_index);
const char *MagicMotion_GetCameraURI(unsigned int camera_index);
const char *MagicMotion_GetCameraSerialNumber(unsigned int camera_index);
const Frustum *MagicMotion_GetCameraFrustums(void);
Mat4 MagicMotion_GetCameraTransform(unsigned int camera_index);
void MagicMotion_SetCameraTransform(unsigned int camera_index, Mat4 transform);

void MagicMotion_CaptureFrame(void);

void MagicMotion_GetColorImageResolution(unsigned int camera_index, int *width, int *height);
void MagicMotion_GetDepthImageResolution(unsigned int camera_index, int *width, int *height);

const Color *MagicMotion_GetColorImage(unsigned int camera_index);
const float *MagicMotion_GetDepthImage(unsigned int camera_index);

unsigned int MagicMotion_GetCloudSize(void);
V3 *MagicMotion_GetPositions(void);
Color *MagicMotion_GetColors(void);
MagicMotionTag *MagicMotion_GetTags(void);

Voxel *MagicMotion_GetVoxels(void); // Return the full voxel grid as an array of length NUM_VOXELS

#ifdef __cplusplus
}
#endif
#endif /* end of include guard: MAGIC_MOTION_H_ */

