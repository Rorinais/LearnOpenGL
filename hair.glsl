#version 330 core

in vec3 Texcoord0;
in vec3 Normal0;
in vec3 WorldPos0;
out vec4 FragColor;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
    vec3 Direction;
};

uniform DirectionalLight gDirectionalLight;
uniform sampler2D gSampler0;
uniform sampler2D gSampler1;

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gMatSpecularPower;

void main()
{
    vec4 AmbientColor = vec4(gDirectionalLight.Color * gDirectionalLight.AmbientIntensity, 1.0);
    vec3 LightDirection = -gDirectionalLight.Direction;
    vec3 Normal = normalize(Normal0);
    float DiffuseFactor = dot(Normal, LightDirection);
    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);
    float SpecularFactor = 0.0;

    if (DiffuseFactor > 0)
    {
        DiffuseColor = vec4(gDirectionalLight.Color * gDirectionalLight.DiffuseIntensity * DiffuseFactor, 1.0);
        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
        vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
        SpecularFactor = dot(VertexToEye, LightReflect);
    }

    if (SpecularFactor > 0)
    {
        SpecularFactor = pow(SpecularFactor, gMatSpecularPower);
        SpecularColor = vec4(gDirectionalLight.Color * gMatSpecularIntensity * SpecularFactor, 1.0);
    }
    vec4 texture0=texture2D(gSampler0, Texcoord0.xy);
    vec4 texture1=texture2D(gSampler1, Texcoord0.xy);
    // 获取红色通道的值
    float red = texture1.r;

    // 获取绿色通道的值
    float green = texture1.g;

    // 获取蓝色通道的值
    float blue = texture1.b;

        // 将红色通道转换为灰度值
    float gray_red = dot(vec3(red), vec3(0.299, 0.587, 0.114));

    // 将绿色通道转换为灰度值
    float gray_green = dot(vec3(green), vec3(0.299, 0.587, 0.114));

    // 将蓝色通道转换为灰度值
    float gray_blue = dot(vec3(blue), vec3(0.299, 0.587, 0.114));

    //FragColor = texture0 * (AmbientColor + DiffuseColor + SpecularColor)+texture1;
     FragColor = vec4(gray_green, gray_green, gray_green, 1.0);
    //FragColor = texture2D(gSampler0, Texcoord0.xy);
    FragColor.a = 1.0;
}
