#include <iostream>
#include <string>
#include <chrono> 
#include "student.pb.h"
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

int main (int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    CMPE275::Student student;
    student.set_id(12345);
    student.set_name("qin wang");
    student.set_gender(0);
    student.set_major("software engineer");
    student.set_address("San Jose State University, San Jose, 95112");
    student.set_email("xxx@sjsu.edu");
    student.set_phone("111-111-1111");

    string serializedStr;
    student.SerializeToString(&serializedStr);
    cout << "student debugString:" << student.DebugString();
    cout << "serialization result:" << serializedStr << endl;
    
    CMPE275::Student deserializedStudent;
    if (!deserializedStudent.ParseFromString(serializedStr)) {
      cerr << "Failed to parse student." << endl;
      return -1;
    }
    cout << "deserializedStudent debugString:" << deserializedStudent.DebugString();
    
    int size = serializedStr.size();
    cout << "size of serializedStr is: " << size << " bytes." << endl;

    auto start = high_resolution_clock::now();
    for (int i=0; i<100000; i++) {
        string tmp_serializedStr;
        student.SerializeToString(&tmp_serializedStr);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    cout << "100000 serializeToProto time: " << duration.count() << " microseconds." << endl;
    
    start = high_resolution_clock::now();
    for (int i=0; i<100000; i++) {
        CMPE275::Student tmp_deserializedStudent;
        tmp_deserializedStudent.ParseFromString(serializedStr);
    }
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start); 
    cout << "100000 deserializeToObj time: " << duration.count() << " microseconds." << endl;
    
    google::protobuf::ShutdownProtobufLibrary();
}