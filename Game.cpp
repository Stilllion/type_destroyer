#include "Game.h"
#include <cmath>
#include <fstream>

// SHORT : 1-4
// MED : 5-8
// SHORT : 

#define PI 3.14159265

Game::Game() : /*distY(80, 100)*/ distY(30, 560), distXS(-350, -100), distXM(-400, -200),
								  distXL(-500, -300),  distWS(0, 2183), distWM(0, 5459),
								  distWL(0, 2243), distYStars(0, 600), distXStars(0, 1200),
								  distSizeStars(0, 2), distStarSpeed(0, 100)  // DistY(25, 80)
{
	window.create(sf::VideoMode(screen_width, screen_height), "Type Destroyer!");
	srand(time(0));
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	mt.seed(std::time(0));

	////////////////////////////////////////////////////
	bg_texture.loadFromFile("assets/background.png");
	bg.setTexture(bg_texture);
	font.loadFromFile("assets/Old_R.ttf");
	
	txt_score.setFont(font);
	txt_score.setCharacterSize(12);
	txt_score.setFillColor(sf::Color::White);
	txt_score.setString("Score: " + std::to_string(score));
	txt_score.setPosition(1000, 650);
	
	txt_wave.setFont(font);
	txt_wave.setCharacterSize(12);
	txt_wave.setFillColor(sf::Color::White);
	txt_wave.setString("Wave: " + std::to_string(wave_num));
	txt_wave.setPosition(850, 650);
	
	txt_wave.setFont(font);
	txt_wave.setCharacterSize(12);
	txt_wave.setFillColor(sf::Color::White);
	txt_wave.setString("Wave: " + std::to_string(wave_num));
	txt_wave.setPosition(850, 650);
	
	txt_deflectors.setFont(font);
	txt_deflectors.setCharacterSize(12);
	txt_deflectors.setFillColor(sf::Color::White);
	txt_deflectors.setString("Deflector shields: " + std::to_string(deflectors) + "%");
	txt_deflectors.setPosition(550, 650);
	
	texture_gun.loadFromFile("assets/gun_rotated.png");
	spr_gun.setTexture(texture_gun);
	spr_gun.setOrigin(32, 41.5);
	spr_gun.rotate(90);
	spr_gun.setScale(0.4, 0.4);
	spr_gun.setPosition(sf::Vector2f(947, 298)); 
	
	texture_laser.loadFromFile("assets/double_laser_rotated.png");
	spr_laser.setTexture(texture_laser);
	spr_laser.setTextureRect(sf::IntRect(0, 0, 28, 4));
	spr_laser.setOrigin(14, 0);
	spr_laser.rotate(90);
	laser_initial_pos.x = 930; // 863
	laser_initial_pos.y = 298; // 174
	spr_laser.setPosition(laser_initial_pos); 
	
	texture_star_dest.loadFromFile("assets/sd.png");
	texture_star_dest.setSmooth(true);
	spr_star_dest.setTexture(texture_star_dest);
	
	spr_star_dest.setOrigin(991/2, 1576/2);
	spr_star_dest.setScale(0.24, 0.24);
	spr_star_dest.rotate(270);
	spr_star_dest.setPosition(1000, 300);
		
	
	texture_small_aster.loadFromFile("assets/asteriod_small.png");
	texture_med_aster.loadFromFile("assets/asteriod_med.png");
	texture_large_aster.loadFromFile("assets/asteroid_large.png");
	
	texture_star1.loadFromFile("assets/star1.png");
	texture_star2.loadFromFile("assets/star2.png");
	texture_star3.loadFromFile("assets/star3.png");
	
	star_textures[0] = texture_star1;
	star_textures[1] = texture_star2;
	star_textures[2] = texture_star3;
	
	LoadWords();

	// Place stars
	sf::Sprite star;
	for(int i = 0; i < 200; ++i){
		star.setTexture(star_textures[distSizeStars(mt)]);
		star.setPosition(distXStars(mt), distYStars(mt));
		stars.push_back(star);
	}
	
	BubleSort_reverse(asteroids);
	
	shield.setRadius(190);
	shield.setOrigin(50, 50);
	shield.setOutlineThickness(2);
	shield.setFillColor(sf::Color(0, 0, 0, 0));
	shield.setOutlineColor(sf::Color::Blue);
	shield.setPosition(857, 162);

	
	if(!texture.loadFromFile("assets/t.png")){
		return;
	}

	////////////////////////////////////////////////////
	
	current_state = States::game_state;
	
	clock.restart();
}

void Game::SpawnSmallAsteroids()
{
	Asteroid a(words_short[distWS(mt)], font);
	
	a.shape.setTexture(texture_small_aster);
	a.txt_name.setFillColor(sf::Color::White);
		
	a.txt_name.setPosition(sf::Vector2f(distXS(mt), distY(mt)));
	
	a.shape.setPosition(a.txt_name.getPosition().x, a.txt_name.getPosition().y + 10);
	
	asteroids.push_back(a);
}

void Game::SpawnMedAsteroids()
{
	Asteroid a(words_med[distWM(mt)], font);
	a.shape.setTexture(texture_med_aster);
	a.txt_name.setFillColor(sf::Color::White);
		

	a.txt_name.setPosition(sf::Vector2f(distXM(mt), distY(mt)));
	
	a.shape.setPosition(a.txt_name.getPosition().x, a.txt_name.getPosition().y + 10);
		
	asteroids.push_back(a);	
}

void Game::SpawnLargeAsteroids()
{
	Asteroid a(words_long[distWL(mt)], font);
	a.shape.setTexture(texture_large_aster);
	a.txt_name.setFillColor(sf::Color::White);
		

	a.txt_name.setPosition(sf::Vector2f(distXL(mt), distY(mt)));
	
	a.shape.setPosition(a.txt_name.getPosition().x, a.txt_name.getPosition().y + 10);
	asteroids.push_back(a);		
}

void Game::SpawnWave(int wave_num)
{	
	for(wave_num; wave_num >= 0; --wave_num){		
		SpawnSmallAsteroids();
		
		if(wave_num > 2){
			SpawnMedAsteroids();
		}
		if(wave_num > 3){
			SpawnLargeAsteroids();
		}
	}
}

void Game::Intermission()
{
	for(int i = 0; i < stars.size(); ++i){
		stars[i].move(sf::Vector2f(9, 0));
		
		if(stars[i].getPosition().x > screen_width){
			stars[i].setPosition(stars[i].getPosition().x - screen_width - 10, stars[i].getPosition().y);
		}	
	}
}


void Game::BubleSort_reverse(std::vector<Asteroid>& container)
{
	for(int i = 0; i < container.size(); ++i){
		for(int j = 0; j < container.size() - 1; ++j){
			if(container[j].txt_name.getPosition().x < container[j+1].txt_name.getPosition().x){
				std::swap(container[j], container[j+1]);
			}
		}
	}
}

void Game::MoveStars()
{
	for(int i = 0; i < stars.size(); ++i){
		
		if(i < 80){
			stars[i].move(sf::Vector2f(0.5, 0));			
		} else {
			stars[i].move(sf::Vector2f(1, 0));			
		}
		
		if(stars[i].getPosition().x > screen_width){
			stars[i].setPosition(stars[i].getPosition().x - screen_width - 10, stars[i].getPosition().y);
		}
	}
}

void Game::LoadWords()
{
	std::ifstream words_file;
	words_file.open("words_short.txt");
	std::string word;
	
	while(words_file >> word){
		words_short.push_back(word);
	}
	
	words_file.close();
	
	words_file.open("words_medium.txt");
		
	while(words_file >> word){
		words_med.push_back(word);
	}
	
	words_file.close();
	
	words_file.open("words_long.txt");
		
	while(words_file >> word){
		words_long.push_back(word);
	}
	
	words_file.close();
}

void Game::Update()
{
	while(window.pollEvent(event)){
		if(event.type == sf::Event::Closed){
			window.close();
		}
	}
	
	if(elapsed.asSeconds() >= frametime){
	
		MoveStars();
		
		if(asteroids.empty()){
			++wave_num;
			txt_wave.setString("Wave: " + std::to_string(wave_num));
			SpawnWave(wave_num);
			BubleSort_reverse(asteroids);
		}
			
		for(int i = 0; i < asteroids.size(); ++i){
			asteroids[i].fly(sf::Vector2f(1, 0));
		}
		
		
		if(selected_asteroid >= 0){
			// Means we alredy shot the last laser at selected_asteroid and now can shoot other ones
			if(asteroids[selected_asteroid].str_name.length() == 0){
				selected_asteroid = -1;
			}	
		}

		for(int i = 0; i < lasers.size(); ++i){
			
			LaserAnimation(lasers[i]);
					
			lasers[i].target_vector.x = (lasers[i].sprite.getPosition().x - asteroids[lasers[i].target_asteroid_index].shape.getPosition().x) * - 1;
			lasers[i].target_vector.y = (lasers[i].sprite.getPosition().y - asteroids[lasers[i].target_asteroid_index].shape.getPosition().y) * - 1;
			
			float magnitueTargetV = std::sqrt(std::pow(lasers[i].target_vector.x, 2) + std::pow(lasers[i].target_vector.y, 2));
			// Unit vector of target_vector
			sf::Vector2f unitV_of_targetV;
			unitV_of_targetV.x = lasers[i].target_vector.x / magnitueTargetV;
			unitV_of_targetV.y = lasers[i].target_vector.y / magnitueTargetV;
			
			// Reconstruct target_vector with new magnitude
			lasers[i].target_vector.x = unitV_of_targetV.x * lasers[i].speed;
			lasers[i].target_vector.y = unitV_of_targetV.y * lasers[i].speed;
			
			lasers[i].shoot();
			
			// Check collision with asteroids
			if(asteroids[lasers[i].target_asteroid_index].shape.getGlobalBounds().intersects(lasers[i].sprite.getGlobalBounds())){
				
				ShootLetter(lasers[i].trimmed_str, lasers[i].target_asteroid_index);
				
				// Add score for hitting a letter
				score += 5;
				txt_score.setString("Score: " + std::to_string(score));
				

				// Asteroid has been shot down!
				if(lasers[i].target_asteroid_index >= 0){
					if(asteroids[lasers[i].target_asteroid_index].txt_name.getString().getSize() == 0){
						std::swap(asteroids[lasers[i].target_asteroid_index], asteroids.back());	
						asteroids.pop_back();
					}	
				}
				
				if(lasers.size() > 1){
					std::swap(lasers[i], lasers.back());
				}
				
				lasers.pop_back();
				break;
			}
			
			if(deflectors == 0){
				
			}
			
		}

		// Asteroid hits the ship && Asteroid gets deleted if goes out of bounds
		for(int i = 0; i < asteroids.size(); ++i){
			if(asteroids[i].shape.getGlobalBounds().intersects(spr_star_dest.getGlobalBounds())){
		
				deflectors -= 25; 
				txt_deflectors.setString("Deflector shields: " + std::to_string(deflectors) + "%");
			
				std::swap(asteroids[i], asteroids.back());
				asteroids.pop_back();
				
				if(i == selected_asteroid){
					selected_asteroid = -1;
				}
				
				
				asteroids[i].txt_name.setString("");
	
			}
			if(asteroids[i].shape.getPosition().x > 1200){
				std::swap(asteroids[i], asteroids.back());
				asteroids.pop_back();	
				if(i == selected_asteroid){
					selected_asteroid = -1;
				}
			}
		}
		
		
		elapsed -= sf::seconds(frametime);
	}
}

void Game::ShootLetter(std::string trimmed_str, int index)
{				
	asteroids[index].txt_name.setString(trimmed_str);		
}

void Game::HandleInput()
{
	if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Escape)){
			was_released = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		if(current_state == States::game_state && was_released){
			current_state = States::pause_state;
			was_released = false;
			return;
		}
		if(current_state == States::pause_state && was_released){
			current_state = States::game_state;
			was_released = false;
			elapsed -= elapsed;
			return;
		}

	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && lasers.empty()){
		asteroids[selected_asteroid].txt_name.setFillColor(sf::Color::White);
		selected_asteroid = -1;
	}
	
	
	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode < 128){
			for(int i = 0; i < asteroids.size(); ++i){
				if(asteroids[i].str_name.front() == static_cast<char>(event.text.unicode) && selected_asteroid < 0){
					selected_asteroid = i;
					asteroids[selected_asteroid].txt_name.setFillColor(sf::Color::Red);
					break;
				}	
			}
			if(selected_asteroid >= 0){
				if(asteroids[selected_asteroid].str_name.front() == static_cast<char>(event.text.unicode))
				{
					// Rotate gun
					float opp = spr_gun.getPosition().x - asteroids[selected_asteroid].shape.getPosition().x;
					float adj = spr_gun.getPosition().y - asteroids[selected_asteroid].shape.getPosition().y;
					
					float angle_deg = std::atan(opp/adj) * 180 / PI;
					float laser_angle_deg = angle_deg;
		
					if(angle_deg > 0){
						angle_deg = 180 - angle_deg;
						laser_angle_deg = 270 - laser_angle_deg;
					}
					if(angle_deg < 0){
						angle_deg = 0 + (-1 * angle_deg);
						laser_angle_deg = 90 + (- 1 * laser_angle_deg);
					}
					
					
					spr_gun.setRotation(angle_deg);

					spr_laser.setPosition(laser_initial_pos); // Second would be a bit lower, at 241
\
					asteroids[selected_asteroid].str_name = asteroids[selected_asteroid].str_name.substr(1, asteroids[selected_asteroid].str_name.length()-1);
					
	
					Laser laser;
					laser.trimmed_str = asteroids[selected_asteroid].str_name;
					laser.sprite = spr_laser;
					laser.target_asteroid_index = selected_asteroid;
					
					laser.target_vector.x = opp * -1;
					laser.target_vector.y = adj * -1;
					
					lasers.push_back(laser);
				}
			}
		}
	}


	// Mouse input
	if(restart_btn.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
		restart_btn.setOutlineThickness(2);
		restart_btn.setOutlineColor(sf::Color(0, 0, 0));
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && current_state == States::game_over){
			
			current_state = States::game_state;
		}
	} else {restart_btn.setOutlineThickness(0);}
}

int Game::LaserAnimation(Laser& laser)
{
	animation_frame_time += elapsed.asSeconds();

	static sf::Vector2f pos(0, 0);
	static sf::Vector2f dimensions(4, 28);
	
	if(animation_frame_time >= 0.004f && laser.frame_counter <= 9){ // 0.008

		switch(laser.frame_counter){
			case 0: {frame_rect = sf::IntRect(0, 177, 28, 4);}
			break;
			
			case 1: {frame_rect = sf::IntRect(0, 170, 28, 8);  }
			break;
			
			case 2: {frame_rect = sf::IntRect(0, 158, 28, 10);}
			break;
			
			case 3: {frame_rect = sf::IntRect(0, 146, 28, 11);}
			break;

			case 4: {frame_rect = sf::IntRect(0, 133, 28, 14);}
			break;
			
			case 5: {frame_rect = sf::IntRect(0, 117, 28, 16);}
			break;
			
			case 6: {frame_rect = sf::IntRect(0, 98, 28, 20); }
			break;
			
			case 7: {frame_rect = sf::IntRect(0, 77, 28, 22);}
			break;
			
			case 8: {frame_rect = sf::IntRect(0, 58, 28, 25);}
			break;
			
			case 9: {frame_rect = sf::IntRect(0, 27, 28, 27);}
			break;
		}
		laser.sprite.setTextureRect(frame_rect);
		animation_frame_time = 0;
		laser.frame_counter += 1;
	}
}

void Game::Render()
{
	window.clear(sf::Color::Black);
	///////////////////////
	
	for(auto s : stars){
		window.draw(s);
	}
	window.draw(spr_star_dest);
	
	for(int i = 0; i < asteroids.size(); ++i){
		window.draw(asteroids[i].shape);
	}
	for(int i = 0; i < asteroids.size(); ++i){
		window.draw(asteroids[i].txt_name);
	}
	
	window.draw(spr_gun);
	
	for(int i = 0; i < lasers.size(); ++i){
		window.draw(lasers[i].sprite);
	}
	
	
	
	window.draw(txt_score);
	window.draw(txt_wave);
	window.draw(txt_deflectors);

	
	window.display();
}