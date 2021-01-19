#version 330 core

uniform vec4 	  arrowColor;
out     vec4      pixelColor;

void main() {
    pixelColor = arrowColor;
    pixelColor = vec4 (1.f,0.f,0.f,1.f); 
}

