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
		input[i] = static_cast<char>(tolower(input[i]));
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

void clearRecents(vector<int> &recents) {
    recents.clear();
    recents.push_back(INT8_MAX);
}

/*--------COMMAND FUNCTIONS--------*/

void a(vector<Entry> &master, deque<int> &excerpts);
void b(deque<int> &excerpts);
void c(unordered_map<string, vector<int>> &cats, vector<int> &recents);
void d(deque<int> &excerpts);
void e(deque<int> &excerpts);
void g(vector<Entry> &master, vector<int> &recents);
void k(unordered_map<string, set<int>> &keys, vector<int> &recents);
void l(vector<Entry> &master, deque<int> &excerpts);
void m(vector<Entry> &master, vector<int> &recents);
void p(vector<Entry> &master, deque<int> &excerpts);
void r(vector<int> &recent, deque<int> &excerpts);
void s(vector<Entry> &master, deque<int> &excerpts);
void t(vector<Entry> &master, vector<int> &recents);

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
	unordered_map<string, vector<int>> cats;
	unordered_map<string, set<int>> kWords;
	string entry;
	int idCount = 0;

	logFile.open(file);

	while (getline(logFile, entry)) {
		string fullEntry = to_string(idCount) + "|" + entry;
		int long long timestamp = stampCast(entry.substr(0, 14));
		size_t line = entry.find('|', 15);
		string category = entry.substr(15, line - 15);
        toLower(category);
        vector<string> keywords = extractKeywords(entry.substr(15, entry.length())); // the entry.length() might be wrong

		Entry curr(keywords, category, fullEntry, timestamp, idCount++);
		master.push_back(curr);
	} // while

	logFile.close();
	

	Entry::timeComp comp;
	sort(master.begin(), master.end(), comp);

    for (size_t i = 0; i < master.size(); ++i) {
        cats[master[i].category].push_back(static_cast<int>(i));
        for (auto k = master[i].keywords.begin(); k != master[i].keywords.end(); ++k) {
            kWords[*k].insert(static_cast<int>(i));
        }
    }

    cout << master.size() << " entries read\n";

	/*---------------------------------*/
	/*--------COMMAND LOOP--------*/

	vector<int> recentSearch;
	deque<int> excerptList;
	string param;
	char command;
	do {
		cout << "% ";
		cin >> command;

		switch (command) {
		case 'a':
			a(master, excerptList);
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
			g(master, recentSearch);
			break;
		case 'k':
			k(kWords, recentSearch);
			break;
		case 'l':
			l(master, excerptList);
			break;
		case 'm':
			m(master, recentSearch);
			break;
		case 'p':
			p(master, excerptList);
			break;
		case 'r':
			r(recentSearch, excerptList);
			break;
		case 's':
			s(master, excerptList);
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

void a(vector<Entry> &master, deque<int> &excerpts) {
	size_t idx;
	cin >> idx;

	if (idx < master.size()) {
		excerpts.push_back(static_cast<int>(idx));
		cout << "log entry " << idx << " appended\n";
    }
    else {
        cerr << "Index out of range\n";
    }
}

void b(deque<int> &excerpts) {
	size_t idx;
	cin >> idx;

	if (idx < excerpts.size()) {
		excerpts.push_front(excerpts[idx++]);
		excerpts.erase(excerpts.begin() + idx);
		cout << "Moved excerpt list entry " << --idx << "\n";
	}
    else {
        cerr << "Index out of range\n";
    }
}

void c(unordered_map<string, vector<int>> &cats, vector<int> &recents) {
	string cat;
    getline(cin, cat);
    cat.erase(0, 1);

	toLower(cat);

	clearRecents(recents);

    cout << "Category search: ";

    if (cats.find(cat) != cats.end()) {
        for (size_t i = 0; i < cats.at(cat).size(); ++i) {
            recents.push_back(cats.at(cat)[i]);
        }

        cout << cats.at(cat).size();
    }
    else {
        cout << "0";
    }

    cout << " entries found\n";
}

void d(deque<int> &excerpts) {
	size_t idx;
	cin >> idx;

	if (idx < excerpts.size()) {
		excerpts.erase(excerpts.begin() + idx);
		cout << "Deleted excerpt list entry " << idx << "\n";
    }
    else {
        cerr << "Index out of range\n";
    }
}

void e(deque<int> &excerpts) {
	size_t idx;
	cin >> idx;

	if (idx < excerpts.size()) {
		excerpts.push_back(excerpts[idx]);
		excerpts.erase(excerpts.begin() + idx);
		cout << "Moved excerpt list entry " << idx << "\n";
	}
    else {
        cerr << "Index out of range\n";
    }
}

void g(vector<Entry> &master, vector<int> &recents) {
	if (!recents.empty()) {
		for (size_t i = 1; i < recents.size(); ++i) {
			cout << master[recents[i]].fullEntry << "\n";
		}
	}
    else {
        cerr << "No recent searches\n";
    }
}

void k(unordered_map<string, set<int>> &keys, vector<int> &recents) {
	string input;
	getline(cin, input);
	toLower(input);

	clearRecents(recents);

	vector<string> keywords = extractKeywords(input);
	set<int> matches(keys[keywords[0]]);

	for (size_t i = 1; i < keywords.size(); ++i) {
        set<int> result;
		set_intersection(matches.begin(), matches.end(), keys[keywords[i]].begin(), keys[keywords[i]].end(), inserter(result, result.begin()));
        matches = result;
	}

	for (auto it = matches.begin(); it != matches.end(); ++it) {
		recents.push_back(*it);
	}

	cout << "Keyword search: " << matches.size() << " entries found\n";
}

void l(vector<Entry> &master, deque<int> &excerpts) {
	cout << "excerpt list cleared\n";
	if (excerpts.empty()) cout << "(previously empty)\n";
	else {
        cout << "previous contents:\n";
		cout << "0|" << master[excerpts.front()].fullEntry << "\n";
		cout << "...\n" << excerpts.size() - 1 << "|" << master[excerpts.back()].fullEntry << "\n";

		excerpts.clear();
	}
}

void m(vector<Entry> &master, vector<int> &recents) {
	string stamp;
	cin >> stamp;

    if (stamp.length() != 14) {
        cerr << "Invalid timestamp\n";
        return;
    }

	int long long ts = stampCast(stamp);
	Entry hi;
	Entry::timeComp comp;
	hi.timestamp = ts;

	auto it = lower_bound(master.begin(), master.end(), hi, comp);

	clearRecents(recents);

	int count = 0;
	while ((*it).timestamp == ts) {
		recents.push_back(static_cast<int>(it++ - master.begin()));
		++count;
	}

	cout << "Timestamp search: " << count << " entries found\n";
}

void p(vector<Entry> &master, deque<int> &excerpts) {
	for (size_t i = 0; i < excerpts.size(); ++i) {
		cout << i << "|" << master[excerpts[i]].fullEntry << "\n";
	}
}

void r(vector<int> &recent, deque<int> &excerpts) {

    if (!recent.empty()) {
        for (size_t i = 1; i < recent.size(); ++i) {
            excerpts.push_back(recent[i]);
        }

        cout << recent.size() - 1 << " log entries appended\n";
    }
    else {
        cerr << "No recents searches\n";
    }
}

void s(vector<Entry> &master, deque<int> &excerpts) {

	cout << "excerpt list sorted\n";
	if (excerpts.empty()) cout << "(previously empty)\n";
	else {
		cout << "previous ordering:\n";
		cout << "0|" << master[excerpts.front()].fullEntry << "\n";
		cout << "...\n" << excerpts.size() - 1 << "|" << master[excerpts.back()].fullEntry << "\n";
		sort(excerpts.begin(), excerpts.end());
		cout << "new ordering:\n";
		cout << "0|" << master[excerpts.front()].fullEntry << "\n";
		cout << "...\n" << excerpts.size() - 1 << "|" << master[excerpts.back()].fullEntry << "\n";
	}
}

void t(vector<Entry> &master, vector<int> &recents) {
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
        clearRecents(recents);
	}

	s2 = s1.substr(15, 14);
	s1 = s1.substr(0, 14);

	dummy1.timestamp = stampCast(s1);
	dummy2.timestamp = stampCast(s2);

	auto front = lower_bound(master.begin(), master.end(), dummy1, comp);
	auto back = lower_bound(front, master.end(), dummy2, comp);

	for (auto it = front; it != back; ++it) {
		recents.push_back(static_cast<int>(it - master.begin()));
	}

	cout << "Timestamps search: " << recents.size() - 1 << " entries found\n";
}