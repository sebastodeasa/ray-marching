#include <SFML\Graphics.hpp>
#include <iostream>
#include <chrono>
#include "Vector3.h"
#include "SceneObjects.h"
#include "Camera.h"
#include "Scene.h"
using namespace std;

/*
* Sets the color of a pixel in the screen array(pixels) of shape imageResolution,
* given the pixel's coordinates (x and y) and the color to set it to (c).
*/
void setPixel(sf::Uint8* pixels, const int x, const int y, const sf::Color c, const sf::Vector2i imageResolution)
{
    int i = (y * imageResolution.x + x) * 4; //index of the first part of the pixel in the screen array
    pixels[i] = c.r;
    pixels[i + 1] = c.g;
    pixels[i + 2] = c.b;
    pixels[i + 3] = c.a;
}

/*
* Handles keyboard and mouse input to move and rotate the camera (cam).
*/
void processInputs(Camera& cam, const sf::RenderWindow& window, const sf::Vector2i& WINDOW_SIZE)
{
    // move logic
    sf::Vector3i moveInput; //x component is forward/backward, y is up/down, z is left/right strafe

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        moveInput.x++;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        moveInput.x--;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        moveInput.y++;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        moveInput.y--;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        moveInput.z++;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        moveInput.z--;

    cam.move(moveInput);

    // look logic
    sf::Vector2i mouseInput = sf::Vector2i(WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 2) - sf::Mouse::getPosition(window); //cause mouse position is 0 at top left not bottom right
    sf::Mouse::setPosition(sf::Vector2i(WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 2), window);

    cam.look(mouseInput);

	// zoom camera's FOV if left control is held
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        cam.settings.fov = 0.2;
    else
        cam.settings.fov = 1;
}


int main()
{

	const sf::Vector2i WINDOW_SIZE(2000, 2000); // actual pixel resolution of the window on the screen

    sf::Vector2i imageResolution(300, 300); // resolution that the ray marcher renders at -- will be scaled up to fit the window
	sf::Uint8* pixels = new sf::Uint8[imageResolution.x * imageResolution.y * 4]; // array of RGBA values for each pixel rendered

	// create SFML window and texture objects for rendering
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "ray marching", sf::Style::Close | sf::Style::Titlebar);
    sf::Texture screenTexture;
    screenTexture.create(imageResolution.x, imageResolution.y);

	sf::Mouse::setPosition(sf::Vector2i(WINDOW_SIZE.x / 2, WINDOW_SIZE.y / 2), window); // set mouse position to center of window
    window.setMouseCursorVisible(false);

	// load font for UI text
    sf::Font font;
    if (!font.loadFromFile("font data/SourceCodePro-Regular.ttf"))
    {
        cerr << "error loading font\n";
        exit(1);
    }

	// create scene and camera objects
    Scene* scene = new Scene();
    Camera cam(scene, Vector3(-2, 0.2, 0));
    cam.calculateLighting = false;
    cam.findsMinDistance = true; // make false for boolean rendering

	// add hardcoded objects to scene for testing (scene owns pointers so they will be deleted when scene is deleted)

    // uncomment following block to add some primitive solids to the scene:
    /*
    scene->addObject(new Box(Vector3(2, 0, 0), Vector3(0.5, 0.5, 0.5), sf::Color::Yellow));
    scene->addObject(new Sphere(Vector3(2, 0.5, -0.5), 0.5, sf::Color::Green, true));
	Sphere* s1 = new Sphere(Vector3(2, 0, 0.5), 0.5, sf::Color::Magenta, true); // used for temporary testing animation, so a pointer is kept here to change its position every frame
    scene->addObject(s1);
    */

    // uncomment following block to add a mandelbulb to the scene:
    
    MandelBulb* mb = new MandelBulb(12, 8, sf::Color::White);
    scene->addObject(mb);
    

    unsigned int frames = 0; // counter of total frames rendered
    unsigned int framesToMeasure = 5; // number of frames to render between each FPS measurement
	float fps = -1;
	auto lastTimeStamp = chrono::high_resolution_clock::now(); // used for calculating and displaying FPS
    auto currentTimeStamp = chrono::high_resolution_clock::now(); // ^


    while (window.isOpen())
    {
        // check for closed window event
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

		if (window.hasFocus()) // only render / process inputs if window is focused
        {
            processInputs(cam, window, WINDOW_SIZE);

            // animating hardcoded sphere up and down (comment if not using primitive solids scene)
            /*
            s1->setPosition(Vector3(s1->getPosition().x, sinf((float)frames / 50), s1->getPosition().z));
            */

			// process inputs for editing mandelbulb parameters (uncomment only if mandelbulb is in scene)
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                mb->power += 0.05;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                mb->power -= 0.05;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                mb->iters--;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                mb->iters++;
            

            // render
            for (int i = 0; i < imageResolution.x; i++)
            {
                for (int j = 0; j < imageResolution.y; j++)
                {
                    // set pixel to value returned by marchRay
                    setPixel(pixels, i, j, cam.marchRay((i + 0.5) / imageResolution.x, (j + 0.5) / imageResolution.y), imageResolution); // added 0.5 to use center of pixel as ray direction
                }
            }

			// update window with new rendered image
            screenTexture.update(pixels); // make texture from pixels
            sf::Sprite screenSprite(screenTexture); // make sprite from texture
            screenSprite.setScale((float)WINDOW_SIZE.x / imageResolution.x, (float)WINDOW_SIZE.y / imageResolution.y); // scale sprite to fit whole window
            window.draw(screenSprite); // draw sprite to window

			string debugText = "FPS: " + to_string(fps);
            debugText.append("\niterations: " + to_string(mb->iters) + "\npower: " + to_string(mb->power)); // uncomment for displaying mandelbulb parameters:
            window.draw(sf::Text(debugText, font, 50));


            window.display(); // display window


			if (frames % framesToMeasure == 0) // calculate and display FPS every framesToMeasure frames
			{
				currentTimeStamp = chrono::high_resolution_clock::now();
				fps = framesToMeasure / chrono::duration<float>(currentTimeStamp - lastTimeStamp).count();
				lastTimeStamp = currentTimeStamp;
			}

            frames++;
        }
        
    }

	
    delete[] pixels;
	delete scene;

    return 0;
}