#ifndef MANAGED_RESOURCE_H
#define MANAGED_RESOURCE_H

class ManagedResource
{
public:
	ManagedResource();
	ManagedResource(bool isGlobalResource);
	virtual ~ManagedResource() = 0;
};

#endif // MANAGED_RESOURCE_H
