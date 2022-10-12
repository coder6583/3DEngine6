#include "interpreter.hpp"

Interpreter::Interpreter()
{
}
Interpreter::~Interpreter()
{
	
}

commandVector Interpreter::Interpret(std::string code)
{
	std::stringstream ss(code);
	std::string commandLine;
	this -> v_command.clear();
	while(std::getline(ss, commandLine))
	{
		std::stringstream liness(commandLine);
		std::string command;
		glm::vec4 comPara;
		liness >> command;
		if(command == "MOVE")
		{
			liness >> comPara.x >> comPara.y >> comPara.z;
			comPara.w = -1.0f;
			this -> v_command.push_back(std::make_pair(COMMAND::MOVE, comPara));
		}
		else if(command == "MOVETO")
		{
			liness >> comPara.x >> comPara.y >> comPara.z;
			comPara.w = -1.0f;
			this -> v_command.push_back(std::make_pair(COMMAND::MOVETO, comPara));
		}
		else if(command == "ROTATE")
		{
			liness >> comPara.x >> comPara.y >> comPara.z >> comPara.w;
			this -> v_command.push_back(std::make_pair(COMMAND::ROTATE, comPara));
		}
		else if(command == "SCALE")
		{
			liness >> comPara.x >> comPara.y >> comPara.z;
			comPara.w = -1.0f;
			this -> v_command.push_back(std::make_pair(COMMAND::SCALE, comPara));
		}
		else if(command == "LOOP")
		{	
			this -> hasLoop = true;
			std::string loopName, loopNum;
			liness >> loopName >> loopNum;
			if(loopNum == "INF")
			{
				commandVector loopCommands;
// 				if(m_loopCommand.find(model -> GetName()) == m_loopCommand.end())
// 					m_loopCommand.insert(std::make_pair(model -> GetName(), loopCommands));
				while(std::getline(ss, commandLine))
				{
					std::stringstream loopLiness(commandLine);
					loopLiness >> command;
					if(command == loopName)
					{
						break;
					}
					else if(command == "MOVE")
					{
						loopLiness >> comPara.x >> comPara.y >> comPara.z;
						comPara.w = -1.0f;
						loopCommands.push_back(std::make_pair(COMMAND::MOVE, comPara));
					}	
					else if(command == "MOVETO")
					{
						loopLiness >> comPara.x >> comPara.y >> comPara.z;
						comPara.w = -1.0f;
						loopCommands.push_back(std::make_pair(COMMAND::MOVETO, comPara));
					}
					else if(command == "ROTATE")
					{
						loopLiness >> comPara.x >> comPara.y >> comPara.z >> comPara.w;
						loopCommands.push_back(std::make_pair(COMMAND::ROTATE, comPara));
					}
					else if(command == "SCALE")
					{
						loopLiness >> comPara.x >> comPara.y >> comPara.z;
						comPara.w = -1.0f;
						loopCommands.push_back(std::make_pair(COMMAND::SCALE, comPara));
					}				
				}
				return loopCommands;
			}
		}
	}
	return commandVector();
}

void Interpreter::Run()
{

}
















