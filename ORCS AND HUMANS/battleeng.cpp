#include "pch.h"
#include "battleeng.h"

// RANDIT ************************

dll::RandIt::RandIt()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}
dll::RandIt::RandIt(RandIt& other)
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}
dll::RandIt::~RandIt()
{
	if (twister)delete twister;
}

int dll::RandIt::operator()(int min, int max)
{
	std::uniform_int_distribution distrib(min, max);

	return distrib((*twister));
}

//////////////////////////////////

// EXCEPTION ********************

dll::EXCEPTION::EXCEPTION(int what) :error_occurred(what) {};

LPCWSTR dll::EXCEPTION::GetError()const
{
	if (error_occurred == 1)return error1;

	return error2;
}


// PROTON ***********************

dll::PROTON::PROTON()
{
	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	radiusX = _width / 2.0f;
	radiusY = _height / 2.0f;

	center.x = start.x + radiusX;
	center.y = start.y + radiusY;
}
dll::PROTON::PROTON(float _sx, float _sy)
{
	start.x = _sx;
	start.y = _sy;

	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	radiusX = _width / 2.0f;
	radiusY = _height / 2.0f;

	center.x = start.x + radiusX;
	center.y = start.y + radiusY;
}
dll::PROTON::PROTON(float _sx, float _sy, float _swidth, float _sheight)
{
	start.x = _sx;
	start.y = _sy;

	_width = _swidth;
	_height = _sheight;

	end.x = start.x + _width;
	end.y = start.y + _height;

	radiusX = _width / 2.0f;
	radiusY = _height / 2.0f;

	center.x = start.x + radiusX;
	center.y = start.y + radiusY;
}

void dll::PROTON::SetEdges()
{
	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + radiusX;
	center.y = start.y + radiusY;
}
void dll::PROTON::NewWidth(float _new_width)
{
	_width = _new_width;

	end.x = start.x + _width;

	radiusX = _width / 2.0f;

	center.x = start.x + radiusX;
}
void dll::PROTON::NewHeight(float _new_height)
{
	_height = _new_height;

	end.y = start.y + _height;

	radiusY = _height / 2.0f;

	center.y = start.y + radiusY;
}
void dll::PROTON::NewDims(float _new_width, float _new_height)
{
	_width = _new_width;
	_height = _new_height;

	end.x = start.x + _width;
	end.y = start.y + _height;

	radiusX = _width / 2.0f;
	radiusY = _height / 2.0f;

	center.x = start.x + radiusX;
	center.y = start.y + radiusY;
}

float dll::PROTON::GetWidth() const
{
	return _width;
}
float dll::PROTON::GetHeight() const
{
	return _height;
}

/////////////////////////////////

// ASSETS **********************

int dll::ASSETS::global_counter = 0;

dll::ASSETS::ASSETS(obstacle what_type, float start_x, float start_y) :PROTON(start_x, start_y)
{
	type = what_type;

	switch (type)
	{
	case obstacle::rock:
		NewDims(100.0f, 100.0f);
		break;

	case obstacle::orc_castle:
		NewDims(210.0f, 200.0f);
		goods_delay = 100;
		goods_in_asset = 1000;
		lifes = 1000;
		break;

	case obstacle::human_castle:
		NewDims(260.0f, 260.0f);
		goods_delay = 100;
		goods_in_asset = 1000;
		lifes = 1200;
		max_frames = 7;
		frame_delay = 9;
		break;

	case obstacle::small_tree:
		NewDims(24.0f, 40.0f);
		goods_delay = 60;
		lifes = 60;
		goods_in_asset = 10;
		break;

	case obstacle::mid_tree:
		NewDims(39.0f, 43.0f);
		goods_delay = 80;
		lifes = 80;
		goods_in_asset = 15;
		break;

	case obstacle::big_tree:
		NewDims(49.0f, 45.0f);
		goods_delay = 90;
		lifes = 100;
		goods_in_asset = 20;
		break;

	case obstacle::mine:
		NewDims(100.0f, 135.0f);
		goods_delay = 100;
		lifes = 150;
		goods_in_asset = 50;
		max_frames = 124;
		frame_delay = 1;
		break;
	}

	max_frame_delay = frame_delay;
	max_goods_delay = goods_delay;

	number = global_counter;
	++global_counter;
}
int dll::ASSETS::GetFrame()
{
	frame_delay--;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > max_frames)frame = 0;
	}
	return frame;
}
int dll::ASSETS::GetMyNumber() const
{
	return number;
}
int dll::ASSETS::GetGoods()
{
	if (goods_delay > 0)goods_delay--;
	else
	{
		goods_delay = max_goods_delay;
		lifes--;
		if (lifes <= 0)return goods_in_asset;
	}

	return 0;
}
void dll::ASSETS::Release()
{
	delete this;
}

////////////////////////////////

// UNITS **********************

int dll::UNITS::global_counter = 0;

dll::UNITS::UNITS(unit_type what, float start_x, float start_y) :PROTON(start_x, start_y)
{
	type = what;

	RandIt rand_gen{};

	switch (type)
	{
	case unit_type::peon:
		NewDims(85.0f, 52.0f);
		lifes = 50 + rand_gen(0, 20);
		speed = 0.5f + (float)(rand_gen(0, 2)) / 10.0f;
		strenght = 5 + rand_gen(0, 5);
		range = 150.0f;
		max_frames = 40;
		max_frame_delay = 2;
		frame_delay = max_frame_delay;
		attack_delay = 100;
		break;

	case unit_type::orc_warrior:
		NewDims(117.0f, 100.0f);
		lifes = 100 + rand_gen(0, 30);
		speed = 0.8f + (float)(rand_gen(0, 3)) / 10.0f;
		strenght = 25 + rand_gen(0, 5);
		range = 200.0f;
		max_frames = 9;
		max_frame_delay = 7;
		frame_delay = max_frame_delay;
		attack_delay = 80;
		break;

	case unit_type::orc_knight:
		NewDims(150.0f, 101.0f);
		lifes = 100 + rand_gen(0, 30);
		speed = 0.4f + (float)(rand_gen(0, 3)) / 10.0f;
		strenght = 50 + rand_gen(0, 10);
		range = 200.0f;
		max_frames = 30;
		max_frame_delay = 2;
		frame_delay = max_frame_delay;
		attack_delay = 80;
		break;

	case unit_type::orc_archer:
		NewDims(100.0f, 106.0f);
		lifes = 70 + rand_gen(0, 30);
		speed = 0.6f + (float)(rand_gen(0, 3)) / 10.0f;
		strenght = 40 + rand_gen(0, 5);
		range = 250.0f;
		max_frames = 18;
		max_frame_delay = 3;
		frame_delay = max_frame_delay;
		attack_delay = 120;
		break;

	case unit_type::peasant:
		NewDims(80.0f, 85.0f);
		lifes = 50 + rand_gen(0, 20);
		speed = 0.5f + (float)(rand_gen(0, 2)) / 10.0f;
		strenght = 5 + rand_gen(0, 5);
		range = 150.0f;
		max_frames = 19;
		max_frame_delay = 4;
		frame_delay = max_frame_delay;
		attack_delay = 100;
		break;

	case unit_type::warrior:
		NewDims(40.0f, 90.0f);
		lifes = 100 + rand_gen(0, 30);
		speed = 0.8f + (float)(rand_gen(0, 3)) / 10.0f;
		strenght = 25 + rand_gen(0, 5);
		range = 200.0f;
		max_frames = 3;
		max_frame_delay = 10;
		frame_delay = max_frame_delay;
		attack_delay = 80;
		break;

	case unit_type::knight:
		NewDims(100.0f, 103.0f);
		lifes = 100 + rand_gen(0, 30);
		speed = 0.4f + (float)(rand_gen(0, 3)) / 10.0f;
		strenght = 50 + rand_gen(0, 10);
		range = 200.0f;
		max_frames = 24;
		max_frame_delay = 3;
		frame_delay = max_frame_delay;
		attack_delay = 80;
		break;

	case unit_type::archer:
		NewDims(67.0f, 90.0f);
		lifes = 70 + rand_gen(0, 30);
		speed = 0.6f + (float)(rand_gen(0, 3)) / 10.0f;
		strenght = 40 + rand_gen(0, 5);
		range = 250.0f;
		max_frames = 41;
		max_frame_delay = 2;
		frame_delay = max_frame_delay;
		attack_delay = 120;
		break;
	}

	max_attack_delay = attack_delay;
	number = global_counter;
	++global_counter;
}
void dll::UNITS::SetPath(float _where_x, float _where_y)
{
	hor_path = false;
	vert_path = false;

	start_move_x = start.x;
	start_move_y = start.y;

	final_move_x = _where_x;
	final_move_y = _where_y;

	if (start_move_x < final_move_x)dir = dirs::right;
	else dir = dirs::left;

	if (final_move_x == start_move_x || (final_move_x > start_move_x && final_move_x < end.x))
	{
		vert_path = true;
		return;
	}
	if (final_move_y == start_move_y || (final_move_y > start_move_y && final_move_y < end.y))
	{
		hor_path = true;
		return;
	}

	slope = (final_move_y - start_move_y) / (final_move_x - start_move_x);
	intercept = start_move_y - start_move_x * slope;
}
void dll::UNITS::Release()
{
	delete this;
}
int dll::UNITS::GetFrame()
{
	frame_delay--;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++frame;
		if (frame > max_frames)frame = 0;
	}
	return frame;
}
int dll::UNITS::GetMyNumber() const
{
	return number;
}
void dll::UNITS::Move(float gear, BAG<ASSETS>obstacles)
{
	if (obstacles.size() < 1)return;

	float now_speed = speed * gear / 10.0f;

	char obstacle = 0b00000000;

	char up = 0b00000001;
	char down = 0b00000010;
	char left = 0b00000100;
	char right = 0b00001000;

	char up_left = 0b00000101;
	char up_right = 0b00001001;
	char down_left = 0b00000110;
	char down_right = 0b00001010;

	float remaining_x{ 0 };
	float remaining_y{ 0 };

	if (final_move_x > start_move_x)remaining_x = final_move_x - start_move_x;
	else remaining_x = start_move_x - final_move_x;

	if (final_move_y > start_move_y)remaining_y = final_move_y - start_move_y;
	else remaining_y = start_move_y - final_move_y;

	for (size_t i = 0; i < obstacles.size(); ++i)
	{
		if (Intersect(FRECT(FPOINT(start.x, start.y), FPOINT(end.x, start.y), FPOINT(start.x, end.y), FPOINT(end.x, end.y)),
			FRECT(FPOINT(obstacles[i].start.x, obstacles[i].start.y),
				FPOINT(obstacles[i].end.x, obstacles[i].start.y),
				FPOINT(obstacles[i].start.x, obstacles[i].end.y),
				FPOINT(obstacles[i].end.x, obstacles[i].end.y))))
		{

			if (obstacles[i].type == obstacle::small_tree || obstacles[i].type == obstacle::mid_tree
				|| obstacles[i].type == obstacle::big_tree || obstacles[i].type == obstacle::mine)
				current_action = actions::harvest;
			
			if (current_action == actions::harvest)break;

			if (start.y >= obstacles[i].start.y && start.y <= obstacles[i].end.y)obstacle |= up;
			if (end.y >= obstacles[i].start.y && end.y <= obstacles[i].end.y)obstacle |= down;

			if (end.x >= obstacles[i].start.x && end.x <= obstacles[i].end.x)obstacle |= right;
			if (start.x >= obstacles[i].start.x && start.x <= obstacles[i].end.x)obstacle |= left;

			break;
		}

	}

	if (obstacle != 0)
	{
		if (obstacle & up_right)SetPath(start.x - remaining_x, end.y + remaining_y);
		else if (obstacle & up_left)SetPath(start.x + remaining_x, end.y + remaining_y);
		else if (obstacle & down_right)SetPath(start.x - remaining_x, end.y - remaining_y);
		else if (obstacle & down_left)SetPath(start.x + remaining_x, end.y - remaining_y);
		else if (obstacle & up)SetPath(start.x, end.y + remaining_y);
		else if (obstacle & down)SetPath(start.x, start.y - remaining_y);
		else if (obstacle & left)SetPath(start.x + remaining_x, end.y);
		else if (obstacle & right)SetPath(start.x - remaining_x, start.y);
	}

	if (vert_path)
	{
		if (start_move_y > final_move_y)
		{
			if (start.y - now_speed >= sky && start.y - now_speed > final_move_y)
			{
				start.y -= now_speed;
				SetEdges();
				return;
			}
		}
		else if (end.y + now_speed <= ground && end.y + now_speed < final_move_y)
		{
			start.y += now_speed;
			SetEdges();
			return;
		}
	}
	if (hor_path)
	{
		if (start_move_x > final_move_x && start.x - now_speed > final_move_x)
		{
			if (start.x - now_speed >= 0)
			{
				start.x -= now_speed;
				SetEdges();
				return;
			}
		}
		else if (end.x + now_speed <= scr_width && end.x + now_speed < final_move_x)
		{
			start.x += now_speed;
			SetEdges();
			return;
		}
	}

	if (start_move_x > final_move_x && start.x - now_speed > final_move_x)
	{
		float next_y = (start.x - now_speed) * slope + intercept;

		if (start.x - now_speed >= 0 && next_y >= sky && next_y + GetHeight() <= ground)
		{
			start.x -= now_speed;
			start.y = start.x * slope + intercept;
			SetEdges();
			return;
		}
	}
	if (start_move_x < final_move_x && end.x + now_speed < final_move_x)
	{
		float next_y = (start.x + now_speed) * slope + intercept;

		if (start.x + now_speed >= 0 && next_y >= sky && next_y + GetHeight() <= ground)
		{
			start.x += now_speed;
			start.y = start.x * slope + intercept;
			SetEdges();
			return;
		}
	}

}
int dll::UNITS::Attack()
{
	--attack_delay;
	if (attack_delay <= 0)
	{
		attack_delay = max_attack_delay;
		return strenght;
	}
	return 0;
}

////////////////////////////////

// FUNCTIONS *******************************************

bool dll::Intersect(FRECT first, FRECT second)
{
	if (!(first.up_left.x >= second.up_right.x || first.up_right.x <= second.up_left.x
		|| first.up_left.y >= second.down_left.y || first.down_left.y <= second.up_left.y))return true;

	return false;
}
float dll::Distance(FPOINT first, FPOINT second)
{
	float a = (float)(pow(abs(second.x - first.x), 2));
	float b = (float)(pow(abs(second.y - first.y), 2));

	return (float)(sqrt(a + b));
}
void dll::Sort(BAG<UNITS>& army, FPOINT target)
{
	if (army.size() < 2)return;

	bool is_ok = false;

	while (!is_ok)
	{
		is_ok = true;

		for (size_t count = 0; count < army.size() - 1; ++count)
		{
			if (Distance(army[count].center, target) > Distance(army[count + 1].center, target))
			{
				UNITS temp = army[count];
				army[count] = army[count + 1];
				army[count + 1] = temp;
				is_ok = false;
			}
		}
	}
}

PLATFORM_API dll::ASSETS* dll::AssetFactory(obstacle what, float startx, float starty)
{
	ASSETS* ret{ nullptr };
	ret = new ASSETS(what, startx, starty);

	return ret;
}

PLATFORM_API dll::UNITS* dll::UnitFactory(unit_type what_unit, float startx, float starty)
{
	UNITS* ret{ nullptr };

	ret = new UNITS(what_unit, startx, starty);

	return ret;
}

PLATFORM_API ACTPARAMS dll::AINextMove(UNITS my_unit, BAG<UNITS>& BadArmy, BAG<ASSETS>& Obstacles, ACTPARAMS params)
{
	ACTPARAMS ret{};

	switch (my_unit.type)
	{
	case unit_type::peon:
		if (!BadArmy.empty())
		{
			for (size_t count = 0; count < BadArmy.size(); ++count)
			{
				if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y), FPOINT(my_unit.end.x, my_unit.start.y),
					FPOINT(my_unit.start.x, my_unit.end.y), FPOINT(my_unit.end.x, my_unit.end.y)),
					FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
						FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
						FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
						FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
				{
					ret.next_action = actions::flee;
					break;
				}
			}

			if (ret.next_action == actions::flee)break;
		}
		ret.next_action = my_unit.current_action;
		if (my_unit.current_action == actions::move)
		{
			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y), FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y), FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(Obstacles[count].start.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].start.x, Obstacles[count].end.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].end.y))))
					{
						if (Obstacles[count].type == obstacle::small_tree || Obstacles[count].type == obstacle::mid_tree
							|| Obstacles[count].type == obstacle::big_tree)
						{
							ret.asset_involved = Obstacles[count].GetMyNumber();
							ret.next_action = actions::harvest;
							break;
						}
						else if (Obstacles[count].type == obstacle::mine)
						{
							ret.asset_involved = Obstacles[count].GetMyNumber();
							ret.next_action = actions::mining;
							break;
						}
					}
				}
			}
		}
		else if (my_unit.current_action == actions::harvest)
		{
			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Obstacles[count].GetMyNumber() == params.asset_involved && Obstacles[count].lifes <= 0)
					{
						ret.asset_involved = params.asset_involved;
						ret.next_action = actions::return_wood;
						break;
					}
				}
			}
		}
		else if (my_unit.current_action == actions::mining)
		{
			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Obstacles[count].lifes <= 0)
					{
						ret.next_action = actions::return_gold;
						break;
					}
				}
			}
		}
		break;

	case unit_type::peasant:
		if (!BadArmy.empty())
		{
			for (size_t count = 0; count < BadArmy.size(); ++count)
			{
				if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
					FPOINT(my_unit.end.x, my_unit.start.y),
					FPOINT(my_unit.start.x, my_unit.end.y),
					FPOINT(my_unit.end.x, my_unit.end.y)),
					FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
						FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
						FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
						FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
				{
					ret.next_action = actions::flee;
					break;
				}
			}

			if (ret.next_action == actions::flee)break;
		}
		ret.next_action = my_unit.current_action;
		if (my_unit.current_action == actions::move)
		{
			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(Obstacles[count].start.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].start.x, Obstacles[count].end.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].end.y))))
					{
						if (Obstacles[count].type == obstacle::small_tree || Obstacles[count].type == obstacle::mid_tree
							|| Obstacles[count].type == obstacle::big_tree)
						{
							ret.asset_involved = Obstacles[count].GetMyNumber();
							ret.next_action = actions::harvest;
							break;
						}
						else if (Obstacles[count].type == obstacle::mine)
						{
							ret.asset_involved = Obstacles[count].GetMyNumber();
							ret.next_action = actions::mining;
							break;
						}
					}
				}
			}
		}
		else if (my_unit.current_action == actions::harvest)
		{
			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Obstacles[count].GetMyNumber() == params.asset_involved && Obstacles[count].lifes <= 0)
					{
						ret.asset_involved = params.asset_involved;
						ret.next_action = actions::return_wood;
						break;
					}
				}
			}
		}
		else if (my_unit.current_action == actions::mining)
		{
			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Obstacles[count].lifes <= 0)
					{
						ret.next_action = actions::return_gold;
						break;
					}
				}
			}
		}
		break;

	case unit_type::orc_warrior:
		ret.next_action = my_unit.current_action;
		if (my_unit.current_action == actions::move)
		{
			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::spotted;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}
			}

			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(Obstacles[count].start.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].start.x, Obstacles[count].end.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].end.y))))
					{
						ret.asset_involved = Obstacles[count].GetMyNumber();
						ret.next_action = actions::stop;
						break;
					}
				}
			}
		}
		else if (my_unit.current_action == actions::melee)
		{
			if (!BadArmy.empty())
			{
				ret.next_action = actions::stop;

				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}

				if (ret.next_action != actions::melee)
				{
					for (size_t count = 0; count < BadArmy.size(); ++count)
					{
						if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
						{
							ret.next_action = actions::spotted;
							ret.enemy_involved = BadArmy[count].GetMyNumber();
							break;
						}
					}
				}
			}
			else ret.next_action = actions::stop;
		}
		else if (my_unit.current_action == actions::spotted)
		{
			ret.next_action = actions::stop;

			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::spotted;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}


				}
			}
		}
		break;

	case unit_type::warrior:
		ret.next_action = my_unit.current_action;
		if (my_unit.current_action == actions::move)
		{
			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::spotted;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}
			}

			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(Obstacles[count].start.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].start.x, Obstacles[count].end.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].end.y))))
					{
						ret.asset_involved = Obstacles[count].GetMyNumber();
						ret.next_action = actions::stop;
						break;
					}
				}
			}
		}
		else if (my_unit.current_action == actions::melee)
		{
			if (!BadArmy.empty())
			{
				ret.next_action = actions::stop;

				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}

				if (ret.next_action != actions::melee)
				{
					for (size_t count = 0; count < BadArmy.size(); ++count)
					{
						if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
						{
							ret.next_action = actions::spotted;
							ret.enemy_involved = BadArmy[count].GetMyNumber();
							break;
						}
					}
				}
			}
			else ret.next_action = actions::stop;
		}
		else if (my_unit.current_action == actions::spotted)
		{
			ret.next_action = actions::stop;

			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::spotted;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}
			}
		}
		break;

	case unit_type::orc_knight:
		ret.next_action = my_unit.current_action;
		if (my_unit.current_action == actions::move)
		{
			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::spotted;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}
			}

			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(Obstacles[count].start.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].start.x, Obstacles[count].end.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].end.y))))
					{
						ret.asset_involved = Obstacles[count].GetMyNumber();
						ret.next_action = actions::stop;
						break;
					}
				}
			}
		}
		else if (my_unit.current_action == actions::melee)
		{
			if (!BadArmy.empty())
			{
				ret.next_action = actions::stop;

				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}

				if (ret.next_action != actions::melee)
				{
					for (size_t count = 0; count < BadArmy.size(); ++count)
					{
						if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
						{
							ret.next_action = actions::spotted;
							ret.enemy_involved = BadArmy[count].GetMyNumber();
							break;
						}
					}
				}
			}
			else ret.next_action = actions::stop;
		}
		else if (my_unit.current_action == actions::spotted)
		{
			ret.next_action = actions::stop;

			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::spotted;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}


				}
			}
		}
		break;

	case unit_type::knight:
		ret.next_action = my_unit.current_action;
		if (my_unit.current_action == actions::move)
		{
			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::spotted;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}
			}

			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(Obstacles[count].start.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].start.x, Obstacles[count].end.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].end.y))))
					{
						ret.asset_involved = Obstacles[count].GetMyNumber();
						ret.next_action = actions::stop;
						break;
					}
				}
			}
		}
		else if (my_unit.current_action == actions::melee)
		{
			if (!BadArmy.empty())
			{
				ret.next_action = actions::stop;

				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}

				if (ret.next_action != actions::melee)
				{
					for (size_t count = 0; count < BadArmy.size(); ++count)
					{
						if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
						{
							ret.next_action = actions::spotted;
							ret.enemy_involved = BadArmy[count].GetMyNumber();
							break;
						}
					}
				}
			}
			else ret.next_action = actions::stop;
		}
		else if (my_unit.current_action == actions::spotted)
		{
			ret.next_action = actions::stop;

			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::spotted;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}


				}
			}
		}
		break;

	case unit_type::orc_archer:
		ret.next_action = my_unit.current_action;
		if (my_unit.current_action == actions::move)
		{
			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::shoot;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}
			}

			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(Obstacles[count].start.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].start.x, Obstacles[count].end.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].end.y))))
					{
						ret.asset_involved = Obstacles[count].GetMyNumber();
						ret.next_action = actions::stop;
						break;
					}
				}
			}
		}
		else if (my_unit.current_action == actions::melee)
		{
			if (!BadArmy.empty())
			{
				ret.next_action = actions::stop;

				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}

				if (ret.next_action != actions::melee)
				{
					for (size_t count = 0; count < BadArmy.size(); ++count)
					{
						if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
						{
							ret.next_action = actions::shoot;
							ret.enemy_involved = BadArmy[count].GetMyNumber();
							break;
						}
					}
				}
			}
			else ret.next_action = actions::stop;
		}
		else if (my_unit.current_action == actions::shoot)
		{
			ret.next_action = actions::stop;

			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::shoot;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}


				}
			}
		}
		break;

	case unit_type::archer:
		ret.next_action = my_unit.current_action;
		if (my_unit.current_action == actions::move)
		{
			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::shoot;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}
			}

			if (!Obstacles.empty())
			{
				for (size_t count = 0; count < Obstacles.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(Obstacles[count].start.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].start.y),
							FPOINT(Obstacles[count].start.x, Obstacles[count].end.y),
							FPOINT(Obstacles[count].end.x, Obstacles[count].end.y))))
					{
						ret.asset_involved = Obstacles[count].GetMyNumber();
						ret.next_action = actions::stop;
						break;
					}
				}
			}
		}
		else if (my_unit.current_action == actions::melee)
		{
			if (!BadArmy.empty())
			{
				ret.next_action = actions::stop;

				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
				}

				if (ret.next_action != actions::melee)
				{
					for (size_t count = 0; count < BadArmy.size(); ++count)
					{
						if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
						{
							ret.next_action = actions::shoot;
							ret.enemy_involved = BadArmy[count].GetMyNumber();
							break;
						}
					}
				}
			}
			else ret.next_action = actions::stop;
		}
		else if (my_unit.current_action == actions::shoot)
		{
			ret.next_action = actions::stop;

			if (!BadArmy.empty())
			{
				for (size_t count = 0; count < BadArmy.size(); ++count)
				{
					if (Intersect(FRECT(FPOINT(my_unit.start.x, my_unit.start.y),
						FPOINT(my_unit.end.x, my_unit.start.y),
						FPOINT(my_unit.start.x, my_unit.end.y),
						FPOINT(my_unit.end.x, my_unit.end.y)),
						FRECT(FPOINT(BadArmy[count].start.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].start.y),
							FPOINT(BadArmy[count].start.x, BadArmy[count].end.y),
							FPOINT(BadArmy[count].end.x, BadArmy[count].end.y))))
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::melee;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}
					else if (Distance(my_unit.center, BadArmy[count].center) <= my_unit.range)
					{
						ret.next_action = actions::shoot;
						ret.enemy_involved = BadArmy[count].GetMyNumber();
						break;
					}


				}
			}
		}
		break;
	}

	return ret;
}