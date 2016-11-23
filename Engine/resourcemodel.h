#ifndef _RESOURCEMODEL_H_
#define _RESOURCEMODEL_H_

#include "customstruct.h"

class ResourceModel
{
public:
	ResourceModel();
	~ResourceModel();
	bool Initialize();
	void Shutdown();

	int GetModelCount();
	void GetData(int index, CustomVertexType& customModel);

private:
	int m_modelCount;
	CustomVertexType* m_modelInfoList;
private:
};

#endif
