#ifndef Game_h
#define Game_h
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <time.h>

#include <time.h>
#include "states.h"

struct Laser
{
	sf::Sprite sprite;
	sf::Vector2f target_vector;
	std::string trimmed_str;
	float speed = 30; // Holds magnitude of Vector from turret to asteroid 
	int frame_counter = 0;
	int target_asteroid_index = -1;
	
	void shoot()
	{
		sprite.move(target_vector);
	}
};

struct Asteroid
{
	sf::Sprite shape;
	std::string str_name;
	sf::Text txt_name;
		
	Asteroid(std::string str, sf::Font& font) : str_name(str){
		txt_name.setString(str_name);
		txt_name.setFont(font);
		txt_name.setCharacterSize(14);
		txt_name.setFillColor(sf::Color::Black);
	}
	
	void fly(sf::Vector2f dist){
		txt_name.move(dist);
		shape.move(dist);
	}
};

struct Game
{
	
	sf::CircleShape shield;
	
	// RND
	std::mt19937 mt;
	std::uniform_int_distribution<int> distY;
	std::uniform_int_distribution<int> distXS;
	std::uniform_int_distribution<int> distXM;
	std::uniform_int_distribution<int> distXL;
	std::uniform_int_distribution<int> dist;
	std::uniform_int_distribution<int> distWS;
	std::uniform_int_distribution<int> distWM;
	std::uniform_int_distribution<int> distWL;
	std::uniform_int_distribution<int> distYStars;
	std::uniform_int_distribution<int> distXStars;
	std::uniform_int_distribution<int> distSizeStars;
	std::uniform_int_distribution<int> distStarSpeed;
	
	Game();
	States current_state;
	
	void Update();
	void HandleInput();
	void Render();
	void GameOverAnimation();
	int LaserAnimation(Laser& laser);
	void LoadWords();
	void BubleSort_reverse(std::vector<Asteroid>& container);
	void ResetLasers();
	void ShootLaser(Laser& laser);
	void ShootLetter(std::string trimmed_str, int index);
	void MoveStars();
	void SpawnWave(int wave_num);
	void SpawnSmallAsteroids();
	void SpawnMedAsteroids();
	void SpawnLargeAsteroids();
	void Intermission();
	sf::RenderWindow window;
	sf::Event event;
	sf::Clock clock;
	sf::Time elapsed;
	sf::Font font;
	
	
	std::vector<Asteroid> asteroids;
	std::vector<Laser> lasers;
	
	int selected_asteroid = -1;
	bool fire = false;
	bool hit = true;
	bool is_intermission = false;
	float intermission_timer;
	
	sf::Vector2f target_vector;
	
	//Buttons
	sf::RectangleShape restart_btn;
	
	// TEXT
	sf::Text temp_text;
	
	std::vector<std::string> words_short;
	std::vector<std::string> words_med;
	std::vector<std::string> words_long;
	
	sf::Sprite spr_star_dest;
	sf::Texture texture_star_dest;
	
	sf::Texture texture_small_aster;
	sf::Texture texture_med_aster;
	sf::Texture texture_large_aster;
	
	sf::Sprite spr_gun;
	sf::Texture texture_gun;
	
	sf::Sprite spr_laser;
	sf::Sprite spr_laser_second;
	sf::Texture texture_laser;
	
	sf::Texture texture_star1;
	sf::Texture texture_star2;
	sf::Texture texture_star3;
	
	std::array<sf::Texture, 3> star_textures;
	std::vector<sf::Sprite> stars;
	
	sf::Vector2f laser_initial_pos;
	
	
	bool was_released = true;
	
	float frametime = 1.0/30.0;
	
	int wave_num = 0;
	sf::Text txt_wave;
	
	int score = 0;
	sf::Text txt_score;

	int deflectors = 100;
	sf::Text txt_deflectors;
	
	int screen_width = 1200;
	int screen_height = 700;
	
	
	///////////////////////////////////////////////
	sf::Sprite bg;

	sf::Texture texture;
	sf::Texture bg_texture;
	
	// Animation 
	sf::IntRect frame_rect;
	float animation_frame_time;
	
};

#endif