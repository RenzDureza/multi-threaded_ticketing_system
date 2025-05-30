// DUREZA 2025-05-30 19:37
// TODO:
// - register_user()
// - log_in()
// - log_out()
// - log_in menu

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <ostream>
#include <thread>
#include <string>
#include <random>
#include <vector>
using namespace std;

mutex ticket_mtx;
mutex user_mtx;
mutex event_mtx;

class Event {
  private:
  string event_name;
  string event_date;
  int event_id;
  int available_tickets;

  public:
  Event(string event_name, string event_date, int event_id,int available_tickets) {
    this->event_name = event_name;
    this->event_date = event_date;
    this->event_id = event_id;
    this->available_tickets = available_tickets;
  }

  bool purchase_ticket() {
    lock_guard<mutex> lock(event_mtx);
    if (available_tickets > 0) {
      --available_tickets;
      return true;
    } else {
      return false;
    }
  }

  void cancel_ticket() {
    lock_guard<mutex> lock(event_mtx);
    ++available_tickets;
  }

  void view_available_tickets() {
    lock_guard<mutex> lock(event_mtx);
    cout << available_tickets << endl;
  }

  string get_event_name() {
    return event_name;
  }

  string get_event_date() {
    return event_date;
  }

  int get_event_id() {
    return event_id;
  }

  void update_event_deets(string new_name, string new_date, int new_ticket) {
    event_name = new_name;
    event_date = new_date;
    available_tickets = new_ticket;
  }
};

class User{
  private:
  string fullname;
  string username;
  string passw;

  public:
  User(string fullname, string username, string passw) {
    this->fullname = fullname;
    this->username = username;
    this->passw = passw;
  }

  string get_user_name() {
    lock_guard<mutex> lock(user_mtx);
    return fullname;
  }
};

// Gumawa na rin ako ng class para sa ticket for testing lang naman, kayo nalang bahala if want nyo palitan or i modify
/* class Ticket{
  private:
  string user_name;
  string event_name;
  string event_date;
  int id;

  public:
  Ticket (string user_name, string event_name, string event_date, int id) {
    this->user_name = user_name;
    this->event_name = event_name;
    this->event_date = event_date;
    this->id = id;
  }

  void display_ticket() {
    cout << "=====================================" << endl;
    cout << "| Username: " << user_name << endl;
    cout << "|====================================" << endl;
    cout << "| Event: " << event_name << endl;
    cout << "| Date: " << event_date << endl;
    cout << "|====================================" << endl;
    cout << "| Ticket ID: " << id << endl;
    cout << "|====================================" << endl;
  }
};

vector<Ticket> ticket_list; */

// not sure pa kung need
// vector<User> user_list;

vector<Event> event_list;

void purchase_ticket(User* usr, Event* evt);
void add_event(string event_name, string event_date, int available_tickets);
void update_event(int event_id);
bool event_exists(string event_date);
bool event_id_exists(int event_id);
void remove_event(int event_id);

int main() {
  string name, date;
  int event_id, available_tickets;
  Event evt1("bday", "12-02-2025", 1, 10);
  User usr1("Aldrin Renz Dureza", "str4t_", "secret");
  purchase_ticket(&usr1, &evt1);

  cout << "done\n";
  return 0;
}

// Gumawa na rin ako ng function para sa pag purchase ng ticket, for testing lang naman, kayo nalang bahala if want nyo palitan or i modify
/*
void purchase_ticket(User* usr, Event* evt) {
  if (evt->purchase_ticket()) {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(1000, 9999);
    int id = dis(gen);

    string user_name = usr->get_user_name();
    string event_name = evt->get_event_name();
    string event_date = evt->get_event_date();

    Ticket new_ticket(user_name, event_name, event_date, id);

    lock_guard<mutex> lock(ticket_mtx);
    ticket_list.push_back(new_ticket);

    cout << usr->get_user_name() << " have purchased a ticket." << endl;
    new_ticket.display_ticket();
  } else {
    cout << "Unable to purchase ticket for " << evt->get_event_name() << " event" << endl;
  }
}
*/

bool event_exists(string event_name, string event_date) {
  for (Event& i : event_list) {
    if (i.get_event_name() == event_name && i.get_event_date() == event_date) {
      return true;
    }
  }
  return false;
}

bool event_id_exists(int event_id) {
  for (Event i : event_list) {
    if (i.get_event_id() == event_id) {
      return true;
    }
  }
  return false;
}

void add_event(string event_name, string event_date, int available_tickets) {
  if (!event_exists(event_date)) {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<> dis(1000, 9999);

    int generate_id = dis(gen);

    Event new_event(event_name, event_date, generate_id, available_tickets);

    lock_guard<mutex> lock(event_mtx);
    event_list.push_back(new_event);

    cout << new_event.get_event_name() << " has been made, and schduled at " << new_event.get_event_date() << endl;
  } else {
    cout << "An existing event is going to held on that day" << endl;
  }
}

void remove_event(int event_id) {
  for (auto it = event_list.begin(); it != event_list.end();) {
    if (it->get_event_id() == event_id) {
      it = event_list.erase(it);
    } else {
      ++it;
    }
  }
}

void update_event(int event_id) {
  string new_name, new_date;
  int new_ticket;

  lock_guard<mutex> lock(event_mtx);
  for (Event& temp : event_list) {
    if (temp.get_event_id() == event_id) {
      cout << "Enter new event name: ";
      cin >> new_name;
      cout << "\nEnter new event date: ";
      cin >> new_date;
      cout << "\nEnter new number of guests: ";
      cin >> new_ticket;

      temp.update_event_deets(new_name, new_date, new_ticket);

      cout << "Successfully updated event ID: " << temp.get_event_id() << endl;
    }
  }
}
