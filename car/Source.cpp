#include <iostream>
#include <conio.h>
#include <thread>
#include <Windows.h>
using namespace std;
using namespace std::literals::chrono_literals;
enum Color { Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White };
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);//получает дискриптор активного окна
void SetColor(Color text, Color background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
//# показывает что дальше идЄт директива, 
// директива  - это указание компил€тору на выполнение некоторых действий.
// ƒиректива include(¬ключить в состав) указывает компил€тору на то, что к нашему исходному файлу нужно 
// подключить другой файл
// ƒиректива define (определить) создаЄт макроопределение(ћакрос) типа »ћя «Ќј„≈Ќ»я
// везде где компил€тор видит им€ макроса вставл€ет значение макроса
//#define MIN_TANK_VOLUME 20
//#define MAX_TANK_VOLUME 80

//const int MIN_TANK_VOLUME = 20;
//const int MAX_TANK_VOLUME = 80;
#define Enter 13
#define Esc 27
class Tank
{
	static const int MIN_TANK_VOLUME = 20;
	static const int MAX_TANK_VOLUME = 80;

	const int VOLUME; //объем бака
	double fuel_level; //уровень топлива
public:
	int getVOLUME()const
	{
		return VOLUME;
	}
	double getFuel_level()const
	{
		return fuel_level;
	}
	Tank(int volume) :VOLUME
	(
		volume < MIN_TANK_VOLUME ? MIN_TANK_VOLUME :
		volume > MAX_TANK_VOLUME ? MAX_TANK_VOLUME :
		volume
	)
	{
		//this->VOLUME = volume;
		this->fuel_level = 0;
		cout << "TConstructor:\t" << this << "\n";
	}
	~Tank()
	{
		cout << "TDestructor:\t" << this << "\n";
	}
	void fill(double amount)
	{
		if (amount < 0) return;
		if (fuel_level + amount > VOLUME) fuel_level = VOLUME;
		else fuel_level += amount;
	}
	double give_fuel(double amount)
	{
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}
	void info()const
	{
		cout << "Volume:\t" << VOLUME << "\n";
		cout << "Fuel level:\t" << fuel_level << "\n";
	}
};
class Engine
{
	static const int MIN_ENGINE_CONSUMPTION = 3;
	static const int MAX_ENGINE_CONSUMPTION = 30;
	const double CONSUPTION;
	double consumption_per_second;
	bool isStarted;
public:
	void set_consumption_per_second(double speed)
	{
		if (isStarted)
		{
			if (speed > 0 && speed <= 60 || speed > 100 && speed <= 140) consumption_per_second = CONSUPTION * 3e-5 *6.66666;
			else if (speed > 60 && speed <= 100) consumption_per_second = CONSUPTION * 3e-5 * 4.66666;
			else if (speed > 140 && speed <= 200) consumption_per_second = CONSUPTION * 3e-5 * 8.33333;
			else if (speed > 200 && speed <= 250) consumption_per_second = CONSUPTION * 3e-5 * 10;
			else if (speed == 0) consumption_per_second = CONSUPTION * 3e-5;
		}
		else consumption_per_second = 0;
	}
	int get_CONSUMPTION()const
	{
		return this->CONSUPTION;
	}
	double get_consumption_per_second()const
	{		
		return this->consumption_per_second;
	}
	Engine(double consumption) :CONSUPTION
	(
		consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
		consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
		consumption
	)
	{
		consumption_per_second = CONSUPTION * 3e-5; // 3*10^-5
		isStarted = false;
		cout << "EConstructor:\t" << this << endl;
	}
	~Engine()
	{
		cout << "EDestructor:\t" << this << endl;
	}

	void start()
	{
		isStarted = true;
	}
	void stop()
	{
		isStarted = false;
	}
	bool started()const
	{
		return isStarted;
	}
	void info()const
	{
		cout << "Consumption per 100 km: " << CONSUPTION << " liters\n";
		cout << "Consumption per 1 sec: " << consumption_per_second << " liters\n";
	}
};
class Car
{
	Engine engine;
	Tank tank;
	bool driver_inside;
	//int/* speed,*/ acceleration = 2, break_car = 2;
	double speed, acceleration = 1.1, break_car = 2;;
	static const int MAX_SPEED_LOW_LIMIT = 90;
	static const int MAX_SPEED_HIGH_LIMIT = 390;
	const int MAX_SPEED;
	static const int length = 100;
	string speedometr[length];
	struct Control
	{
		std::thread panel_thread;
		std::thread engine_idle_thread;
		std::thread free_wheeling_thread;
	}threads;
public:
	Car(int volume, int consumption, int max_speed = 250) :tank(volume), engine(consumption),
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			max_speed > MAX_SPEED_HIGH_LIMIT ? MAX_SPEED_HIGH_LIMIT :
			max_speed
		)
	{
		driver_inside = false;
		speed = 0;
		cout << "Your car is ready, press Enter to get in" << endl;
	}
	~Car()
	{
		cout << "Car is over" << endl;
	}
	//           Methods:
	void get_in()
	{
		driver_inside = true;
		threads.panel_thread = std::thread(&Car::panel, this); //запускаем метод panel() в потоке panel_thread				
	}
	void get_out()
	{
		driver_inside = false;
		if (threads.panel_thread.joinable()) threads.panel_thread.join();
		system("cls");
		cout << "You are out of your car" << endl;
	}
	void start_engine()
	{
		if (driver_inside && tank.getFuel_level())
		{
			engine.start();
			threads.engine_idle_thread = std::thread(&Car::engine_idle, this);
		}
	}
	void stop_engine()
	{
		if (driver_inside)
		{
			engine.stop();
			if (threads.engine_idle_thread.joinable()) threads.engine_idle_thread.join();
		}
	}	
	void control()
	{
		char key = 0;
		do
		{
			key = 0;
			if (_kbhit()) key = _getch();
			switch (key)
			{
			case Enter: driver_inside ? get_out() : get_in(); break;
			case 'I': case 'i': //Ignition - зажигание
				engine.started() ? stop_engine() : start_engine();
				break;
			case 'w': case 'W':		
				if (engine.started() && speed < MAX_SPEED)
				{
					if (speed == 0) threads.free_wheeling_thread = std::thread(&Car::free_wheeling, this);
					speed += acceleration;
				}
				break;
			case 's': case 'S':			
				if (speed > 0)
				{
					speed -= break_car;
				}
				break;
			case 'F': case 'f':
				if (driver_inside) cout << "Get out from car!" << endl;
				else
				{
					double amount;
					cout << "¬ведите объЄм топлива: "; cin >> amount;
					tank.fill(amount);
				}
			case Esc:
				stop_engine();
				get_out();
			}
			if (tank.getFuel_level() == 0)stop_engine();
			if (speed <= 0)
			{
				speed = 0;
				if (threads.free_wheeling_thread.joinable()) threads.free_wheeling_thread.join();
			}
			engine.set_consumption_per_second(speed);
			for (size_t i = 0; i < length; i++)
			{
				if (i <= speed / MAX_SPEED * 100)
				{
					speedometr[i] = '|';
				}
				else speedometr[i] = '.';
			}
		} while (key != Esc);
	}
	void engine_idle()
	{
		//холостой ход двигател€
		while (engine.started() && tank.give_fuel(engine.get_consumption_per_second()))
		{
			std::this_thread::sleep_for(1s);
		}
	}
	void free_wheeling()
	{
		while (speed-- > 0)
		{
			if (speed < 0) speed = 0;
			std::this_thread::sleep_for(1s);
		}
	}
	void panel()const
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //получаем обработчик окна консоли
		while (driver_inside)
		{
			system("cls");
			cout << "Fuel level:\t" << tank.getFuel_level() << " liters\n";
			cout << "Current consumption:\t" << engine.get_consumption_per_second() << " liters\n";
			if (tank.getFuel_level() < 5)
			{
				SetColor(White, Red);
				//SetConsoleTextAttribute(hConsole, 0xCF);  //  0xCF 'C' красный фон, 'F' - белые буквы
				cout << "LOW FUEL";
				SetColor(Black, White);
				//SetConsoleTextAttribute(hConsole, 0xF0);  // чЄрный фон серые буквы
			}
			cout << endl;
			cout << "Engine:\t" << (engine.started() ? "started" : "stoped") << "\n";
			for (size_t i = 0; i < length; i++)
			{
				if (i >= 0 && i <= 40) SetColor(Green, Black);
				else if (i > 40 && i <= 80) SetColor(Yellow, Black);
				else if (i > 80 && i <= 100) SetColor(Red, Black);
				cout <<  speedometr[i];
				SetColor(Black, White);
			}			
			cout << "\n";
			cout << "Speed:\t" << (int)speed << " km/h\n";
			std::this_thread::sleep_for(1s);
		}
	}
	void info()const
	{
		tank.info();
		engine.info();
		cout << "Max speed:" << MAX_SPEED << " km/h\n";
	}
};


//#define TANK_CHEK
//некоторым макросам дают только им€, и не дают никакого значени€,
//такие макросы используютс€ с директивами условной компил€ции #ifdef ... #endif
//#define ENGINE_CHEK	
void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHEK // если определено TANK_CHEK, то нижеследующий код
	// до директивы ednif будет виден компил€тору
	Tank tank(40);
	tank.info();
	do
	{
		int fuel;
		cout << "¬ведите объем топлива: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (_getch() != 27);
#endif // TANK_CHEK
#ifdef ENGINE_CHEK

	Engine engine(10);
	engine.info();
#endif // ENGINE_CHEK
	Car bmw(80, 10);
	bmw.info();
	bmw.control();
	}