var raytraceFS = `
struct Ray {
	vec3 pos;
	vec3 dir;
};

struct Material {
	vec3  k_d;	// diffuse coefficient
	vec3  k_s;	// specular coefficient
	float n;	// specular exponent
};

struct Sphere {
	vec3     center;
	float    radius;
	Material mtl;
};

struct Light {
	vec3 position;
	vec3 intensity;
};

struct HitInfo {
	float    t;
	vec3     position;
	vec3     normal;
	Material mtl;
};

uniform Sphere spheres[ NUM_SPHERES ];
uniform Light  lights [ NUM_LIGHTS  ];
uniform samplerCube envMap;
uniform int bounceLimit;

bool IntersectRay( inout HitInfo hit, Ray ray );

// Shades the given point and returns the computed color.
vec3 Shade( Material mtl, vec3 position, vec3 normal, vec3 view )
{
	vec3 color = vec3(0,0,0);
	for ( int i=0; i<NUM_LIGHTS; ++i ) {
		// Check for shadows

		HitInfo hit;
		Ray r;

		r.pos = position;
		r.dir = normalize(lights[i].position - position); 

		// If not shadowed, perform shading using the Blinn model
		bool isShadowed =  dot(normal, r.dir)<0.0 || IntersectRay(hit, r);
		if(!isShadowed)
		{
			normal = normalize(normal);
			view = normalize(view);

			vec3 h = normalize(r.dir + view);
			float cosphi = dot(h,normal);
			float costheta = dot(r.dir,normal);

			color += lights[i].intensity * (costheta * mtl.k_d + mtl.k_s * pow(cosphi, mtl.n));
			// color += lights[i].intensity * costheta * (mtl.k_d + mtl.k_s * (pow(cosphi, mtl.n)/costheta));
		}

	}
	return color;
}

// Intersects the given ray with all spheres in the scene
// and updates the given HitInfo using the information of the sphere
// that first intersects with the ray.
// Returns true if an intersection is found.
bool IntersectRay( inout HitInfo hit, Ray ray )
{
	hit.t = 1e30;
	bool foundHit = false;
	for ( int i=0; i<NUM_SPHERES; ++i ) {
		// Test for ray-sphere intersection
		
		float a = dot(ray.dir, ray.dir);
		vec3 pminc = ray.pos - spheres[i].center;
		float b = dot(2.0 * ray.dir, pminc);
		float c = dot(pminc, pminc);
		c = c - spheres[i].radius * spheres[i].radius;
		float deltavalue = b*b - (4.0*a*c);

		float t = (-b - sqrt(deltavalue)) / (2.0 * a);

		// If intersection is found, update the given HitInfo
		if(t >= 1e-4 && hit.t > t)
		{
			hit.t = t;
			hit.mtl.k_d = spheres[i].mtl.k_d;
			hit.mtl.k_s = spheres[i].mtl.k_s;
			hit.mtl.n = spheres[i].mtl.n;
			hit.position = ray.pos + t*ray.dir;
			hit.normal = normalize( hit.position - spheres[i].center );
			foundHit = true;
		}
	}
	return foundHit;
}

// Given a ray, returns the shaded color where the ray intersects a sphere.
// If the ray does not hit a sphere, returns the environment color.
vec4 RayTracer( Ray ray )
{
	HitInfo hit;
	if ( IntersectRay( hit, ray ) ) {
		vec3 view = normalize( -ray.dir );
		vec3 clr = Shade( hit.mtl, hit.position, hit.normal, view );
		
		// Compute reflections
		vec3 k_s = hit.mtl.k_s;
		for ( int bounce=0; bounce<MAX_BOUNCES; ++bounce ) {
			if ( bounce >= bounceLimit ) break;
			if ( hit.mtl.k_s.r + hit.mtl.k_s.g + hit.mtl.k_s.b <= 0.0 ) break;
			
			Ray r;	// this is the reflection ray
			HitInfo h;	// reflection hit info
			
			// TO-DO: Initialize the reflection ray
			r.pos = hit.position;
			r.dir = 2.0 * dot(view, hit.normal) * hit.normal - view;
			
			if ( IntersectRay( h, r ) ) {
				// TO-DO: Hit found, so shade the hit point
				view = normalize( -r.dir );

				hit.mtl.k_d = h.mtl.k_d;
				hit.mtl.k_s = k_s;
				hit.mtl.n *= h.mtl.n;

				hit.position = h.position;
				hit.normal = h.normal;
				hit.t = h.t;
				
				clr += k_s * Shade( hit.mtl, hit.position, hit.normal, view );

				// TO-DO: Update the loop variables for tracing the next reflection ray
				k_s *= h.mtl.k_s;
				
				continue;
			} else {
				// The refleciton ray did not intersect with anything,
				// so we are using the environment color
				clr += k_s * textureCube( envMap, r.dir.xzy ).rgb;
				break;	// no more reflections
			}
		}
		return vec4( clr, 1 );	// return the accumulated color, including the reflections
	} else {
		return vec4( textureCube( envMap, ray.dir.xzy ).rgb, 1 );	// return the environment color
	}
}
`;