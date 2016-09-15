/**
* \author       unknown
* \copyright    https://opensource.org/licenses/MIT License
*/
#ifndef IncrwFileParser
#define IncrwFileParser

#include <string>
#include <map>
#include <list>
using namespace std;

/*
   rwFileParser

   Parse structured config files

   rwFileParser files contains lines with name-value assignements in the form "<name> = <value>".
   Trailing and leading whitespace is stripped. Parsed config entries are stored in
   a symbol map.
   
   Lines beginning with '#' are a comment and ignored.

   rwFileParser files may be structured (to arbitrary depth). To start a new config sub group
   (or sub section) use a line in the form of "<name> = (".
   Subsequent entries are stured in the sub group, until a line containing ")" is found.

   Values may reuse already defined names as a variable which gets expanded during
   the parsing process. Names for expansion are searched from the current sub group
   upwards. Finally the process environment is searched, so also environment
   variables may be used as expansion symbols in the config file.

   Errors and warnings are handled by emitting logging messages (see log.h/log.cpp)
   or by calling exit() for severe errors. Depending on project needs this may be replaced
   by exeptions, error return codes, ...
 */


class rwFileParser {
	public:
		/* Parse config file 'configFile'. If the process environment
		 * is provided, environment variables can be used as expansion symbols.
		 */
		rwFileParser(string configFile, char** envp = 0);

		/* no shitty envp */
		rwFileParser(string configFile, int &error) ; 

		~rwFileParser();
		
		// get string config entry
		string pString(string name);

		/* get boolean config entry
		 * A value of Yes/yes/YES/true/True/TRUE leads to true,
		 * all other values leads to false.
		 */
		bool pBool(string name);

		// get double config entry; value is parsed using atof()
		double pDouble(string name);

		// get int config entry; value is parsed using atoi()
		int pInt(string name);

		// get the symbol map (e.g. for iterating over all symbols)
		inline map<string, string>& getSymbols() {
			return symbols;
		}

		// get config sub group
		inline rwFileParser* group(string name) {
			return groups[name];
		}

		// get config sub group map (e.g. for iterating over all groups)
		inline map<string, rwFileParser*>& getGroups() {
			return groups;
		}

	private:
		// private constructor for sub groups
		rwFileParser(string name, string parentDebugInfo);

		// helper functions for parsing
		void add(string name, string value);
		void split(string in, string& left, string& right, char c);
		void trim(string& s);
		void symbolExpand(string& s);
		void symbolExpand(map<string, string>& symbols, string& s);
		void envSymbolExpand(string& s);
		
		// config group symbol map
		map<string, string> symbols;

		// environment symbol map
		map<string, string> envSymbols;

		// config sub group map
		map<string, rwFileParser*> groups;

		// stack of config groups for parsing (only used in top config element)
		list<rwFileParser*> groupStack;

		// debug info used for logging messages
		string debugInfo;
};

#endif




/**

example 






#include <iostream>
using namespace std;

#include "config.h"

int main(int argc, char* argv[], char* envp[])
{
	// read config file with environment variable expansion support
	rwFileParser config("demo.config", envp);
	
	// basic usage of properties
	if (config.pBool("showHello")) {
		int cnt = config.pInt("helloCount");
		string msg = config.pString("helloMessage");
		for (int i = 0; i < cnt; ++i) {
			cout << msg << endl;
		}
		cout << endl;
	}

	// properties with expanded names (no difference in using)
	cout << "tempFolder    = '" << config.pString("tempFolder") << "'" << endl;
	cout << "tempSubFolder = '" << config.pString("tempSubFolder") << "'" << endl;
	cout << endl;

	// get properties for all subgroups starting with prefix
	map<string, rwFileParser*> messages = config.getGroups(); // all groups
	const string messagePrefix = "message"; // prefix for group name
	for (map<string, rwFileParser*>::iterator i = messages.begin(); i != messages.end(); ++i) {
		string groupName = i->first;
		rwFileParser* group = i->second;

		// test group name for prefix
		if (groupName.substr(0, messagePrefix.length()) == messagePrefix) {
			// display group contents
			cout << group->pString("name") << ":" << endl;
			cout << "   " << group->pString("text") << endl;
		}
	}

	return 0;
}


*/