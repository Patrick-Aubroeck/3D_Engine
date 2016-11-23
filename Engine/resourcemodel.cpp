//#include "resourcemodel.h"
//
//#include <stdlib.h>
//#include <time.h>
//
//ResourceModel::ResourceModel()
//{
//
//}
//
//ResourceModel::~ResourceModel()
//{
//
//}
//
//bool ResourceModel::Initialize()
//{
//	CustomVector4 color;
//
//	m_modelInfoList = new CustomModelType();
//	if(!m_modelInfoList)
//	{
//		return false;
//	}
//
//	srand((unsigned int)time(NULL));
//
//	color.t_x = (float)rand() / RAND_MAX;
//	color.t_y = (float)rand() / RAND_MAX;
//	color.t_z = (float)rand() / RAND_MAX;
//	color.t_v = 1.0f; // alpha
//
////	m_modelInfoList->color = color;
////	m_modelInfoList->position.t_x = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
////	m_modelInfoList->position.t_y = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
////	m_modelInfoList->position.t_z = ((((float)rand()-(float)rand())/RAND_MAX) * 10.0f) + 5.0f;
//}
//
//void ResourceModel::Shutdown()
//{
//	if(m_modelInfoList)
//	{
//		delete m_modelInfoList;
//		m_modelInfoList = NULL;
//	}
//}
//
//int ResourceModel::GetModelCount()
//{
//	return m_modelCount;
//}
//
//void ResourceModel::GetData(int index, CustomModelType& customModel)
//{
//	customModel.position = m_modelInfoList[index].position;
//	customModel.color = m_modelInfoList[index].color;
//	//customModel.normal = m_modelInfoList[index].normal;
//	//customModel.texture = m_modelInfoList[index].texture;
//}