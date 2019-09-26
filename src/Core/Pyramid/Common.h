#ifndef DC_PRYAMID_COMMOM_H
#define DC_PRYAMID_COMMOM_H
#include <osg/ref_ptr>
#include <osg/Vec3>
#include <OSG/Geode>
#include <OSG/Geometry>
//#include "Common/BasicTypes.h"
#include "QString"

namespace DC
{
	namespace Pyra
	{
		//һЩ���õ�const��������
		const QString SUFFIX = "pdb";   //�����ļ���չ��
		const QString BOXEXT = "box";   //box�ļ��������

		//������������
		typedef int LevelType;
		typedef osg::Vec3f Point3D;
		typedef Point3D Point_3D;
		//����ʹ�õı�����Ϣ
		enum ErrorType
		{
			eNoError			=	0,
			eError				=	1
		};

		//�ļ�������ʽ
		enum SubNameFormat
		{
			eOSGFormat				=	0,
			eDCFormat				=	1
		};

		//�ָ�type
		enum SplitType
		{
			eAll					=	0,  //�ָд�����в�
			eSingle					=	1   //�ָд��ָ����
		};

		//ȫ�ֺ���
		inline QString GenIndexNameByIndex(unsigned column, unsigned row)
		{
			return QString("%1_%2").arg(column).arg(row);//QString::number(column).append("_").append(QString::number(row));
		}
	}
}

#endif