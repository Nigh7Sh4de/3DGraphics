#version 330 //GLSL version, fit with OpenGL version

uniform mat4 projMat, viewMat;
uniform samplerCube background;
uniform sampler2D scratchTex;

uniform float max_level;
uniform float time;

in vec2 pixelCoords;

// resulting color of the fragment shader
out vec4 outColor;

struct Ray {
    vec3 pos, dir;
};

struct Material {
    vec3 color;
    float roughness;
    float reflection;
    float refraction;
    float ior;  // indice of refraction
};

struct Hit {
    float dist;
    vec3 normal;
    Material mat;
};

const float far_dist = 1000.0;
const float M_PI = 3.1415926535897932;
const float epsilon = 0.001;

Material plane_mat;
Material water_mat;

void initScene()
{
    // plane material
    plane_mat.color = vec3(0.1);
    plane_mat.refraction = 0.0;
    //plane_mat.reflection = 1.0;
    //plane_mat.roughness = 0.0;
    plane_mat.reflection = 0.0;
    plane_mat.roughness = 0.9;
    plane_mat.ior = 1.3;

    // water matrial
    water_mat.color = vec3(0.1);
    water_mat.reflection = 0.2;
    water_mat.refraction = 0.0;
    water_mat.roughness = 0.2;
    water_mat.ior = 1.3;
}

float hash(vec2 p);
vec3 perlinNoiseD( in vec2 x );
vec3 perlinMultifractal(vec2 p);

void texturePlane(in vec2 uv, inout Hit hit)
{
    Material mat = plane_mat;

    vec3 n = 0.1*perlinMultifractal(uv);

    float water = step(n.x, -0.01);

    n*=mix(0.5, 0.05, water);

    mat.color = mix(mat.color, water_mat.color, water);
    mat.reflection = mix(mat.reflection, water_mat.reflection, water);
    mat.refraction = mix(mat.refraction, water_mat.refraction, water);
    mat.roughness = mix(mat.roughness, water_mat.roughness, water);
    mat.ior = mix(mat.ior, water_mat.ior, water);

    hit.normal = normalize(vec3(-n.y, 1.0, -n.z));

    float scratch = texture(scratchTex, uv);

    mat.roughness = 1.0 - (1.0 - mat.roughness) * scratch;
    mat.reflection *= scratch;
    mat.refraction *= scratch;

    hit.mat    = mat;
}

float hash(vec2 p) {
    float h = dot(p,vec2(127.1,311.7));
    return -1.0+2.0*fract(sin(h)*43758.5453123);
}
vec3 perlinNoiseD( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);
    vec2 u = f*f*(3.0-2.0*f);
    float a = hash(p+vec2(0.5,0.5));
    float b = hash(p+vec2(1.5,0.5));
    float c = hash(p+vec2(0.5,1.5));
    float d = hash(p+vec2(1.5,1.5));
    return vec3(a+(b-a)*u.x+(c-a)*u.y+(a-b-c+d)*u.x*u.y,
                6.0*f*(1.0-f)*(vec2(b-a,c-a)+(a-b-c+d)*u.yx));
}
vec3 perlinMultifractal(vec2 p)
{
    //p = p*3.0+vec2(10.0,-1.0);

    float alpha = 0.5;
    float beta = 2.0;

    vec3 r = vec3(0.0);
    vec3 a = vec3(1.0);

    for( int i=0; i<7; i++ )
    {
        vec3 n = perlinNoiseD(p);
        r+=a*n;
        a *= vec3(1.0, beta, beta) * alpha;
        p = p*beta;
    }
    return 0.5*r;
}

// ray horizontal plane with checkerboard texture
Hit rayCheckerboardPlane(in Ray r, in vec3 center, in vec2 size)
{
    Hit h;
    // avoid divide by 0
    float cp = r.dir.y + step(abs(r.dir.y), epsilon);
    // return far dist if original ray y was 0
    h.dist = mix(far_dist, -(r.pos - center).y / r.dir.y, step(epsilon, abs(r.dir.y)));
    // avoid back ray cast
    h.dist = mix(far_dist, h.dist, step(0.0, h.dist));

    // hit position
    vec3 p = r.pos + h.dist*r.dir;
    // clamp plane
    h.dist = mix(h.dist, far_dist, step(0.5, max(abs(p.x-center.x)/size.x,abs(p.z-center.z)/size.y)));

    texturePlane(p.xz, h);

    h.normal.y *= -1.0+2.0*step(0.0, r.pos.y - center.y);

    return h;
}

Hit rayScene(Ray r)
{
    return rayCheckerboardPlane(r, vec3(0.0), vec2(10.0));
}

// fresnel function
float fresnel(vec3 r, vec3 n,  float eta)
{
    float F0 = (1.0-eta)/(1.0+eta); F0 *= F0;
    float t = 1.0 + dot(n, r);
    float t2 = t*t;
    return mix(t2*t2*t, 1.0, F0);
}

// helper to fetch the cubemap with gloss
vec3 ray_far(vec3 dir, float gloss)
{
    vec3 adjust = mix(vec3(1.0), 4.0/textureLod(background, dir, max_level).rgb, gloss);
    return  textureLod(background, dir, gloss * max_level).rgb*adjust;
}

// single light path with multiple consicutive rays
// last argument should be 1 if the intial ray is inside the object, 0 else
vec3 rayLoop(Ray ray, vec3 light_dir, float light_intensity, vec3 ambient_color, float gloss, float in_obj)
{
    vec3 base_color = vec3(0.0);
    vec3 far_factor = vec3(1.0);
    float refl_gloss = gloss;

    // for(int i = 0; i<10; ++i)
    // {
    //     Hit hit = rayScene(ray);

    //     if(hit.dist > far_dist -0.01)
    //         break;

    //     // worst glossiness wins
    //     refl_gloss = 1.0 - (1.0-refl_gloss) * (1.0 - hit.mat.roughness);

    //     // hit point
    //     vec3 hp = ray.pos + hit.dist*ray.dir;

    //     // ior is inverted if we change of medium
    //     float ior = mix(hit.mat.ior, 1.0/hit.mat.ior, in_obj);

    //     // normal is inverted if ray is in the object
    //     hit.normal = mix(hit.normal, -hit.normal, in_obj);

    //     vec3 diffuse_color = ambient_color + vec3(light_intensity * max(0.0, dot(hit.normal, light_dir)));

    //     // reflection
    //     float F = fresnel(ray.dir, (mix(hit.normal, -ray.dir, hit.mat.roughness)), 1.0/ior);
    //     float refl = mix(hit.mat.reflection, 1.0, F);

    //     // chose dominant path between reflection and refraction
    //     float choose_refl = step(0.5*hit.mat.refraction, refl);

    //     // added color
    //     base_color += far_factor*hit.mat.color * (1.0 - hit.mat.refraction) * diffuse_color *(1.0 -refl)*(1.0-in_obj);
    //     //factored color
    //     far_factor *= mix(mix(hit.mat.color * hit.mat.refraction, vec3(1.0), in_obj)* (1.0 -refl), vec3(refl), choose_refl);

    //     // future ray choices
    //     ray.dir = mix(refract(ray.dir, hit.normal, 1.0/ior), reflect(ray.dir, hit.normal), choose_refl);
    //     ray.pos = hp + ray.dir * epsilon;

    //     // update in_obj and inv_ior if refraction
    //     in_obj = mix(1.0-in_obj, in_obj, choose_refl);
    // }

    return base_color + far_factor * ray_far(ray.dir, refl_gloss);
}


void main()
{
    //init materials
    initScene();

    // create initial ray
    mat4 invProj = inverse(projMat);
    mat4 invView = inverse(viewMat);

    Ray start_ray;
    start_ray.pos = (invView * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    vec4 raydirw = invView * invProj * vec4(pixelCoords, 1.0f, 1.0f);
    start_ray.dir = normalize(raydirw.xyz/raydirw.w-start_ray.pos);

    // lights
    vec3 ambient_color = vec3(1.0);
    vec3 light_dir = normalize(vec3(0.1,1.5,1.0));
    float light_intensity = 5.0;

    // ray geometry
    Hit closest = rayScene(start_ray);
    vec3 hitpos = start_ray.pos + closest.dist*start_ray.dir;

    // diffuse color
    vec3 diffuse_color = ambient_color + vec3(light_intensity * max(0.0, dot(closest.normal, light_dir)));

    // result
    vec3 color = closest.mat.color * diffuse_color;

    // reflection
    Ray reflect_ray;
    reflect_ray.dir = reflect(start_ray.dir, closest.normal);
    reflect_ray.pos = hitpos + reflect_ray.dir*epsilon;

    vec3 reflect_color = rayLoop(reflect_ray, light_dir, light_intensity, ambient_color, closest.mat.roughness, 0.0);
    vec3 reflect_color_env = ray_far(reflect_ray.dir, closest.mat.roughness);
    reflect_color =mix(reflect_color, reflect_color_env, closest.mat.roughness);

    float reflected = mix(closest.mat.reflection, 1.0, fresnel(start_ray.dir, (mix(closest.normal, -start_ray.dir, closest.mat.roughness)), 1.0/closest.mat.ior));

    // refraction
    Ray refract_ray;
    refract_ray.dir = refract(start_ray.dir, closest.normal, 1.0/closest.mat.ior);
    refract_ray.pos = hitpos + refract_ray.dir*epsilon;

    vec3 refract_color = closest.mat.color * rayLoop(refract_ray, light_dir, light_intensity, ambient_color, closest.mat.roughness, 1.0);

    color = mix(color, refract_color, closest.mat.refraction);
    color = mix(color, reflect_color, reflected);
    color = mix(color,
                ray_far(start_ray.dir, 0.0), // far color
                step(far_dist-0.1, closest.dist));

    // gama correction
    outColor = vec4(pow(color, vec3(1.0/2.2)), 1.0);
}
