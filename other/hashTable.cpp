#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::cout;
using std::endl;

// Размер хеш-таблицы.
const int HashTableSize = 8;
const float ConstToReHash = 0.75;
const int a = 3; //взаимно просто со степенью двойки

// Хеш-таблица, реализованная методом цепочек.
class CHashTable {
public:
	CHashTable();
	~CHashTable();
	// Проверка наличия ключа в хеш-таблице.
	bool Has(const string& key);
	// Добавления ключа. Возвращает false, если ключ уже лежит в таблице.
	bool Add(const string& key);
	// Удаление ключа. Возвращает false, если ключа не было в таблице.
	bool Remove(const string& key);
	//Распечатывание таблицы
	void Show();
private:
	struct CHashTableNode {
		enum TSituation {
			S_NIL, S_DELETED, S_FILL
		};
		string Key;
		TSituation Situation;
		explicit CHashTableNode() : Key(""), Situation(S_NIL) {}
	};
	// Собственно таблица.
	CHashTableNode** table;
	int tableSize;
	int filled;
	bool grow();
	int StringHash(const string& key, int m);
};

int CHashTable::StringHash(const string& key, int m){
	int hash = 0;
	for (int i = 0; i < key.size(); ++i) {
		hash = (hash * a + key[i]) % m;
	}
	return hash;
}

CHashTable::CHashTable(){
	//table.resize(HashTableSize);
	table = new CHashTableNode*[HashTableSize];
	CHashTableNode example = CHashTableNode();
	tableSize = HashTableSize;
	for (size_t i = 0; i < tableSize; ++i)
	{
		table[i] = new CHashTableNode();
	}
	filled = 0;
}

CHashTable::~CHashTable(){
	for (int i = 0; i < tableSize; ++i) {
		delete table[i];
	}
	delete[] table;
} 

bool CHashTable::grow(){
	int newTableSize = 2 * tableSize;
	CHashTableNode** secondTable = new CHashTableNode*[newTableSize];
	for (size_t i = 0; i < newTableSize; ++i)
	{
		secondTable[i] = new CHashTableNode();
	}
	for (int i = 0; i < tableSize; ++i) {
		if (table[i]->Situation == CHashTableNode::S_FILL) {
			string key = table[i]->Key;
			int j = StringHash(key, newTableSize);
			for (int i = 0; i < newTableSize; ++i) {
				if (secondTable[j]->Situation == CHashTableNode::S_NIL) {
					secondTable[j]->Key = key;
					secondTable[j]->Situation = CHashTableNode::S_FILL;
					break;
				}
				j = (j + i) % newTableSize;
			}
		}
	}
	std::swap(table, secondTable);
	for (int i = 0; i < tableSize; ++i) {
		delete secondTable[i];
	}
	delete[] secondTable;
	tableSize = newTableSize;
	return true;
}

bool CHashTable::Has(const string& key){
	if (float(filled) / float(tableSize) == ConstToReHash) {
		grow();
	}
	int j = StringHash(key, tableSize);
	for (int i = 0; i < tableSize; ++i) {
		if (table[j]->Key == key) {
			return true;
		}
		if (table[j]->Situation == CHashTableNode::S_NIL) {
			return false;
		}
		j = (j + i) % tableSize;
	}
	return false;
}

bool CHashTable::Add(const string& key){
	if (Has(key)) {
		return false;
	}
	int j = StringHash(key, tableSize);
	for (int i = 0; i < tableSize; ++i) {
		if (table[j]->Situation == CHashTableNode::S_DELETED || table[j]->Situation == CHashTableNode::S_NIL) {
			table[j]->Key = key;
			table[j]->Situation = CHashTableNode::S_FILL;
			filled++;
			return true;
		}
		j = (j + i) % tableSize;
	}
}

void CHashTable::Show() {
	cout << "tableSize: " << tableSize << endl;
	for (int i = 0; i < tableSize; ++i) {
		cout << table[i]->Situation << ' ' << table[i]->Key << endl;
	}
}

bool CHashTable::Remove(const string& key){
	if (!Has(key)) {
		return false;
	}
	int j = StringHash(key, tableSize);
	for (int i = 0; i < tableSize; ++i) {
		if (table[j]->Key == key) {
			table[j]->Key = "";
			table[j]->Situation = CHashTableNode::S_DELETED;
			filled--;
			return true;
		}
		j = (j + i) % tableSize;
	}
}

int main(){
	CHashTable table;
	while (true) {
		char command = 0;
		string value;
		std::cin >> command >> value;
		if (std::cin.fail()) {
			break;
		}
		switch (command) {
		case '?':
			std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
			break;
		case '+':
			std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
			break;
		case '-':
			std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
			break;
		}
	}
	return 0;
}
