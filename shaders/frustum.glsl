#if defined(VERTEX_SHADER)

layout(location=0) in vec3 v_position;

uniform mat4 MVP;

void
main()
{
    gl_Position = MVP * vec4(v_position, 1.0);
}

#else

out vec4 color;

void
main()
{
    color = vec4(1.0, 1.0, 1.0, 0.5);
}

#endif

