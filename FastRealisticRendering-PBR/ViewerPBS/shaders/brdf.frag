#version 430

smooth in vec3 eye_normal;
smooth in vec3 eye_vertex;

uniform samplerCube specular_map;
uniform samplerCube diffuse_map;
uniform vec3 fresnel;
uniform float roughness;
uniform float metalness = 0.9f;

flat in vec3 eye_light_pos;
in vec3 cameraPos;

vec3 color = vec3(1.0);

out vec4 frag_color;

const float PI = 3.14159265359;

vec3 XAxis = vec3(1.0, 0.0, 0.0);
vec3 YAxis = vec3(0.0, 1.0, 0.0);
vec3 ZAxis = vec3(0.0, 0.0, 1.0);

float GeometrySchlickGGX(float NdotV, float rough)
{
    float r = (rough + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float rough)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, rough);
    float ggx1 = GeometrySchlickGGX(NdotL, rough);

    return ggx1 * ggx2;
}

float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;

    return nom / denom;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float RadicalInverse_VdC(uint bits)
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
        return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec2 SphereMap(vec3 N)
{
  return vec2(0.5 - atan(N.y, N.x) / (PI*2), acos(N.z) / PI);
}

vec4 textureSphereLod(samplerCube tex, vec3 N, float lod)
{
  //vec4 res = textureLod(tex, SphereMap(N), lod);
    vec4 res = textureLod(tex, N, lod);
  return res;
}

// Random rotation based on the current fragment's coordinates
float randAngle()
{
  uint x = uint(gl_FragCoord.x);
  uint y = uint(gl_FragCoord.y);
  return float(30u * x ^ y + 10u * x * y);
}

// Example function, skewing the sample point and rotating
// Note: E is two values returned from Hammersley function above,
//       from within the same loop.
vec2 DGgxSkew(vec2 E)
{
  float a = roughness * roughness;
  E.x = atan(sqrt((a * E.x) / (1.0 - E.x)));
  E.y = PI*2 * E.y + randAngle();
  return E;
}

// Example function, turn a skewed sample into a 3D vector
// This results in a vector that is looking somewhere in
// the +Z Hemisphere.
vec3 MakeSample(vec2 E)
{
  float SineTheta = sin(E.x);

  float x = cos(E.y) * SineTheta;
  float y = sin(E.y) * SineTheta;
  float z = cos(E.x);

  return vec3(x, y, z);
}

// Computes the exact mip-map to reference for the specular contribution.
// Accessing the proper mip-map allows us to approximate the integral for this
// angle of incidence on the current object.
float compute_lod(uint NumSamples, vec3 N, vec3 H)
{
  vec2 Dimensions = vec2(512.0f,512.0f);
  float dist = DistributionGGX(N, H, roughness);
  float res = 0.5f * (log2(float(Dimensions.x * Dimensions.y) / NumSamples) - log2(dist));
  return res;
}

// Calculates the specular influence for a surface at the current fragment
// location. This is an approximation of the lighting integral itself.
vec3 radiance(vec3 N, vec3 V)
{
  // Precalculate rotation for +Z Hemisphere to microfacet normal.
  vec3 UpVector = abs(N.z) < 0.999 ? ZAxis : XAxis;
  vec3 TangentX = normalize(cross( UpVector, N ));
  vec3 TangentY = cross(N, TangentX);

  float NoV = abs(dot(N, V));

  // Approximate the integral for lighting contribution.
  vec3 fColor = vec3(0.0);
  const uint NumSamples = 256;
  for (uint i = 0; i < NumSamples; ++i)
  {
    vec2 Xi = Hammersley(i, NumSamples);
    vec3 Li = MakeSample(DGgxSkew(Xi));
    vec3 H  = normalize(Li.x * TangentX + Li.y * TangentY + Li.z * N);
    vec3 L  = normalize(-reflect(V, H));

    // Calculate dot products for BRDF
    float NoL = abs(dot(N, L));
    float NoH = abs(dot(N, H));
    float VoH = abs(dot(V, H));
    float lod = compute_lod(NumSamples, N, H);

    vec3 F_ = fresnelSchlickRoughness(max(dot(N, V), 0.0), fresnel, roughness);
    float G_ = GeometrySmith(N, V, L, roughness);
    vec3 LColor = textureLod(specular_map, L, lod).rgb;

    fColor += F_ * G_ * LColor * VoH / (NoH * NoV);
  }

  // Average the results
  return fColor / float(NumSamples);
}

void main (void) {
 // Put the light in the infitine minus z.
 vec3 Light = vec3(0.7, 0.4, 1);
 vec3 lightColor = vec3(1, 1, 1);

 vec3 N = normalize(eye_normal);
 vec3 V = normalize(cameraPos - eye_vertex);
 vec3 R = reflect(-V, N);

 // reflectance equation
 vec3 Lo = vec3(0.0);
 for(int i = 0; i < 1; ++i)
 {
         // calculate per-light radiance
         vec3 L = normalize(Light - eye_vertex);
         vec3 H = normalize(V + L);
         float distance = length(Light - eye_vertex);
         float attenuation = 1.0 / (distance * distance);
         vec3 radiance = lightColor * attenuation;

         // BRDF
         float NDF = DistributionGGX(N, H, roughness);
         float G   = GeometrySmith(N, V, L, roughness);
         vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), fresnel);

         vec3 nominator    = NDF * G * F;
         //vec3 nominator = NDF * F;
         float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
         //float denominator = 1.0f;
         vec3 specular = nominator / denominator;

         vec3 kS = F;
         vec3 kD = vec3(1.0) - kS;

         kD *= 1.0 - metalness;

         // scale light by NdotL
         float NdotL = max(dot(N, L), 0.0);

         // add to outgoing radiance Lo
         Lo += (kD * color / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
     }

 vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), fresnel, roughness);
 vec3 kD = 1.0 - kS;
 kD *= 1.0 - metalness;

 vec3 irradiance = texture(diffuse_map, N).rgb;
 vec3 diffuse = irradiance * color;
 vec3 specular  = radiance(N, V);
 vec3 ambient = (kD * diffuse + specular) * 1.0f;

 vec3 color = ambient + Lo;

 // HDR tonemapping
  color = color / (color + vec3(1.0));
 // gamma correct
 color = pow(color, vec3(1.0/2.2));

 frag_color = vec4(color, 1.0);

}
