#version 330 core
in vec3 vNormal;

out vec4 FragColor;

uniform vec3 uColor;

void main() {
    // Simple single directional light + ambient. No PBR, no shadows.
    vec3  N       = normalize(vNormal);
    vec3  L       = normalize(vec3(0.5, 1.0, 0.3));
    float diffuse = max(dot(N, L), 0.0);
    vec3  color   = uColor * (0.25 + 0.75 * diffuse);
    FragColor     = vec4(color, 1.0);
}
