#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

uniform float u_screenWidth;
uniform float u_screenHeight;

uniform float u_cameraX;
uniform float u_cameraY;

out vec2 vTexCoord;

void main() {

    float x = mix(-1.0, 1.0, (position.x - u_cameraX) / u_screenWidth);
    float y = mix(-1.0, 1.0, (position.y - u_cameraY) / u_screenHeight);
    gl_Position = vec4(x, y, 0.0, 1.0);
    
    vTexCoord = texCoord;
}