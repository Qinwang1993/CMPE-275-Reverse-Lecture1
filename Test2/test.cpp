#include <iostream>
#include <string>
#include <chrono> 
#include "test.pb.h"
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

const int nums = 100000;

template<class T>
int MeasureSerializeTime(const T& pb) {
    auto start = high_resolution_clock::now();
    for (int i=0; i<nums; i++) {
        string serializedStr;
        pb.SerializeToString(&serializedStr);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    return duration.count();
}

template<class T>
int MeasureDeserializedTime(const string& serializedStr, T* pb) {
    const int nums = 100000;
    auto start = high_resolution_clock::now();
    for (int i=0; i<nums; i++) {
        pb->ParseFromString(serializedStr);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    return duration.count();
}

template<class T>
void Report(const string& label, const T& pb) {
    cout << label << endl;
    string serializedStr;
    pb.SerializeToString(&serializedStr);
    cout << "size of serializedStr is: " << serializedStr.size() << " bytes." << endl;
    int se_time = MeasureSerializeTime(pb);
    cout << nums << " serializeToProto time: " << se_time << " microseconds." << endl;
    T dummy;
    int de_time = MeasureDeserializedTime(serializedStr, &dummy);
    cout << nums << " deserializeToObj time: " << de_time << " microseconds." << endl;
    cout << endl;
}

void Test1Func() {
    CMPE275::Test1 pb_t1;
    for (int i=0; i<1000; i++) {
        pb_t1.add_int1(i);
    }
    CMPE275::Test1 pb_t2;
    for (int i=0; i<1000; i++) {
        pb_t2.add_int2(i);
    }
    cout << "Test1: Non packed repeated field VS Packed repeated field" << endl;
    Report("Non packed repeated field:", pb_t1);
    Report("Packed repeated field:", pb_t2);
}

void Test2Func() {
    CMPE275::Test2 double_pb;
    CMPE275::Test2 float_pb;
    CMPE275::Test2 int_pb;
    CMPE275::Test2 uint_pb;
    CMPE275::Test2 sint_pb;
    CMPE275::Test2 fixedint_pb;
    for (int i=0; i<1000; i++) {
        double_pb.add_double1(i);
        float_pb.add_float1(i);
        int_pb.add_int1(i);
        uint_pb.add_int2(i);
        sint_pb.add_int3(i);
        fixedint_pb.add_int4(i);
    }
    cout << "Test1: Different types of digit field" << endl;
    Report("repeated double field:", double_pb);
    Report("repeated float field:", float_pb);
    Report("repeated int32 field:", int_pb);
    Report("repeated uint32 field:", uint_pb);
    Report("repeated sint32 field:", sint_pb);
    Report("repeated fixed32 field:", fixedint_pb);

    int large = 123456789;
    double_pb.mutable_double1()->Clear();
    float_pb.mutable_float1()->Clear();
    int_pb.mutable_int1()->Clear();
    uint_pb.mutable_int2()->Clear();
    sint_pb.mutable_int3()->Clear();
    fixedint_pb.mutable_int4()->Clear();
    for (int i=0; i<1000; i++) {
        double_pb.add_double1(large);
        float_pb.add_float1(large);
        int_pb.add_int1(large);
        uint_pb.add_int2(large);
        sint_pb.add_int3(large);
        fixedint_pb.add_int4(large);
    }
    cout << "Test1: Different types of digit field with large integers" << endl;
    Report("repeated double field:", double_pb);
    Report("repeated float field:", float_pb);
    Report("repeated int32 field:", int_pb);
    Report("repeated uint32 field:", uint_pb);
    Report("repeated sint32 field:", sint_pb);
    Report("repeated fixed32 field:", fixedint_pb);
}

void Test3Func() {
    CMPE275::Test3 pb1;
    for (int i=0; i<1000; i++) {
        auto* sub = pb1.add_sub();
        sub->set_int1(i);
        sub->set_int2(i+1);
        sub->set_int3(i+2);
        sub->set_int4(i+3);
    }
    CMPE275::Test3 pb2;
    for (int i=0; i<1000; i++) {
        pb2.add_int1(i);
        pb2.add_int1(i+1);
        pb2.add_int1(i+2);
        pb2.add_int1(i+3);
    }
    cout << "Test3: repeated Sub-message VS multiple packed repeated fields" << endl;
    Report("repeated sub-message:", pb1);
    Report("multiple packed repeated fields", pb2);
}

int main (int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    Test1Func();
    Test2Func();
    Test3Func();
    
    google::protobuf::ShutdownProtobufLibrary();
}