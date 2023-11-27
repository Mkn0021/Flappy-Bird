#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constants
const int screenWidth = 800;
const int screenHeight = 600;
const int birdWidth = 50;
const int birdHeight = 36;
const int birdScale = 1;
int colorChoice=3;
const int gravity = 2;
const int jumpStrength = 15;
const int pipeWidth = 60;
int pipeSpeed = 5;
const int pipeHeight = 320;
const int numPipes = 5;
const int minGap = 100;
int flapIndex = 0;
int barIndex=0;

// Window and entities
sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Flappy Bird");
sf::Texture birdTexture, upperPipeTexture, lowerPipeTexture, backgroundTexture, centerImageTexture;
sf::Sprite birdSprite, upperPipeSprite, lowerPipeSprite, backgroundSprite;
sf::Font font;
sf::Text scoreText;
sf::Music backgroundMusic;
sf::SoundBuffer jumpBuffer, gameOverBuffer;
sf::Sound jumpSound, gameOverSound;

// Bird and pipe variables
int birdX = screenWidth / 4, birdY, birdVelocity;
int pipeX[numPipes], upperPipeY[numPipes], lowerPipeY[numPipes];
int currentPipe, score;
bool gameOver;

// Function prototypes
bool loadTexture(sf::Texture& texture, const std::string& filename);
void setup(int colorIndex);
bool startingScreen();
bool Music_Setup();
void loadAssets();
void loadFont();
void drawEntity(sf::Sprite& sprite, int x, int y);
void drawBackground();
void DrawBar(int x, int y);
void drawScore();
void update();
bool endScreen();
void handleInput();
void drawBird(int birdX, int birdY, float scale,int colorChoice);

int main()
{
    srand(static_cast<unsigned>(time(0)));
    loadAssets();
    loadFont();
    startingScreen();

    setup(colorChoice);

    while (window.isOpen())
    {
        handleInput();

        if (!gameOver)
        {
            update();
        }

        window.clear();

        drawBackground();
        drawBird(birdX, birdY, 1.0f, colorChoice);
        for (int i = 0; i < numPipes; i++)
        {
            drawEntity(upperPipeSprite, pipeX[i], upperPipeY[i]);
            drawEntity(lowerPipeSprite, pipeX[i], lowerPipeY[i]);
        }
        drawScore();
        DrawBar(0, screenHeight - 43);
        window.display();

        sf::sleep(sf::milliseconds(50));

        if (gameOver)
        {
            gameOverSound.play();
            backgroundMusic.stop();
            colorChoice = rand() % 3 + 1;
            if (!endScreen())
            {
                window.close();
            }
            setup(colorChoice);
        }
    }

    std::cout << "Thank you for playing!" << std::endl;
    return 0;
}


void drawBird(int birdX, int birdY, float scale, int colorIndex)
{
    sf::Texture downFlapTexture, midFlapTexture, upFlapTexture;

    // Set the appropriate textures based on colorIndex
    switch (colorIndex)
    {
    case 1: // Blue
        downFlapTexture.loadFromFile("Res/Image/Skin/bluebird-downflap.png");
        midFlapTexture.loadFromFile("Res/Image/Skin/bluebird-midflap.png");
        upFlapTexture.loadFromFile("Res/Image/Skin/bluebird-upflap.png");
        break;
    case 2: // Red
        downFlapTexture.loadFromFile("Res/Image/Skin/redbird-downflap.png");
        midFlapTexture.loadFromFile("Res/Image/Skin/redbird-midflap.png");
        upFlapTexture.loadFromFile("Res/Image/Skin/redbird-upflap.png");
        break;
    default: // Yellow
        downFlapTexture.loadFromFile("Res/Image/Skin/yellowbird-downflap.png");
        midFlapTexture.loadFromFile("Res/Image/Skin/yellowbird-midflap.png");
        upFlapTexture.loadFromFile("Res/Image/Skin/yellowbird-upflap.png");
        break;
    }

    birdSprite.setPosition(birdX, birdY);
    birdSprite.setScale(scale, scale);

    // Update flapIndex for the animation
    flapIndex = (flapIndex + 1) % 3;

    // Set the appropriate texture based on flapIndex
    if (flapIndex == 0)
    {
        birdSprite.setTexture(downFlapTexture);
    }
    else if (flapIndex == 1)
    {
        birdSprite.setTexture(midFlapTexture);
    }
    else
    {
        birdSprite.setTexture(upFlapTexture);
    }

    // Draw the bird
    window.draw(birdSprite);
}

bool loadTexture(sf::Texture& texture, const std::string& filename)
{
    return texture.loadFromFile(filename);
}

void setup(int colorIndex)
{
    birdY = screenHeight / 2;
    birdVelocity = 0;

    for (int i = 0; i < numPipes; i++)
    {
        pipeX[i] = screenWidth + i * screenWidth / numPipes;
        upperPipeY[i] = rand() % (pipeHeight + 1) - pipeHeight;
        lowerPipeY[i] = rand() % (pipeHeight + 1) + screenHeight - pipeHeight;
        if (lowerPipeY[i] - (upperPipeY[i] + pipeHeight) < minGap)
        {
            upperPipeY[i] = -pipeHeight;
        }
    }

    currentPipe = 0;
    gameOver = false;
    score = 0;

    if (!Music_Setup())
    {
        std::cerr << "Error setting up music" << std::endl;
    }
    else
    {
        backgroundMusic.setLoop(true);
        backgroundMusic.play();
    }
}

bool Music_Setup()
{
    return backgroundMusic.openFromFile("Res/Audio/bgm.wav") &&
           jumpBuffer.loadFromFile("Res/Audio/jump.wav") &&
           gameOverBuffer.loadFromFile("Res/Audio/over.wav") &&
           (jumpSound.setBuffer(jumpBuffer), true) &&
           (gameOverSound.setBuffer(gameOverBuffer), true);
}

void loadAssets()
{
    loadTexture(birdTexture, "Res/Image/flappy.png");
    loadTexture(upperPipeTexture, "Res/Image/pipe_upper.png");
    loadTexture(lowerPipeTexture, "Res/Image/pipe_lower.png");
    loadTexture(backgroundTexture, "Res/Image/bg.jpg");

    birdSprite.setTexture(birdTexture);
    upperPipeSprite.setTexture(upperPipeTexture);
    lowerPipeSprite.setTexture(lowerPipeTexture);
    backgroundSprite.setTexture(backgroundTexture);
}

void loadFont()
{
    if (!font.loadFromFile("Res/Font/arial.ttf"))
    {
        std::cerr << "Error loading font" << std::endl;
    }
}

void drawEntity(sf::Sprite& sprite, int x, int y)
{
    sprite.setPosition(x, y);
    window.draw(sprite);
}

void drawBackground()
{
    drawEntity(backgroundSprite, 0, 0);
}

void DrawBar(int x, int y)
{
    sf::Texture texture;
    if(barIndex%2==0)
        texture.loadFromFile("Res/Image/bar1.jpg");
    else
        texture.loadFromFile("Res/Image/bar2.jpg");

    sf::Sprite sprite(texture);
    sprite.setPosition(x, y);
    window.draw(sprite);
    barIndex++;
}

void drawScore()
{
    // Original text (white)
    scoreText.setString(" " + std::to_string(score));
    scoreText.setPosition(10, 30);
    scoreText.setFont(font);
    scoreText.setCharacterSize(50);

    // Border text (black)
    sf::Text scoreTextBorder = scoreText;
    scoreTextBorder.setFillColor(sf::Color::Black);
    scoreTextBorder.setPosition(scoreText.getPosition() + sf::Vector2f(-2, -2));

    // Draw the border first, then the original text
    window.draw(scoreTextBorder);
    window.draw(scoreText);
}


void update()
{
    birdVelocity += gravity;
    birdY += birdVelocity;
    pipeSpeed = 10+ (score/5);

    for (int i = 0; i < numPipes; i++)
    {
        pipeX[i] -= pipeSpeed;
        if (pipeX[i] < -pipeWidth)
        {
            pipeX[i] = screenWidth;
            upperPipeY[i] = rand() % (pipeHeight+1) - pipeHeight;
            lowerPipeY[i] = rand() % (pipeHeight+1) + screenHeight-pipeHeight;
            if (lowerPipeY[i]-( upperPipeY[i] + pipeHeight ) < minGap )
            {
                upperPipeY[i] = -pipeHeight;
            }
        }

        if (birdX + birdWidth > pipeX[i] && birdX < pipeX[i] + pipeWidth)
        {
            if (birdY < upperPipeY[i] + pipeHeight || birdY + birdHeight > lowerPipeY[i])
            {
                gameOver = true;
            }
        }
    }

// Check for collision with top and bottom of the window
    if (birdY + birdHeight > screenHeight || birdY < 0)
    {
        gameOver = true;
    }


    if (birdX > pipeX[currentPipe] + pipeWidth)
    {
        currentPipe = (currentPipe + 1) % numPipes;
        score++;
    }
}

bool endScreen()
{
    // Load a game over image
    sf::Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("Res/Image/gameover.png"))
    {
        // Handle the error if the image fails to load
        std::cout << "Failed to load game over image!" << std::endl;
        return false;
    }

    sf::Sprite gameOverSprite(gameOverTexture);
    gameOverSprite.setPosition(screenWidth / 4, screenHeight / 4);

    // Original text (white)
    scoreText.setString(" Your Score: " + std::to_string(score) + "\nPlay Again? (Y/N)");
    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setPosition(screenWidth / 4 + 70, screenHeight / 2);

    // Border text (black)
    sf::Text scoreTextBorder = scoreText;
    scoreTextBorder.setFillColor(sf::Color(255, 165, 0));
    scoreTextBorder.setPosition(scoreText.getPosition() + sf::Vector2f(-2, -2));

    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Y)
                {
                    return true;
                }
                else if (event.key.code == sf::Keyboard::N)
                {
                    return false;
                }
            }
        }

        window.clear();
        drawBackground();

        // Draw the game over image
        window.draw(gameOverSprite);

        // Draw the score text
        window.draw(scoreTextBorder);
        window.draw(scoreText);

        window.display();
    }

    return false;
}
bool startingScreen()
{
    sf::Texture centerImageTexture;

    if (!centerImageTexture.loadFromFile("Res/Image/message.png"))
    {
        // Handle the error if the image fails to load
        std::cerr << "Failed to load center image!" << std::endl;
        return false;
    }

    sf::Sprite centerImageSprite(centerImageTexture);
    centerImageSprite.setPosition((screenWidth - centerImageTexture.getSize().x) / 2, (screenHeight - centerImageTexture.getSize().y) / 2);

    sf::Event event;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                return true;
            }
        }

        window.clear();

        // Draw the background image
        window.draw(backgroundSprite);

        // Draw the center image
        window.draw(centerImageSprite);
        drawBird(screenWidth/2-30,screenHeight/2+30,1.75,colorChoice);

        window.display();
    }

    return false;
}

void handleInput()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
            {
                birdVelocity = -jumpStrength;
                jumpSound.play();
            }
        }
    }
}
