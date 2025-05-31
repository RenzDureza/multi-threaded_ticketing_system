#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <ostream>
#include <pthread.h>
#include <string>
#include <random>
#include <vector>
using namespace std;

mutex device_mtx;
mutex user_mtx;

class Device {
  private:
  string device_name;
  int device_id;
  bool power_state;

  public:
  Device(string device_name, int device_id, bool power_state) {
    this->device_name = device_name;
    this->device_id = device_id;
    this->power_state = power_state;
  }

  string get_device_name() {
    return device_name;
  }

  int get_device_id() {
    return device_id;
  }

  void update_device_deets(string new_device_name) {
    device_name = new_device_name;
  }
};

class User{
  private:
  string fullname;
  string username;
  string password;

  public:
  User(string fullname, string username, string password) {
    this->fullname = fullname;
    this->username = username;
    this->password = password;
  }

  string get_user_name() {
    return username;
  }

  string get_password() {
    return password;
  }
};

User* current_user = nullptr;

vector<Device> device_list;
vector<User> user_list;

bool device_exist(string device_name);
void add_new_device(); 
void update_device(int device_id); 
void remove_device(int device_id); 
void register_user();
bool log_in();
void log_out();

int main() {
  return 0;
}

bool device_exist(string device_name) {
  for (Device i : device_list) {
    if(i.get_device_name() == device_name) {
      return true;
    }
  }
  return false;
}

void add_new_device() {
  string device_name;
  bool power_state = false;

  cout << "Enter device name: ";
  getline(cin, device_name);

  if (!device_exist(device_name)) {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(1000, 9999);

    int device_id = dis(gen);

    lock_guard<mutex> lock(device_mtx);
    Device new_device(device_name, device_id, power_state);
    device_list.push_back(new_device);

    cout << new_device.get_device_name() << " is now part of the system" << endl;
  } else {
    cout << device_name << " has failed to join the system" << endl;
  }
}

void update_device(int device_id) {
  string new_name;

  lock_guard<mutex> lock(device_mtx);
  for (Device& temp : device_list) {
    if (temp.get_device_id() == device_id) {
      cout << "Enter new device name: ";
      cin >> new_name;

      temp.update_device_deets(new_name);

      cout << "Successfully updated device ID: " << temp.get_device_id() << endl;
    } else {
      cout << "Can't find device with ID: " << device_id << endl;
    }
  }
} 

void remove_device(int device_id) {
  for (auto it = device_list.begin(); it != device_list.end();) {
    if (it->get_device_id() == device_id) {
      it = device_list.erase(it);
    } else {
      ++it;
    }
  }
}

void register_user() {
  string fullname, user_name, password;

  cout << "Register" << endl;
  cout << "Full Name: ";
  getline(cin, fullname);
  cout << "Username: ";
  getline(cin, user_name);
  cout << "Password: ";
  getline(cin, password);

  lock_guard<mutex> lock(user_mtx);
  User newUser(fullname, user_name, password);
  user_list.push_back(newUser);

  cout << newUser.get_user_name() << " have joined the system" << endl;
}

bool log_in() {
  string user_name, password;
  cout << "Login" << endl;
  cout << "Username: ";
  getline(cin, user_name);
  cout << "Password: ";
  getline(cin, password);

  for (User& i : user_list) {
    if ((i.get_user_name() == user_name) && (i.get_password() == password)) {
      current_user = &i;
      return true;
    }
  }
  return false;
}

void log_out() {
  if (current_user != nullptr) {
    cout << current_user->get_user_name() << " has been logged out." << endl;
    current_user = nullptr;
  } else {
    cout << "No user is currently logged in." << endl;
  }
}
