// PROJECT IDENTIFIER: 01BD41C3BF016AD7E8B6F837DF18926EC3E83350

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <deque>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <cstring>
#include <string>
using namespace std;

// Entry struct

struct Entry {
	vector<string> keywords;
	string category;
	string fullEntry;
	int long long timestamp;
	int entryID;

	Entry(vector<string> keys, string cat, string full, int long long timestamp, int entryID) : keywords(keys), category(cat), fullEntry(full), timestamp(timestamp), entryID(entryID) {}

	Entry() {}

	struct timeComp {
		bool operator()(const Entry &left, const Entry &right) {
			if (left.timestamp == right.timestamp) {
				if (left.category == right.category) {
					return left.entryID < right.entryID;
				}
				return left.category < right.category;
			}
			return left.timestamp < right.timestamp;
		}
	};

	struct timeCompPtr {
		bool operator()(const Entry *left, const Entry *right) {
			if (left->timestamp == right->timestamp) {
				if (left->category == right->category) {
					return left->entryID < right->entryID;
				}
				return left->category < right->category;
			}
			return left->timestamp < right->timestamp;
		}
	};
};

// Helper functions

bool isColon(char i) {
	if (i == ':') return true;
	return false;
}

int long long stampCast(string stamp) {
	stamp.erase(remove_if(stamp.begin(), stamp.end(), isColon), stamp.end());
	return stoll(stamp);
}

void toLower(string &input) {
	for (size_t i = 0; i < input.length(); ++i) {
		input[i] = tolower(input[i]);
	}
}

vector<string> extractKeywords(string words) {
	for (size_t i = 0; i < words.length(); ++i) {
		if (!isalnum(words[i])) words[i] = ' ';
	}

	toLower(words);

	stringstream keys;
	string keyword;
	vector<string> keywords;
	keys << words;

	while (keys >> keyword) {
		keywords.push_back(keyword);
	}

	return keywords;
}

/*--------COMMAND FUNCTIONS--------*/

void a(unordered_map<int, Entry*> ids, deque<Entry*> &excerpts);
void b(deque<Entry*> &excerpts);
void c(unordered_map<string, vector<Entry*>> &cats, vector<Entry*> &recents);
void d(deque<Entry*> &excerpts);
void e(deque<Entry*> &excerpts);
void g(vector<Entry*> &recents);
void k(unordered_map<string, set<Entry*>> &keys, vector<Entry*> &recents);
void l(deque<Entry*> &excerpts);
void m(vector<Entry> &master, vector<Entry*> &recents);
void p(deque<Entry*> &excerpts);
void r(vector<Entry*> &recent, deque<Entry*> &excerpts);
void s(deque<Entry*> &excerpts);
void t(vector<Entry> &master, vector<Entry*> &recents);

/*---------------------------------*/

int main(int argc, char* argv[]) {
	std::ios_base::sync_with_stdio(false);
	// Process command line
	string file;

	if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
		cout << "LOGMAN HELP: This program reads a file of log entries, then allows the user to manipulate those entries with a series of key commands.";
		exit(0);
	}
	else if (argc != 2) {
		cerr << "Ya done fucked up a-aron";
		exit(0);
	}
	else {
		file = argv[1];
	}

	/*--------READ FROM LOGFILE--------*/

	ifstream logFile;
	vector<Entry> master;
	unordered_map<string, vector<Entry*>> cats;
	unordered_map<string, set<Entry*>> kWords;
	unordered_map<int, Entry*> ids;
	//unordered_map<string, vector<
	string entry;
	int idCount = 0;

	logFile.open(file);

	while (getline(logFile, entry)) {
		string fullEntry = to_string(idCount) + "|" + entry;
		int long long timestamp = stampCast(entry.substr(0, 14));
		size_t line = entry.find('|', 15);
		string category = entry.substr(15, line - 15);
		vector<string> keywords = extractKeywords(entry.substr(line, entry.length()));	

		toLower(category);
		keywords.push_back(category);

		Entry curr(keywords, category, fullEntry, timestamp, idCount++);
		master.push_back(curr);
	} // while

	logFile.close();

	// Populate auxiliary containers
	for (size_t i = 0; i < master.size(); ++i) {
		ids[master[i].entryID] = &master[i];
		cats[master[i].category].push_back(&master[i]);
		for (auto k = master[i].keywords.begin(); k != master[i].keywords.end(); ++k) {
			kWords[*k].insert(&master[i]);
		}
	}

	Entry::timeComp comp;
	sort(master.begin(), master.end(), comp);

	/*---------------------------------*/
	/*--------COMMAND LOOP--------*/

	vector<Entry*> recentSearch;
	deque<Entry*> excerptList;
	string param;
	char command;
	do {
		cout << "% ";
		cin >> param;
		command = param[0];
		param.erase(param.begin());

		switch (command) {
		case 'a':
			a(ids, excerptList);
			break;
		case 'b':
			b(excerptList);
			break;
		case 'c':
			c(cats, recentSearch);
			break;
		case 'd':
			d(excerptList);
			break;
		case 'e':
			e(excerptList);
			break;
		case 'g':
			g(recentSearch);
			break;
		case 'k':
			k(kWords, recentSearch);
			break;
		case 'l':
			l(excerptList);
			break;
		case 'm':
			m(master, recentSearch);
			break;
		case 'p':
			p(excerptList);
			break;
		case 'r':
			r(recentSearch, excerptList);
			break;
		case 's':
			s(excerptList);
			break;
		case 't':
			t(master, recentSearch);
			break;
		case '#':
			getline(cin, param);
			break;
		default:
			break;
		}

	} while (command != 'q');

	/*----------------------------*/

	return 0;
}

void a(unordered_map<int, Entry*> ids, deque<Entry*> &excerpts) {
	size_t idx;
	cin >> idx;

	if (idx < ids.size()) {
		excerpts.push_back(ids[idx]);
		cout << "log entry " << idx << " appended\n";
	}
}

void b(deque<Entry*> &excerpts) {
	size_t idx;
	cin >> idx;

	if (idx < excerpts.size()) {
		excerpts.push_front(excerpts[idx++]);
		excerpts.erase(excerpts.begin() + idx);
		cout << "Moved excerpt list entry " << --idx << "\n";
	}
}

void c(unordered_map<string, vector<Entry*>> &cats, vector<Entry*> &recents) {
	string cat;
	cin >> cat;

	toLower(cat);

	if (!recents.empty()) recents.clear();

	for (size_t i = 0; i < cats.at(cat).size(); ++i) {
		recents.push_back(cats.at(cat)[i]);
	}

	cout << "Category search: " << cats.at(cat).size() << " entries found\n";
}

void d(deque<Entry*> &excerpts) {
	size_t idx;
	cin >> idx;

	if (idx < excerpts.size()) {
		excerpts.erase(excerpts.begin() + idx);
		cout << "Deleted excerpt list " << idx << "\n";
	}
}

void e(deque<Entry*> &excerpts) {
	size_t idx;
	cin >> idx;

	if (idx < excerpts.size()) {
		excerpts.push_back(excerpts[idx]);
		excerpts.erase(excerpts.begin() + idx);
		cout << "Moved excerpt list entry " << idx << "\n";
	}
}

void g(vector<Entry*> &recents) {
	if (!recents.empty()) {
		for (size_t i = 0; i < recents.size(); ++i) {
			cout << recents[i]->fullEntry << "\n";
		}
	}
}

void k(unordered_map<string, set<Entry*>> &keys, vector<Entry*> &recents) {
	string input;
	getline(cin, input);
	toLower(input);

	if (!recents.empty()) recents.clear();

	vector<string> keywords = extractKeywords(input);
	set<Entry*> matches(keys[keywords[0]]);

	for (size_t i = 1; i < keywords.size(); ++i) {
		set_intersection(matches.begin(), matches.end(), keys[keywords[i]].begin(), keys[keywords[i]].end(), inserter(matches, matches.begin()));
	}

	for (auto it = matches.begin(); it != matches.end(); ++it) {
		recents.push_back(*it);
	}

	cout << "Keyword search: " << matches.size() << " entries found\n";
}

void l(deque<Entry*> &excerpts) {
	cout << "excerpt list cleared\n";
	if (excerpts.size() == 0) cout << "(previously empty)\n";
	else {
		cout << "0|" << excerpts.front()->fullEntry << "\n"; // Not sure if newline needed here or not
		cout << "...\n" << excerpts.size() - 1 << "|" << excerpts.back()->fullEntry << "\n";

		excerpts.clear();
	}
}

void m(vector<Entry> &master, vector<Entry*> &recents) {
	string stamp;
	cin >> stamp;

	int long long ts = stampCast(stamp);
	Entry hi;
	Entry::timeComp comp;
	hi.timestamp = ts;

	auto it = lower_bound(master.begin(), master.end(), hi, comp);

	if (!recents.empty()) recents.clear();

	int count = 0;
	while ((*it).timestamp == ts) {
		recents.push_back(&(*it++));
		++count;
	}

	cout << "Timestamp search: " << count << " entries found\n";
}

void p(deque<Entry*> &excerpts) {
	for (size_t i = 0; i < excerpts.size(); ++i) {
		cout << i << "|" << excerpts[i]->fullEntry << "\n";
	}
}

void r(vector<Entry*> &recent, deque<Entry*> &excerpts) {
	for (size_t i = 0; i < recent.size(); ++i) {
		excerpts.push_back(recent[i]);
	}
}

void s(deque<Entry*> &excerpts) {
	Entry::timeCompPtr comp;

	cout << "excerpt list sorted\n";
	if (excerpts.empty()) cout << "(previously empty)\n";
	else {
		cout << "previous ordering:\n";
		cout << "0|" << excerpts.front()->fullEntry << "\n";
		cout << "...\n" << excerpts.size() - 1 << "|" << excerpts.back()->fullEntry << "\n";
		sort(excerpts.begin(), excerpts.end(), comp);
		cout << "new ordering:\n";
		cout << "0|" << excerpts.front()->fullEntry << "\n";
		cout << "...\n" << excerpts.size() - 1 << "|" << excerpts.back()->fullEntry << "\n";
	}
}

void t(vector<Entry> &master, vector<Entry*> &recents) {
	Entry dummy1;
	Entry dummy2;
	Entry::timeComp comp;
	string s1;
	string s2;

	cin >> s1;

	if (s1.length() != 29 || s1[14] != '|') {
		cerr << "Well shit, timeRange input isn't good!"; // Not sure if I'm supposed to exit or not
		return;
	}
	else {
		if (!recents.empty()) recents.clear(); // Will need to check if this is slow or not
	}

	s2 = s1.substr(15, 14);
	s1 = s1.substr(0, 14);

	dummy1.timestamp = stampCast(s1);
	dummy2.timestamp = stampCast(s2);

	auto front = lower_bound(master.begin(), master.end(), dummy1, comp);
	auto back = lower_bound(front, master.end(), dummy2, comp);

	for (auto it = front; it != back; ++it) {
		Entry* add = &(*it);
		recents.push_back(add);
	}

	cout << "Timestamp search: " << recents.size() << " entries found\n";
}