#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

// Enemy
class Enemy
{
public:
	// Properties
	const float PI = 3.14159265f;
	Texture texture;
	Sprite sprite;
	Vector2f currentPosition;
	float rotation;
	int speed = 4;
	float angle;
	float scaleSize = 0.25 / 4 * 3;
	FloatRect enemyRectScan;
	FloatRect enemyView;
	bool hasDied = false;
	Color defColor;

	// Functions
	bool getCollision(FloatRect playerCollision)
	{
		if (!hasDied)
		{
			FloatRect enemyCollision = sprite.getGlobalBounds();

			return enemyCollision.intersects(playerCollision);
		}
		else
		{
			return false;
		}
	}
	FloatRect bounds()
	{
		return sprite.getGlobalBounds();
	};
	int setPosition(int X, int Y)
	{
		sprite.setPosition(X, Y);

		return EXIT_SUCCESS;
	}
	int rotate(RenderWindow& app, Vector2f playerLocation)
	{
		float rotateX = sprite.getPosition().x - playerLocation.x;
		float rotateY = sprite.getPosition().y - playerLocation.y;
		angle = (atan2(rotateY, rotateX)) * 180 / PI;

		sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
		sprite.setRotation(angle - 90);

		return EXIT_SUCCESS;
	}
	Vector2f getPosition()
	{
		return sprite.getPosition();
	}
	int init()
	{
		// Create the texture and sprite
		texture.loadFromFile("./images/Enemy.png");
		sprite.setTexture(texture);

		// Re-scale the Enemy
		sprite.scale(Vector2f(scaleSize, scaleSize));
		return EXIT_SUCCESS;
	}
	int preInit() {

		sprite.setPosition(100, 100);

		hasDied = false;

		return EXIT_SUCCESS;
	}
	

	int create(RenderWindow& app, Vector2f currentPosition)
	{
		// Get a random location
		int randomNumberForSpawn = rand() % 3;

		// Get a random adjustment
		int randomNumberAsAdjustment = rand() % 50;

		Vector2f randomSpawn;
		switch (randomNumberForSpawn)
		{
		case 0:
			// Top left
			randomSpawn.x = (10 + randomNumberAsAdjustment);
			randomSpawn.y = (10 + randomNumberAsAdjustment);
			break;
		case 1:
			// Top right
			randomSpawn.x = 1700 - (10 + randomNumberAsAdjustment);
			randomSpawn.y = randomNumberAsAdjustment;
			break;
		case 2:
			// Bottom right
			randomSpawn.x = 1700 - (10 + randomNumberAsAdjustment);
			randomSpawn.y = 900 - (10 + randomNumberAsAdjustment);
			break;
		case 3:
			// Bottom left
			randomSpawn.x = (10 + randomNumberAsAdjustment);
			randomSpawn.y = 900 - (10 + randomNumberAsAdjustment);
			break;
		default:
			// Same as bottom left :)
			randomSpawn.x = (10 + randomNumberAsAdjustment);
			randomSpawn.y = 900 - (10 + randomNumberAsAdjustment);
		};

		// Set the position of the enemy
		sprite.setPosition(randomSpawn.x, randomSpawn.y);

		// Enemy position setting
		currentPosition.x = randomSpawn.x;
		currentPosition.y = randomSpawn.y;

		// Draw the sprites
		app.draw(sprite);

		return EXIT_SUCCESS;
	};
	int kill()
	{
		hasDied = true;

		return EXIT_SUCCESS;
	}
	int move(RenderWindow& app, Vector2f PlayerLocation)
	{
		if (!hasDied)
		{
			Vector2f enemyLocation;
			enemyLocation.x = sprite.getPosition().x;
			enemyLocation.y = sprite.getPosition().y;

			float resultX = enemyLocation.x - PlayerLocation.x;
			float resultY = enemyLocation.y - PlayerLocation.y;

			float moveX = 0;
			float moveY = 0;

			// X pos
			if (resultX > 0)
			{
				// Left
				if (resultX > speed)
				{
					moveX = -speed;
				}
				else
				{
					moveX = -resultX;
				};
			}
			else if (resultX < 0)
			{
				// Right
				if (resultX < -speed)
				{
					moveX = speed;
				}
				else
				{
					moveX = resultX;
				};
			}
			else
			{
				// Correct X pos
			};

			// Y pos
			if (resultY > 0)
			{
				// Down
				if (resultY > speed)
				{
					moveY = -speed;
				}
				else
				{
					moveY = -resultY;
				};
			}
			else if (resultY < 0)
			{
				// Up
				if (resultY < -speed)
				{
					moveY = speed;
				}
				else
				{
					moveY = resultY;
				};
			}
			else
			{
				// Correct Y pos
			};

			sprite.move(moveX, moveY);
			app.draw(sprite);
		}

		return EXIT_SUCCESS;
	};
};

// Bullet class for shooting
class Bullet
{
public:
	// Functions here
	int init()
	{
		texture.loadFromFile("./images/bullet.png");
		sprite.setTexture(texture);

		// Smallen Sprite of bullet
		sprite.scale(Bullet::bulletScale, Bullet::bulletScale);

		return EXIT_SUCCESS;
	}

	int preShoot(Vector2f playerPos, bool isBulletHidden = false) {

		sprite.setPosition(playerPos);
		Vector2f pos = sprite.getPosition();
		isHidden = isBulletHidden;

		return EXIT_SUCCESS;
	}
	int shoot(RenderWindow& app, Vector2f bulletPosition, float bulletRotation)
	{
		if (!isHidden)
		{
			// Rotate the bullet
			sprite.setRotation(bulletRotation - 90);

			// Set position
			sprite.setPosition(bulletPosition.x, bulletPosition.y);
			sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);

			// Calculate new Position of Sprite
			Vector2f bulletMovement;
			bulletMovement.x = (float)cos((bulletRotation * PI) / 180) * bulletSpeed;
			bulletMovement.y = (float)sin((bulletRotation * PI) / 180) * bulletSpeed;

			// Move the actual sprite
			sprite.move(bulletMovement.x, bulletMovement.y);

			// Draw the sprite on the app
			app.draw(sprite);
		};

		return EXIT_SUCCESS;
	};
	bool getCollision(FloatRect enemyCollision)
	{
		if (isHidden) return false;

		FloatRect bulletCollision = sprite.getGlobalBounds();

		return bulletCollision.intersects(enemyCollision);
	};
	bool hide()
	{
		isHidden = true;

		return true;
	};


	// Variables
	const float PI = 3.14159265f;
	Texture texture;
	Sprite sprite;
	float bulletScale = 0.10;
	float bulletSpeed = 20;
	FloatRect bulletRectScan;
	FloatRect View;
	bool isHidden = false;
};
