#include "Camera.h"
#include <cmath>

// creates a vector in world space from a pixel in NDC space
Vector3 Camera::ndcToRay(const float x, const float y) const
{
    // convert ndc space to screen space (-1 = bottom/left, 1 = top/right)
    float screenX = x * 2 - 1;
    float screenY = 1 - y * 2;

    // get world space ray
    Vector3 rightComp = right * screenX * settings.fov;
    Vector3 upComp = up * screenY * settings.fov;
    Vector3 result = forward + rightComp + upComp;
    result.normalize();

    return result;
}

// marches ray. x and y params are in NDC space (0 = top/left, 1 = bottom/right)
sf::Color Camera::marchRay(const float x, const float y) const
{
    Vector3 rayDir = ndcToRay(x, y); // find ray direction
	Vector3 rayPos = pos; // origin of ray is camera position

    float totalRayDist = 0;
    int steps = 0;
    while (totalRayDist <= settings.renderDist)
    {
		float dist = distFunc(rayPos); // find value of distance function at ray position
        
        if (dist < settings.minDist || steps > settings.maxSteps) // if distance is small, ray hit
        {
            if (calculateLighting)
            {
                // get normal vector by estimating gradient of distance function by sampling nearby points
                const float EPSILON = 0.00001;
                float gradX = distFunc(rayPos + Vector3(EPSILON, 0, 0)) - distFunc(rayPos - Vector3(EPSILON, 0, 0));
                float gradY = distFunc(rayPos + Vector3(0, EPSILON, 0)) - distFunc(rayPos - Vector3(0, EPSILON, 0));
                float gradZ = distFunc(rayPos + Vector3(0, 0, EPSILON)) - distFunc(rayPos - Vector3(0, 0, EPSILON));
                Vector3 normal(gradX, gradY, gradZ);
                normal.normalize();

				Vector3 lightPos(3, 3, 2); // hardcoded light position, can be changed to move light origin around scene
                float diffuseIntensity = Vector3::dot(normal, (lightPos - rayPos).normalized()); // intensity of light at this point on surface
                if (diffuseIntensity < 0.15) diffuseIntensity = 0.15; // hardcoded minimum light intensity

                sf::Color objColor = colorAtPoint(rayPos);
                return sf::Color(objColor.r * diffuseIntensity, objColor.g * diffuseIntensity, objColor.b * diffuseIntensity);
            }
            else
            {
                // if not calculating lighting, return shade of magenta based on how many steps ray took (more steps = darker shade)
                return sf::Color(255 - 255 * steps / settings.maxSteps, 0, 255 - 255 * steps / settings.maxSteps);
            }
        }

        // if ray did not hit, march the ray forward
        rayPos += rayDir * dist;
        totalRayDist += dist;
        steps++;
    }

    // the ray didn't hit anything in the scene

    // if calculateLighting is true, return color of sky
    if (calculateLighting)
    {
        int skyValue = (pow(abs(rayDir.y), 0.75) + 1) / 2 * 255;
        return sf::Color(0, skyValue, 255);
    }

    // else, return black with a glow around the scene objects
    int glowFactor = steps * 2;
    return sf::Color(glowFactor, 0, glowFactor);
}

// moves camera based on input vector
void Camera::move(const sf::Vector3i& moveInput)
{
    // flat forward vec is cross of world up and right
    pos += Vector3::cross(Vector3(0, 1, 0), right) * moveInput.x * settings.moveSpeed;
    pos.y += moveInput.y * settings.moveSpeed;
    pos += right * moveInput.z * settings.moveSpeed;
}

// rotates camera based on mouse input vector
void Camera::look(const sf::Vector2i& mouseInput)
{
    //get new pitch & yaw values
    yaw += mouseInput.x * settings.lookSpeed;
	const float TWO_PI = 6.28318531;
	const float HALF_PI = 1.57079633;
    if (yaw < 0)
        yaw += TWO_PI;
    if (yaw > TWO_PI)
        yaw -= TWO_PI;
    pitch += mouseInput.y * settings.lookSpeed;
    if (pitch < -HALF_PI)
        pitch = -HALF_PI;
    if (pitch > HALF_PI)
        pitch = HALF_PI;

    // update forward, up, and right vectors
    float cosP = cosf(pitch);
    
    forward = Vector3(cosf(yaw) * cosP, sinf(pitch), -sinf(yaw) * cosP);
    
    right = Vector3(cosf(yaw - HALF_PI), 0, -sinf(yaw - HALF_PI));

    up = Vector3::cross(right, forward);
}