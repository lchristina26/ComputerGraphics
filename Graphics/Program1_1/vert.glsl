#version 330

uniform mat4 projection;
uniform mat4 View;
uniform mat4 Model;

in vec3 position;
in vec3 color;
out vec3 fcolor;

void main() {
  gl_Position = projection * View * Model * vec4(position, 1);
  fcolor = color;
}
