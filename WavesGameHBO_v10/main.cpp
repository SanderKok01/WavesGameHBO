#include <iostream>
#include <SFML/Graphics.hpp>
#include "main.h"
using namespace sf;
using namespace std;

int main()
{
	// Player color
	string PlayerColor = "red";

	// For spawning
	bool firstTimeSpawningInLoop = false;

	// Enemy class
	Enemy enem = Enemy();
	enem.init();
	enem.preInit();

	// For bullet counter
	const int maxBullets = 5;
	int bulletsLeft;

	// For Enemy spawning
	const int minSpawnPosition = 1;
	const int maxSpawnPosition = 4;

	// PI variable
	const float PI = 3.14159265f;

	// Default size for scaling
	int screenWidth = 1920;
	int screenHeight = 1080;
	int scaleSize = 30;
	float playerScaleSize = 0.25;

	// Render the window and set the frame limit
	RenderWindow app(VideoMode(screenWidth, screenHeight), "Top-Down 8-Bit Shooter Game!");
	app.setFramerateLimit(60);

	// Create a fixed view variable
	View fixedView = app.getView();

	// Hide the default cursor = false
	app.setMouseCursorVisible(true);

	// Create texture variables
	Texture BackgroundTexture;
	Texture PlayerTexture;

	// Load the textures
	BackgroundTexture.loadFromFile("./images/Background.png");
	PlayerTexture.loadFromFile("./images/Player_" + PlayerColor + ".png"); // With other colors

	// Create a sprite for the textures
	Sprite backgroundSprite(BackgroundTexture);
	Sprite playerSprite(PlayerTexture);

	// Make the Player smaller
	playerSprite.scale(Vector2f(playerScaleSize, playerScaleSize));

	

	// Set the position of the background
	backgroundSprite.setPosition(0, 0);
	backgroundSprite.setOrigin(0, 0);

	// Set the position of the Player
	playerSprite.setPosition(770, 350);
	playerSprite.setOrigin(playerSprite.getTexture()->getSize().x / 2, playerSprite.getTexture()->getSize().y / 2);

	// Declare variables for speed and rotation for the Player
	float speed = 0;
	int speedWhileStandingStill = 0;
	float maxMovementSpeed = 7.5;
	float angle = 0;

	// Declare variables for speed and rotation for the Enemy
	float enemySpeed = 0;
	int enemySpeedWhileStandingStill = 0;
	float enemyMaxMovementSpeed = 7.5;
	float enemyAngle = 0;
	float enemyScaleSize = 0.25 / 4 * 3;

	// TEMP for upping waves
	bool hasBeenPressed = false;

	// Bullet position and angle
	Vector2f bulletPosition = Vector2f(0, 0);
	float bulletAngle = 0.f;

	// Shooting bool
	bool shotHasBeenFired = false;

	// Set to max bullets
	bulletsLeft = maxBullets;

	// Enemy counter
	int wave = 1;
	float increaseEnemiesEveryWave = 1.7;
	int totalKilledEnemies = 0;
	int curEnemyCount = 0;
	int prevEnemyCount = 0;

	if (wave == 1)
	{
		// If wave is 1
		curEnemyCount = round(wave * increaseEnemiesEveryWave);
	}
	else if (wave > 1)
	{
		// Set previous amount of enemies
		prevEnemyCount = curEnemyCount;

		// If wave is above 1
		curEnemyCount = round(prevEnemyCount * increaseEnemiesEveryWave);
	}

	// Shooting class
	Bullet bullet;
	bullet.init();
	bullet.preShoot(Vector2f(770, 350), true);

	// While the app is opened
	while (app.isOpen())
	{
		// Create an event variable while the app is opened
		Event e;
		while (app.pollEvent(e))
		{
			// If app will be closed
			if (e.type == Event::Closed)
			{
				app.close();
			}
		}

		// Get the surrounding of the Player
		FloatRect playerCollision = playerSprite.getGlobalBounds();

		// Player position and mouse position
		Vector2f spritePosition = playerSprite.getPosition();
		Vector2i mousePosition = Mouse::getPosition(app);

		// Set the angle for rotation
		float rotateX = spritePosition.x - mousePosition.x;
		float rotateY = spritePosition.y - mousePosition.y;

		// Declare rotation variable
		float PlayerRotation = (atan2(rotateY, rotateX)) * 180 / PI;
		angle = PlayerRotation;

		// Rotate the sprite
		playerSprite.setRotation(PlayerRotation - 90);

		// Create variables for Player movement
		bool Up = false;

		// Check for if certain buttons are pressed
		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		{
			Up = true;
		}

		// Movement variables
		Vector2f movement;
		movement.x = (float)cos((angle * PI) / 180);
		movement.y = (float)sin((angle * PI) / 180);

		Vector2f movementWithSpeed;
		movementWithSpeed.x = movement.x * speed;
		movementWithSpeed.y = movement.y * speed;

		// Set the speed
		if (Up)
		{
			if (Mouse::getPosition().x == playerSprite.getPosition().x || Mouse::getPosition().y == playerSprite.getPosition().y)
			{
				speed = speedWhileStandingStill;
			}
			else
			{
				cout << Mouse::getPosition().x << endl << playerSprite.getPosition().x << endl << Mouse::getPosition().y << endl << playerSprite.getPosition().y << endl << endl;
				speed = maxMovementSpeed;
			}

			// Move the Player
			playerSprite.move(-movementWithSpeed.x, -movementWithSpeed.y);
		}
		else
		{
			speed = speedWhileStandingStill;
		}

		//// TEMP for next wave
		//if (Keyboard::isKeyPressed(Keyboard::Right) && hasBeenPressed == false)
		//{
		//	hasBeenPressed = true;
		//	
		//};

		if (e.type == Event::KeyReleased && e.key.code == Keyboard::Key::Right)
		{
			hasBeenPressed = false;
		};

		// Draw the app HERE

		// Clear the app first
		app.clear(Color::White);

		// Set the view
		app.setView(fixedView);

		// Draw the background
		app.draw(backgroundSprite);

		// When shooting
		if (Mouse::isButtonPressed(Mouse::Button::Left) && !shotHasBeenFired)
		{
			bulletPosition = spritePosition;
			bulletAngle = PlayerRotation;
			bullet.preShoot(bulletPosition);
			bullet.shoot(app, bulletPosition, bulletAngle);
			shotHasBeenFired = true;
		}
		else if (shotHasBeenFired)
		{
			// Movement variables
			Vector2f bulletMovement;
			bulletMovement.x = (float)cos((bulletAngle * PI) / 180);
			bulletMovement.y = (float)sin((bulletAngle * PI) / 180);

			Vector2f bulletMovementWithSpeed;
			bulletMovementWithSpeed.x = bulletMovement.x * bullet.bulletSpeed;
			bulletMovementWithSpeed.y = bulletMovement.y * bullet.bulletSpeed;

			bulletPosition = Vector2f(bulletPosition.x - bulletMovementWithSpeed.x, bulletPosition.y - bulletMovementWithSpeed.y);
			bullet.shoot(app, bulletPosition, bulletAngle);
		}

		// Reload
		if (Mouse::isButtonPressed(Mouse::Button::Right))
		{
			bullet.hide();
			shotHasBeenFired = false;
		}

		// Fixed view for Player
		app.setView(fixedView);

		// Draw the Player's Character
		app.draw(playerSprite);

		/*
		// Re-draw enemies by getting new position and angle
		for (int index = 0; index < curEnemyCount; index++)
		{
			// TODO:
			// Check for the amount of zombies left
			// Then set their position one step closer to Player
			// Then set their rotation facing the Player
			// Repeat.
		}
		*/

		enem.currentPosition = enem.getPosition();
		//app.draw(enem.sprite);
		enem.rotate(app, spritePosition);

		if (enem.getCollision(playerCollision))
		{
			app.close();
		}
		else
		{
			enem.move(app, spritePosition);
		}

		if (!enem.hasDied &&  bullet.getCollision(enem.bounds()))
		{
			bullet.hide();
			enem.kill();

			wave += 1;
			enem.preInit();
			enem.speed++;

		}

		// Display the app
		app.display();
	};

	return EXIT_SUCCESS;
}

/*
PRIO:
1 - Critical
2 - Must include
3 - Would be nice

TODO:
- Fix bug when cursor and Player are equal so the Player glitches (3) => NOT DONE
- Enemy needs to head for the Player (1) => NOT DONE
- Random Enemy spawn between certain points (2) => NOT  DONE
- Add the ability for the Player to shoot (1) => DONE
- Allow the Player to shoot more then once (2) => NOT DONE

HUD: (2) => NOT DONE
- Wave counter on bottom left
- Lives on top left
- Zombie kill counter bottom right
- Pause menu top right
*/
