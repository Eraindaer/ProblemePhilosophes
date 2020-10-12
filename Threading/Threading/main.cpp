
#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <chrono>
using namespace std;

timed_mutex forks[5];
mutex philosophers[5];
condition_variable cond;
int hungryPhilosopher = 0;
int passages = 0;

void Eating(int fork1, int fork2, int turns, bool hungry) {
	while (turns > 0) {
		cout << turns << endl;

		if (hungry) {
			if (forks[fork1].try_lock_for(chrono::seconds(1)) && forks[fork2].try_lock_for(chrono::seconds(1))) {
				cout << "Philosophe n°" << fork1 + 1 << " utilise les fourchettes " << fork1 << " et " << fork2 << endl;
				hungry = false;
				this_thread::sleep_for(chrono::seconds(3));
				forks[fork1].unlock();
				forks[fork2].unlock();
			}
			else {
				cout << "Philosophe n°" << fork1 + 1 << " est affamé " << endl;
				hungryPhilosopher++;
				forks[fork1].unlock();
				forks[fork2].unlock();
			}
		}
		else {
			cout << "Philosophe n°" << fork1 + 1 << " est en trainde penser" << endl;
			hungry = true;
		}
		/*passages++;
		if (passages > 4) {
			cond.notify_all();
			passages = 0;
		}
		unique_lock<mutex> lk(philosophers[fork1]);
		cond.wait(lk, [] {return passages >= 5; });
		lk.unlock();*/
		turns--;

	}
}


int main() {

	thread t1(Eating, 0, 1, 10000, false);
	thread t2(Eating, 1, 2, 10000, false);
	thread t3(Eating, 2, 3, 10000, false);
	thread t4(Eating, 3, 4, 10000, false);
	thread t5(Eating, 4, 0, 10000, false);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	cout << "Nombre de fois qu'un philosophe a été affamé : " << hungryPhilosopher << endl;
	return 0;
}