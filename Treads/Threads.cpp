#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::literals::chrono_literals;

bool finish = false;

void Plus()
{
	while (!finish)
	{
		cout << "+ ";
		this_thread::sleep_for(1s);
	}
}
void Minus()
{
	while (!finish)
	{
		cout << "- ";
		this_thread::sleep_for(1s);
	}
}
int main()
{
	setlocale(LC_ALL, "");
	thread plus_thread(Plus);
	thread minus_thread(Minus);
	cin.get(); // ожидает нажатие энтер
	finish = true;
	if (plus_thread.joinable()) plus_thread.join();
	if (minus_thread.joinable()) minus_thread.join();
}