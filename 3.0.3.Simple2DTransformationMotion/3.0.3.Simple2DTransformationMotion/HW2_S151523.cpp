//20151523 김동현

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "Shaders/LoadShaders.h"

GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 

#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))
#define LOC_VERTEX 0

int win_width = 0, win_height = 0; 
float centerx = 0.0f, centery = 0.0f;

// 2D 물체 정의 부분은 objects.h 파일로 분리
// 새로운 물체 추가 시 prepare_scene() 함수에서 해당 물체에 대한 prepare_***() 함수를 수행함.
// (필수는 아니나 올바른 코딩을 위하여) cleanup() 함수에서 해당 resource를 free 시킴.
#include "objects.h"

//mouse status
int leftbuttonpressed = 0;
int leftkeypressed = 0;
int rightkeypressed = 0;

/*****character status*****/
float char_x = 0;
float char_y = 0;
float char_size = 2;

//sword
float sword_size = 1.2f;
float slash_size_x = 4.0f;
float slash_size_y = 7.0f;

//direct&action status
int walking = 0;
int walking_speed = 3.0;
int looking_L = 0;
int looking_R = 1;
float attack = 1;

//item status
int growup = 0;
int growup_done = 0;
int growup_counter = 0;
int drunk = 0;
int drunk_counter = 0;


/*****object status*****/
//house
float house_x = 200.0f;
float house_y = -30.0f;
float house_temp_x = 200.0f;
float house_temp_y = -30.0f;
float house_rotation = 0;
int house_damage = 0;
int house_broken = 0;

//sun&moon
int day_night = 0;
int sunmoon_radius = 0;
float r = 0, g = 151, b = 255;

//firecracker
float firecracker1_seed_x = 400.0f;
float firecracker1_seed_y = 0.0f;
float firecracker2_seed_x = -200.0f;
float firecracker2_seed_y = 0.0f;
float firecracker1_spark_x = 400.0f;
float firecracker1_spark_y = 300.0f;
float firecracker2_spark_x = -200.0f;
float firecracker2_spark_y = 200.0f;
int firecracker1_explode = 0;
int firecracker2_explode = 0;

//rain
float rain_x = 0.0f;
float rain_y = 200.0f;
int date = 0;

//airplane
float airplane_x = 0;
float airplane_y = 200.0f;
float airplane_turnaround = 0;
float air_temp_x;
float air_temp_y;

//spark
int spark_activated = 0;

/*****item status*****/
//cake
int cake_activated = 1;
int cake_drop = 0;
float cake_x = -250.0f;
float cake_y = -150.0f;
float cake_rotation = 0;
float cake_temp_x = -250.0f;
float cake_temp_y = -150.0f;

//cocktail
int cocktail_activated = 1;
int cocktail_drop = 0;
float cocktail_x = -50.0f;
float cocktail_y = -150.0f;
float cocktail_temp_x = -50.0f;
float cocktail_temp_y = -100.0f;
float cocktail_temp2_x = -50.0f;
float cocktail_temp2_y = -100.0f;
int cocktail_drop_timer;
int cocktail_drop_timer2;

unsigned int timestamp = 0;
int world_timer;
int temp;
int drop_switch = 1;

void timer(int value) {
	timestamp = (timestamp + 1) % UINT_MAX;

	//세상 시간
	world_timer = timestamp % 2001;
	cocktail_drop_timer = timestamp % 121;

	//day change
	if (world_timer <= 1000)
		day_night = 0;

	else 
		day_night = 1;
	
	//change sky color
	if (world_timer >= 600 && world_timer <= 1000) {
		temp = world_timer - 600;
		r = temp / 8; g = 151 - temp / 4; b = 255 - temp / 2.67;
	}

	if (world_timer >= 1600 && world_timer <= 2000) {
		temp = world_timer - 1600;
		r = 51 - temp / 8; g = 51 + temp / 4; b = 102 + temp / 2.67;
	}

	//at day - airplane flight
	//straight flight1
	if (world_timer >= 0 && world_timer < 240) {
		airplane_x = glutGet(GLUT_WINDOW_WIDTH) / 2 - (world_timer / 300.0f) * glutGet(GLUT_WINDOW_WIDTH) / 2;
	}
	if (world_timer == 240) {
		air_temp_x = airplane_x;
		air_temp_y = airplane_y;
	}

	//round flight
	if (world_timer >= 240 && world_timer < 600) {
		temp = world_timer - 240;
		airplane_turnaround = 1;
		airplane_x = air_temp_x - 100 * sqrtf(2 * (1 - cosf(temp * TO_RADIAN))) * cosf(temp / 2 * TO_RADIAN);
		airplane_y = air_temp_y + 100 * sqrtf(2 * (1 - cosf(temp * TO_RADIAN))) * sinf(temp / 2 * TO_RADIAN);
	}

	//straight flight2
	if (world_timer == 600)
		air_temp_x = airplane_x;

	if (world_timer >= 600 && world_timer < 1000) {
		airplane_turnaround = 0;
		temp = world_timer - 600;
		airplane_x = air_temp_x - (temp / 300.0f) * glutGet(GLUT_WINDOW_WIDTH) / 2;
	}

	//item drop
	//cake drop
	if (cake_activated == 0 && abs(airplane_x - cake_x) <= 1) {
		cake_drop = 1;
		cake_y = airplane_y;
	}

	if (cake_drop == 1) {
		cake_rotation = (world_timer % 360);

		if (cake_y > cake_temp_y) {
			cake_y -= 1.0f;
		}

		else {
			cake_drop = 0;
			cake_activated = 1;
			cake_x = cake_temp_x;
			cake_y = cake_temp_y; 
			cake_rotation = 0;
		}
	}

	//cocktail drop
	if (cocktail_activated == 0 && abs(airplane_x - cocktail_x) <= 1) {
		cocktail_drop = 1;
		cocktail_y = airplane_y;
	}

	if (cocktail_drop == 1) {

		if (cocktail_y > cocktail_temp_y && drop_switch == 1) {
			cocktail_x = cocktail_temp_x - 70 * sqrtf(2 * (1 - cosf(cocktail_drop_timer * TO_RADIAN))) * cosf(cocktail_drop_timer / 2 * TO_RADIAN);
			cocktail_y -= 0.5f;
		}

		else if (cocktail_y > cocktail_temp_y && drop_switch == -1) {
			cocktail_drop_timer2 = 120 - cocktail_drop_timer;
			cocktail_x = cocktail_temp_x - 70 * sqrtf(2 * (1 - cosf(cocktail_drop_timer2 * TO_RADIAN))) * cosf(cocktail_drop_timer2 / 2 * TO_RADIAN);
			cocktail_y -= 0.5f;
		}

		else {
			cocktail_drop = 0;
			cocktail_activated = 1;
			cocktail_x = cocktail_temp_x;
			cocktail_y = cocktail_temp_y;
		}

		if (cocktail_drop_timer == 120)
			drop_switch *= -1;
		
	}

	//at night - fireworks
	if (world_timer >= 1000 && world_timer < 1300)
		firecracker1_seed_y = (world_timer - 1000);

	if(world_timer >= 1100 && world_timer < 1400)
		firecracker2_seed_y = world_timer - 1200;

	if (world_timer == 1300)
		firecracker1_explode = 1;

	if (world_timer == 1400)
		firecracker2_explode = 1;

	if (world_timer > 1300 && world_timer < 1800) {
		firecracker1_spark_x = world_timer - 1300;
		firecracker1_spark_y = world_timer - 1300;	
	}

	if (world_timer > 1400 && world_timer < 1800) {
		firecracker2_spark_x = world_timer - 1400;
		firecracker2_spark_y = world_timer - 1400;
	}

	if (world_timer == 1600) {
		firecracker1_seed_y = 0;
		firecracker2_seed_y = 0;
		firecracker1_explode = 0;
		firecracker2_explode = 0;
	}
	
	//next day
	if (world_timer == 2000) {
		date++;
		house_broken = 0;
		house_damage = 0;
		house_x = house_temp_x;
		house_y = house_temp_y;
		house_rotation = 0;
	}
		
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void item(void) {
	//eat cake
	if (cake_activated == 1) {
		if (char_x >= cake_x - 10.0f && char_x <= cake_x + 10.0f 
			&& char_y >= cake_y - 10.0f && char_y <= cake_y + 10.0f) {
			growup = 1;
			cake_activated = 0;
			fprintf(stdout, "You ate cake.\n");
		}
	}

	//drink cocktail
	if (cocktail_activated == 1) {
		if (char_x >= cocktail_x - 10.0f && char_x <= cocktail_x + 10.0f
			&& char_y >= cocktail_y - 10.0f && char_y <= cocktail_y + 10.0f) {
			drunk = 1;
			cocktail_activated = 0;
			fprintf(stdout, "You drunk.\n");
		}
	}
}

void eat_cake() {
	int growup_timer = timestamp % 301;

	//growing
	if (growup_timer >= 120 && growup_timer <= 200 && growup_done != 1) {
		char_size = (float)growup_timer / 60.0f;
		sword_size = (float)growup_timer / 100.0f;
		slash_size_x = (float)growup_timer / 30.0f;
		slash_size_y = (float)growup_timer / 15.0f;
	}

	//sustain time
	if (growup_timer == 200) {
		growup_done = 1;
		growup_counter++;
	}

	//be small again & initialize
	if (growup_done == 1 && growup_counter >= 5) {
		char_size = 2.0f;
		sword_size = 1.2f;
		slash_size_x = 4.0f;
		slash_size_y = 7.0f;
		growup_counter = 0;
		growup_done = 0;
		growup = 0;
	}
}

void drink_cocktail() {
	int drunk_timer = timestamp % 301;

	//change walking speed
	walking_speed = (float)drunk_timer / 90.0f + 1.0f;	

	//sustain time
	if (drunk_counter >= 5) {
		drunk = 0;
		drunk_counter = 0;
	}

	if (drunk_timer == 300) {
		drunk_counter++;
	}
}

void action_attack() {
	
	//attack when looking right side
	if (looking_R) {
		if (attack == 2.5f 
			&& char_x + 10.0f >= house_x - 80.0f && char_x + 10.0f <= house_x + 45.0f 
			&& char_y >= house_y - 80.0f && char_y <= house_y + 30.0f) {
			house_damage++;
			fprintf(stdout, "You attacked the house %d times!\n", house_damage);
		}
	}

	//attack when looking left side
	else {
		if (attack == 2.5f
			&& char_x - 10.0f >= house_x - 45.0f && char_x - 10.0f <= house_x + 80.0f
			&& char_y >= house_y - 80.0f && char_y <= house_y + 30.0f) {
			house_damage++;
			fprintf(stdout, "You attacked the house %d times!\n", house_damage);
		}
	}
	
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		attack = 2.5f;
		leftbuttonpressed = 1;
		action_attack();
		glutPostRedisplay();
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		attack = 1.0f;
		leftbuttonpressed = 0;
		glutPostRedisplay();
	}
		
}

void keyboard_pressed(unsigned char key, int x, int y) {
	int onair_clock;
	switch (key) {
	case 'd': //move right
		if (char_x < glutGet(GLUT_WINDOW_WIDTH) / 2)
			char_x += walking_speed;
		walking = 1;
		looking_L = 0;
		looking_R = 1;
		item();
		glutPostRedisplay();
		break;

	case 'a': //move left
		if (char_x > - glutGet(GLUT_WINDOW_WIDTH) / 2)
			char_x -= walking_speed;
		walking = 1;
		looking_L = 1;
		looking_R = 0;
		item();
		glutPostRedisplay();
		break;

	case 'w': //move up
		if(char_y < 20.0f)
			char_y += walking_speed;
		walking = 1;
		item();
		glutPostRedisplay();
		break;

	case 's': //move down
		if(char_y > - glutGet(GLUT_WINDOW_HEIGHT) / 2)
			char_y -= walking_speed;
		walking = 1;
		item();
		glutPostRedisplay();
		break;

	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}
}

void keyboard_released(unsigned char key, int x, int y) {
	switch (key) {
	case 'd': //move right
		walking = 0;
		glutPostRedisplay();
		break;

	case 'a': //move left
		walking = 0;
		glutPostRedisplay();
		break;

	case 'w': //move up
		walking = 0;
		glutPostRedisplay();
		break;

	case 's': //move down
		walking = 0;
		glutPostRedisplay();
		break;

	case 32:
		break;
	}
}

void special(int key, int x, int y) {
	switch (key) {
	}
}

void motion(int x, int y) {
	if (leftbuttonpressed) {
		glutPostRedisplay();
	}
}

void reshape(int width, int height) {
	win_width = width, win_height = height;

	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0,
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	update_axes();

	glutPostRedisplay();
}

void display(void) {

	//sky color
	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 0.7f);
	glClear(GL_COLOR_BUFFER_BIT);

	int feetchange_clock = timestamp % 100;
	int item_clock = timestamp % 240;
	int drunk_clock = timestamp % 240;
	int house_clock = timestamp % 360;
	int sunmoon_clock = timestamp & 360;
	float temp_x, temp_y;

	glm::mat4 ModelMatrix;
	ModelMatrix = glm::mat4(1.0f);

	//draw sun&moon
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-glutGet(GLUT_WINDOW_WIDTH)/6, 0.0f, 0.0f));

	if (world_timer >= 600 && world_timer <= 1000) {
		sunmoon_radius = ((float)world_timer - 600.0f) / 400.0f * 180.0f;
	}

	if (world_timer >= 1600 && world_timer <= 2000) {
		sunmoon_radius = ((float)world_timer - 1600.0f) / 400.0f * 180.0f + 180.0f;
	}

	ModelMatrix = glm::rotate(ModelMatrix, (35 - sunmoon_radius) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_sunmoon();
	

	//draw airplane
	if (airplane_turnaround) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airplane_x, airplane_y, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, (-90 - (world_timer - 240)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	else {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airplane_x, airplane_y, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, -90 * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
		
	}
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();

	//draw firecracker
	if (date % 2 == 0) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(firecracker1_seed_x, firecracker1_seed_y, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_spark(1);

		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(firecracker2_seed_x, firecracker2_seed_y, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_spark(0);

		if (firecracker1_explode) {
			for (int j = 0; j < 40; j++) {
				for (int i = 0; i < 36; i++) {
					temp_x = sin(10 * i * TO_RADIAN) * std::max((firecracker1_spark_x - j * 5), 0.0f);
					temp_y = cos(10 * i * TO_RADIAN) * std::max((firecracker1_spark_y - j * 5), 0.0f);
					if (sqrtf(pow(temp_x, 2) + pow(temp_y, 2)) <= 100.0f) {
						ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(firecracker1_seed_x + temp_x, firecracker1_seed_y + temp_y, 0.0f));
						ModelMatrix = glm::rotate(ModelMatrix, 10 * i * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
						ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
						ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
						glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
						draw_spark(j % 3);
					}
				}
			}
		}

		if (firecracker2_explode) {
			for (int j = 0; j < 30; j++) {
				for (int i = 0; i < 36; i++) {
					temp_x = sin(10 * i * TO_RADIAN) * std::max((firecracker2_spark_x - j * 5), 0.0f);
					temp_y = cos(10 * i * TO_RADIAN) * std::max((firecracker2_spark_y - j * 5), 0.0f);
					if (sqrtf(pow(temp_x, 2) + pow(temp_y, 2)) <= 100.0f) {
						ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(firecracker2_seed_x + temp_x, firecracker2_seed_y + temp_y, 0.0f));
						ModelMatrix = glm::rotate(ModelMatrix, 10 * i * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
						ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
						ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
						glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
						draw_spark(j % 3 + 1);
					}
				}
			}
		}
	}
	
	else if(day_night == 1) {
		//draw rain
		for (int i = -60; i < 60; i++) {
			for (int j = -50; j < 50; j++) {
				rain_y = -(world_timer % 500);
				if (rain_y - j * 20 * sin(i) * cos(j) >= 0) {
					ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(rain_x + i * 10, rain_y - j * 20 * sin(i) * cos(j), 0.0f));
					ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
					glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
					draw_rain();
				}
			}
		}
	}
	
	//draw land
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -glutGet(GLUT_WINDOW_HEIGHT) / 4, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(glutGet(GLUT_WINDOW_WIDTH) / 200.f, glutGet(GLUT_WINDOW_HEIGHT) / 150.f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_land();


	//draw house
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(house_x, house_y, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, house_rotation * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(4.0f, 4.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_house();


	if (house_damage >= 10) {
		if (house_clock == 0) {
			house_broken = 1;
		}
			
		if (house_broken == 1) {
			house_rotation = house_clock % 360;
			house_x += 3.0f;
			house_y = -30.0f + 300.0f * sinf((float)house_clock * TO_RADIAN);
		}
	}

	//draw character
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(char_x, char_y, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(char_size, char_size, 1.0f));
	
	if (looking_L == 1) 
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));

	if (drunk == 1) {
		if (drunk_clock < 120)
			ModelMatrix = glm::rotate(ModelMatrix, (30 - (drunk_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		else
			ModelMatrix = glm::rotate(ModelMatrix, (-90 + (drunk_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_char(drunk);

	

	//draw walking step
	if (walking) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(char_x, char_y, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(char_size, char_size, 1.0f));
		if (looking_L == 1)
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		if (feetchange_clock <= 50) {
			draw_feet2();
			char_y += 0.2;
		}
			
		else {
			draw_feet1();
			char_y -= 0.2;
		}
	}
	else {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(char_x, char_y, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(char_size, char_size, 1.0f));
		if (looking_L == 1)
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_feet1();
	}

	//draw sword
	if (looking_L == 1) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(char_x - 10.0f, char_y + 15.0f, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
	}
	else
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(char_x + 10.0f, char_y + 15.0f, 0.0f));

	ModelMatrix = glm::rotate(ModelMatrix, -45 * attack * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(sword_size, sword_size, 1.0f));

	if (drunk == 1) {
		if (drunk_clock < 120)
			ModelMatrix = glm::rotate(ModelMatrix, (30 - (drunk_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		else
			ModelMatrix = glm::rotate(ModelMatrix, (-90 + (drunk_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_sword();

	if (leftbuttonpressed) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(char_x, char_y, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(slash_size_x, slash_size_y, 1.0f));
		if (looking_L == 1)
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_slash();
	}
	
	//cocktail
	if (cocktail_activated == 1) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cocktail_x, cocktail_y, 0.0f));
		if (item_clock < 120)
			ModelMatrix = glm::rotate(ModelMatrix, (30 - (item_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		else
			ModelMatrix = glm::rotate(ModelMatrix, (-90 + (item_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));

		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_cocktail();
	}

	//cocktail drop
	if (cocktail_drop == 1) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cocktail_x, cocktail_y, 0.0f));
		if (item_clock < 120)
			ModelMatrix = glm::rotate(ModelMatrix, (30 - (item_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		else
			ModelMatrix = glm::rotate(ModelMatrix, (-90 + (item_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_cocktail();
	}


	//drink cocktail
	if (drunk == 1 && drunk_counter != -1) 
		drink_cocktail();

	//cake
	if (cake_activated == 1) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cake_x, cake_y, 0.0f));
		if (item_clock < 120)
			ModelMatrix = glm::rotate(ModelMatrix, (30 - (item_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		else
			ModelMatrix = glm::rotate(ModelMatrix, (-90 + (item_clock / 2)) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));

		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_cake();
	}

	//cake drop
	if (cake_drop == 1) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cake_x, cake_y, 0.0f));
		ModelMatrix = glm::rotate(ModelMatrix, cake_rotation * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(3.0f, 3.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_cake();
	}

	//eat cake
	if (growup == 1 && growup_counter != -1) 
		eat_cake();
	
	glFlush();	
}   

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);

	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);

	glDeleteVertexArrays(1, &VAO_house);
	glDeleteBuffers(1, &VBO_house);

	glDeleteVertexArrays(1, &VAO_cocktail);
	glDeleteBuffers(1, &VBO_cocktail);

	glDeleteVertexArrays(1, &VAO_cake);
	glDeleteBuffers(1, &VBO_cake);

	glDeleteVertexArrays(1, &VAO_sword);
	glDeleteBuffers(1, &VBO_sword);

	glDeleteVertexArrays(1, &VAO_spark);
	glDeleteBuffers(1, &VBO_spark);

	glDeleteVertexArrays(1, &VAO_char);
	glDeleteBuffers(1, &VBO_char);

	glDeleteVertexArrays(1, &VAO_feet1);
	glDeleteBuffers(1, &VBO_feet1);

	glDeleteVertexArrays(1, &VAO_feet2);
	glDeleteBuffers(1, &VBO_feet2);

	glDeleteVertexArrays(1, &VAO_slash);
	glDeleteBuffers(1, &VBO_slash);

	glDeleteVertexArrays(1, &VAO_land);
	glDeleteBuffers(1, &VBO_land);

	glDeleteVertexArrays(1, &VAO_sunmoon);
	glDeleteBuffers(1, &VBO_sunmoon);

	glDeleteVertexArrays(1, &VAO_rain);
	glDeleteBuffers(1, &VBO_rain);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard_pressed);
	glutKeyboardUpFunc(keyboard_released);
	glutMouseFunc(mouse);
	glutSpecialFunc(special);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glClearColor(255 / 255.0f, 255 / 255.0f, 255 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_airplane();
	prepare_house();
	prepare_cocktail();
	prepare_cake();
	prepare_sword();
	prepare_spark();
	prepare_char();
	prepare_feet1();
	prepare_feet2();
	prepare_slash();
	prepare_land();
	prepare_sunmoon();
	prepare_rain();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program(); 
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

    error = glewInit();
	if (error != GLEW_OK) { 
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 assignment\n");
	fprintf(stdout, "   20151523 Kim Dong-Hyeon\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 2
int main(int argc, char *argv[]) {
	char program_name[68] = "Sogang CSE4170 HW2: OpenGL API 함수를 사용한 2차원 모델링 변환 연습";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used:	'ESC' : quit the program"
		"\n		'w' : move up"
		"\n		's' : move down"
		"\n		'a' : move left"
		"\n		'd' : move right"
		"\n	 - Mouse used:	L-click : attack"
		"\n	 - Item:	cake : get bigger"
		"\n		cocktail : get drunk"
	};

	glutInit (&argc, argv);
 	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize (1200, 800);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}