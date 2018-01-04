#ifndef RESOURCEBASE_H
#define RESOURCEBASE_H

#include "SystemIncludes.h"

// -------------------------------
// Resource abstraites
// -------------------------------

class ResourceBase
{
public:
	//virtual bool Load(const string& name) = 0;
	virtual void Destroy() = 0;
};

#endif

