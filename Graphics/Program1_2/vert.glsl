#version 330

in vec2 position;
uniform mat4 ortho;

void main() {
  gl_Position = ortho * vec4(position.x, position.y, 0, 1);
}
