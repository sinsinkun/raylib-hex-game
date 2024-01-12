uniform float t;
uniform vec2 mouse_pos;
uniform vec2 screen_size;

void main() {
  // normalize pos
  float d = distance(gl_FragCoord.xy, vec2(mouse_pos.x, screen_size.y - mouse_pos.y)) / 25.0;
  gl_FragColor = vec4(0.2, 0.5, 0.7, 1.0 - d);
}