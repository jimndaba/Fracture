#version 450 core

layout (location=0) out vec2 uv;

// extents of grid in world coordinates
float gridSize = 1000.0;

// size of one cell
float gridCellSize = 0.5;

// minimum number of pixels between cell lines before LOD switch should occur. 
const float gridMinPixelsBetweenCells = 5.0;

const vec3 pos[4] = vec3[4](
	vec3(-1.0, 0.0, -1.0),
	vec3( 1.0, 0.0, -1.0),
	vec3( 1.0, 0.0,  1.0),
	vec3(-1.0, 0.0,  1.0)
);

const int indices[6] = int[6](
	0, 1, 2, 2, 3, 0
);

struct FrameData
{
    mat4 Projection;
    mat4 View;
    vec4 Position;
};

layout(std140 , binding = 0) uniform globalData
{
   FrameData frame;
};


void main()
{
	mat4 MVP = frame.Projection * frame.View;

	int idx = indices[gl_VertexID];
	vec3 position = pos[idx] * gridSize;

	gl_Position = MVP * vec4(position, 1.0);
	uv = position.xz;
}