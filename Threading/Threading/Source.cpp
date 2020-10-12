/*#include <iostream>
#include <chrono>
#include <thread>
using namespace std;*/

/*
void fun(int x) {
	while (x-- > 0) {
		cout << x << endl;
	}
}

int main() {

	thread t1(fun, 10);
	t1.join();


	return 0;
}*/
/*
int main() {
	thread t([](int x) {
		while (x-- > 0) {
			cout << x << endl;
		}
	}, 10);
	t.join();

	return 0;
}*/
/*
class Base {
public :
	void operator () (int x) {
		while (x-- > 0) {
			cout << x << endl;
		}
	}
};

int main() {
	thread t((Base()), 10);
	t.join();

	return 0;
}*/
/*
class Base {
public:
	void run(int x) {
		while (x -- > 0){
			cout << x << endl;
		}
	}
};

int main() {
	Base b;
	thread t(&Base::run, &b, 10);
	t.join();

	return 0;
}*/
/*
class Base {
public:
	static void run(int x) {
		while (x-- > 0){
			cout << x << endl;
		} 
	}
};

int main() {
	thread t(&Base::run, 10);
	t.join();

	return 0;
}*/
/*
void run(int count) {
	while (count -- > 0)
		cout << count << " " << "ddkjfkdfjqkj" << endl;
	this_thread::sleep_for(chrono::seconds(5));
	cout << "Thread finished" << endl;
}

int main() {
	thread t1(run, 10);
	cout << "main()" << endl;
	t1.detach();
	if (t1.joinable())
		t1.detach();
	//if (t1.joinable())
		//t1.join();
	cout << "main() after" << endl;
 	return 0;
}*/

///Mutex and race condition
/*
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int myAmount = 0;

mutex m;

void addMoney() {
	m.lock();
	++myAmount;
	m.unlock();
}

int main() {
	thread t1(addMoney);
	thread t2(addMoney);

	t1.join();
	t2.join();

	cout << myAmount << endl;
	
	return 0;
}*/

///Mutex try lock

/*
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int counter = 0;
mutex mtx;

void increadeTheCounterFor100000Time() {
	for (int i = 0; i < 100000; ++i) {
		if (mtx.try_lock()) {
			++counter;
			mtx.unlock();
		}
	}
}

int main() {

	thread t1(increadeTheCounterFor100000Time);
	thread t2(increadeTheCounterFor100000Time);

	t1.join();
	t2.join();

	cout << "Counter could increases upto : " << counter << endl;

	return 0;
}
*/
///std::try_lock
/*
#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>
using namespace std;

int X = 0, Y = 0;
mutex m1, m2;

void doSomeWorkForSeconds(int seconds) { this_thread::sleep_for(chrono::seconds(seconds)); }

void incrementXY(int &XorY, mutex &m, const char* desc) {
	for (int i = 0; i < 5; i++) {
		m.lock();
		++XorY;
		cout << desc << XorY << endl;
		m.unlock();
		doSomeWorkForSeconds(1);
	}
}

void consumeXY() {
	int useCount = 5;
	int XplusY = 0;
	while (1) {
		int lockResult = try_lock(m1, m2);
		if (lockResult == -1) {
			if (X != 0 && Y != 0) {
				--useCount;
				XplusY += X + Y;
				Y = 0;
				X = 0;
				cout << "XplusY " << XplusY << endl;
			}
			m1.unlock();
			m2.unlock();
			if (useCount == 0) break;
		}
	}
}

int main() {

	thread t1(incrementXY, ref(X), ref(m1), "X ");
	thread t2(incrementXY, ref(Y), ref(m2), "Y ");
	thread t3(consumeXY);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}*/

///Timed Mutex - try_lock_for
/*
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

int myAmount = 0;
timed_mutex m;

void increment(int i) {
	if (m.try_lock_for(chrono::seconds(1))) {
		++myAmount;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Thread " << i << " Entered" << endl;
		m.unlock();
	}
	else {
		cout << "Thread " << i << " Couldn't Enter" << endl;
	}
}

int main() {

	thread t1(increment, 1);
	thread t2(increment, 2);

	t1.join();
	t2.join();

	cout << myAmount << endl;
	return 0;
}*/

///try_lock_until

/*
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

int myAmount = 0;
timed_mutex m;

void increment(int i) {
	auto now = chrono::steady_clock::now();
	if (m.try_lock_until(now + chrono::seconds(2))) {
		++myAmount;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "Thread " << i << " Entered" << endl;
		m.unlock();
	}
	else {
		cout << "Thread " << i << " Couldn't Enter" << endl;
	}
}

int main() {

	thread t1(increment, 1);
	thread t2(increment, 2);

	t1.join();
	t2.join();
	
	cout << myAmount << endl;

	return 0;
}*/

///Recursive Mutex
/*
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

recursive_mutex m1;
int buffer = 0;
*/
//Example 1 : Recursion
/*
void recursion(char c, int loopFor) {
	if (loopFor < 0)
		return;

	m1.lock();
	cout << "Thread ID : " <<  c << " " << buffer++ << endl;
	recursion(c, --loopFor);
	m1.unlock();
	cout << "Unlock by thread " << c << endl;
}

int main() {
	thread t1(recursion, '0', 10);
	thread t2(recursion, '1', 10);
	t1.join();
	t2.join();

	return 0;
}*/

//Example 2 : loop
/*
int main() {

	for (int i = 0; i < 5; i++) {
		m1.lock();
		std::cout << "Locked" << i << endl;
	}
	for (int i = 0; i < 5; i++) {
		m1.unlock();
		std::cout << "unlocked" << i << endl;
	}

	return 0;
}*/

///Lock_guard in Threading
/*
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

mutex m1;
int buffer = 0;

void task(const char* threadNumber, int loopFor) {
	lock_guard<mutex> lock(m1);
	for (int i = 0; i < loopFor; ++i) {
		buffer++;
		cout << threadNumber << buffer << endl;
	}
}

int main() {
	thread t1(task, "T0 ", 10);
	thread t2(task, "T1 ", 10);
	t1.join();
	t2.join();

	return 0;

}*/

///unique_lock in Threading 
/*
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m1;
int buffer = 0;

void task(const char* threadNumber, int loopFor) {
	unique_lock<mutex> lock(m1, defer_lock); //Flexible, le lock arrive à lock.lock();
	lock.lock();
	//unique_lock<mutex> lock(m1);
	for (int i = 0; i < loopFor; ++i) {
		buffer++;
		cout << threadNumber << buffer << endl;
	}
}

int main() {

	thread t1(task, "T0 ", 10);
	thread t2(task, "T1 ", 10);

	t1.join();
	t2.join();

	return 0;
}*/

///Condition Variable
/*
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

condition_variable cv;
mutex m;
long balance = 0;


void addMoney(int money) {
	lock_guard<mutex> lg(m);
	balance += money;
	cout << "Amount Added Current Balance: " << balance << endl;
	cv.notify_one();
}

void withDrowMoney(int money) {
	unique_lock<mutex> ul(m);
	cv.wait(ul, [] { return (balance != 0); });
	if (balance >= money) {
		balance -= money;
		cout << "Amount Deducted : " << money << endl;
	}
	else {
		cout << "Amount Can't Be Deducted, Current Balance Is Less Than " << money << endl;
	}
	cout << "Current Balance Is: " << balance << endl;
}

int main() {
	thread t1(withDrowMoney, 500);
	thread t2(addMoney, 500);
	t1.join();
	t2.join();

	return 0;
}*/

///DeadLock
/*
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex m1, m2;

void thread1() {
	m1.lock();
	m2.lock();
	cout << "Critical Section Of Thread Thread One" << endl;
	m1.lock();
	m2.lock();
}

void thread2() {
	m2.lock();
	m1.lock();
	cout << "Critical Section Of Thread Thread Two" << endl;
	m2.unlock();
	m1.unlock();
}

int main() {
	thread t1(thread1);
	thread t2(thread2);

	t1.join();
	t2.join();

	return 0;
}*/

///Thread OR Process Synchronization
/*
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

long long bankBalance = 0;
mutex m;

void addMoney(long long val) {
	m.lock();
	bankBalance += val;
	m.unlock();
}

int main() {

	thread t1(addMoney, 100);
	thread t2(addMoney, 200);
	t1.join();
	t2.join();

	cout << "Final BankBalance : " << bankBalance << endl;
	return 0;
}*/

///std::lock -> Avoid DeadLocks
/*
#include <iostream>
#include <thread>
#include <mutex>

mutex m1, m2;

void task_a() {
	while (1) {
		lock(m1, m2);
		cout << "task a" << endl;
		m1.unlock();
		m2.unlock();
	}
}

void task_b() {
	while (1) {
		lock(m2, m1);
		cout << "Task b" << endl;
		m2.unlock();
		m1.unlock();
	}
}

int main() {

	thread t1(task_a);
	thread t2(task_b);

	t1.join();
	t2.join();

	return 0;
}*/

///Future and promise
/*
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>
using namespace std;
using namespace std::chrono;
typedef long int ull;

void findOdd(promise<ull>&& OddSumPromise, ull start, ull end) {
	ull OddSum = 0;
	for (ull i = start; i <= end; ++i) {
		if (i & 1) {
			OddSum += i;
		}
	}
	OddSumPromise.set_value(OddSum);
}

int main() {

	ull start = 0, end = 190000000000;

	promise<ull> OddSum;
	future<ull> OddFuture = OddSum.get_future();

	cout << "Thread Created!!" << endl;
	thread t1(findOdd, move(OddSum), start, end);
	cout << "Waiting for the result !!" << endl;

	cout << "OddSume : " << OddFuture.get() << endl;

	cout << "Completed !!" << endl;
	t1.join();

	return 0;
}*/

///std::async
/*
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <future>
using namespace std;
using namespace std::chrono;
typedef long int ull;

ull findOdd(ull start, ull end) {
	ull OddSum = 0;
	for (ull i = start; i <= end; ++i) {
		if (i & 1) {
			OddSum += i;
		}
	}
	return OddSum;
}

int main() {

	ull start = 0, end = 190000000000;

	cout << "Thread created if policy is std::launch::async !!" << endl;
	future<ull> OddSum = async(launch::deferred, findOdd, start, end);

	cout << "Waiting for Result !!" << endl;
	cout << "OddSum : " << OddSum.get() << endl;

	cout << "Completed !!" << endl;

	return 0;
}*/

///Producer and Consumer Problem
/*
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
using namespace std;

mutex mu;
condition_variable cond;
deque<int> buffer;
const unsigned int maxBufferSize = 50;

void producer(int val) {
	while (val) {
		unique_lock<mutex> locker(mu);
		cond.wait(locker, []() {return buffer.size() < maxBufferSize; });
		buffer.push_back(val);
		cout << "Produced : " << val << endl;
		val--;
		locker.unlock();
		cond.notify_one();
	}
}

void consumer() {
	while (true) {
		unique_lock<mutex> locker(mu);
		cond.wait(locker, []() {return buffer.size() > 0; });
		int val = buffer.back();
		buffer.pop_back();
		cout << "Consumed : " << val << endl;
		locker.unlock();
		cond.notify_one();
	}
}

int main() {

	thread t1(producer, 100);
	thread t2(consumer);

	t1.join();
	t2.join();

	return 0;
}*/

///Essai Diner des philosophes
/*
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
		lk.unlock();
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
}*/