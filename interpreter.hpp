#pragma once
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// #include "model.hpp"

enum class COMMAND
{
	ROTATE, SCALE, MOVE, MOVETO
};

using commandPair = std::pair<COMMAND, glm::vec4>;
using commandVector = std::vector<commandPair>;
using commandMap = std::map<std::string, commandVector>;

class Interpreter
{
	public:
		Interpreter();
		~Interpreter();
		commandVector Interpret(std::string code);
		void Run();
		commandVector GetCom() {return this -> v_command;}
		bool hasLoop = false;

	private:		
		
		commandVector v_command = commandVector();
};