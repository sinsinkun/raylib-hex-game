uniform float t;
uniform vec2 screen_size;

void main() {
  vec2 coord = gl_FragCoord.xy / screen_size;

  gl_FragColor = vec4(0.7 * coord * (0.3 + 0.5 * cos(t)), 0.2 * coord.y, 1.0);
}