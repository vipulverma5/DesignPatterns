#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <unistd.h>

/**
Namespace defined, an important bit that protects your code from ambiguity in the long run. Here I have written a template based signalID structure that for generic side to the signal keeping in mind the future usage.

If we examine it closely, template type R is used as the argument for the callback. Templates here become a good choice because we simply cannot have too many declarations for each varying argument that signal will have to support. Variadic templates can also be used here to support unknown number of arguments by the signal.

Callback is stored in std::function, this is part of C++11 standard and acts like a function pointer.
*/

namespace ObserverParent {
template<typename R>
struct signalID {

    //Connect function here accepts the callback of the class that wants to observe. This callback is pushed into a vector that is called when the signal is fired.
    void connect(std::function<void(R)> slotFunc) {
        slotVect.push_back(slotFunc);
    }

    //Fire is the function that is called when notification to the observers registered in this signal is to be sent.
    void fire(R &val) {
        for(auto &slotFuncs: slotVect) {
            slotFuncs(val);
        }
    }

private:
    //Stores all the callback functions.
    std::vector<std::function<void(R)>> slotVect;
};

/**
Below is the sample class subject.

In each and every observer pattern, subject has to extend a contract mentioning what kind of service it delivers.

On the basis of kind of service it offers, it will have unique arguments related to it.

Observers will have to register themselves using signals directly. The reason I have separate functions for registration is because of the multiple signalID structure taken. In this way you have a register events corresponding to each signalID struct taken. Using these you’d end up right in the queue of callbacks your functions are to be in.

This type of callback definition is suggested. The benefit of this is, much cleaner contract, no hidden switch cases or if-else conditions.
*/
class Subject {
public:
    Subject() {								//Constructor.

    }

    //RaiseEvent is a mock function that gets called from Main.
    void RaiseEvent(int &val) {
        sig1.fire(val);
    }

    //RaiseEvent is a mock function that gets called from Main.
    void RaiseEvent2(std::string &str) {
        sig2.fire(str);
    }
public:
    ObserverParent::signalID<int> sig1;       		//This is signal 1 of argument of type int.
    ObserverParent::signalID<std::string> sig2; 		//This is signal 2 of argument of type string.
};

/**
If you look closely, Observer slots are registered with the Subject in the constructor of this class.

Slots will perform it's respective action when an event is received.

It is necessary that Slots match the number of arguments and their types with their signal counter parts. Not complying to this rule will result in compiler errors.

Further, it is also not necessary that for every Subject signal there has to be a Observer slot. It is upto the Observer whether it wants to be registered with every Subject signal or not.
*/
class Observer {
public:
    //Constructor: Here we register the slots for events.
    Observer(Subject &subObj) {
        subObj.sig1.connect(std::bind(&Observer::slot1, this, std::placeholders::_1));
        subObj.sig2.connect(std::bind(&Observer::slot2, this, std::placeholders::_1));
    }

    //Slot gets the event and performs the desired action.
    void slot1(int intVal) {
        std::cout << "Integer value is " << intVal << std::endl;
    }

    void slot2(std::string str) {
        std::cout << "String value is " << str << std::endl;
    }
};

}

//Main here is more of a object creator as well as the one who raises events of Subject which in turn call the slots.

int main(int argc, char *argv[])
{
    ObserverParent::Subject sub1;
    ObserverParent::Observer obse1(sub1);

    sleep(2);

    auto i = 1;
    std::string str = "Vipul";
    sub1.RaiseEvent(i);
    sub1.RaiseEvent2(str);

    return 0;
}
