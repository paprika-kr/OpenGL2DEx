#pragma once
GLfloat axes[4][2];
GLfloat axes_color[3] = { 0.0f, 0.0f, 0.0f };
GLuint VBO_axes, VAO_axes;

void prepare_axes(void) { // Draw axes in their MC.
	axes[0][0] = -win_width / 2.5f; axes[0][1] = 0.0f;
	axes[1][0] = win_width / 2.5f; axes[1][1] = 0.0f;
	axes[2][0] = 0.0f; axes[2][1] = -win_height / 2.5f;
	axes[3][0] = 0.0f; axes[3][1] = win_height / 2.5f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_axes);
	glBindVertexArray(VAO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_axes(void) {
	axes[0][0] = -win_width / 2.5f; axes[1][0] = win_width / 2.5f;
	axes[2][1] = -win_height / 2.5f;
	axes[3][1] = win_height / 2.5f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_axes(void) {
	glUniform3fv(loc_primitive_color, 1, axes_color);
	glBindVertexArray(VAO_axes);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6
GLfloat big_wing[6][2] = { { 0.0, 0.0 }, { -20.0, 15.0 }, { -20.0, 20.0 }, { 0.0, 23.0 }, { 20.0, 20.0 }, { 20.0, 15.0 } };
GLfloat small_wing[6][2] = { { 0.0, -18.0 }, { -11.0, -12.0 }, { -12.0, -7.0 }, { 0.0, -10.0 }, { 12.0, -7.0 }, { 11.0, -12.0 } };
GLfloat body[5][2] = { { 0.0, -25.0 }, { -6.0, 0.0 }, { -6.0, 22.0 }, { 6.0, 22.0 }, { 6.0, 0.0 } };
GLfloat back[5][2] = { { 0.0, 25.0 }, { -7.0, 24.0 }, { -7.0, 21.0 }, { 7.0, 21.0 }, { 7.0, 24.0 } };
GLfloat sidewinder1[5][2] = { { -20.0, 10.0 }, { -18.0, 3.0 }, { -16.0, 10.0 }, { -18.0, 20.0 }, { -20.0, 20.0 } };
GLfloat sidewinder2[5][2] = { { 20.0, 10.0 }, { 18.0, 3.0 }, { 16.0, 10.0 }, { 18.0, 20.0 }, { 20.0, 20.0 } };
GLfloat center[1][2] = { { 0.0, 0.0 } };
GLfloat airplane_color[7][3] = {
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // big_wing
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // small_wing
	{ 111 / 255.0f,  85 / 255.0f, 157 / 255.0f },  // body
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // back
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder1
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder2
	{ 255 / 255.0f,   0 / 255.0f,   0 / 255.0f }   // center
};

GLuint VBO_airplane, VAO_airplane;

#define AIRPLANE_ROTATION_RADIUS 200.0f
float airplane_s_factor = 1.0f;

void prepare_airplane() {
	GLsizeiptr buffer_size = sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2) + sizeof(center);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(big_wing), big_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing), sizeof(small_wing), small_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing), sizeof(body), body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body), sizeof(back), back);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back),
		sizeof(sidewinder1), sidewinder1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1), sizeof(sidewinder2), sidewinder2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2), sizeof(center), center);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_airplane);
	glBindVertexArray(VAO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_airplane() { // Draw airplane in its MC.
	glBindVertexArray(VAO_airplane);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BIG_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SMALL_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BACK]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER1]);
	glDrawArrays(GL_TRIANGLE_FAN, 22, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER2]);
	glDrawArrays(GL_TRIANGLE_FAN, 27, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_CENTER]);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 32, 1);
	glPointSize(1.0);
	glBindVertexArray(0);
}

//house
#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

GLfloat roof[3][2] = { { -12.0, 0.0 },{ 0.0, 12.0 },{ 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 },{ -12.0, 0.0 },{ 12.0, 0.0 },{ 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 },{ 6.0, 14.0 },{ 10.0, 14.0 },{ 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 },{ -8.0, -8.0 },{ -4.0, -8.0 },{ -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 },{ 4.0, -2.0 },{ 8.0, -2.0 },{ 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
	{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door)
		+ sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}

//draw cocktail
#define COCKTAIL_NECK 0
#define COCKTAIL_LIQUID 1
#define COCKTAIL_REMAIN 2
#define COCKTAIL_STRAW 3
#define COCKTAIL_DECO 4

GLfloat neck[6][2] = { { -6.0, -12.0 },{ -6.0, -11.0 },{ -1.0, 0.0 },{ 1.0, 0.0 },{ 6.0, -11.0 },{ 6.0, -12.0 } };
GLfloat liquid[6][2] = { { -1.0, 0.0 },{ -9.0, 4.0 },{ -12.0, 7.0 },{ 12.0, 7.0 },{ 9.0, 4.0 },{ 1.0, 0.0 } };
GLfloat remain[4][2] = { { -12.0, 7.0 },{ -12.0, 10.0 },{ 12.0, 10.0 },{ 12.0, 7.0 } };
GLfloat straw[4][2] = { { 7.0, 7.0 },{ 12.0, 12.0 },{ 14.0, 12.0 },{ 9.0, 7.0 } };
GLfloat deco[8][2] = { { 12.0, 12.0 },{ 10.0, 14.0 },{ 10.0, 16.0 },{ 12.0, 18.0 },{ 14.0, 18.0 },{ 16.0, 16.0 },{ 16.0, 14.0 },{ 14.0, 12.0 } };

GLfloat cocktail_color[5][3] = {
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 0 / 255.0f, 63 / 255.0f, 122 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 191 / 255.0f, 255 / 255.0f, 0 / 255.0f },
	{ 218 / 255.0f, 165 / 255.0f, 32 / 255.0f }
};

GLuint VBO_cocktail, VAO_cocktail;
void prepare_cocktail() {
	GLsizeiptr buffer_size = sizeof(neck) + sizeof(liquid) + sizeof(remain) + sizeof(straw)
		+ sizeof(deco);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(neck), neck);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck), sizeof(liquid), liquid);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid), sizeof(remain), remain);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid) + sizeof(remain), sizeof(straw), straw);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck) + sizeof(liquid) + sizeof(remain) + sizeof(straw),
		sizeof(deco), deco);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cocktail);
	glBindVertexArray(VAO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cocktail() {
	glBindVertexArray(VAO_cocktail);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_NECK]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_LIQUID]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_REMAIN]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_STRAW]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_DECO]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 8);

	glBindVertexArray(0);
}

// cake
#define CAKE_FIRE 0
#define CAKE_CANDLE 1
#define CAKE_BODY 2
#define CAKE_BOTTOM 3
#define CAKE_DECORATE 4

GLfloat cake_fire[4][2] = { { -0.5, 14.0 },{ -0.5, 13.0 },{ 0.5, 13.0 },{ 0.5, 14.0 } };
GLfloat cake_candle[4][2] = { { -1.0, 8.0 } ,{ -1.0, 13.0 },{ 1.0, 13.0 },{ 1.0, 8.0 } };
GLfloat cake_body[4][2] = { { 8.0, 5.0 },{ -8.0, 5.0 } ,{ -8.0, 8.0 },{ 8.0, 8.0 } };
GLfloat cake_bottom[4][2] = { { -10.0, 1.0 },{ -10.0, 5.0 },{ 10.0, 5.0 },{ 10.0, 1.0 } };
GLfloat cake_decorate[4][2] = { { -10.0, 0.0 },{ -10.0, 1.0 },{ 10.0, 1.0 },{ 10.0, 0.0 } };

GLfloat cake_color[5][3] = {
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 204 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 102 / 255.0f, 51 / 255.0f, 0 / 255.0f }
};

GLuint VBO_cake, VAO_cake;

void prepare_cake() {
	int size = sizeof(cake_fire);
	GLsizeiptr buffer_size = sizeof(cake_fire) * 5;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, size, cake_fire);
	glBufferSubData(GL_ARRAY_BUFFER, size, size, cake_candle);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, cake_body);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, cake_bottom);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, cake_decorate);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cake);
	glBindVertexArray(VAO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cake() {
	glBindVertexArray(VAO_cake);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_FIRE]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_CANDLE]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BOTTOM]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_DECORATE]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glBindVertexArray(0);
}

// sword

#define SWORD_BODY 0
#define SWORD_BODY2 1
#define SWORD_HEAD 2
#define SWORD_HEAD2 3
#define SWORD_IN 4
#define SWORD_DOWN 5
#define SWORD_BODY_IN 6

GLfloat sword_body[4][2] = { { -6.0, 0.0 },{ -6.0, -4.0 },{ 6.0, -4.0 },{ 6.0, 0.0 } };
GLfloat sword_body2[4][2] = { { -2.0, -4.0 },{ -2.0, -6.0 } ,{ 2.0, -6.0 },{ 2.0, -4.0 } };
GLfloat sword_head[4][2] = { { -2.0, 0.0 },{ -2.0, 16.0 } ,{ 2.0, 16.0 },{ 2.0, 0.0 } };
GLfloat sword_head2[3][2] = { { -2.0, 16.0 },{ 0.0, 19.46 } ,{ 2.0, 16.0 } };
GLfloat sword_in[4][2] = { { -0.3, 0.7 },{ -0.3, 15.3 } ,{ 0.3, 15.3 },{ 0.3, 0.7 } };
GLfloat sword_down[4][2] = { { -2.0, -6.0 } ,{ 2.0, -6.0 },{ 4.0, -8.0 },{ -4.0, -8.0 } };
GLfloat sword_body_in[4][2] = { { 0.0, -1.0 } ,{ 1.0, -2.732 },{ 0.0, -4.464 },{ -1.0, -2.732 } };

GLfloat sword_color[7][3] = {
	{ 100 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f }
};

GLuint VBO_sword, VAO_sword;

void prepare_sword() {
	GLsizeiptr buffer_size = sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down) + sizeof(sword_body_in);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sword_body), sword_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body), sizeof(sword_body2), sword_body2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2), sizeof(sword_head), sword_head);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head), sizeof(sword_head2), sword_head2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2), sizeof(sword_in), sword_in);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in), sizeof(sword_down), sword_down);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down), sizeof(sword_body_in), sword_body_in);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_sword);
	glBindVertexArray(VAO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_sword() {
	glBindVertexArray(VAO_sword);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY2]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD2]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_DOWN]);
	glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 23, 4);

	glBindVertexArray(0);
}

//spark
#define SPARK 0
#define SPARK_RED 1
#define SPARK_GREEN 2
#define SPARK_PURPLE 3

GLfloat spark[5][2] = { { -1.5, 0.0 },{ 1.5, 0.0 },{ 1.5, 3.0 }, {0.0, 5.0}, {-1.5, 3.0} };


GLfloat spark_color[4][3] = {
	{ 255 / 255.0f, 102 / 255.0f, 0 / 255.0f },
	{ 255 / 255.0f, 000 / 255.0f, 102 / 255.0f },
	{ 204 / 255.0f, 255 / 255.0f, 51 / 255.0f },
	{ 204 / 255.0f, 0 / 255.0f, 204 / 255.0f }
};

GLuint VBO_spark, VAO_spark;
void prepare_spark() {
	GLsizeiptr buffer_size = sizeof(spark);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_spark);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_spark);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(spark), spark);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_spark);
	glBindVertexArray(VAO_spark);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_spark);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_spark(int num) {
	switch (num)
	{
	case SPARK:
		glBindVertexArray(VAO_spark);
		glUniform3fv(loc_primitive_color, 1, spark_color[SPARK]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		glBindVertexArray(0);
		break;

	case SPARK_RED:
		glBindVertexArray(VAO_spark);
		glUniform3fv(loc_primitive_color, 1, spark_color[SPARK_RED]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		glBindVertexArray(0);
		break;

	case SPARK_GREEN: 
		glBindVertexArray(VAO_spark);
		glUniform3fv(loc_primitive_color, 1, spark_color[SPARK_GREEN]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		glBindVertexArray(0);
		break;

	case SPARK_PURPLE:
		glBindVertexArray(VAO_spark);
		glUniform3fv(loc_primitive_color, 1, spark_color[SPARK_PURPLE]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		glBindVertexArray(0);
		break;

	default:
		glBindVertexArray(VAO_spark);
		glUniform3fv(loc_primitive_color, 1, spark_color[SPARK]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		glBindVertexArray(0);
		break;
	}

}

//character
#define CHAR_HAIR 0
#define CHAR_HEAD 1
#define CHAR_EYE 2
#define CHAR_BODY 3
#define CHAR_HAND 4
#define CHAR_DRUNK 5

GLfloat char_hair[5][2] = { { 0.0, 14.0 },{ 6.0, 20.0 },{ -3.0, 20.0 },{ -5.0, 12.0 }, {-2.0, 9.0} };
GLfloat char_head[5][2] = { { -2.0, 8.5 },{ 4.5, 8.5 } ,{ 5.0, 10.0 },{ 5.0, 19.0 }, {0.0, 14.0} };
GLfloat char_eye[4][2] = { { 4.0, 16.0 },{ 3.0, 16.0 } ,{ 3.0, 12.0 },{ 4.0, 12.0 } };
GLfloat char_body[5][2] = { { -3.0, 9.0 },{ -4.0, 6.0 } ,{ -3.0, 3.0 }, {6.0, 3.0}, {6.0, 9.0} };
GLfloat char_hand[4][2] = { {-1.0, 6.0 },{ -1.0, 5.0 } ,{ 1.0, 5.0 },{ 1.0, 6.0 } };

GLfloat char_color[6][3] = {
	{ 204 / 255.0f, 102 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 204 / 255.0f, 204 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 51 / 255.0f, 0 / 255.0f, 153 / 255.0f },
{ 255 / 255.0f, 204 / 255.0f, 204 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 102 / 255.0f}
};

GLuint VBO_char, VAO_char;

void prepare_char() {
	GLsizeiptr buffer_size = sizeof(char_hair) + sizeof(char_head) + sizeof(char_eye) + sizeof(char_body) + sizeof(char_hand);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_char);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_char);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(char_hair), char_hair);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(char_hair), sizeof(char_head), char_head);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(char_hair) + sizeof(char_head), sizeof(char_eye), char_eye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(char_hair) + sizeof(char_head) + sizeof(char_eye), sizeof(char_body), char_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(char_hair) + sizeof(char_head) + sizeof(char_eye) + sizeof(char_body), sizeof(char_hand), char_hand);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_char);
	glBindVertexArray(VAO_char);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_char);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_char(int drunk) {
	if (drunk == 0) {
		glBindVertexArray(VAO_char);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_HAIR]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_HEAD]);
		glDrawArrays(GL_TRIANGLE_FAN, 5, 5);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_EYE]);
		glDrawArrays(GL_TRIANGLE_FAN, 10, 4);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_BODY]);
		glDrawArrays(GL_TRIANGLE_FAN, 14, 5);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_HAND]);
		glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

		glBindVertexArray(0);
	}

	else {
		glBindVertexArray(VAO_char);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_HAIR]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_DRUNK]);
		glDrawArrays(GL_TRIANGLE_FAN, 5, 5);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_EYE]);
		glDrawArrays(GL_TRIANGLE_FAN, 10, 4);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_BODY]);
		glDrawArrays(GL_TRIANGLE_FAN, 14, 5);

		glUniform3fv(loc_primitive_color, 1, char_color[CHAR_HAND]);
		glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

		glBindVertexArray(0);
	}
	
}

//feet1
#define FEET1 0

GLfloat feet1[4][2] = { { -1.5, 0.0 },{ 4.0, 0.0 },{ 3.0, 2.0 }, {-1.5, 2.0}};


GLfloat feet1_color[1][3] = {
	{ 153 / 255.0f, 102 / 255.0f, 51 / 255.0f }
};

GLuint VBO_feet1, VAO_feet1;
void prepare_feet1() {
	GLsizeiptr buffer_size = sizeof(feet1);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_feet1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_feet1);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feet1), feet1);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_feet1);
	glBindVertexArray(VAO_feet1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_feet1);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_feet1() {
	glBindVertexArray(VAO_feet1);

	glUniform3fv(loc_primitive_color, 1, feet1_color[FEET1]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
}

//feet2
#define FEET2_LEFT 0
#define FEET2_RIGHT 1

GLfloat feet2_left[4][2] = { { -2.5, 3.0 },{ -4.0, 1.5 },{ -1.0, 0.0 }, {-1.0, 1.5} };
GLfloat feet2_right[4][2] = { { 6.0, 3.0 },{ 4.0, 3.0 },{ 2.0, 2.0 }, {3.0, 0.0} };


GLfloat feet2_color[2][3] = {
	{ 153 / 255.0f, 102 / 255.0f, 51 / 255.0f },
	{ 153 / 255.0f, 102 / 255.0f, 51 / 255.0f }
};

GLuint VBO_feet2, VAO_feet2;
void prepare_feet2() {
	GLsizeiptr buffer_size = sizeof(feet2_left) + sizeof(feet2_right);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_feet2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_feet2);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feet2_left), feet2_left);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(feet2_left), sizeof(feet2_right), feet2_right);
	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_feet2);
	glBindVertexArray(VAO_feet2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_feet2);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_feet2() {
	glBindVertexArray(VAO_feet2);

	glUniform3fv(loc_primitive_color, 1, feet2_color[FEET2_LEFT]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, feet2_color[FEET2_RIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 8);

	glBindVertexArray(0);
}

//slash
#define SLASH 0

GLfloat slash[4][2] = { { 8.0, 1.0 },{ 8.0, 3.0 },{ 3.0, 8.0 }, {9.0, 3.0} };

GLfloat slash_color[1][3] = {
	{ 255 / 255.0f, 255 / 255.0f, 50 / 255.0f }
};

GLuint VBO_slash, VAO_slash;
void prepare_slash() {
	GLsizeiptr buffer_size = sizeof(slash);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_slash);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_slash);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(slash), slash);
	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_slash);
	glBindVertexArray(VAO_slash);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_slash);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_slash() {
	glBindVertexArray(VAO_slash);

	glUniform3fv(loc_primitive_color, 1, slash_color[SLASH]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
}

//land
#define LAND_LAYER1 0
#define LAND_LAYER2 1

GLfloat land_layer1[4][2] = { { -100.0f, 50.0f },{ 100.0f, 50.0f },{ 100.0f, -50.0f }, { -100.0f, -50.0f} };
GLfloat land_layer2[4][2] = { { -100.0f + 5.0f, 50.0f - 7.0f },{ 100.0f - 5.0f, 50.0f - 7.0f },{ 100.0f - 5.0f, -50.0f + 7.0f}, {-100.0f + 5.0f, -50.0f + 7.0f} };


GLfloat land_color[2][3] = {
	{ 102 / 255.0f, 51 / 255.0f, 0 / 255.0f },
	{ 204 / 255.0f, 153 / 255.0f, 102 / 255.0f }
};

GLuint VBO_land, VAO_land;
void prepare_land() {
	GLsizeiptr buffer_size = sizeof(land_layer1) + sizeof(land_layer2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_land);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_land);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(land_layer1), land_layer1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(land_layer1), sizeof(land_layer2), land_layer2);
	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_land);
	glBindVertexArray(VAO_land);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_land);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_land() {
	glBindVertexArray(VAO_land);

	glUniform3fv(loc_primitive_color, 1, land_color[LAND_LAYER1]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, land_color[LAND_LAYER2]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 8);

	glBindVertexArray(0);
}


//sun&moon
#define SUN_ROUND 0
#define MOON_ROUND 1


GLfloat sun_round[8][2] = { {-10.0, 384.0}, { 10.0, 384.0 },{ 34.0, 360.0 }, {34.0, 340.0}, {10.0, 316.0}, {-10.0, 316.0}, {-34.0, 340.0}, {-34.0, 360.0} };
GLfloat moon_round[6][2] = { {-20.0, -374.0}, {24.0, -330.0}, {10.0, -316.0}, {-10.0, -316.0}, {-34.0, -340.0}, {-34.0, -360.0} };



GLfloat sunmoon_color[2][3] = {
	{ 255 / 255.0f, 102 / 255.0f, 102 / 255.0f },
	{ 255 / 255.0f, 255 / 255.0f, 0 / 255.0f }
};

GLuint VBO_sunmoon, VAO_sunmoon;
void prepare_sunmoon() {
	GLsizeiptr buffer_size = sizeof(sun_round) + sizeof(moon_round);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_sunmoon);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sunmoon);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sun_round), sun_round);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sun_round), sizeof(moon_round), moon_round);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_sunmoon);
	glBindVertexArray(VAO_sunmoon);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sunmoon);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_sunmoon() {
	glBindVertexArray(VAO_sunmoon);

	glUniform3fv(loc_primitive_color, 1, sunmoon_color[SUN_ROUND]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, sunmoon_color[MOON_ROUND]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 6);

	glBindVertexArray(0);
}

#define RAIN 0

GLfloat rain[4][2] = { { -0.5, 1.5 },{ 0.5, 1.5 },{ 0.5, -1.5 }, { -0.5, -1.5} };

GLfloat rain_color[1][3] = {
	{ 204 / 255.0f, 255 / 255.0f, 255 / 255.0f }
};

GLuint VBO_rain, VAO_rain;
void prepare_rain() {
	GLsizeiptr buffer_size = sizeof(rain);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_rain);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_rain);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(rain), rain);
	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_rain);
	glBindVertexArray(VAO_rain);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_rain);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_rain() {
	glBindVertexArray(VAO_rain);

	glUniform3fv(loc_primitive_color, 1, rain_color[RAIN]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);
}
