#include <string>
#include <iostream>
#include <chrono> 
#include "json/json.h"
#include "jsoncpp.cpp"
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

struct Student
{
	int id;
	char name[20];
	int gender;
	char major[20]; 
    char address[50];
    char email[20];
    char phone[20];
};

string serializeToJSON(const Student& student) {
    Json::StreamWriterBuilder builder;
    Json::Value person;

    person["id"] = student.id;
    person["name"] = student.name;
    person["gender"] = student.gender;
    person["major"] = student.major;
    person["address"] = student.address;
    person["email"] = student.email;
    person["phone"] = student.phone;

    string strJSON = Json::writeString(builder, person);
    return strJSON;
}

Student deserializeToObj(const string& strJSON) {
    auto rawJsonLength = static_cast<int>(strJSON.length());
    JSONCPP_STRING err;
    Json::Value value;

    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(strJSON.c_str(), strJSON.c_str() + rawJsonLength, &value,
                       &err)) {
      std::cout << "error" << std::endl;
    }

    Student student;
    memset(&student, 0, sizeof(Student));
    student.id = value["id"].asInt();
	strcpy(student.name, value["name"].asString().c_str());  
	student.gender = value["gender"].asInt();
	strcpy(student.major, value["major"].asString().c_str());
    strcpy(student.address, value["address"].asString().c_str());
    strcpy(student.email, value["email"].asString().c_str());
    strcpy(student.phone, value["phone"].asString().c_str());
    return student;
}

int main(int argc, char** argv) {
    Student student;
	student.id = 12345;
	strcpy(student.name,"qin wang");
	student.gender = 0;
	strcpy(student.major,"software engineer");
    strcpy(student.address,"San Jose State University, San Jose, 95112");
    strcpy(student.email,"xxx@sjsu.edu");
    strcpy(student.phone,"111-111-1111");

    string strJSON = serializeToJSON(student);

	cout << "strJSON:" << strJSON << endl;

    Student student1 = deserializeToObj(strJSON);
    cout << "id: " << student1.id << endl;
    cout << "name: " << student1.name << endl;
    cout << "gender: " << student1.gender << endl;
    cout << "major: " << student1.major << endl;
    cout << "address: " << student1.address << endl;
    cout << "email: " << student1.email << endl;
    cout << "phone: " << student1.phone << endl;

    int size = strJSON.size();
    cout << "size of strJSON is: " << size << " bytes." << endl;

    auto start = high_resolution_clock::now();
    for (int i=0; i<100000; i++) {
        string tmp_strJSON = serializeToJSON(student);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    cout << "100000 serializeToJSON time: " << duration.count() << " microseconds." << endl;

    start = high_resolution_clock::now();
    for (int i=0; i<100000; i++) {
        Student tmp_student = deserializeToObj(strJSON);
    }
    end = high_resolution_clock::now();
    duration = duration_cast<microseconds>(end - start); 
    cout << "100000 deserializeToObj time: " << duration.count() << " microseconds." << endl;
}


