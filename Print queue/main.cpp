#include <iostream>
#include <Windows.h>
#include <queue>
#include <string>
#include <chrono>

using namespace std;

// Разработать приложение, имитирующее очередь печати принтера.
// Должны быть клиенты, посылающие запросы на принтер, 
// у каждого из которых есть свой приоритет.
// Каждый новый клиент попадает в очередь в зависимости от своего приоритета.
// Необходимо сохранять статистику печати(пользователь, время)
// в отдельной очереди. Предусмотреть вывод статистики на экран.

struct PrintRequest {
	string user;
	int priority;
	chrono::time_point<chrono::system_clock> requestTime;

	PrintRequest(const string& u, int p) : user(u), priority(p) {
		requestTime = chrono::system_clock::now();
	}
};

int main()
{
	SetConsoleOutputCP(1251);

	priority_queue<PrintRequest, vector<PrintRequest>,
		bool(*)(const PrintRequest&, const PrintRequest&)>
		printQueue([](const PrintRequest& a, const PrintRequest& b)
			{
				return a.priority < b.priority;
			});

	queue<PrintRequest> printHistory;

	auto handlePrintRequest = [&](const PrintRequest& newRequest)
		{
			printQueue.push(newRequest);
			printHistory.push(newRequest);
		};

	handlePrintRequest(PrintRequest("Петя", 1));
	handlePrintRequest(PrintRequest("Вася", 2));
	handlePrintRequest(PrintRequest("Коля", 1));

	cout << "Очередь на печать:" << endl;
	while (!printQueue.empty()) {
		PrintRequest req = printQueue.top();
		cout << req.user << " с приоритетом " << req.priority << endl;
		printQueue.pop();
	}

	cout << endl << "История печати:" << endl;
	while (!printHistory.empty()) {
		PrintRequest req = printHistory.front();
		time_t requestTime = chrono::system_clock::to_time_t(req.requestTime);
		tm timeinfo;
		if (localtime_s(&timeinfo, &requestTime) == 0)
		{
			char buffer[80];
			if (strftime(buffer, sizeof(buffer), "%c", &timeinfo) > 0) {
				cout << req.user << " напечатано в " << buffer << endl;
			}
		}
		printHistory.pop();
	}

	return 0;
}