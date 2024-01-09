uniform float t;
uniform vec2 screen_size;

void main() {
  vec2 coord = gl_FragCoord.xy / screen_size;

  gl_FragColor = vec4(coord * 0.5 * abs(sin(t)), 0.1, 1.0);
}