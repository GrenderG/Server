#include "iostream"
#include "../common/global_define.h"
#include "../common/cli/eqemu_command_handler.h"

#ifndef EQEMU_WORLD_SERVER_COMMAND_HANDLER_H
#define EQEMU_WORLD_SERVER_COMMAND_HANDLER_H

class WorldserverCLI {
public:
	static void CommandHandler(int argc, char **argv);
	static void Version(int argc, char **argv, argh::parser &cmd, std::string &description);
	static void DatabaseVersion(int argc, char **argv, argh::parser &cmd, std::string &description);
	static void DatabaseSetAccountStatus(int argc, char** argv, argh::parser& cmd, std::string& description);
	static void DatabaseGetSchema(int argc, char **argv, argh::parser &cmd, std::string &description);
	static void DatabaseDump(int argc, char **argv, argh::parser &cmd, std::string &description);
	static void TestCommand(int argc, char **argv, argh::parser &cmd, std::string &description);
	static void ExpansionTestCommand(int argc, char **argv, argh::parser &cmd, std::string &description);
	static void EtlGetSettings(int argc, char **argv, argh::parser &cmd, std::string &description);
};


#endif //EQEMU_WORLD_SERVER_COMMAND_HANDLER_H