#include "iostream"
#include "socket_controller.h"


//using namespace std;

int main() {
    SocketController controller(1234);
    controller.listen();
    return 0;
}

