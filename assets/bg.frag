uniform float t;
uniform vec2 screen_size;

const float PI = 3.14159265;

vec3 gradient(vec2 coord) {
  float r = 0.7 * coord.x * (0.3 + 0.5 * cos(t));
  float g = 0.7 * coord.y * (0.3 + 0.5 * cos(t));
  float b = coord.y * (0.5 + 0.3 * sin(t));
  return vec3(r,g,b);
}

void main() {
  vec2 coord = gl_FragCoord.xy / screen_size;
  vec3 final_rgb = gradient(coord);
  gl_FragColor = vec4(final_rgb, 1.0);
}