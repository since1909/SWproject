#include "Character.h"

using namespace Graph_lib;

const int grid_width = 8;
const int grid_height = 8;

Character::Character(Graph_lib::Window* win, int r, int c)
	: my_window{ win }, pos_row { r }, pos_col{ c }, my_enemy{ nullptr }
{
}

void Character::move()
{
	pair<int, int> dir = nextDirection();

	int dr = dir.first;
	int dc = dir.second;
	
	pos_row += dr;
	pos_col += dc;

	int w = my_window->x_max() / grid_width;
	int h = my_window->y_max() / grid_height;
	for (Shape* s : body_shapes)
	{
		s->move(w*dc, h*dr);
	}
}

pair<int, int> Character::nextDirection()
{
	return make_pair(0, 0);
}

Pursuer::Pursuer(Graph_lib::Window * win, int r, int c)
	: Character(win, r, c)
{
	int w = win->x_max() / grid_width;
	int h = win->y_max() / grid_height;

	Graph_lib::Circle* head = new Graph_lib::Circle(Point(c*w + w / 4, r*h + h / 4), w / 4);
	Graph_lib::Rectangle* body = new Graph_lib::Rectangle(Point(c*w, r*h + h / 2), w / 2, h / 2);
	Graph_lib::Polygon* arm1 = new Graph_lib::Polygon;
	arm1->add(Point(c*w, r*h + h));  
	arm1->add(Point(c*w, r*h + h / 2)); 
	arm1->add(Point(c*w - (w / 4), r*h + h / 2));
	Graph_lib::Polygon* arm2 = new Graph_lib::Polygon;
	arm2->add(Point(c*w+w/2, r*h + h));
	arm2->add(Point(c*w+w/2, r*h + h / 2));
	arm2->add(Point(c*w+w/2 +(w / 4), r*h + h / 2));
	
	body_shapes.push_back(head);
	body_shapes.push_back(body);
	body_shapes.push_back(arm1);
	body_shapes.push_back(arm2);
	
	head->set_fill_color(Color::red);
	body->set_fill_color(Color::red);
	arm1->set_fill_color(Color::red);
	arm2->set_fill_color(Color::red);

	win->attach(*body);
	win->attach(*head);
	win->attach(*arm1);
	win->attach(*arm2);
}

pair<int, int> Pursuer::nextDirection()
{
	vector<pair<int, int>> directions = { { -1,0 },{ +1,0 },{ 0,-1 },{ 0,+1 } };
	int dr = 0, dc = 0, new_r = 0, new_c = 0;

	if (pos_row - my_enemy->row() > 0)
	{
		if (pos_col - my_enemy->col() > 0)
		{
			vector<pair<int, int>> dir = { { -1,0 },{ 0,-1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 2; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
				else if (new_r == grid_height)
					return { -1,0 };
				else if (new_c == grid_width)
					return { 0,-1 };
			}
		}
		else if (pos_col - my_enemy->col() < 0)
		{
			vector<pair<int, int>> dir = { { -1,0 },{ 0,+1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 2; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
				else if (new_r == grid_height)
					return { -1,0 };
				else if (new_c == grid_width)
					return { 0,-1 };
			}
		}
		else
		{
			dr = directions[0].first;
			dc = directions[0].second;
			new_r = pos_row + dr;
			new_c = pos_col + dc;

			if (new_r >= 0 && new_r < grid_height &&
				new_c >= 0 && new_c < grid_width)
				return directions[0];
			else if (new_r == grid_height)
				return { -1,0 };
			else if (new_c == grid_width)
				return { 0,-1 };
		}
	}
	else if (pos_row - my_enemy->row() < 0)
	{
		if (pos_col - my_enemy->col() > 0)
		{
			vector<pair<int, int>> dir = { { +1,0 },{ 0,-1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 2; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
				else if (new_r == grid_height)
					return { -1,0 };
				else if (new_c == grid_width)
					return { 0,-1 };
			}
		}
		else if (pos_col - my_enemy->col() < 0)
		{
			vector<pair<int, int>> dir = { { +1,0 },{ 0,+1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 2; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
				else if (new_r == grid_height)
					return { -1,0 };
				else if (new_c == grid_width)
					return { 0,-1 };
			}
		}
		else
		{
			dr = directions[1].first;
			dc = directions[1].second;
			new_r = pos_row + dr;
			new_c = pos_col + dc;

			if (new_r >= 0 && new_r < grid_height &&
				new_c >= 0 && new_c < grid_width)
				return directions[1];
			else if (new_r == grid_height)
				return { -1,0 };
			else if (new_c == grid_width)
				return { 0,-1 };
			
		}
	}
	else
	{
		if (pos_col - my_enemy->col() > 0)
		{
			dr = directions[2].first;
			dc = directions[2].second;
			new_r = pos_row + dr;
			new_c = pos_col + dc;

			if (new_r >= 0 && new_r < grid_height &&
				new_c >= 0 && new_c < grid_width)
				return directions[2];
			else if (new_r == grid_height)
				return { -1,0 };
			else if (new_c == grid_width)
				return { 0,-1 };
			
		}

		else if (pos_col - my_enemy->col() < 0)
		{
		
			dr = directions[3].first;
			dc = directions[3].second;
			new_r = pos_row + dr;
			new_c = pos_col + dc;

			if (new_r >= 0 && new_r < grid_height &&
				new_c >= 0 && new_c < grid_width)
				return directions[3];
			else if (new_r == grid_height)
				return { -1,0 };
			else if (new_c == grid_width)
				return { 0,-1 };
		}
		else
		{
			return { 0,0 };
		}
	}

}

Evader::Evader(Graph_lib::Window * win, int r, int c)
	: Character(win, r, c)
{
	int w = win->x_max() / grid_width;
	int h = win->y_max() / grid_height;

	Graph_lib::Polygon* head = new Graph_lib::Polygon;
	head->add(Point(c*w + w / 2, r*h));
	head->add(Point(c*w + w / 4, r*h + h / 2));
	head->add(Point(c*w + w /2+w/4, r*h+h/2));
	Graph_lib::Rectangle* body = new Graph_lib::Rectangle(Point(c*w + w / 4, r*h + h / 2), w / 2, h / 2);
	Graph_lib::Rectangle* arms = new Graph_lib::Rectangle(Point(c*w , r*h + h / 2), w , h / 4);
	body_shapes.push_back(head);
	body_shapes.push_back(body);
	body_shapes.push_back(arms);
	head->set_fill_color(Color::blue);
	body->set_fill_color(Color::blue);
	arms->set_fill_color(Color::blue);
	win->attach(*head);
	win->attach(*arms);
	win->attach(*body);
}

pair<int, int> Evader::nextDirection()
{
	int dr = 0, dc = 0, new_r = 0, new_c = 0;

	if (pos_row - my_enemy->row() > 0)
	{
		if (pos_col - my_enemy->col() > 0)
		{
			vector<pair<int, int>> dir = { { +1,0 },{ 0,+1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 2; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
				else if (new_r == grid_height)
					return { -1,0 };
				else if (new_c == grid_width)
					return { 0,-1 };
			}
		}
		else if (pos_col - my_enemy->col() < 0)
		{
			vector<pair<int, int>> dir = { { +1,0 },{ 0,-1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 2; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
				else if (new_r == grid_height)
					return { -1,0 };
				else if (new_c == grid_width)
					return { 0,-1 };

			}
		}
		else
		{
			vector<pair<int, int>> dir = { { +1,0 },{ 0,-1 },{ 0,+1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 3; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
			
			}
		}
	}
	else if (pos_row - my_enemy->row() < 0)
	{
		if (pos_col - my_enemy->col() > 0)
		{
			vector<pair<int, int>> dir = { { -1,0 },{ 0,+1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 2; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
				else if (new_r == grid_height)
					return { -1,0 };
				else if (new_c == grid_width)
					return { 0,-1 };
			}
		}
		else if (pos_col - my_enemy->col() < 0)
		{
			vector<pair<int, int>> dir = { { +1,0 },{ 0,+1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 2; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
				else if (new_r == grid_height)
					return { -1,0 };
				else if (new_c == grid_width)
					return { 0,-1 };
			}
		}
		else
		{
			vector<pair<int, int>> dir = { { -1,0 },{ 0,-1 },{ 0,+1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 3; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
			}
		}
	}
	else
	{
		if (pos_col - my_enemy->col() > 0)
		{
			vector<pair<int, int>> dir = { { -1,0 },{ +1,0 },{ 0,+1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 3; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
			}
		}

		else if (pos_col - my_enemy->col() < 0)
		{
			vector<pair<int, int>> dir = { { -1,0 },{ +1,0 },{ 0,-1 } };
			random_shuffle(dir.begin(), dir.end());
			for (int i = 0; i < 3; i++)
			{
				dr = dir[i].first;
				dc = dir[i].second;
				new_r = pos_row + dr;
				new_c = pos_col + dc;

				if (new_r >= 0 && new_r < grid_height &&
					new_c >= 0 && new_c < grid_width)
					return dir[i];
			}
		}
		else
		{
			return { 0,0 };
		}
	}

}