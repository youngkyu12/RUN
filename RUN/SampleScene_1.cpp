﻿#include "pch.h"
#include <Shader.h>
#include <Figure.h>
#include <Camera.h>
#include "SampleScene_1.h"
#include <Engine.h>

extern Engine engine;

SampleScene_1::SampleScene_1() :
#pragma region [BASE VARIABLE]
	_camera{ std::make_shared<Camera>(glm::vec3{ 3.f, 3.f, 3.f }, -90.f - 45.f, -30.f) },
	_color_shader{ std::make_shared<Shader>() },
	_stop_animation{ true },
	_animation_speed{ 100 },
	_click{ false },
	_old_x{ 0 },
	_old_y{ 0 },
	_time{ glutGet(GLUT_ELAPSED_TIME) },
	_old_time{ _time },
	_delta_time{ 0.f }
#pragma endregion
#pragma region [USER-DEFINE METHOD]

#pragma endregion
{
#if _DEBUG
	_color_shader->OnLoad("../Dependencies/shader/Vertex.glsl", "../Dependencies/shader/Color.glsl");
#else
	_color_shader->OnLoad("Data/Shader/Vertex.glsl", "Data/Shader/Color.glsl");
#endif
	CreateObjects();
	CreateGrid();
}

SampleScene_1::~SampleScene_1()
{
	OnRelease();
}

#pragma region [PRE-DEFINED METHOD]
// 정의한 모든 객체 로드
void SampleScene_1::OnLoad()
{
	LoadMultipleObject(&_object, _color_shader);
	LoadMultipleObject(&_grid, _color_shader);
}

// 동적할당한 모든 객체 할당 해제
void SampleScene_1::OnRelease()
{
	ReleaseMultipleObject(&_object);
	ReleaseMultipleObject(&_grid);
}

void SampleScene_1::OnIdleMessage()
{
	CalculateDeltaTime();
}

void SampleScene_1::OnKeyboardMessage(uchar key, int32_t x, int32_t y)
{
	switch (key)
	{
		case 'X':
		{
			RotateX_Cube(define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'x':
		{
			RotateX_Cube(define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'Y':
		{
			Rotate_y_Cube(define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'y':
		{
			Rotate_y_Cube(define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'A': 
		{
			RotateX_Cone(define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'a':
		{
			RotateX_Cone(define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'B':
		{
			Rotate_y_Cone(define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'b':
		{
			Rotate_y_Cone(define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'R': 
		{
			RotateY_Cube(define::ROTATE_DIRECTION::CLOCK);
			RotateY_Cone(define::ROTATE_DIRECTION::CLOCK);
		}
		break;
		case 'r':
		{
			RotateY_Cube(define::ROTATE_DIRECTION::COUNTER_CLOCK);
			RotateY_Cone(define::ROTATE_DIRECTION::COUNTER_CLOCK);
		}
		break;
		case 'C':
		{
			ChangeRenderObject(0, OBJECT::CUBE_0);
			ChangeRenderObject(1, OBJECT::CONE_1);
		}
		break;
		case 'c':
		{
			ChangeRenderObject(0, OBJECT::CONE_0);
			ChangeRenderObject(1, OBJECT::CUBE_1);
		}
		break;
		case 's':
		{
		}
		break;
	}

}

void SampleScene_1::OnSpecialKeyMessage(int32_t key, int32_t x, int32_t y)
{
	_camera->OnSpecialKeyMessage(key, x, y, _delta_time);
}

void SampleScene_1::OnMouseMessage(int32_t button, int32_t x, int32_t y)
{
	float x2{ Convert::ToFloat(x) / (engine.GetWindowWidth() / 2) - 1.f };
	float y2{ 1.f - Convert::ToFloat(y) / (engine.GetWindowHeight() / 2) };

	if (button == GLUT_LEFT_BUTTON)
	{
		_click = true;
	}
}

void SampleScene_1::OnMouseMotionMessage(int32_t x, int32_t y)
{
	float x2{ Convert::ToFloat(x) / (engine.GetWindowWidth() / 2) - 1.f };
	float y2{ 1.f - Convert::ToFloat(y) / (engine.GetWindowHeight() / 2) };

	if (_click)
	{
		_old_x = x;
		_old_y = y;
		_click = false;
	}

	float delta_x{ Convert::ToFloat(x - _old_x) };
	float delta_y{ Convert::ToFloat(_old_y - y) };
	_old_x = x;
	_old_y = y;

	_camera->OnMouseMotionMessage(delta_x, delta_y);
}

void SampleScene_1::OnMouseUpMessage(int32_t button, int32_t x, int32_t y)
{
}

void SampleScene_1::OnAnimate(int32_t index)
{
	if (_stop_animation == false)
	{
		glutTimerFunc(_animation_speed, Engine::OnAnimate, index);
	}
}

void SampleScene_1::OnRender()
{
	glClearColor(BLACK, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// TODO : Object render
	RenderMultipleObject(&_grid, _color_shader);
	RenderMultipleObject(&_render_object, _color_shader);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

// Delta time 계산 함수
void SampleScene_1::CalculateDeltaTime()
{
	_time = glutGet(GLUT_ELAPSED_TIME);

	_delta_time = Convert::ToFloat((_time - _old_time)) / 1000.f;
	_old_time = _time;
}

void SampleScene_1::LoadSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	object->OnLoad(shader);
}

void SampleScene_1::ReleaseSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	delete object;
	object = nullptr;
}

void SampleScene_1::LoadMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader)
{
	for (auto& obj : *object)
	{
		obj->OnLoad(shader);
	}
}

void SampleScene_1::ReleaseMultipleObject(std::vector<Object*>* object)
{
	// 객체들은 Object class를 동적할당 해서 생성했으므로 할당 해제
	for (auto& obj : *object)
	{
		delete obj;
		obj = nullptr;
	}
}

void SampleScene_1::ViewProjection(std::shared_ptr<Shader>& shader)
{
	auto view{ _camera->GetViewMatrix() };
	auto projection{ _camera->GetProjectionMatrix() };

	shader->SetMat4("view", &view);
	shader->SetMat4("projection", &projection);
}

void SampleScene_1::RenderSingleObject(Object* object, std::shared_ptr<Shader>& shader)
{
	// shader program 사용
	shader->OnUse();
	// 뷰 변환 및 투영 변환 적용
	ViewProjection(shader);

	// VAO 바인드
	object->BindVAO();

	// 월드 변환
	object->Transform(shader);

	// 객체의 색상을 셰이더에 적용
	object->ApplyColor();

	glDrawElements(object->GetDrawType(), object->GetIndexNum(), GL_UNSIGNED_INT, 0);
}

void SampleScene_1::RenderMultipleObject(std::vector<Object*>* object, std::shared_ptr<Shader>& shader)
{
	// shader program 사용
	shader->OnUse();
	// 뷰 변환 및 투영 변환 적용
	ViewProjection(shader);

	for (auto& obj : *object)
	{
		// VAO 바인드
		obj->BindVAO();

		// 월드 변환
		obj->Transform(shader);

		// 객체의 색상을 셰이더에 적용
		obj->ApplyColor();

		glDrawElements(obj->GetDrawType(), obj->GetIndexNum(), GL_UNSIGNED_INT, 0);
	}
}

#pragma endregion
#pragma region [USER-DEFINED METHOD]

void SampleScene_1::CreateObjects()
{
	_object.resize(4, nullptr);

	_object[CUBE_0] = new Cube{};
	_object[CUBE_0]->Move(glm::vec3(-1.5f, 0.5f, -0.5f));
	_object[CUBE_0]->SetShader(_color_shader);
	_object[CUBE_0]->SetColor(RAND_COLOR);

	_object[CONE_0] = new Cone{};
	_object[CONE_0]->Move(glm::vec3(-1.5f, 0.5f, -0.5f));
	_object[CONE_0]->SetShader(_color_shader);
	_object[CONE_0]->SetColor(RAND_COLOR);

	_object[CONE_1] = new Cone{};
	_object[CONE_1]->Move(glm::vec3(1.5f, 0.5f, -0.5f));
	_object[CONE_1]->SetShader(_color_shader);
	_object[CONE_1]->SetColor(RAND_COLOR);

	_object[CUBE_1] = new Cube{};
	_object[CUBE_1]->Move(glm::vec3(1.5f, 0.5f, -0.5f));
	_object[CUBE_1]->SetShader(_color_shader);
	_object[CUBE_1]->SetColor(RAND_COLOR);

	_render_object.resize(2, nullptr);

	_render_object[0] = _object[CUBE_0];
	_render_object[1] = _object[CONE_1];
}

void SampleScene_1::CreateGrid()
{
	_grid.resize(3, nullptr);

	// x축
	_grid[0] = new Line{};
	_grid[0]->SetShader(_color_shader);
	_grid[0]->SetColor(RAND_COLOR);

	// y축
	_grid[1] = new Line{};
	_grid[1]->RotateZ(90.f);
	_grid[1]->SetShader(_color_shader);
	_grid[1]->SetColor(RAND_COLOR);

	// z축
	_grid[2] = new Line{};
	_grid[2]->RotateY(90.f);
	_grid[2]->SetShader(_color_shader);
	_grid[2]->SetColor(RAND_COLOR);
}

void SampleScene_1::RotateX_Cube(define::ROTATE_DIRECTION direction)
{
	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object[0]->RotateX(direction);
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object[0]->RotateX(direction);
}

void SampleScene_1::RotateY_Cube(define::ROTATE_DIRECTION direction)
{
	// y축에 대하여 양(시계)의 방향으로 회전
	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object[0]->RotateY(direction);
	// y축에 대하여 음(반시계)의 방향으로 회전
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object[0]->RotateY(direction);
}

void SampleScene_1::Rotate_y_Cube(define::ROTATE_DIRECTION direction)
{
	_render_object[0]->Move(glm::vec3(1.5f, 0.0f, 0.0f));
	// y축에 대하여 양(시계)의 방향으로 회전
	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object[0]->RotateY(direction);
	// y축에 대하여 음(반시계)의 방향으로 회전
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object[0]->RotateY(direction);
	_render_object[0]->Move(glm::vec3(-1.5f, 0.0f, 0.0f));

}

void SampleScene_1::RotateX_Cone(define::ROTATE_DIRECTION direction)
{
	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object[1]->RotateX(direction);
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object[1]->RotateX(direction);
}

void SampleScene_1::RotateY_Cone(define::ROTATE_DIRECTION direction)
{
	// y축에 대하여 양(시계)의 방향으로 회전
	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object[1]->RotateY(direction);
	// y축에 대하여 음(반시계)의 방향으로 회전
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object[1]->RotateY(direction);
}

void SampleScene_1::Rotate_y_Cone(define::ROTATE_DIRECTION direction)
{
	_render_object[1]->Move(glm::vec3(-1.5f, 0.0f, 0.0f));
	// y축에 대하여 양(시계)의 방향으로 회전
	if (direction == define::ROTATE_DIRECTION::CLOCK)
		_render_object[1]->RotateY(direction);
	// y축에 대하여 음(반시계)의 방향으로 회전
	else if (direction == define::ROTATE_DIRECTION::COUNTER_CLOCK)
		_render_object[1]->RotateY(direction);
	_render_object[1]->Move(glm::vec3(1.5f, 0.0f, 0.0f));
}

void SampleScene_1::ChangeRenderObject(int index, OBJECT obj_type)
{
	// 화면에 그릴 객체를 obj_type으로 변경
	_render_object[index] = _object[obj_type];
	_render_object[index] = _object[obj_type];
	
}

#pragma endregion