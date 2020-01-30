#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

// Bullet class for shooting
class Bullet
{
public:
	// Functions here
	// Function: SHOOT
	int shoot(RenderWindow& app, Vector2f bulletPosition, float bulletRotation)
	{
		// PI variable
		const float PI = 3.14159265f;

		// Create the bullet
		Texture bulletTexture;
		bulletTexture.loadFromFile("./images/bullet.png");
		Sprite bulletSprite(bulletTexture);

		// Smallen Sprite of bullet
		bulletSprite.scale(Bullet::bulletScale, Bullet::bulletScale);

		// Rotate the bullet
		bulletSprite.setRotation(bulletRotation - 90);

		// Set variable for screen-scanning
		auto bulletRect = bulletSprite.getGlobalBounds();
		Bullet::bulletRectScan = bulletRect;

		// Set position
		bulletSprite.setPosition(bulletPosition.x, bulletPosition.y);
		bulletSprite.setOrigin(bulletSprite.getTexture()->getSize().x / 2, bulletSprite.getTexture()->getSize().y / 2);

		// Calculate new Position of Sprite
		Vector2f bulletMovement;
		bulletMovement.x = (float)cos((bulletRotation * PI) / 180) * bulletSpeed;
		bulletMovement.y = (float)sin((bulletRotation * PI) / 180) * bulletSpeed;

		// Move the actual sprite
		bulletSprite.move(bulletMovement.x, bulletMovement.y);

		// Draw the sprite on the app
		app.draw(bulletSprite);
		return EXIT_SUCCESS;
	};

	// Variables
	float bulletScale = 0.10;
	float bulletSpeed = 10;
	FloatRect bulletRectScan;
	FloatRect View;
};

int main()
{
	// Player color
	string PlayerColor = "red";

	const int maxBullets = 5;
	int bulletsLeft;

	// PI variable
	const float PI = 3.14159265f;

	// Enemy counter
	float curEnemyCount = 1;
	float newEnemyCount;
	int wave = 1;
	float increaseEveryRound = 1.7;

	// Default size for scaling
	int screenWidth = 1920;
	int screenHeight = 1080;
	int scaleSize = 30;
	float playerScaleSize = 0.25;
	float enemyScaleSize = playerScaleSize / 4 * 3;

	// Render the window and set the frame limit
	RenderWindow app(VideoMode(screenWidth, screenHeight), "Top-Down Shooter Game!");
	app.setFramerateLimit(60);

	// Create a fixed view variable
	View fixedView = app.getView();

	// Hide the default cursor = false
	app.setMouseCursorVisible(true);

	// Create texture variables
	Texture BackgroundTexture;
	Texture PlayerTexture;
	Texture EnemyTexture;

	// Load the textures
	BackgroundTexture.loadFromFile("./images/Background.png");
	PlayerTexture.loadFromFile("./images/Player_" + PlayerColor + ".png"); // With other colors
	EnemyTexture.loadFromFile("./images/Enemy.png");

	// Create a sprite for the textures
	Sprite backgroundSprite(BackgroundTexture);
	Sprite playerSprite(PlayerTexture);
	Sprite enemySprite(EnemyTexture);

	// Make the Player smaller
	playerSprite.scale(Vector2f(playerScaleSize, playerScaleSize));

	// Re-scale the Enemy
	enemySprite.scale(Vector2f(enemyScaleSize, enemyScaleSize));

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

	// TEMP for upping waves
	bool hasBeenPressed = false;

	// :D
	Vector2f bulletPosition = Vector2f(0, 0);
	float bulletAngle = 0.f;

	// Shooting bool
	bool shotHasBeenFired = false;

	// Set to max bullets
	bulletsLeft = maxBullets;

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

		// Shooting class
		Bullet bullet;

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

		// TEMP for next wave
		if (Keyboard::isKeyPressed(Keyboard::Right) && hasBeenPressed == false)
		{
			hasBeenPressed = true;
			wave += 1;
		};

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

		if (Mouse::isButtonPressed(Mouse::Button::Left) > 0 && !shotHasBeenFired)
		{
			bulletPosition = spritePosition;
			bulletAngle = PlayerRotation;
			bulletsLeft -= 1;
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

		// Reload?
		if (Mouse::isButtonPressed(Mouse::Button::Right) && shotHasBeenFired)
		{
			bulletsLeft = maxBullets;
			shotHasBeenFired = false;
		}

		// Fixed view for Player
		app.setView(fixedView);

		// Draw the Player's Character
		app.draw(playerSprite);

		for (int index = 0; index < curEnemyCount; index++)
		{
			// Draw the Enemy Character
			app.draw(enemySprite);
		}

		// Display the app
		app.display();
	}

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
- Random Enemy spawn between certain points (2) =>NOT  DONE
- Add the ability for the Player to shoot (1) => DONE
- Allow the Player to shoot more then once (1) => NOT DONE

HUD: (2) => NOT DONE
- Wave counter on bottom left
- Lives on top left
- Zombie kill counter bottom right
- Pause menu top right
*/
