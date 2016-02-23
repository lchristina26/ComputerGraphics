#version 330

uniform mat4 projection;
uniform mat4 View;

in vec3 position;

void main() {
  gl_Position = projection * View * vec4(position, 1);
}
