#include <iostream>
#include <functional>
#include <memory>
#include <list>
#include <unistd.h>
#include <algorithm>

namespace ObserverParent {

template<typename... Args>
struct Event {
    int subscribe(std::function<void(Args... arg)> &&slotFunc) {
        id++;
        slotList.emplace_back(make_pair(id, slotFunc));
        return id;
    }
    void unsubscribe(int id) {
        auto it = std::find_if(slotList.begin(), slotList.end(), [id](const auto &it)->bool {
            if(it.first == id) return true;
            return false;
        });
        if(it != slotList.end()) slotList.erase(it);
        std::cout << "Slots after unsubscription " << slotList.size() << std::endl;
        return;
    }
    void fire(Args... arg) {
        for(auto &slotPair: slotList) {
            slotPair.second(arg...);
        }
    }

private:
    int id = 0;
    std::list<std::pair<int, std::function<void(Args... arg)>>> slotList;
};

class Subject {
public:
    Subject() {
    }

    //RaiseEvent is a mock function that gets called from Main.
    void RaiseEvent(int &val) {
        sig1.fire(val, 2);
    }

    //RaiseEvent is a mock function that gets called from Main.
    void RaiseEvent2(std::string &str) {
        sig2.fire(str);
    }
public:
    Event<int, int> sig1;       		//This is signal 1 of argument of type int.
    Event<std::string> sig2; 		//This is signal 2 of argument of type string.
};

class Observer {
public:
    Observer(Subject &subObj) {
        handler = subObj.sig1.subscribe(std::bind(&Observer::slot1, this, std::placeholders::_1, std::placeholders::_2));
        handler_1 = subObj.sig1.subscribe(std::bind(&Observer::slot1_1, this, std::placeholders::_1, std::placeholders::_2));
        handler2 = subObj.sig2.subscribe(std::bind(&Observer::slot2, this, std::placeholders::_1));
    }

    //Slot gets the event and performs the desired action.
    void slot1(int val, int val2) {
        std::cout << "Integer value is " << val << " " << val2 << std::endl;
    }

    //Slot gets the event and performs the desired action.
    void slot1_1(int val1, int val2) {
        std::cout << "Integer second slot is " << val1 << " " << val2 << std::endl;
    }

    void slot2(std::string str) {
        std::cout << "String value is " << str << std::endl;
    }

    void destroy(Subject &subObj) {
        subObj.sig1.unsubscribe(handler);
    }

private:
    int handler = -1;
    int handler_1 = -1;
    int handler2 = -1;
};

}

int main(int argc, char *argv[])
{
    ObserverParent::Subject sub1;
    ObserverParent::Observer obse1(sub1);

    auto i = 1;
    std::string str = "test";
    sub1.RaiseEvent(i);
    sub1.RaiseEvent2(str);
    sleep(2);

    obse1.destroy(sub1);
    return 0;
}
