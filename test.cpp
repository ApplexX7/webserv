
#include <iostream>
#include <fstream>

using namespace std;

int main(void) {
    ofstream file;

    file.open("test.conf");
    std::string tmp;
    tmp = "server {\n\
            listen 127.0.0.1:3000;\n\
            autoindex on;\n";
        
    file.write(tmp.data(), tmp.length());
    for (int i = 1; i < 65000; i++) {

        tmp = "location /" + to_string(i) + " {\n\
            root /Users/pro;\n\
        }\n";

        file.write(tmp.data(), tmp.length());
    }
    tmp += "}\n";
    file.write(tmp.data(), tmp.length());
    file.close();
}