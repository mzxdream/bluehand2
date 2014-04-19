#include "BhGateWork.hpp"

BhGateWork::BhGateWork(unsigned uMaxEvents)
	:m_uMaxEvents(uMaxEvents)
{
}
BhGateWork::~BhGateWork()
{
}
int BhGateWork::Init()
{
	if (this->IBhActiveModule::Init() != 0)
	{
		return -1;
	}
	
	return 0;
}
