#include "iostream"
#include "fstream"

#include "socket_controller.h"


using namespace std;

int main() {

//    ofstream of("pipe");
//    for (int i = 0; i < 10; i++) {
//        of << "Hello - " << i << endl;
////        sleep(1);
//    }
//
//    ifstream file{"pipe"};
//    string line;
//    getline(file, line);
//    cout << line << '\n';


    SocketController controller(1234);
    controller.listen();
    return 0;
}
