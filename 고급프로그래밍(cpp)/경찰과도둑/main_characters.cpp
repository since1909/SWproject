#include "Graph.h"
#include "Simple_window.h"
#include "Character.h"

using namespace Graph_lib;

int main()
{
	srand(time(nullptr));

	Point tl(100, 100);
	Simple_window win(tl, 800, 800, "Chasing");

	vector<Character*> characters;
	characters.push_back(new Pursuer(&win, 3, 0));
	characters.push_back(new Evader(&win, 4, 7));

	characters[0]->setEnemy(characters[1]);
	characters[1]->setEnemy(characters[0]);

	while(true)
	{
		win.wait_for_button();
		if (characters[0]->col() == characters[1]->col() && characters[0]->row() == characters[1]->row())
		{
			Graph_lib::Text* end = new Graph_lib::Text(Point(400,400),"The End");
			end->set_color(Color::black);
			win.attach(*end);
			break;
		}
		for (Character* ch : characters)
		{
			ch->move();
			if (characters[0]->col() == characters[1]->col() && characters[0]-> row() == characters[1]->row())
				break;
		}
	}
}
