#version 430 core

in vec2 tex_coords;

out vec4 frag_col;

uniform struct Material {
  sampler2D diff_map;
} material;

void main() {
  //frag_col = vec4(1.0, 1.0, 1.0, 1.0);
  frag_col = texture(material.diff_map, tex_coords);
}
