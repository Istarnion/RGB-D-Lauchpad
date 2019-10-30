#include "sensor_serialization.h"

#include <stdlib.h>
#include <stdio.h>

#define PERSIST_FILE "sensors.ser"

typedef enum
{
    PERSIST_FIELD_END,
    PERSIST_FIELD_URI,
    PERSIST_FIELD_FRUSTUM_POS,
    PERSIST_FIELD_FRUSTUM_ROT,
    PERSIST_FIELD_FRUSTUM_PLANES
} PersistFieldHeader;

void
SaveSensor(const char *uri, Frustum *frustum)
{
    FILE *f = NULL;

    // We want to overwrite the file each run, but still be able to
    // append sensors to it when running.
    // An alternative to this would be to take an array of SerializedSensor's
    // instead of the data of one.
    static bool first_sensor = true;
    if(first_sensor)
    {
        f = fopen(PERSIST_FILE, "w");
        first_sensor = false;
    }
    else
    {
        f = fopen(PERSIST_FILE, "a");
    }

    if(f)
    {
        fprintf(f, "%d %s\n%d %f %f %f\n%d %f %f %f\n%d %f %f\n%d\n",
                PERSIST_FIELD_URI, uri,
                PERSIST_FIELD_FRUSTUM_POS, frustum->position.x, frustum->position.y, frustum->position.z,
                PERSIST_FIELD_FRUSTUM_ROT, frustum->pitch, frustum->yaw, frustum->roll,
                PERSIST_FIELD_FRUSTUM_PLANES, frustum->near_plane, frustum->far_plane,
                PERSIST_FIELD_END);

        fclose(f);
    }
    else
    {
        fprintf(stderr, "Failed to open %s for serializing sensor data\n", PERSIST_FILE);
    }
}

int
LoadSensors(SerializedSensor *sensors, int max_sensors)
{
    int sensors_read = -1;

    FILE *f = fopen(PERSIST_FILE, "r");
    if(f)
    {
        int eof = false;
        sensors_read = 0;

        fseek(f, 0, SEEK_END);
        long int file_length = ftell(f);
        rewind(f);

        if(file_length == 0)
        {
            eof = true;
        }

        for(int i=0; i<max_sensors && !eof; ++i)
        {
            SerializedSensor *sensor = &sensors[sensors_read];
            bool reading = true;

            while(reading)
            {
                char c;
                fread(&c, 1, 1, f);
                eof = feof(f);
                if(!eof)
                {
                    c -= '0';
                    switch((PersistFieldHeader)c)
                    {
                        case PERSIST_FIELD_URI:
                        {
                            sensors_read++;
                            sensor->URI = (char *)malloc(128);
                            fscanf(f, " %127s\n", sensor->URI);
                            sensor->URI[127] = '\0';
                            break;
                        }
                        case PERSIST_FIELD_FRUSTUM_POS:
                        {
                            fscanf(f, " %f %f %f\n",
                                   &sensor->frustum.position.x,
                                   &sensor->frustum.position.y,
                                   &sensor->frustum.position.z);
                            break;
                        }
                        case PERSIST_FIELD_FRUSTUM_ROT:
                        {
                            fscanf(f, " %f %f %f\n",
                                   &sensor->frustum.pitch,
                                   &sensor->frustum.yaw,
                                   &sensor->frustum.roll);
                            break;
                        }
                        case PERSIST_FIELD_FRUSTUM_PLANES:
                        {
                            fscanf(f, " %f %f\n",
                                   &sensor->frustum.near_plane,
                                   &sensor->frustum.far_plane);
                            break;
                        }
                        case PERSIST_FIELD_END:
                        {
                            reading = false;
                        }
                        default: break;
                    }
                }
                else
                {
                    reading = false;
                }
            }
        }
    }

    return sensors_read;
}

