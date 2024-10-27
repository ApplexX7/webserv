#include <iostream>
#include <fstream>

using namespace std;

int main(void) {
    ofstream file;

    file.open("test.conf");
    std::string tmp;
        
    file.write(tmp.data(), tmp.length());
    for (int i = 1; i < 65000; i++) {

        tmp = "server {\n\
                listen 127.0.0.1:" + std::to_string(i) +  ";\n\
                autoindex on;\n" + "}\n";

        file.write(tmp.data(), tmp.length());
    }
    // tmp += "}\n";
    file.close();
}