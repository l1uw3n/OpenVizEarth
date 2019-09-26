#include "GridFilter.h"
#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QFileInfo>
#include "QTextStream"
#include "QFile"
#include "QRegExp"
#include "QStringList"
#include "qglobal.h"
#include <algorithm>
#include <string>
#include <memory>
#include <cmath>
#include <fstream>				// std::ifstream
#include <iostream>				// std::cout
#include <array>

//liblas
#include "liblas/point.hpp"
#include "liblas/reader.hpp"
#include "liblas/writer.hpp"
#include "liblas/factory.hpp"

using namespace std;

GridFilter::GridFilter(std::vector<DCVector3D> inPutPoint, QString fileOut)
	: m_inPutPoint(inPutPoint)
{
	m_fileOut = fileOut;
}

GridFilter::GridFilter(QString fileIn, QString fileOut)
{
	m_fileIn = fileIn;
	m_fileOut = fileOut;
}



GridFilter::~GridFilter()
{

}


void GridFilter::SetFBL(double xFBL, double yFBL)
{
	XFBL = xFBL;
	YFBL = yFBL;
}

void GridFilter::GetOutput(std::vector<DCVector3D> & points)
{
	points = m_LowerPoint;
	m_LowerPoint.clear();
}


void GridFilter::DoFilter()
{
	QFileInfo fileInfo(m_fileIn);

	QString exten = fileInfo.suffix();
	bool readState = false;
	if ("LAS" == exten.toUpper())
	{
		readState = ReadLasFile(m_fileIn);
	}
	else if ("DAT" == exten.toUpper() )
	{
		readState = ReadTXTAscii(m_fileIn);
	}
	
	if (!readState)
	{
		return;
	}

	Fastfast(m_inPutPoint, 0);
	WriteDatAsccii(m_fileOut);
}


void GridFilter::Fastfast(std::vector<DCVector3D> & points, unsigned num)
{

	//����ѡ���ͶӰ�棬������Ӧ�ķ�������

	double tempXfbl = XFBL + num * XFBL/2.0;
	double tempYfbl = YFBL + num * YFBL/2.0;
	m_LowerPoint.clear();

	//��ÿ����x,y,z�������Сֵ
	std::vector<double> Maxminxyz;
	std::sort(points.begin(),points.end(),[](DCVector3D v1,DCVector3D v2)->bool{return (v1.x() < v2.x());});
	Maxminxyz.push_back(points[0].x() + 0);
	Maxminxyz.push_back(points[points.size()-1].x());

	std::sort(points.begin(),points.end(),[](DCVector3D v1,DCVector3D v2)->bool{return (v1.y() < v2.y());});
	Maxminxyz.push_back(points[0].y() + 0);
	Maxminxyz.push_back(points[points.size()-1].y());

	std::sort(points.begin(),points.end(),[](DCVector3D v1,DCVector3D v2)->bool{return (v1.z() < v2.z());});
	Maxminxyz.push_back(points[0].z());
	Maxminxyz.push_back(points[points.size()-1].z());

	double zxxz  = floor(Maxminxyz[0]);
	double zxyz  = floor(Maxminxyz[2]);
	double zxzz  = floor(Maxminxyz[4]);

	//������������õ��Ƶ��кš��кš���ŵ����ֵ
	int xnum  = floor((Maxminxyz[1]-zxxz)/tempXfbl)+1; 
	int ynum  = floor((Maxminxyz[3]-zxyz)/tempYfbl)+1; 
	//int znum  = floor((Maxminxyz[5]-zxzz)/ZFBL)+1;

	//���Ĳ��������������һ������������ܵ���
	int allbox = xnum*ynum; 
	int quzs   = xnum*ynum;
	int Psum   = points.size(); 

	//���岽����ʼ����Ÿ����������
	m_fq32.clear();
	for (int i=0;i<allbox;i++)
	{
		std::vector<DCVector3D> point1;
		fq32 pxzb;
		pxzb.quhao=i;
		pxzb.m_px32=point1;
		m_fq32.push_back(pxzb);    
	}

	//��������������ڸ�������
	//for (auto i=m_points.begin();i<m_points.end();i++)
	for (unsigned i=0; i < points.size();i++)
	{
		int COLUMNNUM = floor(( (points[i])[0]-zxxz )/tempXfbl);
		int ROWNUM    = floor(( (points[i])[1]-zxyz )/tempYfbl);
		//int	LAYERNUM  = floor(( (points[i])[2]-zxzz )/ZFBL);
		int index     = ROWNUM*xnum+COLUMNNUM+1;	
		m_fq32[index-1].m_px32.push_back((points[i]));
	}

	//���߲������е����ֻ�����߳���͵ĵ�ŵ���һ������
	for (int i=0;i<allbox;i++)
	{
		//����ÿ���Zֵ����͵�
		std::vector<DCVector3D> point1;
		point1=m_fq32.at(i).m_px32;
		if (point1.size()>0)
		{			
			//��ǰ����z����
			std::sort(point1.begin(),point1.end(),[](DCVector3D v1,DCVector3D v2)->bool{return (v1.z() < v2.z());});
			//�ӵ�ǰ������ɾ������1/3�Ĳ��̵ָ߳�
			m_LowerPoint.push_back(point1.at(0));
		}
	}
	m_inPutPoint = m_LowerPoint;

}

bool GridFilter::ReadTXTAscii(QString qFilename)
{
	QFile inFile(qFilename);
	if (!inFile.open(QIODevice::ReadOnly))
	{
		return false;
	}

	char currentLine[500];
	auto readLines = inFile.readLine(currentLine, 500);
	if (readLines < 0)
	{
		return false;
	}
	while(readLines > 0)
	{
		QStringList list = QString(currentLine).split(QRegExp(",|\\s+"), QString::SkipEmptyParts);

		if (list.size() >= 3) 
		{
			float vx = list[0].toFloat();
			float vy = list[1].toFloat();
			float vz = list[2].toFloat();
			m_inPutPoint.push_back(osg::Vec3f(vx,vy,vz));
		}
		readLines = inFile.readLine(currentLine, 500);
	}
	if (m_inPutPoint.size() > 0)
	{
		return true;
	}
	return false;
}

bool GridFilter::ReadLasFile(QString fileIn)
{

	QFile inFile(fileIn);
	if (!inFile.exists())
	{
		return nullptr;
	}
	//! ����las��laz�ļ�
	//���ļ�
	ifstream ifs;
	ifs.open(qPrintable(fileIn), std::ios::in | std::ios::binary);

	if (ifs.fail())
	{
		return false;
	}

	liblas::Reader* reader = 0;
	unsigned nbOfPoints = 0;

	try
	{
		reader = new liblas::Reader(liblas::ReaderFactory().CreateWithStream(ifs));
		//����ѹ�����ѹ���ļ�
		liblas::Header const& header = reader->GetHeader();

		//��ȡ�����
		nbOfPoints = header.GetPointRecordsCount();

		if (nbOfPoints == 0)
		{
			delete reader;
			ifs.close();
			return nullptr;
		}
		m_inPutPoint.clear();
	while (reader->ReadNextPoint())
	{
		liblas::Point const& p = reader->GetPoint();

		float vx = p.GetX();
		float vy = p.GetY();
		float vz = p.GetZ();
		m_inPutPoint.push_back(osg::Vec3f(vx,vy,vz));

	}
	}
	catch(...)
	{
		return false;
	}

	if (m_inPutPoint.size() > 0)
	{
		return true;
	}
	return false;
}
/**
	*��ʽ : ���, ����(ʡ��), ������, ������, �߳�  
*/
bool GridFilter::WriteDatAsccii(QString fileOut)
{
	QFile oFile(fileOut);
	if (!oFile.open(QIODevice::ReadWrite))
	{
		return false;
	}
	int i=0;
	for (auto itPoint = m_LowerPoint.begin(); itPoint != m_LowerPoint.end(); itPoint++ )
	{
		QString line;
		line.append(QString("%1").arg(i++));
		line.append(","); //��ӷָ���
		line.append(","); //��ӷָ���
		line.append(QString("%1").arg((*itPoint).x(), 0, 'f', 6));
		line.append(","); //��ӷָ���
		line.append(QString("%1").arg((*itPoint).y(), 0, 'f', 6));
		line.append(","); //��ӷָ���
		line.append(QString("%1").arg((*itPoint).z(), 0, 'f', 3));
		oFile.write(line.toStdString().c_str());
		oFile.write("\n");
	}

	//�رվ��
	oFile.close();
}